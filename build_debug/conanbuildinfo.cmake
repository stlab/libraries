include(CMakeParseArguments)
set(CONAN_BOOST_ROOT "C:/.conan/ureof2/1")
set(CONAN_INCLUDE_DIRS_BOOST "C:/.conan/ureof2/1/include")
set(CONAN_LIB_DIRS_BOOST "C:/.conan/ureof2/1/lib")
set(CONAN_BIN_DIRS_BOOST "C:/.conan/ureof2/1/bin")
set(CONAN_RES_DIRS_BOOST "C:/.conan/ureof2/1/res")
set(CONAN_BUILD_DIRS_BOOST "C:/.conan/ureof2/1/")
set(CONAN_LIBS_BOOST libboost_wave-vc140-mt-gd-1_64 libboost_unit_test_framework-vc140-mt-gd-1_64 libboost_prg_exec_monitor-vc140-mt-gd-1_64 libboost_container-vc140-mt-gd-1_64 libboost_graph-vc140-mt-gd-1_64 libboost_iostreams-vc140-mt-gd-1_64 libboost_locale-vc140-mt-gd-1_64 libboost_log-vc140-mt-gd-1_64 libboost_log_setup-vc140-mt-gd-1_64 libboost_math_c99-vc140-mt-gd-1_64 libboost_math_c99f-vc140-mt-gd-1_64 libboost_math_c99l-vc140-mt-gd-1_64 libboost_math_tr1-vc140-mt-gd-1_64 libboost_math_tr1f-vc140-mt-gd-1_64 libboost_math_tr1l-vc140-mt-gd-1_64 libboost_program_options-vc140-mt-gd-1_64 libboost_random-vc140-mt-gd-1_64 libboost_regex-vc140-mt-gd-1_64 libboost_wserialization-vc140-mt-gd-1_64 libboost_serialization-vc140-mt-gd-1_64 libboost_signals-vc140-mt-gd-1_64 libboost_coroutine-vc140-mt-gd-1_64 libboost_context-vc140-mt-gd-1_64 libboost_timer-vc140-mt-gd-1_64 libboost_thread-vc140-mt-gd-1_64 libboost_chrono-vc140-mt-gd-1_64 libboost_date_time-vc140-mt-gd-1_64 libboost_atomic-vc140-mt-gd-1_64 libboost_filesystem-vc140-mt-gd-1_64 libboost_system-vc140-mt-gd-1_64 libboost_exception-vc140-mt-gd-1_64 libboost_test_exec_monitor-vc140-mt-gd-1_64)
set(CONAN_DEFINES_BOOST -DBOOST_USE_STATIC_LIBS
			-DBOOST_ALL_NO_LIB)
# COMPILE_DEFINITIONS are equal to CONAN_DEFINES without -D, for targets
set(CONAN_COMPILE_DEFINITIONS_BOOST BOOST_USE_STATIC_LIBS
			BOOST_ALL_NO_LIB)

set(CONAN_C_FLAGS_BOOST "")
set(CONAN_CXX_FLAGS_BOOST "")
set(CONAN_SHARED_LINKER_FLAGS_BOOST "")
set(CONAN_EXE_LINKER_FLAGS_BOOST "")

# For modern cmake targets we use the list variables (separated with ;)
set(CONAN_C_FLAGS_BOOST_LIST "")
set(CONAN_CXX_FLAGS_BOOST_LIST "")
set(CONAN_SHARED_LINKER_FLAGS_BOOST_LIST "")
set(CONAN_EXE_LINKER_FLAGS_BOOST_LIST "")


set(CONAN_ZLIB_ROOT "C:/Users/felixp/.conan/data/zlib/1.2.11/conan/stable/package/c32596dcd26b8c708dc3d19cb73738d2b48f12a8")
set(CONAN_INCLUDE_DIRS_ZLIB "C:/Users/felixp/.conan/data/zlib/1.2.11/conan/stable/package/c32596dcd26b8c708dc3d19cb73738d2b48f12a8/include")
set(CONAN_LIB_DIRS_ZLIB "C:/Users/felixp/.conan/data/zlib/1.2.11/conan/stable/package/c32596dcd26b8c708dc3d19cb73738d2b48f12a8/lib")
set(CONAN_BIN_DIRS_ZLIB "C:/Users/felixp/.conan/data/zlib/1.2.11/conan/stable/package/c32596dcd26b8c708dc3d19cb73738d2b48f12a8/bin")
set(CONAN_RES_DIRS_ZLIB "C:/Users/felixp/.conan/data/zlib/1.2.11/conan/stable/package/c32596dcd26b8c708dc3d19cb73738d2b48f12a8/res")
set(CONAN_BUILD_DIRS_ZLIB "C:/Users/felixp/.conan/data/zlib/1.2.11/conan/stable/package/c32596dcd26b8c708dc3d19cb73738d2b48f12a8/")
set(CONAN_LIBS_ZLIB zlibd)
set(CONAN_DEFINES_ZLIB )
# COMPILE_DEFINITIONS are equal to CONAN_DEFINES without -D, for targets
set(CONAN_COMPILE_DEFINITIONS_ZLIB )

