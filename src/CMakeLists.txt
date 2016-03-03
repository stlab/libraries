project(future C CXX)

include_directories("${PROJECT_SOURCE_DIR}/..")

if(WIN32)
  add_definitions(-D_WIN32_WINNT=0x0501)
endif(WIN32)

set(SOURCE
  future.cpp
)

set(HEADERS
  ../stlab/config.hpp
  ../stlab/channel.hpp
  ../stlab/expected.hpp
  ../stlab/future.hpp
)

include_directories(${Boost_INCLUDE_DIRS})

add_library(future ${SOURCE} ${HEADERS})
set_property(TARGET future PROPERTY CXX_STANDARD 14)
set_property(TARGET future PROPERTY CXX_STANDARD_REQUIRED ON)

