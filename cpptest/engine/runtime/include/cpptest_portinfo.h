/*
 * (C) Copyright Parasoft Corporation 1998 - 2019.  All rights reserved.
 * The use of this source code is governed by the general Parasoft License Agreement.
 * Redistribution strictly prohibited.
 */

#ifndef __CppTest_PortInfo_h__
#define __CppTest_PortInfo_h__

/* Used system. */
#if !defined(CPPTEST_SOLARIS) && !defined(CPPTEST_LINUX) && !defined(CPPTEST_VXWORKS) && \
    !defined(CPPTEST_CYGWIN) && !defined(CPPTEST_WIN32) && !defined(CPPTEST_QNX) && !defined(CPPTEST_NOSYSTEM)
#  if (defined(edg_sun) || defined(__sun)) && !defined(__ARMCC_VERSION)
#    define CPPTEST_SOLARIS
#  elif (defined(edg_linux) || defined(linux) || defined(__linux__) || defined(__NIOS2__) || defined(__nios__)) && !defined(__ARMCC_VERSION)
#    define CPPTEST_LINUX
#  elif defined(__VXWORKS) || defined(__VXWORKS__)
#    define CPPTEST_VXWORKS
#  elif defined(__CYGWIN__)
#    define CPPTEST_CYGWIN
#    define CPPTEST_WIN32
#  elif defined(_WIN32_WCE)
#    define CPPTEST_WIN32_CE
#    define CPPTEST_WIN32
#  elif defined(_WIN32) || defined(__WIN32) || defined(WIN32) || defined(__WIN32__) || \
        defined(__WIN16) || defined(WIN16) || defined(_WIN16)
#    define CPPTEST_WIN32
#  elif defined(_AIX)
#    define CPPTEST_AIX
#  elif defined(__QNX__)
#    define CPPTEST_QNX
#  else
#    define CPPTEST_NOSYSTEM
#  endif
#endif

#ifdef __cplusplus
#  define EXTERN_C_LINKAGE extern "C"
#  define EXTERN_C extern "C"
#  define EXTERN_C_LINKAGE_START extern "C" {
#  define EXTERN_C_LINKAGE_END }
#else
#  define EXTERN_C_LINKAGE 
#  define EXTERN_C extern
#  define EXTERN_C_LINKAGE_START
#  define EXTERN_C_LINKAGE_END
#endif

#ifndef CPPTEST_SHARED
#  define CPPTEST_SHARED 1
#endif

/* Values used by CPPTEST_TIME_MODE macro. */
#define CPPTEST_TIME_NOTIME 0           /* No time functions. */
#define CPPTEST_TIME_ANSI 1             /* Used functions: time, difftime */
#define CPPTEST_TIME_GETTIMEOFDAY 2     /* Used functions: gettimeofday, difftime */
#define CPPTEST_TIME_TIMEB 3            /* Used functions: ftime, difftime */
#define CPPTEST_TIME_TICKLIB 4          /* Used functions: tickGet, sysClkRateGet (VxWorks) */
#define CPPTEST_TIME_NXT_SYSTICK 5      /* Used functions: systick_get_ms (LejosNXJ-NXTVM) */

/* Values used by CPPTEST_THREADS_IMPLEMENTATION macro. */
#define CPPTEST_NO_THREADS 0
#define CPPTEST_THREADS_POSIX 1
#define CPPTEST_THREADS_WINDOWS 2
#define CPPTEST_THREADS_VXWORKS_KERNEL 3
#define CPPTEST_THREADS_VXWORKS_RTP 4
#define CPPTEST_THREADS_VXWORKS_SMP 5
#define CPPTEST_THREADS_VXWORKS_KWORD 6

#ifndef CPPTEST_THREADS
#  define CPPTEST_THREADS 0
#endif

#if defined(CPPTEST_EPT_main) || defined(CPPTEST_EPT_wmain) ||  \
    defined(CPPTEST_EPT_WinMain) || defined(CPPTEST_EPT_wWinMain) || \
    defined(CPPTEST_EPT_void_main) || defined(CPPTEST_EPT_main_no_args) || \
    defined(CPPTEST_EPT_void_main_no_args) || \
    defined(CPPTEST_EPT_void_main_no_args_with_exit) || \
    defined(CPPTEST_ENTRY_POINT) || defined(CPPTEST_EPT_no_main)
