#!/bin/bash

OLD=docs/libraries/concurrency/future
NEW=docs/libraries/concurrency/future.hpp
NEW_READY_FUTURE=docs/libraries/concurrency/ready_future.hpp

mkdir -p $NEW/packaged_task
mkdir -p $NEW_READY_FUTURE

move() {
    EXTENSION=${3:-md}  
    git mv $OLD/$1.$EXTENSION $NEW/$2.$EXTENSION
}

move async_example async_example cpp
move async f_async
# blocking_get has been moved to await.hpp
move index index
git mv $OLD/make_exceptional_future.md $NEW_READY_FUTURE/f_make_exceptional_future.md
git mv $OLD/make_ready_future.md $NEW_READY_FUTURE/f_make_ready_future.md
move package_example packaged_task/package_example cpp
move package packaged_task/f_package
move when_all_example when_all_example cpp
move when_all_void_example when_all_void_example cpp
move when_all f_when_all
move when_any_example when_any_example cpp
move when_any_void_example when_any_void_example cpp
move when_any f_when_any

# Arbitrarily copy the existing future docs into the copyable-T-variant
COPYABLE_T_FUTURE=future3CT2C20enable_if_.e055d50a

mkdir -p $NEW/$COPYABLE_T_FUTURE

move future/detach_example $COPYABLE_T_FUTURE/detach_example cpp
move future/detach $COPYABLE_T_FUTURE/m_detach
move future/error $COPYABLE_T_FUTURE/m_error
move future/exception $COPYABLE_T_FUTURE/m_exception
move future/get_try $COPYABLE_T_FUTURE/m_get_try
move future/index $COPYABLE_T_FUTURE/index
move future/is_ready $COPYABLE_T_FUTURE/m_is_ready
move future/operator_co_await_example $COPYABLE_T_FUTURE/operator_co_await_example cpp
# Hyde isn't generating docs for operator co_await, likely because our language version is < 20
move future/operator_pipe $COPYABLE_T_FUTURE/m_operator7C
move future/operator_xor $COPYABLE_T_FUTURE/m_operator5E
move future/recover_example $COPYABLE_T_FUTURE/recover_example cpp
move future/recover $COPYABLE_T_FUTURE/m_recover
move future/reset $COPYABLE_T_FUTURE/m_reset
move future/then_continuation_example $COPYABLE_T_FUTURE/then_continuation_example cpp
move future/then_reduction_example $COPYABLE_T_FUTURE/then_reduction_example cpp
move future/then_split_example $COPYABLE_T_FUTURE/then_split_example cpp
move future/then $COPYABLE_T_FUTURE/m_then
move future/valid $COPYABLE_T_FUTURE/m_valid
