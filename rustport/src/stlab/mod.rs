use std::cmp::max;
use std::num::NonZeroUsize;
use std::sync::Arc;
use std::sync::atomic::{AtomicUsize, Ordering as MemoryOrdering};

mod coarse_priority_queue;
pub use coarse_priority_queue::{Priority, CoarsePriorityQueue};

mod scoped_thread_pool;
use scoped_thread_pool::ScopedThreadPool;

mod waiter;
pub use waiter::Waiter;

pub mod notification_queue;
use notification_queue::NotificationQueue;

/// A type-erased, heap-allocated function object.
pub type Task = Box<dyn FnOnce()->() + Send>;
pub struct PriorityTaskSystem {
    pool: ScopedThreadPool<Vec<NotificationQueue<Task>>>,
    waiters: Arc<Vec<Waiter>>,
    index: AtomicUsize,
    available_parallelism: usize,
    thread_limit: usize,
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
    pub fn new() -> Self {
        // SAFETY: We know 1 is not 0.
        let nonzero_available_parallelism = std::thread::available_parallelism().unwrap_or(unsafe { NonZeroUsize::new_unchecked(1) });
        let available_parallelism = max(usize::from(nonzero_available_parallelism), 2) - 1;
        let thread_limit = max(9, available_parallelism * 4 + 1);
        Self {
            pool: ScopedThreadPool::new(42, move |i, queues| {
                loop {
                    let mut task: Option<Task> = None;
                    for n in 0..available_parallelism {
                        match task {
                            Some(_) => { break } 
                            _ => {
                                task = queues.get((i + n) % available_parallelism).unwrap().try_pop();
                            }
                        }
                    }

                    if task.is_none() {
                        let done: bool;
                        (done, task) = queues.get(i).unwrap().pop();
                        if done { break }
                    }

                    if task.is_some() {
                        task.unwrap()();
                    }
                }
            }, (0..available_parallelism).map(|_| { NotificationQueue::default() }).collect()),
            waiters: Arc::new((0..(thread_limit - available_parallelism)).map(|_| { Waiter::default() }).collect()),
            index: AtomicUsize::new(0),
            available_parallelism,
            thread_limit
        }
    }

    /// Push `f` to the first queue in `queues` whose mutex is not under contention.
    /// If no such queue is found after a single pass, blockingly push `f` to one queue.
    // REVIEW: I'm not sure `execute` is a good name. I think we want `push`, or `push_with_priority`.
    pub fn execute<F>(&self, f: F, p: Priority) where F: FnOnce() -> () + Send + 'static {
        self.execute_task(Box::new(f), p)
    }

    pub fn execute_task(&self, task: Task, priority: Priority) {
        self.pool.execute_immediately(|queues| {
            let mut task: Option<Task> = Some(task);
            let i = self.index.fetch_add(1, MemoryOrdering::SeqCst);
            let n = self.available_parallelism;

            for i in (i..i+n).map(|i| i % n) {
                task = queues.get(i).unwrap().try_push(task.unwrap(), priority);
                if task.is_none() { return }   
            }

            queues.get(i % n).unwrap().push(task.unwrap(), priority);
        });
    }

    pub fn add_thread(&mut self) {
        let waiters = self.waiters.clone();
        let available_parallelism = self.available_parallelism;
        self.pool.add_thread(move |i, queues|{
            loop {
                let mut task: Option<Task> = None;
                for n in 0..available_parallelism {
                    match task {
                        Some(_) => { break } 
                        _ => {
                            task = queues.get((i + n) % available_parallelism).unwrap().try_pop()
                        }
                    }
                }

                if task.is_some() {
                    task.unwrap()();
                    continue;
                }

                // Note: The following means multiple threads may wait on a single `Waiter`.
                if waiters[i - available_parallelism].wait() { break; }
            } 
        });
    }

    // Returns true if any thread was woken.
    pub fn wake(&self) -> bool {
        let any_queue_woken = self.pool.execute_immediately(|queues| {
            return queues.iter().any(|queue| queue.wake())
        });

        if any_queue_woken { true }
        else { self.waiters.iter().any(|waiter| waiter.wake()) }
    }
}

unsafe impl Send for PriorityTaskSystem {}