---
title: Annotated List of Boost Patches
layout: page
tags: [legacy]
comments: true
redirect_from: /legacy/index.php/Annotated_List_of_Boost_Patches
---
#### Contents
{:.no_toc}
1. contents
{:toc}

## Introduction

Every release, this page should break down each of the patches found in the boost patch file with a small description as to its rationale. This will help keep the boost patches in line and, should there be official boost patches to accomplish the same goals, we'll know which of our local patches we can undo.

## The Patches
### range/details/implementation_help.hpp
```
diff -ruN boost_1_34_1/boost/range/detail/implementation_help.hpp boost/range/detail/implementation_help.hpp
--- boost_1_34_1/boost/range/detail/implementation_help.hpp	2007-08-15 13:59:58.000000000 -0700
+++ boost/range/detail/implementation_help.hpp	2007-11-18 22:05:42.000000000 -0800
@@ -110,28 +110,64 @@
         {
             return str_end( s ) - s;
         }
+        /*
+        	ADOBE (sparent@adobe.com) 1464 : Using unused parameter to silence warnings.
+        */
+        template <typename T>
+        inline void adobe_boost_range_size_unused(const T&) { }
+        /*
+        	END ADOBE
+        */

         template< class T, std::size_t sz >
         inline std::size_t array_size( T BOOST_RANGE_ARRAY_REF()[sz], int )
         {
+        /*
+        	ADOBE (sparent@adobe.com) 1464 : Using unused parameter to silence warnings.
+        */
+        	adobe_boost_range_size_unused(boost_range_array);
+        /*
+        	END ADOBE
+        */
             return sz;
         }

         template< class T, std::size_t sz >
         inline std::size_t array_size( const T BOOST_RANGE_ARRAY_REF()[sz], int )
         {
+        /*
+        	ADOBE (sparent@adobe.com) 1464 : Using unused parameter to silence warnings.
+        */
+        	adobe_boost_range_size_unused(boost_range_array);
+        /*
+        	END ADOBE
+        */
             return sz;
         }

         template< class T, std::size_t sz >
         inline std::size_t array_size( T BOOST_RANGE_ARRAY_REF()[sz], char_or_wchar_t_array_tag )
         {
+        /*
+        	ADOBE (sparent@adobe.com) 1464 : Using unused parameter to silence warnings.
+        */
+        	adobe_boost_range_size_unused(boost_range_array);
+        /*
+        	END ADOBE
+        */
             return sz - 1;
         }

         template< class T, std::size_t sz >
         inline std::size_t array_size( const T BOOST_RANGE_ARRAY_REF()[sz], char_or_wchar_t_array_tag )
         {
+        /*
+        	ADOBE (sparent@adobe.com) 1464 : Using unused parameter to silence warnings.
+        */
+        	adobe_boost_range_size_unused(boost_range_array);
+        /*
+        	END ADOBE
+        */
             return sz - 1;
         }
```

Eliminating a warning about unused parameters.

### cast.hpp
```
diff -ruN boost_1_33_1/boost/cast.hpp boost/boost/cast.hpp
--- boost_1_33_1/boost/cast.hpp	2005-06-27 08:05:11.000000000 -0700
+++ boost/boost/cast.hpp	2006-05-26 15:22:26.000000000 -0700
@@ -49,6 +49,7 @@
 # include <boost/type.hpp>
 # include <boost/limits.hpp>
 # include <boost/detail/select_type.hpp>
+# include <boost/detail/workaround.hpp>

 //  It has been demonstrated numerous times that MSVC 6.0 fails silently at link
 //  time if you use a template function which has template parameters that don't
@@ -92,7 +93,11 @@
     template <class Target, class Source>
     inline Target polymorphic_downcast(Source* x BOOST_EXPLICIT_DEFAULT_TARGET)
     {
+#if BOOST_WORKAROUND(BOOST_MSVC, <= 1310)
+        assert( static_cast<const Source*>(dynamic_cast<Target>(x)) == x );  // detect logic error
+#else
         assert( dynamic_cast<Target>(x) == x );  // detect logic error
+#endif
         return static_cast<Target>(x);
     }
```
Eliminates a compiler warning in MSVC.

