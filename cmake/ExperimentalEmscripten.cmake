#
# This toolchain file extends Emscripten.cmake provided by the Emscripten SDK,
# and sets the minimum options required to run the stlab test executables with
# ctest (using a node runner).
#

#
# Find the Emscripten SDK and include its CMake toolchain.
#
find_program(EMSDK_EXECUTABLE emsdk)
if (EMSDK_EXECUTABLE)
    get_filename_component(EMSDK_ROOT ${EMSDK_EXECUTABLE} DIRECTORY)

    message(TRACE "Appending ${EMSDK_ROOT}/upstream/emscripten/cmake/Modules/Platform to CMAKE_MODULE_PATH")
    list(APPEND CMAKE_MODULE_PATH "${EMSDK_ROOT}/upstream/emscripten/cmake/Modules/Platform")

    include (Emscripten)
else()
    message(FATAL_ERROR "Could not find emsdk installation. Please install the Emscripten SDK.\nhttps://emscripten.org/docs/getting_started/downloads.html ")
endif()

#
# Explanation of flags used:
#
# `-pthread`
# STLab uses threads. Without this, the tests will not compile.
#
# `-fwasm-exceptions`:
# STLab uses exceptions. Without this, the tests error out with:
#
#   Pthread 0x005141d0 sent an error! http://localhost:6931/<throwing test>: uncaught exception: 10570976 \
#   - Exception catching is disabled, this exception cannot be caught.
#
# `-sSUPPORT_LONGJMP=wasm`
# Enables experimental support for LONGJMP in functions which may throw exceptions.
# Without this, boost would not compile (LLVM errored out). 
#
# `-sEXIT_RUNTIME=1`
# Indicates the runtime environment (node) should exit when our main() returns.
#
# `-sINITIAL_MEMORY=300MB` 
# Without this, the tests OOM. The first sign is an error out with the following:
#
#   Pthread 0x0058faf8 sent an error! http://localhost:6931/<test>: RuntimeError: unreachable executed
#
# If we run the problematic test in a browser with `emrun`, we can see some javascript errors that explain:
#
#   Aborted(Cannot enlarge memory arrays to size 17457152 bytes (OOM). Either
#   (1) compile with -sINITIAL_MEMORY=X with X higher than the current value 16777216, 
#   (2) compile with -sALLOW_MEMORY_GROWTH which allows increasing the size at runtime, or 
#   (3) if you want malloc to return  NULL (0) instead of this abort, compile with -sABORTING_MALLOC=0)
#
# Note that (2) is not an option because pthread cannot yet be combined with -sALLOW_MEMORY_GROWTH:
# See https://github.com/WebAssembly/design/issues/1271
# Smaller values (150MB, 200MB) produce intermittent failures. 300MB was chosen to give enough headroom for
# tests written in the future. 
#
# `-sPTHREAD_POOL_SIZE=32` 
# Without this, the tests deadlock. Lower values were tested. 
# 8 threads deadlocked consistently, 16 threads passed consistently. 
# 32 was chosen to give enough headroom for tests written in the future. 
#
# `-sPROXY_TO_PTHREAD`
# This flag wraps our executable's main function in a pthread. 
# Without this, we exhaust the thread pool very quickly. The error looks like this:
#
#   Tried to spawn a new thread, but the thread pool is exhausted.
#   This might result in a deadlock unless some threads eventually exit or the code explicitly breaks out to the event loop.
# 
# You can read more about the setting here: https://emscripten.org/docs/porting/pthreads.html#blocking-on-the-main-browser-thread
#

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread -fwasm-exceptions -sSUPPORT_LONGJMP=wasm")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -pthread -fwasm-exceptions -sEXIT_RUNTIME=1 -sINITIAL_MEMORY=300MB -sPTHREAD_POOL_SIZE=32 -sPROXY_TO_PTHREAD")
set(STLAB_WASM_NODE_JS_MIN_VERSION "v16.16.0")

#
# Note: https://www.npmjs.com/package/wasm-check is a useful utility to find 
# which --experimental-wasm-xxx flags are supported by node.
#
set(STLAB_REQUIRED_WASM_EXPERIMENTAL_FLAGS "--experimental-wasm-threads;--experimental-wasm-eh")

#
# Check if NODE_JS_EXECUTABLE (found by find_program() in Emscripten.cmake) is recent enough for STLab.
# Set CMAKE_CROSSCOMPILING_EMULATOR to a sufficiently recent node + required experimental flags.
#
if (NODE_JS_EXECUTABLE)
    message(STATUS "stlab:wasm: Ensuring ${NODE_JS_EXECUTABLE} is at least ${STLAB_WASM_NODE_JS_MIN_VERSION}...")
    execute_process( COMMAND ${NODE_JS_EXECUTABLE} --version OUTPUT_VARIABLE NODE_JS_EXECUTABLE_VERSION OUTPUT_STRIP_TRAILING_WHITESPACE)

    if (NODE_JS_EXECUTABLE_VERSION VERSION_GREATER_EQUAL ${STLAB_WASM_NODE_JS_MIN_VERSION})
        message(STATUS "stlab:wasm: Installed node satisfies requirements: ${NODE_JS_EXECUTABLE_VERSION}")
        set(CMAKE_CROSSCOMPILING_EMULATOR "${NODE_JS_EXECUTABLE};${STLAB_REQUIRED_WASM_EXPERIMENTAL_FLAGS}")
    else()
        message(FATAL_ERROR "stlab:wasm: Unsupported node: ${NODE_JS_EXECUTABLE_VERSION}. Please install ${STLAB_WASM_NODE_JS_MIN_VERSION} or newer.")
    endif()
else()
    message(FATAL_ERROR "stlab:wasm: Unable to find node. Please install ${STLAB_WASM_NODE_JS_MIN_VERSION} or newer.")
endif()

#
# Emscripten supports dynamic linking, but doing so introduces some complexity:
# https://emscripten.org/docs/compiling/Dynamic-Linking.html
# We presently have no need to dynamically link WASM modules, so we instruct
# boost to link statically.
# 
# It would be nice to set a more generic variable here - like (BUILD_SHARED_LIBS OFF)
# which any library could check to set their own corresponding variable.
# But, Boost doesn't consider BUILD_SHARED_LIBS when setting this default.
#
set(Boost_USE_STATIC_LIBS ON)
