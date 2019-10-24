/*
 * (C) Copyright Parasoft Corporation 1998 - 2019.  All rights reserved.
 * The use of this source code is governed by the general Parasoft License Agreement.
 * Redistribution strictly prohibited.
 */


// this is meant for compiling specifically with klibc on linux

#define CPPTEST_GCC 1

#ifndef CPPTEST_EXPORT
#  define CPPTEST_EXPORT
#endif

#ifndef CPPTEST_IMPORT
#  define CPPTEST_IMPORT
#endif

#ifndef CDECL_CALL
#  define CDECL_CALL
#endif

#ifndef CPPTEST_TIME_MODE
#  define CPPTEST_TIME_MODE CPPTEST_TIME_GETTIMEOFDAY
#endif

#ifndef CPPTEST_THREADS_ENABLED
#  define CPPTEST_THREADS_ENABLED 0
#endif

#ifndef CPPTEST_THREADS_IMPLEMENTATION
#  define CPPTEST_THREADS_IMPLEMENTATION CPPTEST_NO_THREADS
#endif

#ifndef CPPTEST_SETJMP_ENABLED
#  define CPPTEST_SETJMP_ENABLED 1
#endif

#if CPPTEST_SETJMP_ENABLED
#  ifndef CPPTEST_USE_ANSI_SETJMP
#    define CPPTEST_USE_ANSI_SETJMP 0
#  endif
#endif

#ifndef CPPTEST_WCHAR_ENABLED
#  define CPPTEST_WCHAR_ENABLED 0
#endif

#ifndef CPPTEST_HAS_SYNC_FETCH_AND_ADD
#  if (__GNUC__ * 1000 + __GNUC_MINOR__ >= 4006)
#    define CPPTEST_HAS_SYNC_FETCH_AND_ADD 1
#  endif
#endif

#define CPPTEST_INTEGER long long
#define CPPTEST_PRINTF_INTEGER CPPTEST_INTEGER
#define CPPTEST_PRINTF_INTEGER_FMT "%lld"

#define CPPTEST_UINTEGER unsigned long long
#define CPPTEST_PRINTF_UINTEGER CPPTEST_UINTEGER
#define CPPTEST_PRINTF_UINTEGER_FMT "%llu"

// this is defined when -mno-80387 or -msoft-float is on the line
#ifdef _SOFT_FLOAT 
#   ifdef CPPTEST_DISABLE_ALL_FLOATING_POINT 
#       undef CPPTEST_DISABLE_ALL_FLOATING_POINT
#   endif
#   define CPPTEST_DISABLE_ALL_FLOATING_POINT 1
#else 
#   ifdef __KERNEL__
#      ifdef CPPTEST_DISABLE_ALL_FLOATING_POINT 
#          undef CPPTEST_DISABLE_ALL_FLOATING_POINT
#      endif
#      define CPPTEST_DISABLE_ALL_FLOATING_POINT 1
#   endif
#endif

#if CPPTEST_DISABLE_ALL_FLOATING_POINT
#define CPPTEST_FLOAT CPPTEST_INTEGER
//#define CPPTEST_PRINTF_FLOAT CPPTEST_FLOAT
//#define CPPTEST_PRINTF_FLOAT_FMT "%e"
//#define CPPTEST_SCANF_FLOAT CPPTEST_FLOAT
//#define CPPTEST_SCANF_FLOAT_FMT "%le"
#endif

#if (defined(_XOPEN_SOURCE) && _XOPEN_SOURCE >= 600) || defined(_STDC_99)
#  ifndef CPPTEST_HAS_STRTOLD
#    define CPPTEST_HAS_STRTOLD 1
#  endif
#endif

#ifndef CPPTEST_USE_UNCAUGHT_EXCEPTION_CHECKING
#  if __GNUC__ < 3 && defined(CPPTEST_THREADS)
#    define CPPTEST_USE_UNCAUGHT_EXCEPTION_CHECKING 0
#    ifndef CPPTEST_USE_SET_TERMINATE
#      define CPPTEST_USE_SET_TERMINATE 1
#    endif
#  else
#    define CPPTEST_USE_UNCAUGHT_EXCEPTION_CHECKING 1
#  endif
#endif

#ifndef CPPTEST_HAS_UNCAUGHT_EXCEPTION_PROBLEM_IN_EXCEPTION_HANDLER_IF_RETHROW
#  if __GNUC__ == 3 && !defined(CPPTEST_WIN32)
#    define CPPTEST_HAS_UNCAUGHT_EXCEPTION_PROBLEM_IN_EXCEPTION_HANDLER_IF_RETHROW 1
#  endif
#endif

#ifndef CPPTEST_C_STREAMS_REDIRECT_ENABLED
#  define CPPTEST_C_STREAMS_REDIRECT_ENABLED 0
#endif

#ifndef CPPTEST_CPP_STREAMS_REDIRECT_ENABLED
#  define CPPTEST_CPP_STREAMS_REDIRECT_ENABLED 0
#endif

#ifndef CPPTEST_HAS_MEMORY_HEADER
#  define CPPTEST_HAS_MEMORY_HEADER 0
#endif

#ifndef CPPTEST_HAS_LONG_DOUBLE
#  define CPPTEST_HAS_LONG_DOUBLE 0
#endif