### time_resolution_traits.hpp
```
diff -ruN boost_1_33_1/boost/date_time/time_resolution_traits.hpp boost/boost/date_time/time_resolution_traits.hpp
--- boost_1_33_1/boost/date_time/time_resolution_traits.hpp	2003-12-02 19:01:05.000000000 -0800
+++ boost/boost/date_time/time_resolution_traits.hpp	2006-05-26 15:22:28.000000000 -0700
@@ -120,11 +120,17 @@
                    + (fractional_seconds_type(minutes)*60)
                    + seconds)*res_adjust()) + fs) * -1);
       }
-      else{
-        return (((fractional_seconds_type(hours)*3600)
-                 + (fractional_seconds_type(minutes)*60)
-                 + seconds)*res_adjust()) + fs;
-      }
+
+		/* ADOBE (fbrereto) :	There is a compiler bug in Darwin GCC doing a
+								release build that says it is possible to
+								reach the end of this function block without
+								returning anything. Eliminating the else
+								block wrapper fixed the bug.
+		*/
+
+		return (((fractional_seconds_type(hours)*3600)
+				 + (fractional_seconds_type(minutes)*60)
+				 + seconds)*res_adjust()) + fs;
     }

   };
```
There is a compiler bug in Darwin GCC doing a release build that says it is possible to reach the end of this function block without returning anything. Eliminating the else block wrapper fixed the bug.
### bool_trait_def.hpp
#### Purpose
Avoids a multiple-define warning in GCC.
#### Sample Compilation Result
_Compiled Using GCC 4.0 under the XCode 2.3 IDE; Mac OS 10.4.7_
```
    cd /Users/fbrereto/opensource/submission/ide_projects/darwin
    /usr/bin/gcc-4.0 -x c++ -arch ppc -pipe -Wno-trigraphs -fasm-blocks -O0 -Werror -DADOBE_SERIALIZATION=1 -DBOOST_SIGNALS_STATIC_LINK=1 -DBOOST_THREAD_USE_LIB=1 -DBOOST_ALL_NO_LIB=1 -fmessage-length=0 -mtune=G4 -fvisibility=hidden -fvisibility-inlines-hidden -mmacosx-version-min=10.3 -gdwarf-2 -I/Users/fbrereto/opensource/submission/ide_projects/darwin/build/asl_dev.build/Debug/adobe_dev.build/asl_dev.hmap -Wall -Wno-long-double -F/Users/fbrereto/opensource/submission/ide_projects/darwin/build/Debug -I/Users/fbrereto/opensource/submission/ide_projects/darwin/build/Debug/include -I../../adobe -I../../adobe/adobe/future/source/carbon -I../../third_party/boost_tp/boost -I../../adobe/adobe/future/widgets/headers -I../../adobe/adobe/future/widgets/headers/mac -I/Users/fbrereto/opensource/submission/ide_projects/darwin/build/asl_dev.build/Debug/adobe_dev.build/DerivedSources -isysroot /Developer/SDKs/MacOSX10.4u.sdk -c /Users/fbrereto/opensource/submission/ide_projects/darwin/../../adobe/source/adam_evaluate.cpp -o /Users/fbrereto/opensource/submission/ide_projects/darwin/build/asl_dev.build/Debug/adobe_dev.build/Objects-normal/ppc/adam_evaluate.o
In file included from ../../third_party/boost_tp/boost/boost/variant/detail/bool_trait_def.hpp:17,
                 from ../../third_party/boost_tp/boost/boost/variant/detail/has_trivial_move.hpp:20,
                 from ../../third_party/boost_tp/boost/boost/variant/detail/has_nothrow_move.hpp:13,
                 from ../../third_party/boost_tp/boost/boost/variant/detail/visitation_impl.hpp:36,
                 from ../../third_party/boost_tp/boost/boost/variant/variant.hpp:31,
                 from ../../third_party/boost_tp/boost/boost/variant.hpp:17,
                 from ../../adobe/adobe/value.hpp:31,
                 from ../../adobe/adobe/array.hpp:32,
                 from ../../adobe/adobe/adam.hpp:34,
                 from ../../adobe/adobe/adam_evaluate.hpp:14,
                 from /Users/fbrereto/opensource/submission/ide_projects/darwin/../../adobe/source/adam_evaluate.cpp:9:
../../third_party/boost_tp/boost/boost/type_traits/detail/bool_trait_def.hpp:44:1: error: "BOOST_TT_AUX_BOOL_TRAIT_DEF1" redefined
In file included from ../../third_party/boost_tp/boost/boost/variant/recursive_wrapper_fwd.hpp:26,
                 from ../../third_party/boost_tp/boost/boost/variant/detail/initializer.hpp:22,
                 from ../../third_party/boost_tp/boost/boost/variant/variant.hpp:28,
                 from ../../third_party/boost_tp/boost/boost/variant.hpp:17,
                 from ../../adobe/adobe/value.hpp:31,
                 from ../../adobe/adobe/array.hpp:32,
                 from ../../adobe/adobe/adam.hpp:34,
                 from ../../adobe/adobe/adam_evaluate.hpp:14,
                 from /Users/fbrereto/opensource/submission/ide_projects/darwin/../../adobe/source/adam_evaluate.cpp:9:
../../third_party/boost_tp/boost/boost/type_traits/detail/bool_trait_def.hpp:44:1: error: this is the location of the previous definition
```
#### Patch
```
diff -ruN boost_1_33_1/boost/type_traits/detail/bool_trait_def.hpp boost/boost/type_traits/detail/bool_trait_def.hpp
--- boost_1_33_1/boost/type_traits/detail/bool_trait_def.hpp	2005-03-16 04:22:22.000000000 -0800
+++ boost/boost/type_traits/detail/bool_trait_def.hpp	2006-05-26 15:23:04.000000000 -0700
@@ -41,6 +41,10 @@
 #   define BOOST_TT_AUX_BOOL_C_BASE(C) : ::boost::integral_constant<bool,C>
 #endif

+/* ADOBE (sparent@adobe.com) : Added undef to avoid multiple define warning in GCC */
+#ifdef BOOST_TT_AUX_BOOL_TRAIT_DEF1
+#undef BOOST_TT_AUX_BOOL_TRAIT_DEF1
+#endif

 #define BOOST_TT_AUX_BOOL_TRAIT_DEF1(trait,T,C) \
 template< typename T > struct trait \
```
### is_base_and_derived.hpp
```
diff -ruN boost_1_33_1/boost/type_traits/is_base_and_derived.hpp boost/boost/type_traits/is_base_and_derived.hpp
--- boost_1_33_1/boost/type_traits/is_base_and_derived.hpp	2005-04-21 08:58:33.000000000 -0700
+++ boost/boost/type_traits/is_base_and_derived.hpp	2006-05-26 15:23:06.000000000 -0700
@@ -37,7 +37,7 @@
 This version detects ambiguous base classes and private base classes
 correctly, and was devised by Rani Sharoni.

-Explanation by Terje Sletteb� and Rani Sharoni.
+Explanation by Terje Sletteb and Rani Sharoni.

 Let's take the multiple base class below as an example, and the following
 will also show why there's not a problem with private or ambiguous base
```
Pulls a high-ASCII character that prevents compilation from succeeding on some build systems with non-ASCII base encodings (e.g., Japanese systems). The conversion of these characters to the base encoding would fail, causing the compilation of the file to fail (even though the character is a part of a comment).
### recursive_mutex.cpp
```
diff -ruN boost_1_33_1/libs/thread/src/recursive_mutex.cpp boost/libs/thread/src/recursive_mutex.cpp
--- boost_1_33_1/libs/thread/src/recursive_mutex.cpp	2005-03-16 17:55:44.000000000 -0800
+++ boost/libs/thread/src/recursive_mutex.cpp	2006-05-26 15:25:07.000000000 -0700
@@ -296,7 +296,13 @@

     res = pthread_mutex_init(&m_mutex, &attr);
     {
-        int res = pthread_mutexattr_destroy(&attr);
+    // ADOBE (fbrereto) :	this fixes an unused variable warning
+    //						in gcc release builds under darwin.
+#	if !defined(NDEBUG)
+        int res =
+#	endif
+			pthread_mutexattr_destroy(&attr);
+
         assert(res == 0);
     }
     if (res != 0)
@@ -452,8 +458,14 @@

     res = pthread_mutex_init(&m_mutex, &attr);
     {
-        int res = pthread_mutexattr_destroy(&attr);
-        assert(res == 0);
+    // ADOBE (fbrereto) :	this fixes an unused variable warning
+    //						in gcc release builds under darwin.
+#	if !defined(NDEBUG)
+        int res =
+#	endif
+			pthread_mutexattr_destroy(&attr);
+
+		assert(res == 0);
     }
     if (res != 0)
         throw thread_resource_error();
```
Fixes an unused variable warning in gcc release builds under darwin.

