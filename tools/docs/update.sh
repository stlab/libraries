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
    -r|--ruby-version)
      if [ -n "$2" ] && [ ${2:0:1} != "-" ]; then
        RUBY_VERSION=$2
        shift 2
      else
        echo "Error: Argument for $1 is missing" >&2
        exit 1
      fi
    ;;
    *)    # unknown option
        POSITIONAL+=("$1") # save it in an array for later
        shift # past argument
    ;;
esac
done
set -- "${POSITIONAL[@]}" # restore positional parameters

if [ -z ${RUBY_VERSION+x} ]; then
    rbenv local $(rbenv install -l | grep -v - | tail -1)
else
    rbenv install $RUBY_VERSION
    rbenv local $RUBY_VERSION
fi

gem install bundler
rbenv rehash

rm ./Gemfile.lock

if [[ $LOCK = YES ]]; then
    bundle lock --update
else
    bundle update
fi
