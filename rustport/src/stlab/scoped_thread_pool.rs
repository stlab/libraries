use std::thread::JoinHandle;

pub struct ScopedThreadPool<T: Default + Send + Sync + 'static> {
    threads: Vec<JoinHandle<()>>,
    // SAFETY: We store this pointer as a `*mut` so it can be dropped via Box::from_raw, but
    // we only ever hand out const-refs to the underlying data. 
    data: *mut T
}

impl <T: Default + Send + Sync + 'static> Drop for ScopedThreadPool <T> {
    fn drop(&mut self) {
        for thread in std::mem::take(&mut self.threads) {
            let _ = thread.join(); // TODO handle error? what's the rule for drop?
        }

        // SAFETY: We only call from_raw once, in this `drop`. We do not permit copies of this
        // object, which would present the risk of a double-free on the copied pointer.
        unsafe { drop(Box::from_raw(self.data)) }; 
    }
}

impl <T: Default + Send + Sync + 'static> ScopedThreadPool <T> {
    pub fn new<F: Fn(usize, &T) + Copy + Send + 'static>(num_threads: usize, f: F, data: T) -> Self {
        let data = Box::into_raw(Box::new(data));
        Self {
            threads: (0..num_threads).map(|i| {
                let f = f.clone();
                // SAFETY: We only hand out immutable references to this data.
                let data: &T = unsafe { &*data };
                std::thread::spawn(move || {
                    f(i, data);
                })
            }).collect(),
            data
        }
    }

    pub fn execute_immediately<Return, F: FnOnce(&T) -> Return>(&self, f: F) -> Return {
        // SAFETY: We only hand out immutable references to this data.
        let data: &T = unsafe { &*self.data };
        f(data)
    }

    pub fn add_thread<F: Fn(usize, &T) + Send + 'static>(&mut self, f: F) {
        let i = self.threads.len();
        self.threads.push({
            // SAFETY: We only hand out immutable references to this data.
            let data: &T = unsafe { &*self.data };
            std::thread::spawn(move || {
                f(i, data);
            })
        });
    }
}