#  define _CPPTEST_ENTRY_POINT_TYPE_DEFINED
#endif


#if defined(__ghs__) || defined(__ghs)
#  include "config/cpptest_ghs.h"
#elif defined(__QNX__)
#  include "config/cpptest_qcc.h"
#elif defined(__DCC__) && defined(CPPTEST_VXWORKS)
#  include "config/cpptest_dcc_vxworks.h"
#elif defined(__DCC__)
#  include "config/cpptest_dcc.h"
#elif defined(__ARMCC_VERSION) || defined(__ARMCOMPILER_VERSION)
#  include "config/cpptest_armcc.h"
#elif defined(__SUNPRO_C) || defined(__SUNPRO_CC)
#  include "config/cpptest_sunprocc.h"
#elif defined(_ICC)
#  include "config/cpptest_icc.h"
#elif defined(__TASKING__)
#  include "config/cpptest_vxtc.h"
#elif defined(__TRICORE__)
#  include "config/cpptest_tricoregcc.h"
#elif defined(__sh__)
#  include "config/cpptest_sh4gcc.h"
#elif defined(__GNUC__) && defined(PARASOFT_CPPTEST_GCC_AUTO_CONFIG)
#  include "config/cpptest_gcc_auto.h"
#elif defined(CPPTEST_GNUARM_NXT)
#  include "config/cpptest_gnuarm4NXT.h"
#elif defined(__GNUC__) && defined(CPPTEST_CYGWIN)
#  include "config/cpptest_gcc_cygwin.h"
#elif defined(__GNUC__) && defined(CPPTEST_VXWORKS)
#  include "config/cpptest_gcc_vxworks.h"
#elif defined(__GNUC__) && defined(__NIOS2__)
#  include "config/cpptest_nios2.h"
#elif defined(__GNUC__) && defined(__nios__)
#  include "config/cpptest_nios.h"
#elif defined(__TI_COMPILER_VERSION__) || defined(_dsp) || defined(_TMS320C6X) || defined(__TMS320C2000__) || defined(__TMS320C55X__) || defined(__TMS320C5XX__)
#  include "config/cpptest_ti.h"
#elif defined(__GNUC__) && defined(__arm__) && defined(__ARMEL__)
#  include "config/cpptest_armgcc.h"
#elif defined(__GNUC__) && defined(__MSP430__)
#  include "config/cpptest_mspgcc.h"
#elif defined(__MINGW32__)
#  include "config/cpptest_mingw.h"
#elif defined(_MSC_VER) && defined(UNDER_CE)
#  include "config/cpptest_evc.h"
#elif defined(_MSC_VER)
#  include "config/cpptest_msvc.h"
#elif defined(__IBMCPP__) || defined(__IBMC__)
#  include "config/cpptest_xl.h"
#elif defined(__ICCARM__)
#  include "config/cpptest_iccarm.h"
#elif defined(__ICC430__)
#  include "config/cpptest_icc430.h"
#elif defined(__ICCRX__)
#  include "config/cpptest_iccrx.h"
#elif defined(__ICCRL78__)
#  include "config/cpptest_iccrl78.h"
#elif defined(__RENESAS__) && defined(__RX)
#  include "config/cpptest_renrx.h"
#elif defined(__RENESAS__) && defined(_SH)
#  include "config/cpptest_shc.h"
#elif defined (__COMPILER_FCC911__)
#  include "config/cpptest_fr.h"
#elif defined(__GNUC__) && defined(__KLIBC__)
#  include "config/cpptest_gcc_klibc.h"
#elif defined(__GNUC__)
#  include "config/cpptest_gcc.h"
#else
#  include "config/cpptest_unknown.h"
#endif

#ifndef CPPTEST_HAS_LONG_LONG
#  define CPPTEST_HAS_LONG_LONG 1
#endif

#ifndef CPPTEST_HAS_LONG_DOUBLE
#  define CPPTEST_HAS_LONG_DOUBLE 1
#endif

