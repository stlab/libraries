project(future C CXX)

include_directories("${PROJECT_SOURCE_DIR}/..")

#set(SOURCE
#)

set(HEADERS
  ../stlab/concurrency/channel.hpp
  ../stlab/concurrency/config.hpp
  ../stlab/concurrency/default_executor.hpp
  ../stlab/concurrency/executor_base.hpp
  ../stlab/concurrency/future.hpp
  ../stlab/concurrency/immediate_executor.hpp
  ../stlab/concurrency/main_executor.hpp
  ../stlab/concurrency/progress.hpp
  ../stlab/concurrency/system_timer.hpp
  ../stlab/concurrency/traits.hpp
  ../stlab/concurrency/tuple_algorithm.hpp
  ../stlab/concurrency/utility.hpp)

include_directories(${Boost_INCLUDE_DIRS})
