add_executable( stlab.playground main.cpp )

target_link_libraries( stlab.playground
                       PUBLIC ${Boost_LIBRARIES} 
                       PUBLIC stlab )

set_property( TARGET stlab.playground PROPERTY CXX_STANDARD 14 )
set_property( TARGET stlab.playground PROPERTY CXX_STANDARD_REQUIRED ON )
