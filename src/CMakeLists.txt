project(future C CXX)

include_directories("${PROJECT_SOURCE_DIR}/..")

set(SOURCE
  default_executor.cpp
)

set(HEADERS
  ../stlab/channel.hpp
  ../stlab/config.hpp
  ../stlab/default_executor.hpp
  ../stlab/executor_base.hpp
  ../stlab/expected.hpp
  ../stlab/future.hpp
  ../stlab/immediate_executor.hpp
  ../stlab/main_executor.hpp
  ../stlab/progress.hpp
  ../stlab/tuple_algorithm.hpp
  ../stlab/utility.hpp  )

include_directories(${Boost_INCLUDE_DIRS})

add_library(future ${SOURCE} ${HEADERS})
set_property(TARGET future PROPERTY CXX_STANDARD 14)
set_property(TARGET future PROPERTY CXX_STANDARD_REQUIRED ON)

