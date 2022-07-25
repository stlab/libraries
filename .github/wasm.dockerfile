FROM emscripten/emsdk

# Firefox required for `emrun`.
# Might need to change this to a nightly build.
# See https://emscripten.org/docs/porting/pthreads.html#running-code-and-tests
RUN apt update -y \
  && apt install firefox -y

RUN apt-get update -y \
  && apt-get install -y ninja-build \
  # Firefox dependencies? Might remove.
  && apt-get install -y libpci-dev \
  && apt-get install -y libgl1-mesa-dev \
  && apt-get remove --purge cmake -y \
  # cmake dependencies per [LINK]
  && apt-get install build-essential libssl-dev wget -y \
  && wget https://github.com/Kitware/CMake/releases/download/v3.23.2/cmake-3.23.2-linux-x86_64.sh \
  && /bin/bash cmake-3.23.2-linux-x86_64.sh --prefix=/usr/local/ --exclude-subdir

WORKDIR /install

RUN git clone --depth 1 --recurse-submodules -j8 https://github.com/boostorg/boost.git

WORKDIR /build/boost-wasm/

RUN emcmake cmake \
  -GNinja \
  -DCMAKE_CXX_FLAGS="-pthread" \
  -DCMAKE_BUILD_TYPE=Release \
  -DCMAKE_INSTALL_PREFIX=/emsdk/upstream/emscripten/cache/sysroot/$HOME/build/boost-release-emscripten-install \
  -DBOOST_EXCLUDE_LIBRARIES="fiber;beast;context;coroutine;asio;log" \
  /install/boost/

RUN ninja -v

RUN ninja install

COPY . /src

WORKDIR /build/stlab

RUN emcmake cmake \
  -GNinja \
  -DBOOST_ROOT=$HOME/build/boost-release-emscripten-install \
  -DCMAKE_CXX_STANDARD=23 \
  -DCMAKE_BUILD_TYPE=Release \
  -DSTLAB_NO_STD_COROUTINES=TRUE \
  -DCMAKE_CXX_FLAGS="-pthread" \
  -DCMAKE_EXE_LINKER_FLAGS="-pthread" \
  /src

RUN ninja -j4 -v

# Suppress an exception thrown by emrun trying to find a favicon.
COPY ./assets/favicon.ico /build/stlab/test/

RUN echo "#!/bin/bash\nemrun --verbose --kill_exit --browser_args=\"--headless\" test/stlab.test.\$1.html" >> runner.sh \
  && chmod +x runner.sh

# Usage: `docker run <this-image's-name> channel`
ENTRYPOINT ["./runner.sh"]
