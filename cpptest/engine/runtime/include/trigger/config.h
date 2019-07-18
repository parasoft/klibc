/*
 * (C) Copyright Parasoft Corporation 1998 - 2019.  All rights reserved.
 * The use of this source code is governed by the general Parasoft License Agreement.
 * Redistribution strictly prohibited.
 */

#ifndef __trigger_config_h__
#define __trigger_config_h__

#ifndef TRIGGER_NAMESPACE_ALLOWED
#  define TRIGGER_NAMESPACE_ALLOWED 1
#endif /* TRIGGER_NAMESPACE_ALLOWED */

#if TRIGGER_NAMESPACE_ALLOWED
#  define TRIGGER_NAMESPACE           trigger
#  define TRIGGER_NAMESPACE_START     namespace TRIGGER_NAMESPACE {
#  define TRIGGER_NAMESPACE_END       } /* namespace trigger */
#else /* !TRIGGER_NAMESPACE_ALLOWED */
#  define TRIGGER_NAMESPACE
#  define TRIGGER_NAMESPACE_START
#  define TRIGGER_NAMESPACE_END
#endif /* TRIGGER_NAMESPACE_ALLOWED */

#define TRIGGER_BOOLEAN int
#define TRIGGER_TRUE 1
#define TRIGGER_FALSE 0

#ifdef __cplusplus
#  define TRIGGER_EXTERN_C_START  extern "C" {
#  define TRIGGER_EXTERN_C_END    }
#else /* !__cplusplus */
#  define TRIGGER_EXTERN_C_START
#  define TRIGGER_EXTERN_C_END
#endif /* __cplusplus */


#ifndef TRIGGER_INLINE
#  if defined(__cplusplus)
#    define TRIGGER_INLINE inline
#  elif defined(_MSC_VER)
#    define TRIGGER_INLINE __inline
#  endif
#endif

#ifndef TRIGGER_INLINE
#  ifdef __GNUC__ 
#    define TRIGGER_WEAK_ATTRIBUTE __attribute__((weak))
#  endif
#endif

#ifndef TRIGGER_INLINE_RUNTIME
#  if defined(TRIGGER_INLINE)
#    define TRIGGER_INLINE_RUNTIME 1
#  elif defined(TRIGGER_WEAK_ATTRIBUTE)
#    define TRIGGER_INLINE_RUNTIME 1
#  else
#    define TRIGGER_INLINE_RUNTIME 0
#  endif
#endif /* TRIGGER_INLINE_RUNTIME */

#ifdef TRIGGER_WEAK_ATTRIBUTE
#  define _TRIGGER_WEAK_ATTRIBUTE TRIGGER_WEAK_ATTRIBUTE
#else
#  define _TRIGGER_WEAK_ATTRIBUTE
#endif

#ifdef TRIGGER_INLINE
#  define _TRIGGER_INLINE TRIGGER_INLINE
#else
#  define _TRIGGER_INLINE
#endif

#ifndef TRIGGER_DECL
#  if TRIGGER_INLINE_RUNTIME
#    define TRIGGER_DECL _TRIGGER_WEAK_ATTRIBUTE _TRIGGER_INLINE
#  else
#    define TRIGGER_DECL
#  endif /* TRIGGER_INLINE_RUNTIME */
#endif /* TRIGGER_DECL */


#ifndef TRIGGER_IMPLEMENTATION
#  define TRIGGER_IMPLEMENTATION TRIGGER_INLINE_RUNTIME
#endif /* CPPTEST_RUNTIME_IMPLEMENTATION */

#ifndef TRIGGER_CDECL
#  if defined(_MSC_VER) && !defined(__GNUC__)
#    define TRIGGER_CDECL __cdecl
#  else /* !_MSC_VER */
#    define TRIGGER_CDECL
#  endif /* _MSC_VER */
#endif

#ifndef TRIGGER_HAS_LONG_LONG
#  define TRIGGER_HAS_LONG_LONG 1
#endif /* TRIGGER_HAS_LONG_LONG */

#if TRIGGER_HAS_LONG_LONG
#  define TRIGGER_INTEGER long long
#  define TRIGGER_UINTEGER unsigned long long
#else /* !TRIGGER_HAS_LONG_LONG */
#  define TRIGGER_INTEGER long
#  define TRIGGER_UINTEGER unsigned long
#endif /* TRIGGER_HAS_LONG_LONG */

#ifndef __KERNEL__
#ifndef TRIGGER_HAS_LONG_DOUBLE
#  define TRIGGER_HAS_LONG_DOUBLE 1
#endif /* TRIGGER_HAS_LONG_DOUBLE */
#endif

#if TRIGGER_HAS_LONG_DOUBLE
#  define TRIGGER_FLOATING long double
#else /* !TRIGGER_HAS_LONG_DOUBLE */
#  define TRIGGER_FLOATING double
#endif /* TRIGGER_HAS_LONG_DOUBLE */

#ifndef TRIGGER_HAS_BUILTIN_WCHAR_T
#  define TRIGGER_HAS_BUILTIN_WCHAR_T 0
#endif /* TRIGGER_HAS_BUILTIN_WCHAR_T */

#ifndef TRIGGER_WCHAR_T
#  if TRIGGER_HAS_BUILTIN_WCHAR_T
#    define TRIGGER_WCHAR_T wchar_t
#  else /* !TRIGGER_HAS_BUILTIN_WCHAR_T */
#    define TRIGGER_WCHAR_T short
#  endif /* TRIGGER_HAS_BUILTIN_WCHAR_T */
#endif /* TRIGGER_WCHAR_T */

#ifndef TRIGGER_MALLOC_FUNC
#  define TRIGGER_MALLOC_FUNC malloc
#endif /* TRIGGER_MALLOC_FUNC */

#ifndef TRIGGER_FREE_FUNC
#  define TRIGGER_FREE_FUNC free
#endif /* TRIGGER_FREE_FUNC */

#ifndef TRIGGER_ALLOCATION_HEADER
#  define TRIGGER_ALLOCATION_HEADER <stdlib.h>
#endif /* TRIGGER_MALLOC_FREE_HEADER */

#ifndef TRIGGER_RPORT_IF_MORE_THAN_ONE_C_RUNNER_AT_THE_SAME_TIME
#  define TRIGGER_RPORT_IF_MORE_THAN_ONE_C_RUNNER_AT_THE_SAME_TIME 0
#endif /* TRIGGER_RPORT_IF_MORE_THAN_ONE_C_RUNNER_AT_THE_SAME_TIME */

#endif /* __trigger_config_h__ */
