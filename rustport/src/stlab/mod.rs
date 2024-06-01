use std::cmp::max;
use std::num::NonZeroUsize;
use std::sync::atomic::{AtomicUsize, Ordering as MemoryOrdering};
use std::sync::Arc;

mod coarse_priority_queue;
pub use coarse_priority_queue::Priority;

mod drop_join_thread_pool;
use drop_join_thread_pool::DropJoinThreadPool;

mod waiter;
pub use waiter::Waiter;

pub mod notification_queue;
use notification_queue::NotificationQueue;

/// A type-erased, heap-allocated function object.
pub type Task = Box<dyn FnOnce() -> () + Send>;

/// A portable work-stealing task scheduler with three priorities.
///
/// This scheduler spins up a number of threads corresponding to the amount of parallelism available
/// on the target platform, namely, std::thread::available_parallelism() - 1. Each thread is
/// assigned a threadsafe priority queue. To reduce contention on push and pop operations, a thread 
/// will first attempt to acquire the lock for its own queue without blocking.
/// If that fails, it will attempt the same non-blocking push/pop for each other priority queue in
/// the scheduler. Finally, if each of those attempts also fail, the thread will attempt a blocking
/// push/pop on its own priority queue.
///
/// The `add_thread` API is intended to mitigate the possibility of deadlock by spinning up a new
/// worker thread that non-blockingly polls all of the system's priority queues, and then sleeps
/// until `wake()` is called.
pub struct PriorityTaskSystem {
    pool: DropJoinThreadPool<Vec<NotificationQueue<Task>>>,
    waiters: Arc<Vec<Waiter>>,
    index: AtomicUsize,
    available_parallelism: usize,
}

impl Drop for PriorityTaskSystem {
    fn drop(&mut self) {
        self.pool.execute_immediately(|queues| {
            for queue in queues.iter() {
                queue.done()
            }
        });

        for waiter in self.waiters.iter() {
            waiter.done()
        }
    }
}

impl PriorityTaskSystem {
    /// Creates a new PriorityTaskSystem.
    pub fn new() -> Self {
        // SAFETY: We know 1 is not 0.
        let nonzero_available_parallelism = std::thread::available_parallelism()
            .unwrap_or(unsafe { NonZeroUsize::new_unchecked(1) });
        let available_parallelism = max(usize::from(nonzero_available_parallelism), 2) - 1;
        let thread_limit = max(9, available_parallelism * 4 + 1);
        let queues = (0..available_parallelism)
            .map(|_| NotificationQueue::default())
            .collect();

        let mut pool = DropJoinThreadPool::new(thread_limit, queues);

        pool.spawn_n(
            move |i, queues| loop {
                let mut task = Self::try_pop(queues, i, available_parallelism);

                if task.is_none() {
                    let done: bool;
                    (done, task) = queues.get(i).unwrap().pop();
                    if done {
                        break;
                    }
                }

                if task.is_some() {
                    task.unwrap()();
                }
            },
            available_parallelism,
        );

        Self {
            pool,
            waiters: Arc::new(
                (0..(thread_limit - available_parallelism))
                    .map(|_| Waiter::default())
                    .collect(),
            ),
            index: AtomicUsize::new(0),
            available_parallelism,
        }
    }

    /// Push `f` to the first queue in `queues` whose mutex is not under contention.
    /// If no such queue is found after a single pass, blockingly push `f` to one queue.
    pub fn execute<F>(&self, f: F, p: Priority)
    where
        F: FnOnce() -> () + Send + 'static,
    {
        self.execute_task(Box::new(f), p)
    }

    /// Push `task` to the first queue in `queues` whose mutex is not under contention.
    /// If no such queue is found after a single pass, blockingly push `task` to one queue.
    pub fn execute_task(&self, task: Task, priority: Priority) {
        self.pool.execute_immediately(|queues| {
            let mut task: Option<Task> = Some(task);
            let i = self.index.fetch_add(1, MemoryOrdering::SeqCst);
            let n = self.available_parallelism;

            // Attempt to push to each queue without blocking.
            for i in (i..i + n).map(|i| i % n) {
                task = queues.get(i).unwrap().try_push(task.unwrap(), priority);
                if task.is_none() {
                    return;
                } // An empty return means push was successful.
            }

            // Otherwise, attempt to blockingly push to one queue.
            queues.get(i % n).unwrap().push(task.unwrap(), priority);
        });
    }

    /// Add a work-stealing thread to the scheduler to mitigate deadlock.
    pub fn add_thread(&mut self) {
        let waiters = self.waiters.clone();
        let n = self.available_parallelism;
        self.pool.spawn(move |i, queues| {
            loop {
                if let Some(task) = Self::try_pop(queues, i, n) {
                    task();
                    continue;
                }

                // Note: The following means multiple threads may wait on a single `Waiter`.
                if waiters[i - n].wait() {
                    break;
                }
            }
        });
    }

    // Returns true if any thread was woken.
    pub fn wake(&self) -> bool {
        let any_queue_woken = self
            .pool
            .execute_immediately(|queues| return queues.iter().any(|queue| queue.wake()));

        if any_queue_woken {
            true
        } else {
            self.waiters.iter().any(|waiter| waiter.wake())
        }
    }

    /// Attempt to non-blockingly pop a task from each queue in the system, starting at index
    /// `starting_at`.
    fn try_pop(
        queues: &Vec<NotificationQueue<Task>>,
        starting_at: usize,
        modulo: usize,
    ) -> Option<Task> {
        for i in (starting_at..starting_at + modulo).map(|i| i % modulo) {
            match queues.get(i).unwrap().try_pop() {
                Some(t) => return Some(t),
                None => continue,
            };
        }
        return None;
    }
}

unsafe impl Send for PriorityTaskSystem {}
