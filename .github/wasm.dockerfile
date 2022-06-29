FROM emscripten/emsdk

# Hi, grepper. Boost Dependency: 1.74.0
ARG BOOST_MAJOR=1
ARG BOOST_MINOR=74
ARG BOOST_PATCH=0

WORKDIR /installdir

# Install cmake 3.23 from Kitware directly.
# Note that snap is difficult to get working inside docker,
# and as of this writing, `pip install cmake` installs 3.22.
RUN apt-get update -y \
  && apt-get upgrade -y \
  && apt-get remove --purge cmake -y \
  # Install cmake dependencies.
  && apt-get install build-essential libssl-dev wget -y \
  && wget https://github.com/Kitware/CMake/releases/download/v3.23.2/cmake-3.23.2-linux-x86_64.sh \
  && /bin/bash cmake-3.23.2-linux-x86_64.sh --prefix=/usr/local/ --exclude-subdir

# Required for `emrun`.
# Might need to change this to a nightly build.
# See https://emscripten.org/docs/porting/pthreads.html#running-code-and-tests
RUN apt install firefox -y

WORKDIR /build/boost-wasm

RUN wget https://boostorg.jfrog.io/artifactory/main/release/${BOOST_MAJOR}.${BOOST_MINOR}.${BOOST_PATCH}/source/boost_${BOOST_MAJOR}_${BOOST_MINOR}_${BOOST_PATCH}.tar.gz \
    && tar xfz boost_${BOOST_MAJOR}_${BOOST_MINOR}_${BOOST_PATCH}.tar.gz \
    && rm boost_${BOOST_MAJOR}_${BOOST_MINOR}_${BOOST_PATCH}.tar.gz

WORKDIR /build/boost-wasm/boost_${BOOST_MAJOR}_${BOOST_MINOR}_${BOOST_PATCH}

RUN ./bootstrap.sh

RUN ./b2 \
    -q \
    link=static \
    toolset=emscripten \
    variant=release \
    threading=single \
    cxxflags="-pthread" \
    # This is only required for Boost < 1.79.0.
    # See https://github.com/boostorg/build/commit/003a3c29c12427c5a424f2332aa4ba00a8554a88
    archiveflags="-r" \
    # Find other libraries with `./b2 --show-libraries`.
    --with-test \
    install

COPY . /src

WORKDIR /build

RUN emmake cmake \
  -DEMSCRIPTEN=YES \
  -DCMAKE_CXX_STANDARD=17 \
  -DSTLAB_NO_STD_COROUTINES=TRUE \
  -DSTLAB_THREAD_SYSTEM=pthread-emscripten \
  -DSTLAB_TASK_SYSTEM=emscripten \
  -DSTLAB_MAIN_EXECUTOR=emscripten \
  -DBoost_USE_STATIC_LIBS=YES \
  /src

RUN VERBOSE=1 emmake cmake --build .

# CMD emrun --kill_exit --browser_args="--headless" /build/Stlab.html
