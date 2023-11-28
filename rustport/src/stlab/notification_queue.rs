use crate::stlab::coarse_priority_queue::{Priority, CoarsePriorityQueue};

/// A threadsafe priority queue.
pub struct NotificationQueue<T> {
    protected: std::sync::Mutex<NotificationQueueProtectedData<T>>,
    ready: std::sync::Condvar,
}

impl<T> std::default::Default for NotificationQueue<T> {
    fn default() -> Self {
        Self {
            protected: std::sync::Mutex::new(NotificationQueueProtectedData::<T>::default()),
            ready: std::sync::Condvar::new(),
        }
    }
}

impl<T> NotificationQueue<T> {

    /// Try to pop from the queue without blocking.
    /// Returns `None` if our mutex is already locked or if the queue is empty.
    pub fn try_pop(&self) -> Option<T> {
        if let Ok(ref mut this) = self.protected.try_lock() {
            return this.queue.pop();
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
        self.ready.notify_one();
        return true;
    }

    /// Pop from the queue, suspending the current thread until an element is available.
    /// The returned `bool` indicates if this object is `done()`.
    pub fn pop(&self) -> (bool, Option<T>) {
        let mut this = self.protected.lock().expect("the mutex is not poisoned");
        this.waiting = true;
        while !this.queue.is_empty() && !this.done && this.waiting {
            this = self.ready.wait(this).expect("the mutex is not poisoned");
        }
        this.waiting = false;
        if this.queue.is_empty() {
            return (this.done, None);
        } 
        return (false, this.queue.pop());
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
            this.queue.push(element, priority);
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
            this.queue.push(element, priority);
        }
        self.ready.notify_one();
    }
}

unsafe impl <T> Send for NotificationQueue<T> {}
unsafe impl <T> Sync for NotificationQueue<T> {}

/// The fields of `NotificationQueue` which must be protected by a `Mutex`.
struct NotificationQueueProtectedData <T> {
    queue: CoarsePriorityQueue<T>,
    done: bool,
    waiting: bool,
}

impl<T> std::default::Default for NotificationQueueProtectedData<T> {
    fn default() -> Self {
        Self {
            queue: CoarsePriorityQueue::new(),
            done: false,
            waiting: false
        }
    }
}