set(CONAN_C_FLAGS_ZLIB "")
set(CONAN_CXX_FLAGS_ZLIB "")
set(CONAN_SHARED_LINKER_FLAGS_ZLIB "")
set(CONAN_EXE_LINKER_FLAGS_ZLIB "")

# For modern cmake targets we use the list variables (separated with ;)
set(CONAN_C_FLAGS_ZLIB_LIST "")
set(CONAN_CXX_FLAGS_ZLIB_LIST "")
set(CONAN_SHARED_LINKER_FLAGS_ZLIB_LIST "")
set(CONAN_EXE_LINKER_FLAGS_ZLIB_LIST "")



### Definition of global aggregated variables ###

set(CONAN_PACKAGE_NAME None)
set(CONAN_PACKAGE_VERSION None)

set(CONAN_SETTINGS_ARCH "x86_64")
set(CONAN_SETTINGS_BUILD_TYPE "Debug")
set(CONAN_SETTINGS_COMPILER "Visual Studio")
set(CONAN_SETTINGS_COMPILER_RUNTIME "MDd")
set(CONAN_SETTINGS_COMPILER_VERSION "14")
set(CONAN_SETTINGS_OS "Windows")

set(CONAN_DEPENDENCIES Boost zlib)
# Storing original command line args (CMake helper) flags
set(CONAN_CMD_CXX_FLAGS ${CONAN_CXX_FLAGS})

set(CONAN_CMD_SHARED_LINKER_FLAGS ${CONAN_SHARED_LINKER_FLAGS})
set(CONAN_CMD_C_FLAGS ${CONAN_C_FLAGS})
# Defining accumulated conan variables for all deps

set(CONAN_INCLUDE_DIRS "C:/.conan/ureof2/1/include"
			"C:/Users/felixp/.conan/data/zlib/1.2.11/conan/stable/package/c32596dcd26b8c708dc3d19cb73738d2b48f12a8/include" ${CONAN_INCLUDE_DIRS})
set(CONAN_LIB_DIRS "C:/.conan/ureof2/1/lib"
			"C:/Users/felixp/.conan/data/zlib/1.2.11/conan/stable/package/c32596dcd26b8c708dc3d19cb73738d2b48f12a8/lib" ${CONAN_LIB_DIRS})
set(CONAN_BIN_DIRS "C:/.conan/ureof2/1/bin"
			"C:/Users/felixp/.conan/data/zlib/1.2.11/conan/stable/package/c32596dcd26b8c708dc3d19cb73738d2b48f12a8/bin" ${CONAN_BIN_DIRS})
set(CONAN_RES_DIRS "C:/.conan/ureof2/1/res"
			"C:/Users/felixp/.conan/data/zlib/1.2.11/conan/stable/package/c32596dcd26b8c708dc3d19cb73738d2b48f12a8/res" ${CONAN_RES_DIRS})
set(CONAN_LIBS libboost_wave-vc140-mt-gd-1_64 libboost_unit_test_framework-vc140-mt-gd-1_64 libboost_prg_exec_monitor-vc140-mt-gd-1_64 libboost_container-vc140-mt-gd-1_64 libboost_graph-vc140-mt-gd-1_64 libboost_iostreams-vc140-mt-gd-1_64 libboost_locale-vc140-mt-gd-1_64 libboost_log-vc140-mt-gd-1_64 libboost_log_setup-vc140-mt-gd-1_64 libboost_math_c99-vc140-mt-gd-1_64 libboost_math_c99f-vc140-mt-gd-1_64 libboost_math_c99l-vc140-mt-gd-1_64 libboost_math_tr1-vc140-mt-gd-1_64 libboost_math_tr1f-vc140-mt-gd-1_64 libboost_math_tr1l-vc140-mt-gd-1_64 libboost_program_options-vc140-mt-gd-1_64 libboost_random-vc140-mt-gd-1_64 libboost_regex-vc140-mt-gd-1_64 libboost_wserialization-vc140-mt-gd-1_64 libboost_serialization-vc140-mt-gd-1_64 libboost_signals-vc140-mt-gd-1_64 libboost_coroutine-vc140-mt-gd-1_64 libboost_context-vc140-mt-gd-1_64 libboost_timer-vc140-mt-gd-1_64 libboost_thread-vc140-mt-gd-1_64 libboost_chrono-vc140-mt-gd-1_64 libboost_date_time-vc140-mt-gd-1_64 libboost_atomic-vc140-mt-gd-1_64 libboost_filesystem-vc140-mt-gd-1_64 libboost_system-vc140-mt-gd-1_64 libboost_exception-vc140-mt-gd-1_64 libboost_test_exec_monitor-vc140-mt-gd-1_64 zlibd ${CONAN_LIBS})
set(CONAN_DEFINES -DBOOST_USE_STATIC_LIBS
			-DBOOST_ALL_NO_LIB ${CONAN_DEFINES})
set(CONAN_CMAKE_MODULE_PATH "C:/.conan/ureof2/1/"
			"C:/Users/felixp/.conan/data/zlib/1.2.11/conan/stable/package/c32596dcd26b8c708dc3d19cb73738d2b48f12a8/" ${CONAN_CMAKE_MODULE_PATH})

