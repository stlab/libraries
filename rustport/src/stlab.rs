use std::cmp::{Ordering, max, Eq, Ord, PartialEq, PartialOrd};
use std::mem::MaybeUninit;
use std::num::NonZeroUsize;
use std::sync::{Mutex, Once};
use std::sync::atomic::{AtomicUsize, Ordering as MemoryOrdering};

/// A type-erased, heap-allocated function object.
type Task = Box<dyn FnOnce()->()>;

/// A `usize` constraining valid values to [0, 4) with runtime assertions.
#[derive(Eq, PartialEq, Ord, PartialOrd, Copy, Clone)]
pub struct Priority(pub usize);

impl Priority {
    pub fn new(value: usize) -> Self {
        assert!((0..4).contains(&value), "Priorities must be in [0, 4)");
        Self(value)
    }

    /// Returns a usize of the form 0bXX000000 where XX is a binary representation of this priority.
    /// The value of this priority is guaranteed to fit in two bits because `Priority` values are constrained to [0b00, 0b11].
    pub fn to_highbit_mask(&self) -> usize {
        &self.0 << (usize::BITS - 2)
    }
}

/// Pairs an instance of `T` with a `Priority`. 
/// Equality and ordering of a Prioritized<T> only considers `priority`, disregarding `element`.
struct Prioritized <T> {
    priority: Priority,
    element: T
}

impl<T> PartialEq for Prioritized <T> {
    #[inline]
    fn eq(&self, other: &Self) -> bool {
        self.priority == other.priority
    }
}

impl <T> Eq for Prioritized <T> {}

impl<T> PartialOrd for Prioritized <T> {
    #[inline]
    fn partial_cmp(&self, other: &Self) -> Option<Ordering> {
        self.priority.partial_cmp(&other.priority)
    }
}

impl<T> Ord for Prioritized <T> {
    #[inline]
    fn cmp(&self, other: &Self) -> Ordering {
        self.priority.cmp(&other.priority)
    }
}

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
    pub fn try_pop(&mut self) -> Option<T> {
        if let Ok(ref mut this) = self.protected.try_lock() {
            if !this.heap.is_empty() { 
                return Some(this.heap.pop().unwrap().element);
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
        return (false, Some(this.heap.pop().unwrap().element));
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
            this.heap.push(Prioritized{ element, priority });
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
            this.heap.push(Prioritized{ element, priority });
        }
        self.ready.notify_one();
    }
}

/// The fields of `PriorityTaskSystem` which must be protected by a `Mutex`.
#[derive(Default)]
struct PriorityTaskSystemProtectedData {
    threads: Vec<std::thread::JoinHandle<()>>,
    waiters: Vec<Waiter>,
}

impl PriorityTaskSystemProtectedData {
    fn spawn_thread<F>(thread_name: &'static str, f: F) -> std::thread::JoinHandle<()> 
        where F: FnOnce() -> () + 'static + Send {
        std::thread::Builder::new().name(thread_name.to_string())
            .spawn(f).expect("spawning a thread does not fail")
    }

    /// Spawn `thread_count` threads, each of which pops tasks from an associated queue in `queues`, 
    /// or steals tasks from other queues if no task is available.
    pub(self) fn spawn_baseline_threads(&mut self, thread_count: usize, available_parallelism: usize) {
        self.threads.extend((0..thread_count).map(|i| {
            Self::spawn_thread("cc.stlab.default_executor", move || {
                loop {
                    let this = PriorityTaskSystem::singleton(); // why doesn't this have to be mut?
                    let mut task: Option<Task> = None;
                    for n in 0..available_parallelism {
                        match task {
                            Some(_) => { break } 
                            _ => {
                                task = this.queues[(i + n) % available_parallelism].try_pop()
                            }
                        }
                    }

                    if task.is_none() {
                        let done: bool;
                        (done, task) = this.queues[i].pop();
                        if done { break }
                    }

                    if task.is_some() {
                        task.unwrap()();
                    }
                }
            })
        }))
    }

    /// Spawn one thread which will repeatedly check each queue in `queues` for tasks, and execute them.
    /// If no tasks are found, the thread is suspended.
    pub(self) fn spawn_expansion_thread(&mut self, available_parallelism: usize) {
        let protected = PriorityTaskSystem::singleton().protected.get_mut().expect("the mutex is not poisoned");
        let i = protected.threads.len();

        if i == PriorityTaskSystem::singleton().available_parallelism {
            eprintln!("Thread limit reached")
        }
        
        self.threads.push(Self::spawn_thread("cc.stlab.default_executor.expansion", move || {
            loop {
                let this = PriorityTaskSystem::singleton(); // why doesn't this have to be mut?
                let mut task: Option<Task> = None;
                for n in 0..available_parallelism {
                    match task {
                        Some(_) => { break } 
                        _ => {
                            task = this.queues[(i + n) % available_parallelism].try_pop()
                        }
                    }
                }

                if task.is_some() {
                    task.unwrap()();
                    continue;
                }

                let protected = this.protected.get_mut().expect("the mutex is not poisoned");

                // Note: The following means multiple threads may wait on a single `Waiter`.
                if protected.waiters[i - available_parallelism].wait() { break; }
            }
        }));
    }
}

