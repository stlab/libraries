add_library( development INTERFACE )
add_library( stlab::development ALIAS development )

include( stlab/development/AppleClang )
include( stlab/development/Clang )
include( stlab/development/GNU )
include( stlab/development/MSVC )