set(CONAN_CXX_FLAGS " ${CONAN_CXX_FLAGS}")
set(CONAN_SHARED_LINKER_FLAGS " ${CONAN_SHARED_LINKER_FLAGS}")
set(CONAN_EXE_LINKER_FLAGS " ${CONAN_EXE_LINKER_FLAGS}")
set(CONAN_C_FLAGS " ${CONAN_C_FLAGS}")


###  Definition of macros and functions ###

macro(conan_define_targets)
    if(${CMAKE_VERSION} VERSION_LESS "3.1.2")
        message(FATAL_ERROR "TARGETS not supported by your CMake version!")
    endif()  # CMAKE > 3.x
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${CONAN_CMD_CXX_FLAGS}")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${CONAN_CMD_C_FLAGS}")
    set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} ${CONAN_CMD_SHARED_LINKER_FLAGS}")


    conan_find_libraries_abs_path("${CONAN_LIBS_BOOST}" "${CONAN_LIB_DIRS_BOOST}"
                                  CONAN_FULLPATH_LIBS_BOOST)
    conan_find_libraries_abs_path("${CONAN_LIBS_BOOST_DEBUG}" "${CONAN_LIB_DIRS_BOOST_DEBUG}"
                                  CONAN_FULLPATH_LIBS_BOOST_DEBUG)
    conan_find_libraries_abs_path("${CONAN_LIBS_BOOST_RELEASE}" "${CONAN_LIB_DIRS_BOOST_RELEASE}"
                                  CONAN_FULLPATH_LIBS_BOOST_RELEASE)

    add_library(CONAN_PKG::Boost INTERFACE IMPORTED)

    # Property INTERFACE_LINK_FLAGS do not work, necessary to add to INTERFACE_LINK_LIBRARIES
    set_property(TARGET CONAN_PKG::Boost PROPERTY INTERFACE_LINK_LIBRARIES ${CONAN_FULLPATH_LIBS_BOOST} ${CONAN_SHARED_LINKER_FLAGS_BOOST_LIST} ${CONAN_EXE_LINKER_FLAGS_BOOST_LIST}
                                                                 $<$<CONFIG:Release>:${CONAN_FULLPATH_LIBS_BOOST_RELEASE} ${CONAN_SHARED_LINKER_FLAGS_BOOST_RELEASE_LIST} ${CONAN_EXE_LINKER_FLAGS_BOOST_RELEASE_LIST}>
                                                                 $<$<CONFIG:RelWithDebInfo>:${CONAN_FULLPATH_LIBS_BOOST_RELEASE} ${CONAN_SHARED_LINKER_FLAGS_BOOST_RELEASE_LIST} ${CONAN_EXE_LINKER_FLAGS_BOOST_RELEASE_LIST}>
                                                                 $<$<CONFIG:MinSizeRel>:${CONAN_FULLPATH_LIBS_BOOST_RELEASE} ${CONAN_SHARED_LINKER_FLAGS_BOOST_RELEASE_LIST} ${CONAN_EXE_LINKER_FLAGS_BOOST_RELEASE_LIST}>
                                                                 $<$<CONFIG:Debug>:${CONAN_FULLPATH_LIBS_BOOST_DEBUG} ${CONAN_SHARED_LINKER_FLAGS_BOOST_DEBUG_LIST} ${CONAN_EXE_LINKER_FLAGS_BOOST_DEBUG_LIST}>
                                                                 CONAN_PKG::zlib)
    set_property(TARGET CONAN_PKG::Boost PROPERTY INTERFACE_INCLUDE_DIRECTORIES ${CONAN_INCLUDE_DIRS_BOOST}
                                                                      $<$<CONFIG:Release>:${CONAN_INCLUDE_DIRS_BOOST_RELEASE}>
                                                                      $<$<CONFIG:RelWithDebInfo>:${CONAN_INCLUDE_DIRS_BOOST_RELEASE}>
                                                                      $<$<CONFIG:MinSizeRel>:${CONAN_INCLUDE_DIRS_BOOST_RELEASE}>
                                                                      $<$<CONFIG:Debug>:${CONAN_INCLUDE_DIRS_BOOST_DEBUG}>)
    set_property(TARGET CONAN_PKG::Boost PROPERTY INTERFACE_COMPILE_DEFINITIONS ${CONAN_COMPILE_DEFINITIONS_BOOST}
                                                                      $<$<CONFIG:Release>:${CONAN_COMPILE_DEFINITIONS_BOOST_RELEASE}>
                                                                      $<$<CONFIG:RelWithDebInfo>:${CONAN_COMPILE_DEFINITIONS_BOOST_RELEASE}>
                                                                      $<$<CONFIG:MinSizeRel>:${CONAN_COMPILE_DEFINITIONS_BOOST_RELEASE}>
                                                                      $<$<CONFIG:Debug>:${CONAN_COMPILE_DEFINITIONS_BOOST_DEBUG}>)
    set_property(TARGET CONAN_PKG::Boost PROPERTY INTERFACE_COMPILE_OPTIONS ${CONAN_C_FLAGS_BOOST_LIST} ${CONAN_CXX_FLAGS_BOOST_LIST}
                                                                  $<$<CONFIG:Release>:${CONAN_C_FLAGS_BOOST_RELEASE_LIST} ${CONAN_CXX_FLAGS_BOOST_RELEASE_LIST}>
                                                                  $<$<CONFIG:RelWithDebInfo>:${CONAN_C_FLAGS_BOOST_RELEASE_LIST} ${CONAN_CXX_FLAGS_BOOST_RELEASE_LIST}>
                                                                  $<$<CONFIG:MinSizeRel>:${CONAN_C_FLAGS_BOOST_RELEASE_LIST} ${CONAN_CXX_FLAGS_BOOST_RELEASE_LIST}>
                                                                  $<$<CONFIG:Debug>:${CONAN_C_FLAGS_BOOST_DEBUG_LIST}  ${CONAN_CXX_FLAGS_BOOST_DEBUG_LIST}>)
 

    conan_find_libraries_abs_path("${CONAN_LIBS_ZLIB}" "${CONAN_LIB_DIRS_ZLIB}"
                                  CONAN_FULLPATH_LIBS_ZLIB)
    conan_find_libraries_abs_path("${CONAN_LIBS_ZLIB_DEBUG}" "${CONAN_LIB_DIRS_ZLIB_DEBUG}"
                                  CONAN_FULLPATH_LIBS_ZLIB_DEBUG)
    conan_find_libraries_abs_path("${CONAN_LIBS_ZLIB_RELEASE}" "${CONAN_LIB_DIRS_ZLIB_RELEASE}"
                                  CONAN_FULLPATH_LIBS_ZLIB_RELEASE)

    add_library(CONAN_PKG::zlib INTERFACE IMPORTED)

    # Property INTERFACE_LINK_FLAGS do not work, necessary to add to INTERFACE_LINK_LIBRARIES
    set_property(TARGET CONAN_PKG::zlib PROPERTY INTERFACE_LINK_LIBRARIES ${CONAN_FULLPATH_LIBS_ZLIB} ${CONAN_SHARED_LINKER_FLAGS_ZLIB_LIST} ${CONAN_EXE_LINKER_FLAGS_ZLIB_LIST}
                                                                 $<$<CONFIG:Release>:${CONAN_FULLPATH_LIBS_ZLIB_RELEASE} ${CONAN_SHARED_LINKER_FLAGS_ZLIB_RELEASE_LIST} ${CONAN_EXE_LINKER_FLAGS_ZLIB_RELEASE_LIST}>
                                                                 $<$<CONFIG:RelWithDebInfo>:${CONAN_FULLPATH_LIBS_ZLIB_RELEASE} ${CONAN_SHARED_LINKER_FLAGS_ZLIB_RELEASE_LIST} ${CONAN_EXE_LINKER_FLAGS_ZLIB_RELEASE_LIST}>
                                                                 $<$<CONFIG:MinSizeRel>:${CONAN_FULLPATH_LIBS_ZLIB_RELEASE} ${CONAN_SHARED_LINKER_FLAGS_ZLIB_RELEASE_LIST} ${CONAN_EXE_LINKER_FLAGS_ZLIB_RELEASE_LIST}>
                                                                 $<$<CONFIG:Debug>:${CONAN_FULLPATH_LIBS_ZLIB_DEBUG} ${CONAN_SHARED_LINKER_FLAGS_ZLIB_DEBUG_LIST} ${CONAN_EXE_LINKER_FLAGS_ZLIB_DEBUG_LIST}>
                                                                 )
    set_property(TARGET CONAN_PKG::zlib PROPERTY INTERFACE_INCLUDE_DIRECTORIES ${CONAN_INCLUDE_DIRS_ZLIB}
                                                                      $<$<CONFIG:Release>:${CONAN_INCLUDE_DIRS_ZLIB_RELEASE}>
                                                                      $<$<CONFIG:RelWithDebInfo>:${CONAN_INCLUDE_DIRS_ZLIB_RELEASE}>
                                                                      $<$<CONFIG:MinSizeRel>:${CONAN_INCLUDE_DIRS_ZLIB_RELEASE}>
                                                                      $<$<CONFIG:Debug>:${CONAN_INCLUDE_DIRS_ZLIB_DEBUG}>)
    set_property(TARGET CONAN_PKG::zlib PROPERTY INTERFACE_COMPILE_DEFINITIONS ${CONAN_COMPILE_DEFINITIONS_ZLIB}
                                                                      $<$<CONFIG:Release>:${CONAN_COMPILE_DEFINITIONS_ZLIB_RELEASE}>
                                                                      $<$<CONFIG:RelWithDebInfo>:${CONAN_COMPILE_DEFINITIONS_ZLIB_RELEASE}>
                                                                      $<$<CONFIG:MinSizeRel>:${CONAN_COMPILE_DEFINITIONS_ZLIB_RELEASE}>
                                                                      $<$<CONFIG:Debug>:${CONAN_COMPILE_DEFINITIONS_ZLIB_DEBUG}>)
    set_property(TARGET CONAN_PKG::zlib PROPERTY INTERFACE_COMPILE_OPTIONS ${CONAN_C_FLAGS_ZLIB_LIST} ${CONAN_CXX_FLAGS_ZLIB_LIST}
                                                                  $<$<CONFIG:Release>:${CONAN_C_FLAGS_ZLIB_RELEASE_LIST} ${CONAN_CXX_FLAGS_ZLIB_RELEASE_LIST}>
                                                                  $<$<CONFIG:RelWithDebInfo>:${CONAN_C_FLAGS_ZLIB_RELEASE_LIST} ${CONAN_CXX_FLAGS_ZLIB_RELEASE_LIST}>
                                                                  $<$<CONFIG:MinSizeRel>:${CONAN_C_FLAGS_ZLIB_RELEASE_LIST} ${CONAN_CXX_FLAGS_ZLIB_RELEASE_LIST}>
                                                                  $<$<CONFIG:Debug>:${CONAN_C_FLAGS_ZLIB_DEBUG_LIST}  ${CONAN_CXX_FLAGS_ZLIB_DEBUG_LIST}>)
 
    set(CONAN_TARGETS CONAN_PKG::Boost CONAN_PKG::zlib)

