PROJECT(playground)

set(SOURCE
  main.cpp
)

set(HEADERS
)

add_executable(playground ${SOURCE} ${HEADERS})

include_directories(${Boost_INCLUDE_DIRS})

target_link_libraries(
    playground
    ${Boost_LIBRARIES} 
    ${CMAKE_THREAD_LIBS_INIT} 
    ${CONAN_LIBS})


set_property(TARGET playground PROPERTY CXX_STANDARD 14)
set_property(TARGET playground PROPERTY CXX_STANDARD_REQUIRED ON)
