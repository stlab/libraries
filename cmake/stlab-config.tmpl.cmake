@PACKAGE_INIT@

include(CMakeFindDependencyMacro)

# Find thread library if needed
set(STLAB_THREAD_SYSTEM "@STLAB_THREAD_SYSTEM@")
if(NOT STLAB_THREAD_SYSTEM STREQUAL "none")
  find_dependency(Threads REQUIRED)
endif()

# Find task system dependencies
set(STLAB_TASK_SYSTEM "@STLAB_TASK_SYSTEM@")
if(STLAB_TASK_SYSTEM STREQUAL "libdispatch")
  find_dependency(libdispatch REQUIRED)
endif()

# Find main executor dependencies
set(STLAB_MAIN_EXECUTOR "@STLAB_MAIN_EXECUTOR@")
if(STLAB_MAIN_EXECUTOR STREQUAL "libdispatch")
  find_dependency(libdispatch REQUIRED)
elseif(STLAB_MAIN_EXECUTOR STREQUAL "qt5")
  find_dependency(Qt5 REQUIRED COMPONENTS Core)
elseif(STLAB_MAIN_EXECUTOR STREQUAL "qt6")
  find_dependency(Qt6 REQUIRED COMPONENTS Core)
endif()

include("${CMAKE_CURRENT_LIST_DIR}/stlab-targets.cmake")

# Find stlab dependencies only if they weren't embedded/bundled with stlab
# Check if the targets were exported as part of stlab's installation
if(NOT TARGET stlab::copy-on-write)
  find_dependency(copy-on-write REQUIRED)
endif()

if(NOT TARGET stlab::enum-ops)
  find_dependency(enum-ops REQUIRED)
endif()

check_required_components(stlab)