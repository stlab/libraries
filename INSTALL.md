 # Software Technology Lab Concurrency Library
 
 ## Requirements
 * C++ 14 compatible compiler
    * Visual Studio 2015 Update 5 or later
    * Clang 3.8 or later
    * GCC 4.8.5 or later
    
 * boost.optional, boost.variant (only for C++14, see below for further details)
 * boost.test (only for the unit tests)
    
 ## Configuration Options
 The concurrency library is a header only library. Add ./ to the include path.
 It is necessary to add the compiler flag -fpermissive for all gcc versions < 7,
 because of a gcc bug. The compiler complains about a static constexpr statement 
 in the `stlab::task class`. It accepts the code with the -fpermissive flag, but
 the compiler produces a huge ammount of warnings, that can only be silenced with
 -w flag.
 
 The library uses per default boost::optional and boost::variant. 
 
 Under certain conditions, it uses different implementations for optional in the following order:
 * boost::optional, if the define STLAB_FORCE_BOOST_OPTIONAL is set, or
 * std::optional, if C++17 is enabled and std::optional is available, or
 * std::experimental::optional, if it is available, otherwise
 * boost::optional
 
 Under certain conditions, it uses different implementations for variant in the following order:
  * boost::variant, if the define STLAB_FORCE_BOOST_VARIANT is set, or
  * std::variant, if C++17 is enabled and std::variant is available, or
  * boost::variant
  
  

 
 