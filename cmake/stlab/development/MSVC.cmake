set(stlab_MSVC_base_flags
       /permissive- # Strict standards conformance
       /W4 # Warning level 4
       /WX # Treat warnings as errors
       /bigobj # Increase object file section limit
       # /D_SILENCE_ALL_CXX17_DEPRECATION_WARNINGS
       /Zc:__cplusplus # Correct __cplusplus macro
       /Zc:preprocessor # Standards-conforming preprocessor
       /Zc:inline # Remove unreferenced functions
       /Zc:externConstexpr # Enable external constexpr
       /Zc:throwingNew # Assume operator new throws
       /Zc:hiddenFriend # Conform to hidden friend lookup rules
       /Zc:referenceBinding # Enforce reference binding rules
       /Zc:rvalueCast # Enforce type conversion rules
       /Zc:strictStrings # Disable string literal type conversion
       /Zc:templateScope # Fix template parameter scope
       /Zc:ternary # Enforce conditional operator rules
       /volatile:iso # Use ISO-compliant volatile
)
set(stlab_MSVC_debug_flags
       /analyze:external- # Static analysis but filter external headers (boost test)
)
set(stlab_MSVC_coverage_flags)
set(stlab_MSVC_release_flags)

string(CONCAT generator
       "${stlab_MSVC_base_flags};"
       "$<$<OR:$<CONFIG:DEBUG>,"
       "$<CONFIG:RELWITHDEBINFO>>:${stlab_MSVC_debug_flags};>"
       "$<$<OR:$<CONFIG:RELEASE>,"
       "$<CONFIG:RELWITHDEBINFO>,"
       "$<CONFIG:MINSIZEREL>>:${stlab_MSVC_release_flags};>"
       "$<$<AND:$<CONFIG:DEBUG>,"
       "$<BOOL:${stlab.coverage}>>:${stlab_MSVC_debug_flags};>")


target_compile_options(development INTERFACE
       $<$<CXX_COMPILER_ID:MSVC>:${generator}>)
