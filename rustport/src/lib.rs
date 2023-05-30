use std::ffi::c_void;

mod stlab;

/// Enqueues a the execution of `f(context)` on the PriorityTaskSystem.
#[no_mangle]
pub extern "C" fn execute(context: *mut c_void, f: extern fn(*mut c_void)) -> i32 {
  stlab::PriorityTaskSystem::singleton().execute(move||{
    f(context)
  }, stlab::Priority(0));
  0
}

/// Enqueues a the execution of `f(context)` on the PriorityTaskSystem at the given `priority`.
#[no_mangle]
pub extern "C" fn execute_priority(context: *mut c_void, f: extern fn(*mut c_void), priority: usize) -> i32 {
  stlab::PriorityTaskSystem::singleton().execute(move||{
    f(context)
  }, stlab::Priority(priority));
  0
}

#[cfg(test)]
mod tests {
    use super::*;
}
