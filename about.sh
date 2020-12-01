#!/bin/bash
set -x

cd `dirname $0`

if [ ! -d _data ]; then
    mkdir _data
fi

cd _data

curl -L https://api.github.com/repos/stlab/libraries/contributors -o contributors.json
curl -L https://api.github.com/repos/stlab/libraries/releases -o releases.json
curl -L https://api.github.com/repos/stlab/libraries/tags -o tags.json