#ifndef CPPTEST_SYSTEM_HEADERS_ALLOWED
#  ifdef CPPTEST_NOSYSTEM
#    define CPPTEST_SYSTEM_HEADERS_ALLOWED 0
#  else
#    define CPPTEST_SYSTEM_HEADERS_ALLOWED 1
#  endif
#endif

#ifndef CPPTEST_HAS_GETENV
#  define CPPTEST_HAS_GETENV CPPTEST_SYSTEM_HEADERS_ALLOWED
#endif

#ifndef CPPTEST_HAS_IO_HEADER
#  define CPPTEST_HAS_IO_HEADER CPPTEST_SYSTEM_HEADERS_ALLOWED
#endif

#ifndef CPPTEST_HAS_ASSERT_HEADER
#  define CPPTEST_HAS_ASSERT_HEADER CPPTEST_SYSTEM_HEADERS_ALLOWED
#endif

#ifndef CPPTEST_HAS_SIGNAL_HEADER
#  define CPPTEST_HAS_SIGNAL_HEADER CPPTEST_SYSTEM_HEADERS_ALLOWED
#endif

#ifndef CPPTEST_HAS_MEMORY_HEADER
#  define CPPTEST_HAS_MEMORY_HEADER CPPTEST_SYSTEM_HEADERS_ALLOWED
#endif

#ifndef CPPTEST_HAS_UNISTD_HEADER
#  define CPPTEST_HAS_UNISTD_HEADER CPPTEST_SYSTEM_HEADERS_ALLOWED
#endif

#ifndef CPPTEST_HAS_FCNTL_HEADER
#  define CPPTEST_HAS_FCNTL_HEADER CPPTEST_SYSTEM_HEADERS_ALLOWED
#endif

#ifndef CPPTEST_HAS_SYS_UIO_HEADER
#  define CPPTEST_HAS_SYS_UIO_HEADER CPPTEST_SYSTEM_HEADERS_ALLOWED
#endif

#ifndef CPPTEST_HAS_SYS_MMAN_HEADER
#  define CPPTEST_HAS_SYS_MMAN_HEADER CPPTEST_SYSTEM_HEADERS_ALLOWED
#endif

#ifndef CPPTEST_HAS_SYS_STAT_HEADER
#  define CPPTEST_HAS_SYS_STAT_HEADER CPPTEST_SYSTEM_HEADERS_ALLOWED
#endif

#ifndef CPPTEST_HAS_SYS_TIME_HEADER
#  define CPPTEST_HAS_SYS_TIME_HEADER CPPTEST_SYSTEM_HEADERS_ALLOWED
#endif

#ifndef CPPTEST_HAS_AIO_HEADER
#  define CPPTEST_HAS_AIO_HEADER CPPTEST_SYSTEM_HEADERS_ALLOWED
#endif

#ifndef CPPTEST_HAS_UTIME_HEADER
#  define CPPTEST_HAS_UTIME_HEADER CPPTEST_SYSTEM_HEADERS_ALLOWED
#endif

#ifndef CPPTEST_HAS_STRTOLD
#  define CPPTEST_HAS_STRTOLD 0
#endif

#ifndef CPPTEST_HAS_STRTOD
#  define CPPTEST_HAS_STRTOD 0
#endif

#ifndef CPPTEST_HAS_INTERLOCKED_INCREMENT
#  define CPPTEST_HAS_INTERLOCKED_INCREMENT 0
#endif

#ifndef CPPTEST_HAS_SYNC_FETCH_AND_ADD
#  define CPPTEST_HAS_SYNC_FETCH_AND_ADD 0
#endif

#ifndef CPPTEST_HAS_SNPRINTF
#  define CPPTEST_HAS_SNPRINTF 1
#endif

#include "cpptest_features.h"


#ifndef CPPTEST_GETHOSTBYNAME_ENABLED
#  define CPPTEST_GETHOSTBYNAME_ENABLED 1
#endif

