# `stlab` Documentation

Pull requests for typos, examples, and other improvements are welcome. To file an issue, please use the [libraries repository](https://github.com/stlab/libraries).

# Branch states

\[ These are from the old site - FIX ME \]

- **`master`:** [![master build](https://travis-ci.org/stlab/stlab.github.io.svg?branch=master)](https://travis-ci.org/stlab/stlab.github.io) [![master coverage](https://codecov.io/github/stlab/stlab.github.io/coverage.svg?branch=master)](https://codecov.io/gh/stlab/stlab.github.io/branch/master)

- **`develop`:** [![develop build](https://travis-ci.org/stlab/stlab.github.io.svg?branch=develop)](https://travis-ci.org/stlab/stlab.github.io)
[![develop coverage](https://codecov.io/github/stlab/stlab.github.io/coverage.svg?branch=develop)](https://codecov.io/gh/stlab/stlab.github.io/branch/develop)

# Building (Mac)

You'll need Homebrew to make sure you have the most recent version of Ruby.

## Setup
1. Clone the repo
2. `brew install ruby`
3. `sudo gem install bundle jekyll github-pages liquid`


Periodically run `gem update` and `bundle update` to make sure you have the latest jekyll tooling.

## Building Docs
1. `cd` to repo directory
2. `bundle exec jekyll serve --watch`
3. Documentation viewable at `localhost:4000`
4. Modifying sources should auto-regenerate the documentation

## Building Examples
1. `./build.sh` will download dependencies, build, and run all the `*.cpp` files in the `libraries` directory.