### oserializer.hpp

```
diff -ruN ~/development/projects/submission/third_party/boost_tp/boost/boost/archive/detail/oserializer.hpp ~/development/projects/sandbox/sparent/third_party/boost_tp/boost/boost/archive/detail/oserializer.hpp

--- /Users/sparent/development/projects/submission/third_party/boost_tp/boost/boost/archive/detail/oserializer.hpp	2006-10-04 12:11:21.000000000 -0700
+++ /Users/sparent/development/projects/sandbox/sparent/third_party/boost_tp/boost/boost/archive/detail/oserializer.hpp	2006-10-10 12:01:41.000000000 -0700
@@ -115,7 +115,8 @@
         return boost::serialization::implementation_level<T>::value
             >= boost::serialization::object_class_info;
     }
-    virtual bool tracking(const unsigned int flags) const {
+    // ADOBE (sparent@adobe.com) : Removed an unused parameter to silence a warning.
+    virtual bool tracking(const unsigned int) const {
 //        if(0 != (flags &  no_tracking))
 //            return false;
         return boost::serialization::tracking_level<T>::value == boost::serialization::track_always
```
Fixes an unused argument warning in gcc builds under darwin.

### darwin.jam
```
diff -ruN boost_1_33_1/tools/build/v2/tools/darwin.jam boost/tools/build/v2/tools/darwin.jam
--- boost_1_33_1/tools/build/v2/tools/darwin.jam	2005-07-15 06:31:13.000000000 -0700
+++ boost/tools/build/v2/tools/darwin.jam	2006-06-30 14:54:40.000000000 -0700
@@ -41,8 +41,35 @@

 feature framework : : free ;

+# ADOBE : Adding architecture variant to build i386, ppc, or universal binaries;
+#         also adding static c++ lib settings and dead stripping on release
+
+feature sdkroot : 10.4 10.3.9 10.2.8 : propagated link-incompatible ;
+flags darwin.compile OPTIONS <sdkroot>10.4 : -isysroot /Developer/SDKs/MacOSX10.4u.sdk -mmacosx-version-min=10.4 ;
+flags darwin.link OPTIONS <sdkroot>10.4 : -isysroot /Developer/SDKs/MacOSX10.4u.sdk -mmacosx-version-min=10.4 ;
+flags darwin.compile OPTIONS <sdkroot>10.3.9 : -isysroot /Developer/SDKs/MacOSX10.3.9.sdk -mmacosx-version-min=10.3.9 ;
+flags darwin.link OPTIONS <sdkroot>10.3.9 : -isysroot /Developer/SDKs/MacOSX10.3.9.sdk -mmacosx-version-min=10.3.9 ;
+flags darwin.compile OPTIONS <sdkroot>10.2.8 : -isysroot /Developer/SDKs/MacOSX10.2.8.sdk -mmacosx-version-min=10.2.8 ;
+flags darwin.link OPTIONS <sdkroot>10.2.8 : -isysroot /Developer/SDKs/MacOSX10.2.8.sdk -mmacosx-version-min=10.2.8 ;
+
+feature arch : native ppc i386 fat : composite propagated ;
+flags darwin.compile OPTIONS <arch>fat : -arch i386 -arch ppc ;
+flags darwin.link OPTIONS <arch>fat : -arch i386 -arch ppc ;
+flags darwin.compile OPTIONS <arch>i386 : -arch i386 ;
+flags darwin.link OPTIONS <arch>i386 : -arch i386 ;
+flags darwin.compile OPTIONS <arch>ppc : -arch ppc ;
+flags darwin.link OPTIONS <arch>ppc : -arch ppc ;
+
+feature c++-lib : dynamic static : link-incompatible composite propagated ;
+flags darwin.compile OPTIONS <c++-lib>static : -fvisibility=hidden -fvisibility-inlines-hidden ;
+flags darwin.link OPTIONS <c++-lib>static : -nodefaultlibs -shared-libgcc -lstdc++-static -lgcc_eh -lgcc ;
+
+flags darwin.link OPTIONS <variant>release : -Wl,-dead_strip -no_dead_strip_inits_and_terms ;
+
+# ADOBE end changes
+
 flags darwin.compile OPTIONS <link>shared : -dynamic ;
-flags darwin.compile OPTIONS : -Wno-long-double -no-cpp-precomp  ;
+flags darwin.compile OPTIONS : -Wno-long-double -no-cpp-precomp -gdwarf-2 ;

 flags darwin.link FRAMEWORK <framework> ;

@@ -64,6 +91,10 @@

 actions piecemeal archive
 {
-    ar -c -r -s $(ARFLAGS) "$(<:T)" "$(>:T)"
+	# ADOBE : We use libtool instead of ar to support universal binary linking
+
+	libtool -static  -o "$(<:T)"  $(ARFLAGS)  "$(>:T)"
+
+	# ADOBE end changes
 }
```
These patches add several new features available to bjam when building under darwin (Mac OS X):
* In General
  * We use libtool instead of ar to support universal binary linking.
  * <code>-Wno-long-double -no-cpp-precomp -gdwarf-2</code> is passed to the compiler in all cases.
  * Building the release variant of an artifact asserts the following:
    * <code>-Wl,-dead_strip -no_dead_strip_inits_and_terms</code> is passed to the linker in all cases.
