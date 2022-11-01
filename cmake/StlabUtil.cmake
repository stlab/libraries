# This module provides utility functions that determine host system
# functionality useful for stlab builds.

include( CheckCXXSymbolExists )
include( CheckCXXSourceRuns )

# Determine if the selected C++ compiler has functional versions of
# 'std::variant' and 'std::optional'. Set the specified 'result_var' to 'TRUE'
# if they are determined to be disfunctional and 'FALSE' otherwise. Note that
# this check consists of a smoke test and does not check all the ways these
# library components may be deficient.
function( stlab_check_disfunctional_variant_optional result_var )
  check_cxx_source_runs( "
    #include <variant>
    #include <optional>

    int main() {
        std::optional<int> op = 3;
        op = std::nullopt;

        std::variant<int, char> v = 12;
        return 0;
    }" STLAB_HAVE_FUNCTIONAL_VARIANT_OPTIONAL )
  if( STLAB_HAVE_FUNCTIONAL_VARIANT_OPTIONAL )
    set( ${result_var} FALSE PARENT_SCOPE )
  else()
    set( ${result_var} TRUE PARENT_SCOPE )
  endif()
endfunction()

# Determine if the selected C++ compiler has functional coroutines. Set the
# specified 'result_var' to 'TRUE' if they are determined to be disfunctional
# and 'FALSE' otherwise. Note that this check consists of a smoke test and does
# not check all the ways this feature may be deficient.
function( stlab_check_disfunctional_coroutines result_var )
  check_cxx_source_runs( "
#include <concepts>
#include <coroutine>
#include <exception>

struct ReturnObject {
  struct promise_type {
    ReturnObject get_return_object() { return {}; }
    std::suspend_never initial_suspend() { return {}; }
    std::suspend_never final_suspend() noexcept { return {}; }
    void unhandled_exception() {}
  };
};

struct Awaiter {
  std::coroutine_handle<> *hp_;
  constexpr bool await_ready() const noexcept { return false; }
  void await_suspend(std::coroutine_handle<> h) { *hp_ = h; }
  constexpr void await_resume() const noexcept {}
};

ReturnObject
counter(std::coroutine_handle<> *continuation_out)
{
  Awaiter a{continuation_out};
  for (unsigned i = 0;; ++i) {
    co_await a;
  }
}

int
main()
{
  std::coroutine_handle<> h;
  counter(&h);
  for (int i = 0; i < 3; ++i) {
    h();
  }
  h.destroy();
}
" STLAB_HAVE_FUNCTIONAL_COROUTINES )
  if( STLAB_HAVE_FUNCTIONAL_COROUTINES )
    set( ${result_var} FALSE PARENT_SCOPE )
  else()
    set( ${result_var} TRUE PARENT_SCOPE )
  endif()
endfunction()

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
    if( CMAKE_SYSTEM_NAME STREQUAL "Emscripten" )
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
# | Result value | Task system                                                    |
# |--------------+----------------------------------------------------------------|
# | libdispatch  | libdispatch (aka. Grand Central Dispatch )                     |
# | portable     | A portable task system provided by stlab (supports Emscripten) |
# | windows      | Windows's task system                                          |
function( stlab_detect_task_system result_var )
  find_package( Threads QUIET )
  if( APPLE )
    set( result "libdispatch")
  elseif( WIN32 )
    set( result "windows")
  elseif( CMAKE_SYSTEM_NAME STREQUAL "Emscripten" )
    set( result "portable")
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

# Find a main executor variant that is compatable with the target platform. The
# following table shows the correspondence between result values and main
# executor variants.
#
# | Result value | Main Executor                              |
# |--------------+--------------------------------------------|
# | libdispatch  | libdispatch (aka. Grand Central Dispatch ) |
# | emscripten   | Emscripten's executor                      |
# | qt           | Qt's event framework                       |
# | none         | None                                       |
function( stlab_detect_main_executor result_var )
  find_package(Qt6 QUIET COMPONENTS Core)
  stlab_detect_task_system( task_system )

  if( task_system STREQUAL "libdispatch" )
    set( result "libdispatch")
  elseif( CMAKE_SYSTEM_NAME STREQUAL "Emscripten" )
    set( result "emscripten")
  elseif( Qt6Core_FOUND )
    set( result "qt")
  else()
    set( result "none")
  endif()

  set( ${result_var} ${result} PARENT_SCOPE )
endfunction()

# Generate 'stlab/config.hpp' in the binary directory using active CMake
# variables and 'stlab/config.hpp.in'.
function( stlab_generate_config_file )
  if ( STLAB_THREAD_SYSTEM STREQUAL "win32" )
    set( STLAB_THREADS_WIN32 TRUE )
  elseif ( STLAB_THREAD_SYSTEM STREQUAL "pthread" )
    set( STLAB_THREADS_PTHREAD TRUE )
  elseif ( STLAB_THREAD_SYSTEM STREQUAL "pthread-emscripten" )
    set( STLAB_THREADS_PTHREAD_EMSCRIPTEN TRUE )
  elseif ( STLAB_THREAD_SYSTEM STREQUAL "pthread-apple" )
    set( STLAB_THREADS_PTHREAD_APPLE TRUE )
  else ()
    set( STLAB_THREADS_NONE TRUE )
  endif()

  if (STLAB_TASK_SYSTEM STREQUAL "portable")
    set( STLAB_TASK_SYSTEM_PORTABLE TRUE )
  elseif (STLAB_TASK_SYSTEM STREQUAL "libdispatch")
    set( STLAB_TASK_SYSTEM_LIBDISPATCH TRUE )
  elseif (STLAB_TASK_SYSTEM STREQUAL "emscripten")
    set( STLAB_TASK_SYSTEM_EMSCRIPTEN TRUE )
  elseif (STLAB_TASK_SYSTEM STREQUAL "windows")
    set( STLAB_TASK_SYSTEM_WINDOWS TRUE )
  endif()

  if (STLAB_MAIN_EXECUTOR STREQUAL "libdispatch")
    set( STLAB_MAIN_EXECUTOR_LIBDISPATCH TRUE )
  elseif (STLAB_MAIN_EXECUTOR STREQUAL "emscripten")
    set( STLAB_MAIN_EXECUTOR_EMSCRIPTEN TRUE )
  elseif (STLAB_MAIN_EXECUTOR STREQUAL "qt")
    set( STLAB_MAIN_EXECUTOR_QT TRUE )
  elseif (STLAB_MAIN_EXECUTOR STREQUAL "none")
    set( STLAB_MAIN_EXECUTOR_NONE TRUE )
  endif()

  configure_file(
    "${PROJECT_SOURCE_DIR}/stlab/config.hpp.in"
    "${PROJECT_BINARY_DIR}/stlab/config.hpp"
    @ONLY
  )
endfunction()
