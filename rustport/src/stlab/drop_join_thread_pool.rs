use std::{io::Write, thread::JoinHandle};

/// A thread pool which joins all spawned threads when dropped.
///
/// This pool also holds a pointer to a heap-allocated object which is shared by all spawned
/// threads. As such, it must be Send + Sync, and 'static (having no non-static references). When
/// tasks in this pool are spawned, they are passed an immutable reference to said object (there is
/// no way to acquire a mutable reference).
///
/// Note: it remains to be seen if it is possible (or useful) to loosen the 'static requirement here
/// to instead allow any lifetimes which do not outlive this pool. Early attempts at this polluted
/// the API with explicit lifetimes. See
/// https://users.rust-lang.org/t/access-to-implicit-lifetime-of-containing-object-aka-self-lifetime/18917
/// for a relevant disucssion.
pub struct DropJoinThreadPool<T: Default + Send + Sync + 'static> {
    threads: Vec<JoinHandle<()>>,
    // SAFETY: We store this pointer as a `*mut` so it can be dropped via Box::from_raw, but
    // we only ever hand out immutable references to the pointee.
    data: *mut T,
}

impl<T: Default + Send + Sync + 'static> Drop for DropJoinThreadPool<T> {
    /// Join all spawned threads, and drop `data` manually.
    fn drop(&mut self) {
        for thread in std::mem::take(&mut self.threads) {
            let _ = thread.join(); // TODO handle error? what's the rule for drop?
        }

        // SAFETY: We only call from_raw once, in this `drop`. We do not permit copies of this
        // object, which would present the risk of a double-free on the copied pointer.
        unsafe { drop(Box::from_raw(self.data)) };
    }
}

impl<T: Default + Send + Sync + 'static> DropJoinThreadPool<T> {
    /// Creates a new `DropJoinThreadPool` with a maximum thread capacity of `thread_limit`, moving
    /// `data` onto the heap.
    pub fn new(thread_limit: usize, data: T) -> Self {
        Self {
            threads: Vec::<JoinHandle<()>>::with_capacity(thread_limit),
            data: Box::into_raw(Box::new(data)),
        }
    }

    /// Performs an operation with an immutable reference to this pool's `data` on the current
    /// thread.
    pub fn execute_immediately<Return, F: FnOnce(&T) -> Return>(&self, f: F) -> Return {
        // SAFETY: We only hand out immutable references to this data.
        let data: &T = unsafe { &*self.data };
        f(data)
    }

    /// Spawns at most `n` threads with the task `f` until this pool's thread capacity is reached.
    ///
    /// If spawning all `n` threads would exceed this pool's thread capacity, a message is logged to
    /// stderr.
    pub fn spawn_n<F: Fn(usize, &T) + Copy + Send + 'static>(&mut self, f: F, mut n: usize) {
        if self.threads.len() + n > self.threads.capacity() {
            Self::log_err(b"stlab: Unable to spawn all threads; capacity reached.");
            n = self.threads.capacity() - self.threads.len();
        }

        self.threads.extend((0..n).map(|i| {
            let f = f.clone();
            // SAFETY: We only hand out immutable references to this data.
            let data: &T = unsafe { &*self.data };
            std::thread::spawn(move || {
                f(i, data);
            })
        }));
    }

    /// Spawns a single thread with the task `f`, unless doing so would exceed this pool's thread
    /// capacity.
    ///
    /// If the thread is not spawned due to inadequate space, a message is logged to stderr.
    pub fn spawn<F: Fn(usize, &T) + Send + 'static>(&mut self, f: F) {
        let i = self.threads.len();

        if i == self.threads.capacity() {
            Self::log_err(b"stlab: Unable to spawn thread; capacity reached.");
            return;
        }

        self.threads.push({
            // SAFETY: We only hand out immutable references to this data.
            let data: &T = unsafe { &*self.data };
            std::thread::spawn(move || {
                f(i, data);
            })
        });
    }

    /// Write `buf` to stderr, discarding the ioresult.
    fn log_err(buf: &[u8]) {
        let mut stderr = std::io::stderr().lock();
        let _ = stderr.write_all(buf);
    }
}
