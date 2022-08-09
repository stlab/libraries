#!/bin/bash

OLD=docs/libraries/concurrency/task
NEW=docs/libraries/concurrency/task.hpp
NEW_CLASS=task3CR2028Args293E

mkdir -p $NEW/$NEW_CLASS

move() {
    EXTENSION=${3:-md}  
    git mv $OLD/$1.$EXTENSION $NEW/$2.$EXTENSION
}

move index index
move operator!= f_operator213D
move "operator()" m_operator2829
move operator== m_operator3D3D
move swap m_swap

move task/index $NEW_CLASS/index
move task/operator_bool $NEW_CLASS/m_operator20bool
move task/"operator=" $NEW_CLASS/m_operator3D
move task/swap $NEW_CLASS/f_swap
move task/target_type $NEW_CLASS/m_target_type
move task/target $NEW_CLASS/m_target
move task/task $NEW_CLASS/m_$NEW_CLASS
