set( stlab_Clang_base_flags -Wall -Wextra -Wpedantic -Werror -ftemplate-backtrace-limit=0 -DBOOST_NO_AUTO_PTR=1)
set( stlab_Clang_debug_flags -gdwarf-3 )
set( stlab_Clang_coverage_flags --coverage )
set( stlab_Clang_release_flags )

string(CONCAT generator
  "${stlab_Clang_base_flags};"
  "$<$<OR:$<CONFIG:DEBUG>,"
         "$<CONFIG:RELWITHDEBINFO>>:${stlab_Clang_debug_flags};>"
  "$<$<OR:$<CONFIG:RELEASE>,"
         "$<CONFIG:RELWITHDEBINFO>,"
         "$<CONFIG:MINSIZEREL>>:${stlab_Clang_release_flags};>"
  "$<$<AND:$<CONFIG:DEBUG>,"
          "$<BOOL:${stlab.coverage}>>:${stlab_Clang_debug_flags};>")

target_compile_options(development INTERFACE
  $<$<CXX_COMPILER_ID:Clang>:${generator}>)

target_link_libraries(development INTERFACE
  $<$<CXX_COMPILER_ID:Clang>:${generator}>)

