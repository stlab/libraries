set( stlab_base_flags "-Wall;-ftemplate-backtrace-limit=0;" )
set( stlab_debug_flags "-gdwarf-3;" )
set( stlab_coverage_flags "-fprofile-arcs;-ftest-coverage;" )
set( stlab_release_flags "" )
# gcc version < 7 has a bug in static constexpr members and reports tons of errors/warnings.
# By using -fpermissive and -w is the only way to shut the compiler up
# Remove when we remove to C++17
set( stlab_interface_flags "-std=gnu++14;-fpermissive;-w")