#if CPPTEST_THREADS_ENABLED
#  ifndef CPPTEST_WEAK_THREADS_ENABLED
#    if defined(CPPTEST_WIN32) || defined (CPPTEST_VXWORKS)
#      define CPPTEST_WEAK_THREADS_ENABLED 0
#    else
#      define CPPTEST_WEAK_THREADS_ENABLED 1
#    endif
#  endif
#endif

#ifndef CPPTEST_FILE_IO_NOT_THREAD_SAFE
#  define CPPTEST_FILE_IO_NOT_THREAD_SAFE 0
#endif

#ifndef CPPTEST_SOCKET_IO_NOT_THREAD_SAFE
#  define CPPTEST_SOCKET_IO_NOT_THREAD_SAFE 0
#endif

#ifndef CPPTEST_RUNTIME_LOG_ENABLED
#  define CPPTEST_RUNTIME_LOG_ENABLED 1
#endif

#if defined(USE_CUSTOM_SERIAL_COMMUNICATION_CHANNEL)
#  define USE_CUSTOM_COMMUNICATION_CHANNEL 1
#endif

#if !defined(CPPTEST_USE_FILE_COMMUNICATION) && \
    !defined(CPPTEST_USE_FILE_BUFFERED_COMMUNICATION) && \
    !defined(CPPTEST_USE_FILE_SPLIT_COMMUNICATION) && \
    !defined(CPPTEST_USE_UNIX_SOCKET_COMMUNICATION) && \
    !defined(CPPTEST_USE_UNIX_SOCKET_UDP_COMMUNICATION) && \
    !defined(CPPTEST_USE_WIN_SOCKET_COMMUNICATION) && \
    !defined(CPPTEST_USE_RS232_WIN_COMMUNICATION) && \
    !defined(CPPTEST_USE_RS232_UNIX_COMMUNICATION) && \
    !defined(CPPTEST_USE_ITM_COMMUNICATION) && \
    !defined(CPPTEST_USE_RS232_STM32F103ZE_COMMUNICATION) && \
    !defined(CPPTEST_USE_CUSTOM_COMMUNICATION) && \
    !defined(CPPTEST_USE_HEW_SIMIO_COMMUNICATION) && \
    !defined(CPPTEST_USE_LAUTERBACH_FDX_COMMUNICATION) && \
    !defined(CPPTEST_USE_TEMPLATE_COMMUNICATION) && \
    !defined(CPPTEST_USE_RENRX_GDB_COMMUNICATION) && \
    !defined(USE_CUSTOM_COMMUNICATION_CHANNEL)
#  define CPPTEST_USE_FILE_COMMUNICATION
#endif

#ifndef CPPTEST_NAMESPACES_ALLOWED
#  define CPPTEST_NAMESPACES_ALLOWED 1
#endif /* CPPTEST_NAMESPACES_ALLOWED */

#ifndef CPPTEST_USE_STD_NS
#  define CPPTEST_USE_STD_NS 1
#endif

#ifndef CPPTEST_STD_NS_NAME
#  define CPPTEST_STD_NS_NAME std
#endif 

#if CPPTEST_USE_STD_NS && defined(__cplusplus)
#  define CPPTEST_std CPPTEST_STD_NS_NAME:: /* prefix for C++-lib symbols */
#  ifdef CPPTEST_USE_CPP_HEADERS /* using C++-style headers, eg. cstdlib, cstdio */
#    define CPPTEST_C_std CPPTEST_std  /* prefix for C-lib symbols */
#    define CPPTEST_using_namespace_std using namespace CPPTEST_STD_NS_NAME;
#  else
#    define CPPTEST_C_std
#    define CPPTEST_using_namespace_std
#  endif
#else
#  define CPPTEST_std
#  define CPPTEST_C_std
#  define CPPTEST_using_namespace_std
#endif

#ifndef CPPTEST_WCHAR_TYPE
#  define CPPTEST_WCHAR_TYPE wchar_t
#endif

#ifndef CPPTEST_const_wchar_t_ptr_CAST
#  define CPPTEST_const_wchar_t_ptr_CAST
#endif