endmacro()


macro(conan_basic_setup)
    set(options TARGETS NO_OUTPUT_DIRS SKIP_RPATH)
    cmake_parse_arguments(ARGUMENTS "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN} )
    if(CONAN_EXPORTED)
        message(STATUS "Conan: called by CMake conan helper")
    endif()
    conan_check_compiler()
    if(NOT ARGUMENTS_NO_OUTPUT_DIRS)
        conan_output_dirs_setup()
    endif()
    conan_set_find_library_paths()
    if(NOT ARGUMENTS_TARGETS)
        message(STATUS "Conan: Using cmake global configuration")
        conan_global_flags()
    else()
        message(STATUS "Conan: Using cmake targets configuration")
        conan_define_targets()
    endif()
    if(NOT ARGUMENTS_SKIP_RPATH)
        message(STATUS "Conan: Adjusting default RPATHs Conan policies")
        conan_set_rpath()
    endif()
    conan_set_vs_runtime()
    conan_set_libcxx()
    conan_set_find_paths()
endmacro()

macro(conan_set_find_paths)
    # CMAKE_MODULE_PATH does not have Debug/Release config, but there are variables
    # CONAN_CMAKE_MODULE_PATH_DEBUG to be used by the consumer
    # CMake can find findXXX.cmake files in the root of packages
    set(CMAKE_MODULE_PATH ${CONAN_CMAKE_MODULE_PATH} ${CMAKE_MODULE_PATH})

    # Make find_package() to work
    set(CMAKE_PREFIX_PATH ${CONAN_CMAKE_MODULE_PATH} ${CMAKE_PREFIX_PATH})

    # Set the find root path (cross build)
    set(CMAKE_FIND_ROOT_PATH ${CONAN_CMAKE_FIND_ROOT_PATH} ${CMAKE_FIND_ROOT_PATH})
    if(CONAN_CMAKE_FIND_ROOT_PATH_MODE_PROGRAM)
        set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM ${CONAN_CMAKE_FIND_ROOT_PATH_MODE_PROGRAM})
    endif()
    if(CONAN_CMAKE_FIND_ROOT_PATH_MODE_LIBRARY)
        set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ${CONAN_CMAKE_FIND_ROOT_PATH_MODE_LIBRARY})
    endif()
    if(CONAN_CMAKE_FIND_ROOT_PATH_MODE_INCLUDE)
        set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ${CONAN_CMAKE_FIND_ROOT_PATH_MODE_INCLUDE})
    endif()
