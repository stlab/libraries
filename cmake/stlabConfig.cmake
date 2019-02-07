include( CMakeFindDependencyMacro )
find_dependency(Boost 1.60.0)
find_dependency(Threads)
include( "${CMAKE_CURRENT_LIST_DIR}/stlabTargets.cmake" )
