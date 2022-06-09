 # Software Technology Lab Concurrency Library
 
 ## Requirements
 * C++ 14 compatible compiler
    * Visual Studio 2015 Update 5 or later
    * Clang 3.8 or later
    * GCC 4.8.5 or later
    
 * boost.optional, boost.variant
 * boost.test (only for the unit tests)
    
 ## Configuration Options
 The concurrency library is a header only library. Add ./ to the include path.
 It is necessary to add the compiler flag -fpermissive for all gcc versions < 7,
 because of a gcc bug. The compiler complains about a static constexpr statement 
 in the `stlab::task class`. It accepts the code with the -fpermissive flag, but
 the compiler produces a huge ammount of warnings, that can only be silenced with
 -w flag.
