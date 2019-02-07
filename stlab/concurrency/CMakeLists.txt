target_sources( stlab INTERFACE
  $<BUILD_INTERFACE:
    ${CMAKE_CURRENT_SOURCE_DIR}/config.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/default_executor.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/executor_base.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/future.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/immediate_executor.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/main_executor.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/optional.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/progress.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/system_timer.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/task.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/traits.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/tuple_algorithm.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/utility.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/variant.hpp>
  $<INSTALL_INTERFACE:
    include/stlab/concurrency/channel.hpp
    include/stlab/concurrency/config.hpp
    include/stlab/concurrency/default_executor.hpp
    include/stlab/concurrency/executor_base.hpp
    include/stlab/concurrency/future.hpp
    include/stlab/concurrency/immediate_executor.hpp
    include/stlab/concurrency/main_executor.hpp
    include/stlab/concurrency/optional.hpp
    include/stlab/concurrency/progress.hpp
    include/stlab/concurrency/system_timer.hpp
    include/stlab/concurrency/task.hpp
    include/stlab/concurrency/traits.hpp
    include/stlab/concurrency/tuple_algorithm.hpp
    include/stlab/concurrency/utility.hpp
    include/stlab/concurrency/variant.hpp>)

if(CXX_COMPILER_ID MATCHES MSVC)
  if(CXX_COMPILER_VERSION VERSION_LESS 99)
    get_target_property(wants_coroutines stlab COROUTINES)
    if(wants_coroutines)
      message(STATUS "Not including concurrency/channel.hpp in the target sources for stlab")
      return()
    endif()
  endif()
endif()

target_sources(stlab INTERFACE
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/channel.hpp>
  $<INSTALL_INTERFACE:include/stlab/concurrency/channel.hpp>)