/// A thread-scalable priority task system.
pub struct PriorityTaskSystem {
    available_parallelism: usize, // _count in C++ implementation.
    thread_limit: usize,
    queues: Vec<NotificationQueue<Task>>,
    index: AtomicUsize,
    protected: Mutex<PriorityTaskSystemProtectedData>
}

impl PriorityTaskSystem {
    
    // TODO: Roll back the singleton pattern, and find a way to make instances immovable.
    /// Return the singleton instance of `PriorityTaskSystem`, creating it on the first invocation.
    pub fn singleton() -> &'static mut Self {
        static mut INSTANCE: MaybeUninit<PriorityTaskSystem> = MaybeUninit::uninit();
        static ONCE: Once = Once::new();
        
        ONCE.call_once(|| {
            // This clunky expression is to convert from a NonZeroUsize to a usize for the subsequent arithmetic. I'd like a better way.
            // SAFETY: we know 1 is not 0.
            let nonzero_available_parallelism = std::thread::available_parallelism().unwrap_or(unsafe { NonZeroUsize::new_unchecked(1) });
            let available_parallelism = max(usize::from(nonzero_available_parallelism), 2) - 1;
            let thread_limit = max(9, available_parallelism * 4 + 1);

            unsafe { 
                INSTANCE.as_mut_ptr().write(Self {
                    available_parallelism,
                    thread_limit,
                    queues: {
                        let mut v = Vec::with_capacity(available_parallelism);
                        for _ in 0..available_parallelism { v.push(NotificationQueue::<Task>::default()) }
                        v
                    },
                    index: AtomicUsize::new(0),
                    protected: Mutex::new(PriorityTaskSystemProtectedData {
                        waiters: Vec::with_capacity(thread_limit - available_parallelism),
                        threads: Vec::with_capacity(available_parallelism),
                    })
                }); 
            }

            // Defer spawning threads so they can never witness an uninitialized `INSTANCE`.
            // Note `get_mut` statically enforces exclusive access; no locking will take place.
            unsafe {
                // `unwrap` should never panic; that would imply the mutex we just created, and never
                // handed out, has been poisoned.
                (*INSTANCE.as_mut_ptr()).protected.get_mut().unwrap().spawn_baseline_threads(available_parallelism, available_parallelism);
            }
        });

        unsafe { &mut *INSTANCE.as_mut_ptr() }
    }

    /// Mark all subobjects for teardown, and join all threads spawned by this object.
    pub fn join(&mut self) {
        for queue in &self.queues {
            queue.done();
        }

        // Consume the contents under the mutex, leaving them empty.
        let this = std::mem::take(self.protected.get_mut().expect("the mutex is not poisoned"));
        for e in this.waiters {
            e.done();
        }
        for e in this.threads {
            let _ = e.join();
        }

        self.queues.clear();
    }

    /// Push `f` to the first queue in `queues` whose mutex is not under contention.
    /// If no such queue is found after a single pass, blockingly push `f` to one queue.
    // REVIEW: I'm not sure `execute` is a good name. I think we want `push`, or `push_with_priority`.
    pub fn execute<F>(&mut self, f: F, p: Priority) where F: FnOnce() -> () + 'static {
        let mut task: Option<Task> = Some(Box::new(f));
        // Use SeqCst to match the default for C++'s std::atomic<unsigned>::operator++(int).
        let i = self.index.fetch_add(1, MemoryOrdering::SeqCst); 
        for n in 0..i {
            task = self.queues[(i + n) % self.available_parallelism].try_push(task.unwrap(), p);
            if task.is_none() { return }
        }

        self.queues[i % self.available_parallelism].push(task.unwrap(), p);
    }

    /// Spawn an expansion thread which will steal tasks from `queues`.
    pub fn add_thread(&mut self) {
        let mut this = self.protected.lock().expect("the mutex is not poisoned");
        if this.threads.len() == self.thread_limit {
            eprintln!("Unable to add thread; thread_limit reached: {}", self.thread_limit);
            return;
        }

        this.spawn_expansion_thread(self.available_parallelism);
    }

    /// Get the number of waiters.
    fn waiters_size(&self) -> usize {
        let this = self.protected.lock().expect("the mutex is not poisoned");
        this.threads.len() - self.available_parallelism
    }

    /// Wake every `Waiter` in `waiters`. Return true if any indicate they are `done()`, otherwise `false`.
    pub fn wake(&mut self) -> bool {
        for queue in &self.queues {
            if queue.wake() { return true } 
        }

        let size = self.waiters_size();
        // Note: get_mut ensures no locking occurs; uniqueness enforced by the type system.
        let this = self.protected.get_mut().expect("the mutex is not poisoned");
        for n in 0..size {
            if this.waiters[n].wake() {
                return true;
            }
        }

        return false;
    }
}