endmacro()

macro(conan_set_find_library_paths)
    # CMAKE_INCLUDE_PATH, CMAKE_LIBRARY_PATH does not have Debug/Release config, but there are variables
    # CONAN_INCLUDE_DIRS_DEBUG/RELEASE CONAN_LIB_DIRS_DEBUG/RELEASE to be used by the consumer
    # For find_library
    set(CMAKE_INCLUDE_PATH ${CONAN_INCLUDE_DIRS} ${CMAKE_INCLUDE_PATH})
    set(CMAKE_LIBRARY_PATH ${CONAN_LIB_DIRS} ${CMAKE_LIBRARY_PATH})
endmacro()

macro(conan_set_vs_runtime)
    if(CONAN_LINK_RUNTIME)
        if(DEFINED CMAKE_CXX_FLAGS_RELEASE)
            string(REPLACE "/MD" ${CONAN_LINK_RUNTIME} CMAKE_CXX_FLAGS_RELEASE ${CMAKE_CXX_FLAGS_RELEASE})
        endif()
        if(DEFINED CMAKE_CXX_FLAGS_DEBUG)
            string(REPLACE "/MDd" ${CONAN_LINK_RUNTIME} CMAKE_CXX_FLAGS_DEBUG ${CMAKE_CXX_FLAGS_DEBUG})
        endif()
        if(DEFINED CMAKE_C_FLAGS_RELEASE)
            string(REPLACE "/MD" ${CONAN_LINK_RUNTIME} CMAKE_C_FLAGS_RELEASE ${CMAKE_C_FLAGS_RELEASE})
        endif()
        if(DEFINED CMAKE_C_FLAGS_DEBUG)
            string(REPLACE "/MDd" ${CONAN_LINK_RUNTIME} CMAKE_C_FLAGS_DEBUG ${CMAKE_C_FLAGS_DEBUG})
        endif()
    endif()
