set( stlab_GNU_base_flags -Wall -Wextra -Wpedantic -Werror -ftemplate-backtrace-limit=0 -D_GLIBCXX_USE_CXX11_ABI=0)
set( stlab_GNU_debug_flags -gdwarf-3 )
set( stlab_GNU_coverage_flags --coverage )
set( stlab_GNU_release_flags )

string(CONCAT generator
  "${stlab_GNU_base_flags};"
  "$<$<OR:$<CONFIG:DEBUG>,"
         "$<CONFIG:RELWITHDEBINFO>>:${stlab_GNU_debug_flags};>"
  "$<$<OR:$<CONFIG:RELEASE>,"
         "$<CONFIG:RELWITHDEBINFO>,"
         "$<CONFIG:MINSIZEREL>>:${stlab_GNU_release_flags};>"
  "$<$<AND:$<CONFIG:DEBUG>,"
          "$<BOOL:${stlab.coverage}>>:${stlab_GNU_debug_flags};>")

target_compile_options(development INTERFACE
  $<$<CXX_COMPILER_ID:GNU>:${generator}>)

target_link_libraries(development INTERFACE
  $<$<CXX_COMPILER_ID:GNU>:${generator}>)