#ifndef CPPTEST_USE_TRY_CATCH_EXCEPTION_CHECKING
#  if CPPTEST_EXCEPTIONS_ENABLED && !CPPTEST_USE_UNCAUGHT_EXCEPTION_CHECKING
#    define CPPTEST_USE_TRY_CATCH_EXCEPTION_CHECKING 1
#  else
#    define CPPTEST_USE_TRY_CATCH_EXCEPTION_CHECKING 0
#  endif
#endif

#ifndef CPPTEST_USE_TRY_CATCH_EXCEPTION_CHECKING_IN_HANDLER
#  if CPPTEST_EXCEPTIONS_ENABLED && !CPPTEST_USE_UNCAUGHT_EXCEPTION_CHECKING
#    define CPPTEST_USE_TRY_CATCH_EXCEPTION_CHECKING_IN_HANDLER 1
#  else
#    define CPPTEST_USE_TRY_CATCH_EXCEPTION_CHECKING_IN_HANDLER 0
#  endif
#endif

#ifndef CPPTEST_USE_SET_TERMINATE
#  if CPPTEST_EXCEPTIONS_ENABLED && CPPTEST_USE_UNCAUGHT_EXCEPTION_CHECKING
#    define CPPTEST_USE_SET_TERMINATE 1
#  else
#    define CPPTEST_USE_SET_TERMINATE 0
#  endif
#endif

#ifndef CPPTEST_HAS_UNCAUGHT_EXCEPTION_PROBLEM_IN_EXCEPTION_HANDLER_IF_RETHROW
#  define CPPTEST_HAS_UNCAUGHT_EXCEPTION_PROBLEM_IN_EXCEPTION_HANDLER_IF_RETHROW 0
#endif

#ifndef CPPTEST_CSV_DATA_SOURCE_POSIX
#  if CPPTEST_DATA_SOURCES_ENABLED
#    if CPPTEST_CSV_DATA_SOURCE_ENABLED
#      define CPPTEST_CSV_DATA_SOURCE_POSIX 0
#    endif
#  endif
#endif

#ifndef CPPTEST_MAX_MESSAGE_SIZE
#    define CPPTEST_MAX_MESSAGE_SIZE 8192U
#endif

#ifndef CPPTEST_COLLECT_STACK_TRACE
#  define CPPTEST_COLLECT_STACK_TRACE 1
#endif

#ifndef CPPTEST_STACKTRACE_BLOCK_SIZE
#  define CPPTEST_STACKTRACE_BLOCK_SIZE 24U
#endif

#ifndef CPPTEST_DEFINE_INVALID_SIZE_ARGUMENT_FOR_IOC
#  define CPPTEST_DEFINE_INVALID_SIZE_ARGUMENT_FOR_IOC 1
#endif

#ifndef _CPPTEST_ENTRY_POINT_TYPE_DEFINED
#  define CPPTEST_EPT_main
#  define _CPPTEST_ENTRY_POINT_TYPE_DEFINED
#endif

#ifndef CPPTEST_RUNTIME_API
#  ifdef CPPTEST_RUNTIME_IMPLEMENTATION
#    define CPPTEST_RUNTIME_API EXTERN_C CPPTEST_EXPORT
#  else
#    define CPPTEST_RUNTIME_API EXTERN_C CPPTEST_IMPORT
#  endif
#endif

#ifndef CPPTEST_ALLOC_IMPL
#  define CPPTEST_ALLOC_IMPL 0
#endif

#ifndef CPPTEST_PRINTF_IMPL
#  define CPPTEST_PRINTF_IMPL 0
#endif

#ifndef CPPTEST_USE_TARGET_SYSTEM
#  if CPPTEST_ALLOC_IMPL
#    define CPPTEST_USE_TARGET_SYSTEM 0
#  else
#    define CPPTEST_USE_TARGET_SYSTEM 1
#  endif
#endif

#ifndef CPPTEST_EXIT
/* both abort() and exit() are declared in <stdlib.h>
 in case of own CPPTEST_EXIT provide appropriate CPPTEST_EXIT_DECL as well */
#  define CPPTEST_EXIT_DECL
#  ifdef CPPTEST_USE_ABORT_FOR_EXIT
#     define CPPTEST_EXIT(code) abort()
#  else
#     define CPPTEST_EXIT(code) exit(code)
#  endif
#endif

