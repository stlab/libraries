# Version 1.1 2017-11-28

## Fixed Issues
The following issues are fixed in this release:
* [Use of moved executor in futures.hpp:1234](https://github.com/stlab/libraries/issues/94)
* [Stack overflow with exception handling in futures](https://github.com/stlab/libraries/issues/93)
* [(non-default) Executors and make_ready_future](https://github.com/stlab/libraries/issues/85)
* [stlab::async will not accept non-const reference parameters](https://github.com/stlab/libraries/issues/80)
* [when_all does not support continuing futures that return void](https://github.com/stlab/libraries/issues/79)
* [Support move-only tasks](https://github.com/stlab/libraries/issues/69)
* [task_system and notification_queue doesn't work with move only lambda](https://github.com/stlab/libraries/issues/59)
* [channel TODO get process state under mutex?](https://github.com/stlab/libraries/issues/26)
* [future TODO get_try()](https://github.com/stlab/libraries/issues/18)

## New Functionality
The following enhancements are part of this release:
* [Feature request: flatten futures or then_f.](https://github.com/stlab/libraries/issues/95) and 
[future TODO: reduction on future<void>](https://github.com/stlab/libraries/issues/38)
Now reduction of futures is supported. So a future<future<T>> is reduced to future<T>. Currently
this is restricted to copyable types T.
* A [serial_queue](https://github.com/stlab/libraries/blob/master/stlab/concurrency/serial_queue.hpp) 
and a [scope](https://github.com/stlab/libraries/blob/master/stlab/scope.hpp) were added as new concurrency elements.
* A [copy_on_write](https://github.com/stlab/libraries/blob/master/stlab/copy_on_write.hpp) template class is now part of the library.
* A [for_each_argument](https://github.com/stlab/libraries/blob/master/stlab/utility.hpp) helper function is now part of the library.
* A suite of archetype test classes [regular, move_only, and annotate](https://github.com/stlab/libraries/blob/master/stlab/test/model.hpp) were added as well.

## Breaking Changes
The following breaking change was necessary.
* With [Issue #85](https://github.com/stlab/libraries/issues/85) the interface of make_ready_future and 
make_exceptional_future now requires as second parameter an executor.