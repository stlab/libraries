#!/bin/bash
set -x

cd `dirname $0`

if [ ! -d _data ]; then
    mkdir _data
fi

cd _data

curl -L https://api.github.com/repos/stlab/stlab/contributors -o contributors.json
curl -L https://api.github.com/repos/stlab/stlab/releases -o releases.json
curl -L https://api.github.com/repos/stlab/stlab/tags -o tags.json
