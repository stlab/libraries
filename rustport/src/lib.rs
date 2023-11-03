use std::{ffi::c_void, sync::Mutex};
use once_cell::sync::Lazy;

use stlab::PriorityTaskSystem;
mod stlab;

static TASK_SYSTEM: Lazy<Mutex<PriorityTaskSystem>> = Lazy::new(|| Mutex::new(PriorityTaskSystem::new()) );
struct ThreadsafeCFnWrapper {
  context: *mut c_void,
  fn_ptr: extern fn(*mut c_void)
}

impl ThreadsafeCFnWrapper {
  pub(crate) fn new(context: *mut c_void, fn_ptr: extern fn(*mut c_void)) -> Self {
      Self {
          context,
          fn_ptr
      }
  }

  pub(crate) fn call(&self) {
      (self.fn_ptr)(self.context)
  }
}

unsafe impl Send for ThreadsafeCFnWrapper {}

/// Enqueues a the execution of `f(context)` on the PriorityTaskSystem.
#[no_mangle]
pub extern "C" fn execute(context: *mut c_void, fn_ptr: extern fn(*mut c_void)) -> i32 {
  execute_priority(context, fn_ptr, 0);
  0
}

/// Enqueues a the execution of `f(context)` on the PriorityTaskSystem at the given `priority`.
#[no_mangle]
pub extern "C" fn execute_priority(context: *mut c_void, fn_ptr: extern fn(*mut c_void), p: usize) -> i32 {
  let wrap = ThreadsafeCFnWrapper::new(context, fn_ptr);
  TASK_SYSTEM.lock().unwrap().execute(move||{
    wrap.call()
  }, stlab::Priority(p));
  0
}