endmacro()

macro(conan_flags_setup)
    # Macro maintained for backwards compatibility
    conan_set_find_library_paths()
    conan_global_flags()
    conan_set_rpath()
    conan_set_vs_runtime()
    conan_set_libcxx()
endmacro()



function(conan_find_libraries_abs_path libraries package_libdir libraries_abs_path)
    foreach(_LIBRARY_NAME ${libraries})
        unset(CONAN_FOUND_LIBRARY CACHE)
        find_library(CONAN_FOUND_LIBRARY NAME ${_LIBRARY_NAME} PATHS ${package_libdir} NO_DEFAULT_PATH NO_CMAKE_FIND_ROOT_PATH)
        if(CONAN_FOUND_LIBRARY)
            message(STATUS "Library ${_LIBRARY_NAME} found ${CONAN_FOUND_LIBRARY}")
            set(CONAN_FULLPATH_LIBS ${CONAN_FULLPATH_LIBS} ${CONAN_FOUND_LIBRARY})
        else()
            message(STATUS "Library ${_LIBRARY_NAME} not found in package, might be system one")
            set(CONAN_FULLPATH_LIBS ${CONAN_FULLPATH_LIBS} ${_LIBRARY_NAME})
        endif()
    endforeach()
    unset(CONAN_FOUND_LIBRARY CACHE)
    set(${libraries_abs_path} ${CONAN_FULLPATH_LIBS} PARENT_SCOPE)
endfunction()

macro(conan_set_libcxx)
    if(DEFINED CONAN_LIBCXX)
        message(STATUS "Conan C++ stdlib: ${CONAN_LIBCXX}")
        if(CONAN_COMPILER STREQUAL "clang" OR CONAN_COMPILER STREQUAL "apple-clang")
            if(CONAN_LIBCXX STREQUAL "libstdc++" OR CONAN_LIBCXX STREQUAL "libstdc++11" )
                set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libstdc++")
            elseif(CONAN_LIBCXX STREQUAL "libc++")
                set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libc++")
            endif()
        endif()
        if(CONAN_COMPILER STREQUAL "sun-cc")
            if(CONAN_LIBCXX STREQUAL "libCstd")
                set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -library=Cstd")
            elseif(CONAN_LIBCXX STREQUAL "libstdcxx")
                set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -library=stdcxx4")
            elseif(CONAN_LIBCXX STREQUAL "libstlport")
                set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -library=stlport4")
            elseif(CONAN_LIBCXX STREQUAL "libstdc++")
                set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -library=stdcpp")
            endif()
        endif()
        if(CONAN_LIBCXX STREQUAL "libstdc++11")
            add_definitions(-D_GLIBCXX_USE_CXX11_ABI=1)
        elseif(CONAN_LIBCXX STREQUAL "libstdc++")
            add_definitions(-D_GLIBCXX_USE_CXX11_ABI=0)
        endif()
    endif()
endmacro()

macro(conan_set_rpath)
    if(APPLE)
        # https://cmake.org/Wiki/CMake_RPATH_handling
        # CONAN GUIDE: All generated libraries should have the id and dependencies to other
        # dylibs without path, just the name, EX:
        # libMyLib1.dylib:
        #     libMyLib1.dylib (compatibility version 0.0.0, current version 0.0.0)
        #     libMyLib0.dylib (compatibility version 0.0.0, current version 0.0.0)
        #     /usr/lib/libc++.1.dylib (compatibility version 1.0.0, current version 120.0.0)
        #     /usr/lib/libSystem.B.dylib (compatibility version 1.0.0, current version 1197.1.1)
        set(CMAKE_SKIP_RPATH 1)  # AVOID RPATH FOR *.dylib, ALL LIBS BETWEEN THEM AND THE EXE
                                 # SHOULD BE ON THE LINKER RESOLVER PATH (./ IS ONE OF THEM)
    endif()
endmacro()

macro(conan_output_dirs_setup)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/bin)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})

    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/lib)
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELEASE ${CMAKE_ARCHIVE_OUTPUT_DIRECTORY})
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG ${CMAKE_ARCHIVE_OUTPUT_DIRECTORY})
endmacro()

macro(conan_split_version VERSION_STRING MAJOR MINOR)
    #make a list from the version string
    string(REPLACE "." ";" VERSION_LIST "${${VERSION_STRING}}")

    #write output values
    list(GET VERSION_LIST 0 ${MAJOR})
    list(GET VERSION_LIST 1 ${MINOR})
endmacro()

