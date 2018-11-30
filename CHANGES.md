## v1.4.0 - 2018 - December - 
- Fixed Issues
    - [#206](https://github.com/stlab/libraries/issues/206): channels does not allow processes as lvalues, only as rvalue
    - [#204](https://github.com/stlab/libraries/issues/204): when_all does not support move-only types
    - [#192](https://github.com/stlab/libraries/issues/192): Issues with executor_t and when_all()

- Enhancements
    - [#199](https://github.com/stlab/libraries/issues/199): add operator|() as continuable operator
    - It is now possible to create with channel<void>() a receiver<void> (not a pair<sender<void>, receiver<void>>)

- Library Changes
    - Now C++17 is required to use the channels (This change was necessary, because the code became really messy to dispatch between copyable, move-only and void types)

- Additional Comments
    - It is necessary to patch std::variant for the combination of clang version <= 7 and stdlibc++. For further details see https://stackoverflow.com/questions/46506387/getstring-for-variants-fail-under-clang-but-not-g and ./travis/patches/patch_std_variant.sh. This is because the clang bug https://bugs.llvm.org/show_bug.cgi?id=31852

## v1.3.3 - 2018 - October - 25

- Fixed Issues 
   - [#154](https://github.com/stlab/libraries/issues/154) : Compilation error with gcc
   - Race condition in timed blocking_get

- Enhancements
   - The library can now be included as cmake dependency (Many thanks to Austin McCartney)  
    
## v1.3.2 - 2018 - July - 28

- Fixed Issues 
   - [#169](https://github.com/stlab/libraries/issues/169) : when_all() appears to ignore the passed-in executor

## v1.3.1 - 2018 - July - 21

- Fixed Issues 
   - [#169](https://github.com/stlab/libraries/issues/169) : when_all() appears to ignore the passed-in executor
   - [#168](https://github.com/stlab/libraries/issues/168) : Main Executor requires "widgets" to be dragged into Qt build
   - [#167](https://github.com/stlab/libraries/issues/167) : Main Executor has potential segfault
   - [#166](https://github.com/stlab/libraries/issues/166) : Multiple task_system and system_timer are being created

## v1.3.0 - 2018 - July - 15

- Fixed Issues 
   - [#160](https://github.com/stlab/libraries/issues/160) : main_executor for lib dispatch: `operator()` missing const qualifier
   - [#156](https://github.com/stlab/libraries/issues/156) : `set_error()` does not get called
   - [#134](https://github.com/stlab/libraries/issues/134) : Using `future<>::detach()` is a possible memory leak
   - [#107](https://github.com/stlab/libraries/issues/107) : Some channel functions have problematic names
- Library Breaking Changes
   - Concurrency Library
       - The functionality of channel's `zip` has changed. Now it behaves comparable to the ranges TS zip. The previous functionality is replaced with `zip_with`.
       - The `merge` and `join` functions are now deprecated and are replaced with `merge_channel<unordered_t>`and `merge_channel<round_robin_t>`.

## v1.2.0 - 15 June 2018

- Fixed Issues 
   - [#146](https://github.com/stlab/libraries/issues/146) : when_all() does not compile with move-only objects
   - [#142](https://github.com/stlab/libraries/issues/142) : boost/variant.hpp: No such file or directory
   - [#119](https://github.com/stlab/libraries/issues/119) : blocking_get randomly does not return
- Library Additions
   - Concurrency Library
       - Adding usage of std::optional, std::variant or std::experimental::optional when compiling with C++17 mode. So the dependency to boost.optional is optional.
       - Adding co-routine support for futures. (Only for VS 2017 and clang 5.0)
       - Adding main_executor for Qt support
   
## v1.1.1 - 13 December 2017

- Fixed Issues
   - [#113](https://github.com/stlab/libraries/issues/113) : Reduction does not appear to work for failed futures; Many thanks to Frans-Willem!
   - [#115](https://github.com/stlab/libraries/issues/115) : Version number is forgotten in version.hpp?

## v1.1 - 28 November 2017

- Fixed Issues
   - [#94](https://github.com/stlab/libraries/issues/94) : Use of moved executor in futures.hpp:1234
   - [#93](https://github.com/stlab/libraries/issues/93) : Stack overflow with exception handling in futures
   - [#85](https://github.com/stlab/libraries/issues/85) : (non-default) Executors and `make_ready_future`
   - [#80](https://github.com/stlab/libraries/issues/80) : `stlab::async` will not accept non-const reference parameters
   - [#79](https://github.com/stlab/libraries/issues/79) : `when_all` does not support continuing futures that return `void`
   - [#69](https://github.com/stlab/libraries/issues/69) : Support move-only tasks
   - [#59](https://github.com/stlab/libraries/issues/59) : `task_system` and `notification_queue` doesn't work with move only lambda
   - [#26](https://github.com/stlab/libraries/issues/26) : channel TODO get process state under mutex?
   - [#18](https://github.com/stlab/libraries/issues/18) : future TODO `get_try()`
- Library Additions
    - Concurrency library
        - [serial queue](stlab/concurrency/serial_queue.hpp)
        - [task](stlab/concurrency/task.hpp)
        - Added feature requests [#95](https://github.com/stlab/libraries/issues/95) (flatten futures) and [#38](https://github.com/stlab/libraries/issues/38) (reduction on `future<void>`). Reduction of futures is now supported. So a `future<future<T>>` is reduced to `future<T>`. Currently
this is restricted to copyable types `T`.
    - [copy on write](stlab/copy_on_write.hpp)
    - [scope](stlab/scope.hpp)
    - [Test library](stlab/test/model.hpp)
        - annotate
        - regular
        - move-only
- Build & Test Improvements
    - CMake overhaul
    - Extended Travis CI support
    - AppVeyor support
- Breaking Changes
    - With [Issue #85](https://github.com/stlab/libraries/issues/85) the interface of `make_ready_future` and 
`make_exceptional_future` now require an executor as a second parameter.

## v1.0 - 10 June 2017

- Added the concurrency library:
    - futures
    - executors
    - channels