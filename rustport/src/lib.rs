use once_cell::sync::Lazy;
use std::{ffi::c_void, sync::Mutex};

use stlab::{Priority, PriorityTaskSystem};
mod stlab;

/// A static instance of the task system which is invoked through the `execute` functions below.
static TASK_SYSTEM: Lazy<Mutex<PriorityTaskSystem>> =
    Lazy::new(|| Mutex::new(PriorityTaskSystem::new()));


/// A function pointer paired with a context, akin to a C++ lambda and its captures.
/// 
/// "Threadsafe" is not a guarantee, it is a requirement. These pointers are assumed to be able to
/// be sent to another thread, and therefore must not rely on thread-local state. 
struct ThreadsafeCFnWrapper {
    context: *mut c_void,
    fn_ptr: extern "C" fn(*mut c_void),
}

impl ThreadsafeCFnWrapper {
    pub(crate) fn new(context: *mut c_void, fn_ptr: extern "C" fn(*mut c_void)) -> Self {
        Self { context, fn_ptr }
    }

    // Note: there is no way in stable rust to make a struct invocable with () syntax.
    pub(crate) fn call(&self) {
        (self.fn_ptr)(self.context)
    }
}

/// `ThreadsafeCFnWrapper` may not rely on thread-local state.
unsafe impl Send for ThreadsafeCFnWrapper {}

/// Enqueues a the execution of `f(context)` on the PriorityTaskSystem.
/// 
/// Precondition: Neither `context` nor `fn_ptr` may rely on thread-local state.
#[no_mangle]
pub extern "C" fn execute(context: *mut c_void, fn_ptr: extern "C" fn(*mut c_void)) -> i32 {
    execute_priority(context, fn_ptr, Priority::Default);
    0
}

/// Enqueues a the execution of `f(context)` on the PriorityTaskSystem at the given `priority`.
/// 
/// Precondition: Neither `context` nor `fn_ptr` may rely on thread-local state.
#[no_mangle]
pub extern "C" fn execute_priority(
    context: *mut c_void,
    fn_ptr: extern "C" fn(*mut c_void),
    p: Priority,
) -> i32 {
    let wrap = ThreadsafeCFnWrapper::new(context, fn_ptr);
    TASK_SYSTEM.lock().unwrap().execute(move || wrap.call(), p);
    0
}
