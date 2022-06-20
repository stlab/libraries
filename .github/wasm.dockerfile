FROM emscripten/emsdk

RUN apt remove --purge cmake -y && \ 
    pip install cmake --upgrade

RUN cd /home && wget https://boostorg.jfrog.io/artifactory/main/release/1.79.0/source/boost_1_79_0.tar.gz \
  && tar xfz boost_1_79_0.tar.gz \
  && rm boost_1_79_0.tar.gz \
  && cd boost_1_79_0 \
  && ./bootstrap.sh --prefix=/home/boost-wasm/ --with-libraries=test \
  && ./b2 install --prefix=/home/boost-wasm/ toolset=emscripten link=static variant=release threading=single runtime-link=static \
  && cd /home \
  && rm -rf boost_1_79_0

WORKDIR /home/boost-wasm/lib

RUN emar q libboost_prg_exec_monitor.a libboost_prg_exec_monitor.bc \
    && emar q libboost_test_exec_monitor.a libboost_test_exec_monitor.bc \
    && emar q libboost_unit_test_framework.a libboost_unit_test_framework.bc

COPY . /src

WORKDIR /build

RUN emcmake cmake --trace /src -DBOOST_ROOT=/home/boost-wasm
