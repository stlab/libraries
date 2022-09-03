#!/bin/bash

POSITIONAL=()
OPTIONS=""
while [[ $# -gt 0 ]]
do
key="$1"

case $key in
    -l|--lock)
        LOCK=YES
        shift # past argument
    ;;
    *)    # unknown option
        POSITIONAL+=("$1") # save it in an array for later
        shift # past argument
    ;;
esac
done
set -- "${POSITIONAL[@]}" # restore positional parameters

cd ./docs

rm ./Gemfile.lock

if [[ $LOCK = YES ]]; then
    bundle lock --update
else
    bundle update
fi

# git submodule update --recursive --remote
