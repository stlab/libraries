target_sources( stlab INTERFACE
  FILE_SET stlab
  TYPE HEADERS
  BASE_DIRS ../
  FILES
    algorithm/reverse.hpp

    concurrency/config.hpp
    concurrency/default_executor.hpp
    concurrency/executor_base.hpp
    concurrency/future.hpp
    concurrency/immediate_executor.hpp
    concurrency/main_executor.hpp
    concurrency/optional.hpp
    concurrency/progress.hpp
    concurrency/system_timer.hpp
    concurrency/task.hpp
    concurrency/traits.hpp
    concurrency/tuple_algorithm.hpp
    concurrency/utility.hpp
    concurrency/variant.hpp

    iterator/set_next.hpp

    test/model.hpp

    copy_on_write.hpp
    enum_ops.hpp
    forest_algorithms.hpp
    forest.hpp
    functional.hpp
    memory.hpp
    scope.hpp
    utility.hpp
    version.hpp
)

if(CXX_COMPILER_ID MATCHES MSVC)
  if(CXX_COMPILER_VERSION VERSION_LESS 99)
    get_target_property(wants_coroutines stlab COROUTINES)
    if(wants_coroutines)
      message(STATUS "Not including concurrency/channel.hpp in the target sources for stlab")
      return()
    endif()
  endif()
endif()

target_sources( stlab INTERFACE
  FILE_SET stlab
  TYPE HEADERS
  BASE_DIRS ../
  FILES concurrency/channel.hpp
)
