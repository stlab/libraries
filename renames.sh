#!/bin/bash

OLD=docs/libraries/concurrency/channel
NEW=docs/libraries/concurrency/channel.hpp

mkdir -p $NEW

move() {
    git mv $OLD/$1.md $NEW/$2.md
}

# argument_of does not exist in the old docs.

mkdir -p $NEW/buffer_size

move receiver/buffer_size/buffer_size buffer_size/m_buffer_size
move receiver/buffer_size/index buffer_size/index

# channel_error does not exist in the old docs

# first_ does not exist in the old docs

# function_process does not exist in the old docs

# identity does not exist in the old docs

# receiver3CT3E/...

mkdir -p $NEW/receiver3CT3E

move receiver/index receiver3CT3E/index
git mv $OLD/receiver/operator_pipe_example.cpp $NEW/receiver3CT3E/operator_pipe_example.cpp
move receiver/operator_pipe receiver3CT3E/m_operator7C
move receiver/ready receiver3CT3E/m_ready
git mv $OLD/receiver/set_ready_example.cpp $NEW/receiver3CT3E/set_ready_example.cpp
move receiver/set_ready receiver3CT3E/set_ready

# result_of does not exist in the old docs.

# round_robin_t does not exist in the old docs

# Moving all old sender stuff into the copyable variant, arbitrarily.
COPYABLE_SENDER=sender3CT2C20enable_if_.a01d6424

mkdir -p $NEW/$COPYABLE_SENDER

git mv $OLD/sender/call_operator_example.cpp $NEW/$COPYABLE_SENDER/call_operator_example.cpp
git mv $OLD/sender/close_example.cpp $NEW/$COPYABLE_SENDER/close_example.cpp
move sender/close $COPYABLE_SENDER/m_close
move sender/index $COPYABLE_SENDER/index
move "sender/operator()" $COPYABLE_SENDER/m_operator2829

# unordered_t does not exist in the old docs.

move channel f_channel
# for_each_n does not exist in the old docs
move join f_join
move merge_channel f_merge_channel
move merge f_merge
move zip_with f_zip_with
move zip f_zip
move index index
