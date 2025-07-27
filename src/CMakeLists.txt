target_sources(stlab PRIVATE pre_exit.cpp concurrency/default_executor.cpp)
target_include_directories(stlab PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../include ${PROJECT_BINARY_DIR}/include)
target_compile_definitions(stlab PRIVATE $<$<CXX_COMPILER_ID:MSVC>:NOMINMAX>)

if(BUILD_SHARED_LIBS)
    set_target_properties(stlab PROPERTIES WINDOWS_EXPORT_ALL_SYMBOLS ON)
endif()
