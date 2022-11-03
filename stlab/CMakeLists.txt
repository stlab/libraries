target_sources( stlab INTERFACE
  FILE_SET stlab
  TYPE HEADERS
  BASE_DIRS ../
  FILES
    algorithm/reverse.hpp

    concurrency/await.hpp
    concurrency/channel.hpp
    concurrency/concurrency.hpp
    concurrency/default_executor.hpp
    concurrency/executor_base.hpp
    concurrency/future.hpp
    concurrency/immediate_executor.hpp
    concurrency/main_executor.hpp
    concurrency/optional.hpp
    concurrency/progress.hpp
    concurrency/ready_future.hpp
    concurrency/serial_queue.hpp
    concurrency/set_current_thread_name.hpp
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
    forest.hpp
    forest_algorithms.hpp
    functional.hpp
    memory.hpp
    pre_exit.hpp
    scope.hpp
    utility.hpp
    version.hpp
)

stlab_generate_config_file()

target_sources( stlab INTERFACE
  FILE_SET stlab
  TYPE HEADERS
  BASE_DIRS ${PROJECT_BINARY_DIR}
  FILES
    ${PROJECT_BINARY_DIR}/stlab/config.hpp
)
