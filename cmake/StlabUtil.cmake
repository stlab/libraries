# This module provides utility functions that determine host system
# functionality useful for stlab builds.

include (CheckCXXSymbolExists)

# Detect the target platform's thread system and set the specified 'result_var'
# to the result. The following table shows the correspondence between result
# values and threading systems.
#
# | Result value       | Threading system   |
# |--------------------+--------------------|
# | pthread            | POSIX threads      |
# | win32              | Windows threads    |
# | pthread-emscripten | Emscripten threads |
# | pthread-apple      | Apple threads      |
# | none               | None of the above  |
function( stlab_detect_thread_system result_var )
  find_package( Threads QUIET )
  if( CMAKE_USE_WIN32_THREADS_INIT )
    set( result "win32" )
  elseif( CMAKE_USE_PTHREADS_INIT )
    check_cxx_symbol_exists("__EMSCRIPTEN_PTHREADS__" "pthread.h" HAS_EMSCRIPTEN_PTHREADS )
    if( HAS_EMSCRIPTEN_PTHREADS )
      set( result "pthread-emscripten" )
    elseif( APPLE )
      set( result "pthread-apple" )
    else( )
      set( result "pthread" )
    endif()
  else()
    set( result "none" )
  endif()
  set( ${result_var} ${result} PARENT_SCOPE )
endfunction()

# Detect the target platform's task system and set the specified 'result_var'
# to the result. The following table shows the correspondence between result
# values and task systems.
#
# | Result value | Task system                                |
# |--------------+--------------------------------------------|
# | libdispatch  | libdispatch (aka. Grand Central Dispatch ) |
# | portable     | A portable task system provided by stlab   |
# | emscripten   | Emscripten's task system                   |
# | windows      | Windows's task system                      |
function( stlab_detect_task_system result_var )
  if( APPLE )
    set( result "libdispatch")
  elseif( CMAKE_SYSTEM_NAME STREQUAL "Emscripten" )
    check_cxx_symbol_exists("__EMSCRIPTEN_PTHREADS__" "pthread.h" HAS_EMSCRIPTEN_PTHREADS )
    if( HAS_EMSCRIPTEN_PTHREADS )
      set( result "portable")
    else()
      set( result "emscripten")
    endif()
  elseif( WIN32 )
    set( result "windows")
  else()
    find_package( libdispatch )
    if( libdispatch_FOUND )
      set( result "libdispatch")
    else()
      set( result "portable")
    endif()
  endif()
  set( ${result_var} ${result} PARENT_SCOPE )
endfunction()
