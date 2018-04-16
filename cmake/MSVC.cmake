set( stlab_base_flags "" ) # removed /EHsc
set( stlab_debug_flags "" )
set( stlab_coverage_flags "" )
set( stlab_release_flags "" )
set( stlab_interface_flags "-D_WIN32_WINNT=0x0601;/DNOMINMAX;/std:c++latest;/D_HAS_AUTO_PTR_ETC=1;/bigobj;/D_SILENCE_CXX17_RESULT_OF_DEPRECATION_WARNING")
set( stlab_coroutine_flags "/std:c++latest;/await;")
