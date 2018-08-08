if(CMAKE_VERSION VERSION_LESS "3.10")
  if(DEFINED stlab.cmake)
    return()
  endif()

  set(stlab.cmake "" CACHE INTERNAL "")
else()
  include_gaurd(GLOBAL)
endif()

include(stlab/warnings)
include(stlab/coverage)
include(stlab/coroutines)