* Additional Features
* sdkroot
: Allows the client to specify the sdkroot against which the binary artifact will be built. Options are:
  * 10.4 (default)
  : Builds against the 10.4 Universal SDK (for Universal Binary support)
  * 10.3.9
  : Builds against the most up-to-date version of the 10.3 SDK.
  * 10.2.8
  : Builds against the most up-to-date version of the 10.2 SDK.
* arch
: Allows the client to specify the target architecture for the binary artifact generated. Options are:
  * native (default)
  : Builds to the platform architecture of the machine building the artifact.
  * ppc
  : Builds to PPC platform.
  * i386
  : Builds to Intel (MacTel) platform.
  * fat
  : Builds to both the PPC and i386 platforms under a single artifact (Universal Binary).
  * Note that when building the release variant of ASL, the fat setting for this feature is asserted.
* c++-lib
: Allows the client to specify the C++ runtime library against which their binary artifact will bind. Options are:
  * dynamic (default)
  : Binds against the runtime-linked C++ runtime.
  * static
  : Binds against the compile-time linked C++ runtime. Note that this feature has extra implications for the building of the binary:
    * <code>-fvisibility=hidden -fvisibility-inlines-hidden</code> is specified to the compiler
    * <code>-nodefaultlibs -shared-libgcc -lstdc++-static -lgcc_eh -lgcc</code> is specified to the linker