#ifndef CPPTEST_DEFAULT_RUNTIME_AUTO_INIT
#  define CPPTEST_DEFAULT_RUNTIME_AUTO_INIT 0
#endif

#ifndef CPPTEST_DEFAULT_DELAY_COMMUNICATION_INIT
#  if CPPTEST_DEFAULT_RUNTIME_AUTO_INIT
#    define CPPTEST_DEFAULT_DELAY_COMMUNICATION_INIT 0
#  else
#    define CPPTEST_DEFAULT_DELAY_COMMUNICATION_INIT 1
#  endif
#endif

#ifndef CPPTEST_USE_EXTENDED_DATA_SOURCE_ROW_SIGNATURE
#  define CPPTEST_USE_EXTENDED_DATA_SOURCE_ROW_SIGNATURE 0
#endif

#ifndef CPPTEST_USE_GLOBAL_OBJECTS_TO_INIT_RUNTIME
#  define CPPTEST_USE_GLOBAL_OBJECTS_TO_INIT_RUNTIME 1
#endif

#ifndef CPPTEST_USE_GLOBAL_AUTO_INITIALIZER
#  define CPPTEST_USE_GLOBAL_AUTO_INITIALIZER CPPTEST_USE_GLOBAL_OBJECTS_TO_INIT_RUNTIME
#endif

#ifndef CPPTEST_USE_GLOBAL_AUTO_FINALIZER
#  define CPPTEST_USE_GLOBAL_AUTO_FINALIZER CPPTEST_USE_GLOBAL_OBJECTS_TO_INIT_RUNTIME
#endif


#ifndef CPPTEST_MIN_ALLOWED_ALL_ALLOCATIONS_SIZE
#  define CPPTEST_MIN_ALLOWED_ALL_ALLOCATIONS_SIZE 1024U
#endif

#ifndef CPPTEST_MAX_ALLOWED_ALL_ALLOCATIONS_SIZE
#  define CPPTEST_MAX_ALLOWED_ALL_ALLOCATIONS_SIZE (UINT_MAX/4U)
#endif

#ifndef CPPTEST_MIN_ALLOWED_INVALID_ALLOCATIONS_COUNT
#  define CPPTEST_MIN_ALLOWED_INVALID_ALLOCATIONS_COUNT 8U
#endif

#ifndef CPPTEST_MAX_ALLOWED_INVALID_ALLOCATIONS_COUNT
#  define CPPTEST_MAX_ALLOWED_INVALID_ALLOCATIONS_COUNT 128U
#endif

#ifndef CPPTEST_ALLOCATION_SUFFIX_SIZE
#  define CPPTEST_ALLOCATION_SUFFIX_SIZE 8U
#endif

#ifndef CPPTEST_MAX_ALLOWED_STACK_TRACE_DEPTH
#  define CPPTEST_MAX_ALLOWED_STACK_TRACE_DEPTH 65535U
#endif /* CPPTEST_MAX_ALLOWED_STACK_TRACE_DEPTH */

#ifndef CPPTEST_MAX_MEM_EQUAL_ASSERTION_SIZE_ALLOWED
#  define CPPTEST_MAX_MEM_EQUAL_ASSERTION_SIZE_ALLOWED 1024U
#endif

#ifndef CPPTEST_MAX_POST_CONDITION_MEM_BUFF_SIZE_ALLOWED
#  define CPPTEST_MAX_POST_CONDITION_MEM_BUFF_SIZE_ALLOWED 1024U
#endif

#ifndef CPPTEST_DEFAULT_SIZE_OF_CHAR
#  define CPPTEST_DEFAULT_SIZE_OF_CHAR 8U
#endif

#ifndef CPPTEST_MAX_ALLOWED_PATH_LENGTH
#  define CPPTEST_MAX_ALLOWED_PATH_LENGTH 260
#endif

#ifndef CPPTEST_FMT_BUFFER_SIZE
#  define CPPTEST_FMT_BUFFER_SIZE 24
#endif

/* Using 1 as TRUE may causes some optymalizations which may broke the code */
#define CPPTEST_INT_TRUE 33

