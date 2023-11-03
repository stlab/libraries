use std::cmp::max;
use std::num::NonZeroUsize;
use std::sync::{Mutex, Arc};
use std::sync::atomic::{AtomicUsize, Ordering as MemoryOrdering};
use std::sync::OnceLock;

mod prioritized;
pub use prioritized::{Priority, Prioritized};

/// A type-erased, heap-allocated function object.
type Task = Box<dyn FnOnce()->() + Send>;

/// The fields of `Waiter` which must be protected by a `Mutex`.
struct WaiterProtectedData {
    waiting: bool,
    done: bool,
}

/// A utility for suspending a thread using a condition variable.
struct Waiter {
    protected: Mutex<WaiterProtectedData>,
    ready: std::sync::Condvar,
}

impl Default for Waiter {
    fn default() -> Self {
        Waiter::new() 
    }
}

impl Waiter {

    /// Constructs a new Waiter, with `waiting` and `done` set to `false`. 
    pub fn new() -> Self {
        Self {
            protected: Mutex::new(WaiterProtectedData {
                waiting: false,
                done: false,
            }),
            ready: std::sync::Condvar::new()
        }
    }

    /// Sets `done` to `true`, and notifies one waiter of our condition variable.
    pub fn done(&self) {
        {
            let mut this = self.protected.lock().expect("the mutex is not poisoned");
            this.done = true;
        }
        self.ready.notify_one();
    }

    /// Sets waiting to `false`. If waiting was `true`, wake one waiter and return `true`. Otherwise, return `false`.
    /// If `try_lock` fails, return `false`. (REVIEW: why?)
    /// (REVIEW: is it redundant to express that `waiting` and `done` are accesed under a mutex?)
    pub fn wake(&self) -> bool {
        if let Ok(ref mut this) = self.protected.try_lock() {
            if !this.waiting {
                return false;
            }
            this.waiting = false;
        } else {
            return false;
        }
        self.ready.notify_one();
        return true;
    }

    /// Block this thread until `wake()` or `done()` is called.
    /// Returns `true` if `done()` has been called, otherwise `false`.
    pub fn wait(&self) -> bool {
        let mut this = self.protected.lock().expect("the mutex is not poisoned");
        this.waiting = true;
        while this.waiting && !this.done {
            this = self.ready.wait(this).expect("the mutex is not poisoned");
        }
        this.waiting = false;
        return this.done;
    }
}

/// The fields of `NotificationQueue` which must be protected by a `Mutex`.
struct NotificationQueueProtectedData <T> {
    heap: std::collections::BinaryHeap<Prioritized<T>>,
    count: usize,
    done: bool,
    waiting: bool,
}

impl<T> std::default::Default for NotificationQueueProtectedData<T> {
    fn default() -> Self {
        Self {
            heap: std::collections::BinaryHeap::new(),
            count: 0,
            done: false,
            waiting: false
        }
    }
}

/// A threadsafe priority queue.
struct NotificationQueue<T> {
    // In the C++ implementation, we use a single lock for multiple data fields.
    // In Rust, we require exactly one mutex per protected field. 
    // So, put protected fields into a separate struct, and lock on that.
    // Note the transformation to use multiple locks is non-trivial, because 
    // this would require the ordering of acquired locks to be identical in all
    // code paths to prevent deadlock.
    protected: Mutex<NotificationQueueProtectedData<T>>,
    ready: std::sync::Condvar,
}

impl<T> std::default::Default for NotificationQueue<T> {
    fn default() -> Self {
        Self {
            protected: Mutex::new(NotificationQueueProtectedData::<T>::default()),
            ready: std::sync::Condvar::new(),
        }
    }
}

impl<T> NotificationQueue<T> {

    /// Merge priority and count into a single usize, storing the former in the
    /// two highest bits of the result. This requires priority be in [0, 4) i.e.,
    /// takes up two bits.
    fn merge_priority_count(priority: Priority, count: usize) -> Priority {
        Priority(priority.to_highbit_mask() | count)
    }

    /// Try to pop from the queue without blocking.
    /// Returns `None` if our mutex is already locked or if the queue is empty.
    pub fn try_pop(&self) -> Option<T> {
        if let Ok(ref mut this) = self.protected.try_lock() {
            if !this.heap.is_empty() { 
                return Some(this.heap.pop().unwrap().take_element());
            }
        } 
        return None;
    }

    /// If waiting in `pop()`, wakes and returns true. Otherwise returns false.
    pub fn wake(&self) -> bool {
        if let Ok(ref mut this) = self.protected.try_lock() {
            if !this.waiting {
                return false;
            }
            this.waiting = false; // triggers wake
        }
        return false;
    }

    /// Pop from the queue, suspending the current thread until an element is available.
    /// The returned `bool` indicates if this object is `done()`.
    pub fn pop(&self) -> (bool, Option<T>) {
        let mut this = self.protected.lock().expect("the mutex is not poisoned");
        this.waiting = true;
        while !this.heap.is_empty() && !this.done && this.waiting {
            this = self.ready.wait(this).expect("the mutex is not poisoned");
        }
        this.waiting = false;
        if this.heap.is_empty() {
            return (this.done, None);
        } 
        return (false, Some(this.heap.pop().unwrap().take_element()));
    }

    /// Mark this object for teardown, and wake any thread awaiting an available element in `pop()`.
    pub fn done(&self) {
        {
            let mut this = self.protected.lock().expect("the mutex is not poisoned");
            this.done = true
        }
        self.ready.notify_one();
    }

