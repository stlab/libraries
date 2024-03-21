cmake_minimum_required(VERSION 3.25)

include(FetchContent)

FetchContent_Declare(
    Corrosion
    GIT_REPOSITORY https://github.com/corrosion-rs/corrosion.git
    GIT_TAG master # Needed for experimental feature `corrosion_experimental_cbindgen`.
)

# Set any global configuration variables such as `Rust_TOOLCHAIN` before this line!

FetchContent_MakeAvailable(Corrosion)

corrosion_import_crate(MANIFEST_PATH ./Cargo.toml)

corrosion_experimental_cbindgen(
        TARGET default_executor
        HEADER_NAME "bindings.h"
)

add_library(RustyDefaultExecutor INTERFACE)

target_include_directories(RustyDefaultExecutor
    # PRIVATE
    #     # where the library itself will look for its internal headers
    #     ${CMAKE_CURRENT_SOURCE_DIR}/src
    INTERFACE
        # where top-level project will look for the library's public headers
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/cppshim/include>
)

target_link_libraries(RustyDefaultExecutor INTERFACE default_executor)
