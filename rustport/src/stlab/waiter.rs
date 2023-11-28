use std::sync::Mutex;

// REVISIT: It may be wise to reimplement this in terms of std::thread::park.

/// A utility for suspending a thread using a condition variable.
pub struct Waiter {
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
            ready: std::sync::Condvar::new(),
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

unsafe impl Send for Waiter {}
unsafe impl Sync for Waiter {}

/// The fields of `Waiter` which must be protected by a `Mutex`.
struct WaiterProtectedData {
    waiting: bool,
    done: bool,
}
