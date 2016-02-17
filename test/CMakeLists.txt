project(future_test)

if(WIN32)
  add_definitions(-D_WIN32_WINNT=0x0501)
ENDIF(WIN32)

set(SOURCE
  main.cpp
)

include_directories("${PROJECT_SOURCE_DIR}/..")

add_executable(future_test ${SOURCE} ${HEADERS})

include_directories(${Boost_INCLUDE_DIRS})
LINK_DIRECTORIES(${Boost_LIBRARY_DIRS})
add_definitions(-DBOOST_ALL_NO_LIB)

add_dependencies(future_test future)

set_property(TARGET future_test PROPERTY CXX_STANDARD 14)
set_property(TARGET future_test PROPERTY CXX_STANDARD_REQUIRED ON)
target_link_libraries(future_test ${Boost_LIBRARIES} future)


