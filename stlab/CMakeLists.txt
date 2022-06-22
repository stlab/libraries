target_sources( stlab INTERFACE
  FILE_SET stlab
  TYPE HEADERS
  BASE_DIRS ../
  FILES
    algorithm/reverse.hpp

    concurrency/channel.hpp
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

stlab_generate_config_file()

target_sources( stlab INTERFACE
  FILE_SET stlab
  TYPE HEADERS
  BASE_DIRS ${CMAKE_BINARY_DIR}
  FILES
    ${CMAKE_BINARY_DIR}/stlab/config.hpp
)