    /// Try to push `element` to the queue without blocking, returning `element` if our mutex is already locked.
    /// If the push succeeds, wake a thread which may be awaiting an element in `pop()`.
    pub fn try_push(&self, element: T, priority: Priority) -> Option<T> {
        if let Ok(ref mut this) = self.protected.try_lock() {
            let priority = Self::merge_priority_count(priority, this.count);
            this.count += 1;
            this.heap.push(Prioritized::new(priority, element));
        } else {
            return Some(element);
        }
        
        // We successfully locked the mutex, did our push, and released the lock.
        self.ready.notify_one();
        return None;
    }

    /// Push `element` to the queue, blocking if our mutex is already locked.
    /// When the push succeeds, wake a thread which may be awaiting an element in `pop()`.
    pub fn push(&self, element: T, priority: Priority) {
        {
            let mut this = self.protected.lock().expect("the mutex is not poisoned");
            let priority = Self::merge_priority_count(priority, this.count);
            this.count += 1;
            this.heap.push(Prioritized::new(priority, element));
        }
        self.ready.notify_one();
    }
}

unsafe impl <T> Sync for NotificationQueue<T> {}
/// The fields of `PriorityTaskSystem` which must be protected by a `Mutex`.
#[derive(Default)]
struct PriorityTaskSystemProtectedData {
    threads: Vec<std::thread::JoinHandle<()>>,
    waiters: Vec<Waiter>,
}

/// A thread-scalable priority task system.
pub struct PriorityTaskSystem {
    available_parallelism: usize, // _count in C++ implementation.
    thread_limit: usize,
    queues: Arc<Vec<NotificationQueue<Task>>>,
    index: AtomicUsize,
    protected: Arc<Mutex<PriorityTaskSystemProtectedData>>
}

impl PriorityTaskSystem {

    pub fn new() -> Self {
        // SAFETY: We know 1 is not 0.
        let nonzero_available_parallelism = std::thread::available_parallelism().unwrap_or(unsafe { NonZeroUsize::new_unchecked(1) });
        let available_parallelism = max(usize::from(nonzero_available_parallelism), 2) - 1;
        let thread_limit = max(9, available_parallelism * 4 + 1);

        
        let queues = Arc::new({
            let mut v = Vec::with_capacity(available_parallelism);
            for _ in 0..available_parallelism { v.push(NotificationQueue::<Task>::default()) }
            v
        });

        Self {
            available_parallelism,
            thread_limit,
            queues: queues.clone(),
            index: AtomicUsize::new(0),
            protected: Arc::new(Mutex::new(PriorityTaskSystemProtectedData { 
                waiters: (0..(thread_limit - available_parallelism)).map(|_| { Waiter::default() }).collect(),
                threads: (0..available_parallelism).map(|i| {
                    let queues = queues.clone();
                    std::thread::spawn(move ||{
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
                    })
                }).collect() 
            }))
        }
    }

    /// Mark all subobjects for teardown, and join all threads spawned by this object.
    pub fn join(&self) {
        for queue in self.queues.iter() {
            queue.done();
        }

        // Consume the contents under the mutex, leaving them empty.
        let mut this = self.protected.lock().expect("the mutex is not poisoned");
        for e in &this.waiters {
            e.done();
        }
        for e in std::mem::take(&mut this.threads) {
            let _ = e.join();
        }

        // self.queues.clear(); BIG REVISIT
    }

    /// Push `f` to the first queue in `queues` whose mutex is not under contention.
    /// If no such queue is found after a single pass, blockingly push `f` to one queue.
    // REVIEW: I'm not sure `execute` is a good name. I think we want `push`, or `push_with_priority`.
    pub fn execute<F>(&mut self, f: F, p: Priority) where F: FnOnce() -> () + Send + 'static {
        self.execute_task(Box::new(f), p)
    }

    pub fn execute_task(&mut self, task: Task, priority: Priority) {
        let mut task: Option<Task> = Some(task);
        // Use SeqCst to match the default for C++'s std::atomic<unsigned>::operator++(int).
        let i = self.index.fetch_add(1, MemoryOrdering::SeqCst); 
        for n in 0..i {
            task = self.queues.get((i + n) % self.available_parallelism).unwrap().try_push(task.unwrap(), priority);
            // task = self.queues[(i + n) % self.available_parallelism].try_push(task.unwrap(), priority);
            if task.is_none() { return }
        }

        self.queues.get(i % self.available_parallelism).unwrap().push(task.unwrap(), priority);
    }

    /// Spawn an expansion thread which will steal tasks from `queues`.
    pub fn add_thread(&self) {
        let mut this = self.protected.lock().expect("the mutex is not poisoned");
        if this.threads.len() == self.thread_limit {
            eprintln!("Unable to add thread; thread_limit reached: {}", self.thread_limit);
            return;
        }

        let queues = self.queues.clone();
        let protected = self.protected.clone();
        let available_parallelism = self.available_parallelism.clone();
        let i = this.threads.len();

        this.threads.push(std::thread::spawn(move ||{
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

                let protected = protected.lock().expect("the mutex is not poisoned");

                // Note: The following means multiple threads may wait on a single `Waiter`.
                if protected.waiters[i - available_parallelism].wait() { break; }
            }
        }))
    }

    /// Get the number of waiters.
    fn waiters_size(&self) -> usize {
        let this = self.protected.lock().expect("the mutex is not poisoned");
        this.threads.len() - self.available_parallelism
    }

    /// Wake every `Waiter` in `waiters`. Return true if any indicate they are `done()`, otherwise `false`.
    pub fn wake(&mut self) -> bool {
        for queue in self.queues.iter() {
            if queue.wake() { return true } 
        }

        let size = self.waiters_size();
        let this = self.protected.lock().expect("the mutex is not poisoned");
        for n in 0..size {
            if this.waiters[n].wake() {
                return true;
            }
        }

        return false;
    }
}
