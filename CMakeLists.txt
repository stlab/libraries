cmake_minimum_required( VERSION 3.8 )
set( CMAKE_CONFIGURATION_TYPES "Debug;Release" CACHE STRING "Supported configuration types" FORCE )

project( stlab VERSION 1.0.0 LANGUAGES C CXX )

################################################################################              
# Options
################################################################################
option( coverage "Enable binary instrumentation to collect test coverage information in the DEBUG configuration" )
option( stlab_testing "Compile the stlab tests and integrate with ctest" ${BUILD_TESTING} )


################################################################################              
# Library
################################################################################
add_library( stlab INTERFACE )
target_include_directories(stlab INTERFACE
        $<BUILD_INTERFACE:${stlab_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>)
set( CMAKE_THREAD_PREFER_PTHREAD TRUE )
find_package( Threads )
target_link_libraries( stlab INTERFACE Threads::Threads ${CONAN_LIBS} )
target_compile_features( stlab INTERFACE cxx_std_14 )


################################################################################              
# Flags
################################################################################
if (MSVC)
  target_compile_options( stlab INTERFACE "-D_WIN32_WINNT=0x0601 -DNOMINMAX" )
endif()


################################################################################              
# Installation
################################################################################
install(TARGETS stlab EXPORT stlabTargets
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib
        RUNTIME DESTINATION bin
        INCLUDES DESTINATION include)
install(EXPORT stlabTargets
        FILE stlabTargets.cmake
        NAMESPACE stlab::
        DESTINATION lib/cmake/stlab)

include(CMakePackageConfigHelpers)
write_basic_package_version_file("stlabConfigVersion.cmake"
        VERSION ${stlab_VERSION}
        COMPATIBILITY SameMajorVersion)
install(FILES config/stlabConfig.cmake 
              ${stlab_BINARY_DIR}/stlabConfigVersion.cmake
        DESTINATION lib/cmake/stlab)
install(DIRECTORY include/stlab/concurrency DESTINATION include/stlab 
  FILES_MATCHING PATTERN "*.hpp")

################################################################################              
# Directories
################################################################################
if ( stlab_testing )
  enable_testing()
  add_subdirectory( test )
endif()

