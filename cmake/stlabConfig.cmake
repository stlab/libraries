include( CMakeFindDependencyMacro )
find_dependency( Boost 1.60.0 REQUIRED COMPONENTS unit_test_framework )
find_package(Threads REQUIRED)
include( "${CMAKE_CURRENT_LIST_DIR}/stlabTargets.cmake" )
