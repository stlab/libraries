set( stlab_AppleClang_base_flags -Wall -Wextra -Wpedantic -Werror -ftemplate-backtrace-limit=0 )
set( stlab_AppleClang_debug_flags -gdwarf-3 )
set( stlab_AppleClang_coverage_flags --coverage )
set( stlab_AppleClang_release_flags )

string(CONCAT generator
  "${stlab_AppleClang_base_flags};"
  "$<$<OR:$<CONFIG:DEBUG>,"
         "$<CONFIG:RELWITHDEBINFO>>:${stlab_AppleClang_debug_flags};>"
  "$<$<OR:$<CONFIG:RELEASE>,"
         "$<CONFIG:RELWITHDEBINFO>,"
         "$<CONFIG:MINSIZEREL>>:${stlab_AppleClang_release_flags};>"
  "$<$<AND:$<CONFIG:DEBUG>,"
          "$<BOOL:${stlab.coverage}>>:${stlab_AppleClang_debug_flags};>")

target_compile_options(development INTERFACE
  $<$<CXX_COMPILER_ID:AppleClang>:${generator}>)

target_link_libraries(development INTERFACE
  $<$<CXX_COMPILER_ID:AppleClang>:${generator}>)

