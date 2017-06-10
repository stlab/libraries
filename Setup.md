Quick setup instruction
=======================

* checkout the code with git clone https://github.com/stlab/libraries
* Have a recent cmake in your path
* Have at least boost 1.60.0 with library test build
* Linux / MacOS
** mkdir stlab_build
* Windows 
** md stlab_build
* cd stlab_build
* cmake -DBOOST_ROOT=<path to the boost folder> ../libraries
* make
* ./test/future_test

