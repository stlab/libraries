include( CMakeFindDependencyMacro )
find_dependency( Boost 1.60.0 REQUIRED COMPONENTS unit_test_framework )
include( "${CMAKE_CURRENT_LIST_DIR}/stlabTargets.cmake" )