### gcc.jam

```
diff -ruN boost_1_33_1/tools/build/v2/tools/gcc.jam boost/tools/build/v2/tools/gcc.jam
--- boost_1_33_1/tools/build/v2/tools/gcc.jam	2005-06-15 05:24:06.000000000 -0700
+++ boost/tools/build/v2/tools/gcc.jam	2006-05-30 14:22:17.000000000 -0700
@@ -236,9 +236,14 @@
         {
         # we can't pass -s to ld unless we also pass -static
         # so we removed -s completly from OPTIONS and add it
-        # to ST_OPTIONS
-        flags $(toolset).link ST_OPTIONS $(condition)/<debug-symbols>off : -s
-           : unchecked ;
+        # to ST_OPTIONS
+
+        # ADOBE :	XCode 2.2 does not strip properly; removing this setting
+        #			until Apple figures out what the problem is
+        #flags $(toolset).link ST_OPTIONS $(condition)/<debug-symbols>off : -s
+        #   : unchecked ;
+        # ADOBE end changes
+
         flags $(toolset).link RPATH $(condition) : <dll-path> : unchecked ;
         flags $(toolset).link RPATH_LINK $(condition) : <xdll-path> : unchecked ;
         }
```
XCode 2.2 (and 2.3) does not strip properly; removing this setting until Apple figures out what the problem is. _Update_: Apple is slowly leaning away from the -s flag for stripping, and recommends the use of <code>strip</code> instead.