macro(conan_error_compiler_version)
    message(FATAL_ERROR "Incorrect '${CONAN_COMPILER}' version 'compiler.version=${CONAN_COMPILER_VERSION}'"
                        " is not the one detected by CMake: '${CMAKE_CXX_COMPILER_ID}=" ${VERSION_MAJOR}.${VERSION_MINOR}')
endmacro()

set(_CONAN_CURRENT_DIR ${CMAKE_CURRENT_LIST_DIR})
function(conan_get_compiler CONAN_INFO_COMPILER CONAN_INFO_COMPILER_VERSION)
    MESSAGE(STATUS "Current conanbuildinfo.cmake directory: " ${_CONAN_CURRENT_DIR})
    if(NOT EXISTS ${_CONAN_CURRENT_DIR}/conaninfo.txt)
        message(STATUS "WARN: conaninfo.txt not found")
        return()
    endif()

    file (READ "${_CONAN_CURRENT_DIR}/conaninfo.txt" CONANINFO)

    string(REGEX MATCH "compiler=([-A-Za-z0-9_ ]+)" _MATCHED ${CONANINFO})
    if(DEFINED CMAKE_MATCH_1)
        string(STRIP "${CMAKE_MATCH_1}" _CONAN_INFO_COMPILER)
        set(${CONAN_INFO_COMPILER} ${_CONAN_INFO_COMPILER} PARENT_SCOPE)
    endif()

    string(REGEX MATCH "compiler.version=([-A-Za-z0-9_.]+)" _MATCHED ${CONANINFO})
    if(DEFINED CMAKE_MATCH_1)
        string(STRIP "${CMAKE_MATCH_1}" _CONAN_INFO_COMPILER_VERSION)
        set(${CONAN_INFO_COMPILER_VERSION} ${_CONAN_INFO_COMPILER_VERSION} PARENT_SCOPE)
    endif()
endfunction()

function(check_compiler_version)
    CONAN_SPLIT_VERSION(CMAKE_CXX_COMPILER_VERSION VERSION_MAJOR VERSION_MINOR)
    if(CMAKE_CXX_COMPILER_ID MATCHES MSVC)
        # https://cmake.org/cmake/help/v3.2/variable/MSVC_VERSION.html
        if( (CONAN_COMPILER_VERSION STREQUAL "14" AND NOT VERSION_MAJOR STREQUAL "19") OR
            (CONAN_COMPILER_VERSION STREQUAL "12" AND NOT VERSION_MAJOR STREQUAL "18") OR
            (CONAN_COMPILER_VERSION STREQUAL "11" AND NOT VERSION_MAJOR STREQUAL "17") OR
            (CONAN_COMPILER_VERSION STREQUAL "10" AND NOT VERSION_MAJOR STREQUAL "16") OR
            (CONAN_COMPILER_VERSION STREQUAL "9" AND NOT VERSION_MAJOR STREQUAL "15") OR
            (CONAN_COMPILER_VERSION STREQUAL "8" AND NOT VERSION_MAJOR STREQUAL "14") OR
            (CONAN_COMPILER_VERSION STREQUAL "7" AND NOT VERSION_MAJOR STREQUAL "13") OR
            (CONAN_COMPILER_VERSION STREQUAL "6" AND NOT VERSION_MAJOR STREQUAL "12") )
            conan_error_compiler_version()
        endif()
    elseif(CONAN_COMPILER STREQUAL "gcc" OR CONAN_COMPILER MATCHES "clang" OR CONAN_COMPILER STREQUAL "sun-cc")
        if(NOT ${VERSION_MAJOR}.${VERSION_MINOR} VERSION_EQUAL CONAN_COMPILER_VERSION)
           conan_error_compiler_version()
        endif()
    else()
        message(STATUS "WARN: Unknown compiler '${CONAN_COMPILER}', skipping the version check...")
    endif()
endfunction()

function(conan_check_compiler)
    if(NOT DEFINED CMAKE_CXX_COMPILER_ID)
        if(DEFINED CMAKE_C_COMPILER_ID)
            message(STATUS "This project seems to be plain C, using '${CMAKE_C_COMPILER_ID}' compiler")
            set(CMAKE_CXX_COMPILER_ID ${CMAKE_C_COMPILER_ID})
            set(CMAKE_CXX_COMPILER_VERSION ${CMAKE_C_COMPILER_VERSION})
        else()
            message(FATAL_ERROR "This project seems to be plain C, but no compiler defined")
        endif()
    endif()
    if(CONAN_DISABLE_CHECK_COMPILER)
        message(STATUS "WARN: Disabled conan compiler checks")
        return()
    endif()

    if(NOT DEFINED CONAN_COMPILER)
        conan_get_compiler(CONAN_COMPILER CONAN_COMPILER_VERSION)
        if(NOT DEFINED CONAN_COMPILER)
            message(STATUS "WARN: CONAN_COMPILER variable not set, please make sure yourself that "
                       "your compiler and version matches your declared settings")
            return()
        endif()
    endif()

    if(NOT CMAKE_HOST_SYSTEM_NAME STREQUAL ${CMAKE_SYSTEM_NAME})
        set(CROSS_BUILDING 1)
    endif()

    # Avoid checks when cross compiling, apple-clang crashes because its APPLE but not apple-clang
    # Actually CMake is detecting "clang" when you are using apple-clang, only if CMP0025 is set to NEW will detect apple-clang
    if( (CONAN_COMPILER STREQUAL "Visual Studio" AND NOT CMAKE_CXX_COMPILER_ID MATCHES MSVC) OR
        (CONAN_COMPILER STREQUAL "gcc" AND NOT CMAKE_CXX_COMPILER_ID MATCHES "GNU") OR
        (CONAN_COMPILER STREQUAL "apple-clang" AND NOT CROSS_BUILDING AND (NOT APPLE OR NOT CMAKE_CXX_COMPILER_ID MATCHES "Clang")) OR
        (CONAN_COMPILER STREQUAL "clang" AND NOT CMAKE_CXX_COMPILER_ID MATCHES "Clang") OR
        (CONAN_COMPILER STREQUAL "sun-cc" AND NOT CMAKE_CXX_COMPILER_ID MATCHES "SunPro") )
        message(FATAL_ERROR "Incorrect '${CONAN_COMPILER}', is not the one detected by CMake: '${CMAKE_CXX_COMPILER_ID}'")
    endif()


    if(NOT DEFINED CONAN_COMPILER_VERSION)
        message(STATUS "WARN: CONAN_COMPILER_VERSION variable not set, please make sure yourself "
                       "that your compiler version matches your declared settings")
        return()
    endif()
    check_compiler_version()
endfunction()

macro(conan_set_flags build_type)
    set(CMAKE_CXX_FLAGS${build_type} "${CMAKE_CXX_FLAGS${build_type}} ${CONAN_CXX_FLAGS${build_type}}")
    set(CMAKE_C_FLAGS${build_type} "${CMAKE_C_FLAGS${build_type}} ${CONAN_C_FLAGS${build_type}}")
    set(CMAKE_SHARED_LINKER_FLAGS${build_type} "${CMAKE_SHARED_LINKER_FLAGS${build_type}} ${CONAN_SHARED_LINKER_FLAGS${build_type}}")
    set(CMAKE_EXE_LINKER_FLAGS${build_type} "${CMAKE_EXE_LINKER_FLAGS${build_type}} ${CONAN_EXE_LINKER_FLAGS${build_type}}")
endmacro()

macro(conan_global_flags)
    if(CONAN_SYSTEM_INCLUDES)
        include_directories(SYSTEM ${CONAN_INCLUDE_DIRS}
                                   "$<$<CONFIG:Release>:${CONAN_INCLUDE_DIRS_RELEASE}>"
                                   "$<$<CONFIG:RelWithDebInfo>:${CONAN_INCLUDE_DIRS_RELEASE}>"
                                   "$<$<CONFIG:MinSizeRel>:${CONAN_INCLUDE_DIRS_RELEASE}>"
                                   "$<$<CONFIG:Debug>:${CONAN_INCLUDE_DIRS_DEBUG}>")
    else()
        include_directories(${CONAN_INCLUDE_DIRS}
                            "$<$<CONFIG:Release>:${CONAN_INCLUDE_DIRS_RELEASE}>"
                            "$<$<CONFIG:RelWithDebInfo>:${CONAN_INCLUDE_DIRS_RELEASE}>"
                            "$<$<CONFIG:MinSizeRel>:${CONAN_INCLUDE_DIRS_RELEASE}>"
                            "$<$<CONFIG:Debug>:${CONAN_INCLUDE_DIRS_DEBUG}>")
    endif()

    link_directories(${CONAN_LIB_DIRS})

    conan_find_libraries_abs_path("${CONAN_LIBS_DEBUG}" "${CONAN_LIB_DIRS_DEBUG}"
                                  CONAN_LIBS_DEBUG)
    conan_find_libraries_abs_path("${CONAN_LIBS_RELEASE}" "${CONAN_LIB_DIRS_RELEASE}"
                                  CONAN_LIBS_RELEASE)

    add_compile_options(${CONAN_DEFINES}
                        "$<$<CONFIG:Debug>:${CONAN_DEFINES_DEBUG}>"
                        "$<$<CONFIG:Release>:${CONAN_DEFINES_RELEASE}>"
                        "$<$<CONFIG:RelWithDebInfo>:${CONAN_DEFINES_RELEASE}>"
                        "$<$<CONFIG:MinSizeRel>:${CONAN_DEFINES_RELEASE}>")

    conan_set_flags("")
    conan_set_flags("_RELEASE")
    conan_set_flags("_DEBUG")

endmacro()

macro(conan_target_link_libraries target)
    if(CONAN_TARGETS)
        target_link_libraries(${target} ${CONAN_TARGETS})
    else()
        target_link_libraries(${target} ${CONAN_LIBS})
        foreach(_LIB ${CONAN_LIBS_RELEASE})
            target_link_libraries(${target} optimized ${_LIB})
        endforeach()
        foreach(_LIB ${CONAN_LIBS_DEBUG})
            target_link_libraries(${target} debug ${_LIB})
        endforeach()
    endif()
endmacro()


### Definition of user declared vars (user_info) ###