#define CPPTEST_QUOTE_SLAVE(X) #X
#define CPPTEST_QUOTE(X) CPPTEST_QUOTE_SLAVE(X)

#ifndef CPT_TO_STRING
#  define CPT_TO_STRING(X) #X
#endif

#ifndef CPPTEST_ANALYZE_HEAP
#  define CPPTEST_ANALYZE_HEAP 0
#endif

#ifndef CPPTEST_MARSHALLING
#  if defined(__i386__) || defined(_M_IX86)
/* optimization for little endian systems
 TODO: could be done in per-compiler headers instead
*/
#    define CPPTEST_MARSHALLING 0
#  else
/* marshalling enabled by default */
#    define CPPTEST_MARSHALLING 1
#  endif
#endif

#ifndef CPPTEST_HAS_RVALUE_REFERENCES
#  if !defined(__cplusplus)
#    define CPPTEST_HAS_RVALUE_REFERENCES 0
#  elif defined(__cplusplus) && __cplusplus >= 201100
#    define CPPTEST_HAS_RVALUE_REFERENCES 1
#  elif defined(_MSC_VER) && _MSC_VER >= 1600
#    define CPPTEST_HAS_RVALUE_REFERENCES 1
#  elif defined(__GXX_EXPERIMENTAL_CXX0X__)
#    define CPPTEST_HAS_RVALUE_REFERENCES 1
#  else
#    define CPPTEST_HAS_RVALUE_REFERENCES 0
#  endif
#endif

#ifndef CPPTEST_TRIGGER_ENABLED
#  define CPPTEST_TRIGGER_ENABLED 1
#endif /* CPPTEST_TRIGGER_ENABLED */
 
#ifndef TRIGGER_NAMESPACE_ALLOWED
#  define TRIGGER_NAMESPACE_ALLOWED CPPTEST_NAMESPACES_ALLOWED
#endif /* TRIGGER_NAMESPACE_ALLOWED */

#ifndef TRIGGER_INLINE_RUNTIME
#  define TRIGGER_INLINE_RUNTIME 0
#endif /* TRIGGER_INLINE_RUNTIME */

#ifndef TRIGGER_HAS_LONG_LONG
#  define TRIGGER_HAS_LONG_LONG CPPTEST_HAS_LONG_LONG
#endif

#ifndef TRIGGER_HAS_LONG_DOUBLE
#  define TRIGGER_HAS_LONG_DOUBLE CPPTEST_HAS_LONG_DOUBLE
#endif

#ifndef TRIGGER_FLOAT_AND_DOUBLE_ARE_DISTINCT
#  define TRIGGER_FLOAT_AND_DOUBLE_ARE_DISTINCT 1
#endif /* TRIGGER_FLOAT_AND_DOUBLE_ARE_DISTINCT */

#if CPPTEST_WCHAR_ENABLED
#  ifndef TRIGGER_WCHAR_T
#    define TRIGGER_WCHAR_T CPPTEST_WCHAR_TYPE
#  endif /* TRIGGER_WCHAR_T */
#endif /* CPPTEST_WCHAR_ENABLED */

#ifndef TRIGGER_ERROR_HANDLER
#  define TRIGGER_ERROR_HANDLER   cpptestTriggerError
#endif

#ifndef TRIGGER_FAIL_HANDLER
#  define TRIGGER_FAIL_HANDLER  cpptestTriggerAssertion
#endif

#ifndef TRIGGER_REPORT_HANDLER
#  define TRIGGER_REPORT_HANDLER  cpptestTriggerReport
#endif

#ifndef TRIGGER_MALLOC_FUNC
#  define TRIGGER_MALLOC_FUNC cpptestMalloc
#endif /* TRIGGER_MALLOC_FUNC */

#ifndef TRIGGER_FREE_FUNC
#  define TRIGGER_FREE_FUNC cpptestFree
#endif /* TRIGGER_FREE_FUNC */

#ifdef CPPTEST_DRIVER
#  define TRIGGER_IMPLEMENTATION 1
#endif /* CPPTEST_DRIVER */


#endif  /* __CppTest_PortInfo_h__ */
