project(future C CXX)

include_directories("${PROJECT_SOURCE_DIR}/..")

set(SOURCE
  future.cpp
)

set(HEADERS
  ../stlab/config.hpp
  ../stlab/channel.hpp
  ../stlab/expected.hpp
  ../stlab/future.hpp
  ../stlab/progress.hpp
  ../stlab/tuple_algorithm.hpp
)

include_directories(${Boost_INCLUDE_DIRS})

add_library(future ${SOURCE} ${HEADERS})
set_property(TARGET future PROPERTY CXX_STANDARD 14)
set_property(TARGET future PROPERTY CXX_STANDARD_REQUIRED ON)

