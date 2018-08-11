add_library( coroutines INTERFACE )
add_library( stlab::coroutines ALIAS coroutines )

include( stlab/coroutines/AppleClang )
include( stlab/coroutines/Clang )
include( stlab/coroutines/GNU )
include( stlab/coroutines/MSVC )

if( stlab.coroutines )
  #
  # If the `stlab.coroutines` variable is set to `ON`, then a custom target
  # interface property `COROUTINES` is established and added to the coroutines
  # target and appended to the `COMPATIBLE_INTERFACE_BOOL` property list
  # property. The latter step ensures the property will propagate to targets
  # linking to the coroutines target.
  #
  set_target_properties( coroutines PROPERTIES INTERFACE_COROUTINES ON )
  set_property( TARGET coroutines APPEND PROPERTY COMPATIBLE_INTERFACE_BOOL COROUTINES )
endif()

#
# If coroutines are not enabled on the target, the minimum C++ version is
# increased to C++17
#
target_compile_features( coroutines INTERFACE
  $<$<BOOL:$<TARGET_PROPERTY:COROUTINES>>:cxx_std_17> )


