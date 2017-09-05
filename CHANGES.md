## v1.1 - 09 Sept 2017

- Library Additions
    - serial queue (concurrency library)
    - task (concurrency library)
    - copy on write
    - scope
    - Test library
        - annotate
        - regular
        - move-only
- Build & Test
    - CMake overhaul
    - Extended Travis CI support
    - AppVeyor support
- Issues
    - [#80](https://github.com/stlab/libraries/issues/80): `stlab::async` will not accept non-const reference parameters
    - [#79](https://github.com/stlab/libraries/issues/79) `when_all` does not support continuing futures that return `void`
    - [#69](https://github.com/stlab/libraries/issues/69) Support move-only tasks
    - [#59](https://github.com/stlab/libraries/issues/59) `task_system` and `notification_queue` doesn't work with move only lambda

## v1.0 - 10 June 2017

- Added the concurrency library:
    - futures
    - executors
    - channels