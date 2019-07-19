/*
 * (C) Copyright Parasoft Corporation 1998 - 2019.  All rights reserved.
 * The use of this source code is governed by the general Parasoft License Agreement.
 * Redistribution strictly prohibited.
 */

#define CPPTEST_DRIVER

#include "cpptest.h"
#include "cpptest_runtime.h"

/* Make sure the __MINGW_USE_UNDERSCORE_PREFIX will be 0 for x64 */
#if !defined(_WIN64) && defined(CPPTEST_WIN32) && defined(__x86_64)
#  define _WIN64
#endif
/* Fix problem with asm keyword used in excpt.h */
#if defined(__x86_64) && defined(__STRICT_ANSI__) && defined(__GNUC__) && !defined(__cplusplus)
#  define asm __asm__
#endif

#if defined(CPPTEST_WIN32) && CPPTEST_SEH_EXCEPTIONS_ENABLED
# define NOMINMAX
# include <windows.h>
#endif

#if CPPTEST_EXTERNAL_ASSERTS_AND_POST_CONDITIONS_ENABLED
# include CPPTEST_string
# include CPPTEST_stdio
# if !defined(_MSC_VER) && !defined(__MINGW32__)
#  include CPPTEST_stdlib
#  include <unistd.h>
#  include <sys/types.h>
#  include <sys/wait.h>
# endif
#endif

CPPTEST_EXIT_DECL

#if CPPTEST_USE_TARGET_SYSTEM
# include CPPTEST_string
#endif

#if CPPTEST_DATA_SOURCES_ENABLED
# include CPPTEST_stdio
# define CPPTEST_DS_MALLOC(TYPE) (TYPE*)cpptestDsMalloc(sizeof(TYPE))
# define CPPTEST_DS_FREE(PTR) cpptestDsFree(PTR)
# if CPPTEST_CSV_DATA_SOURCE_ENABLED
#  include CPPTEST_string
#  if CPPTEST_CSV_DATA_SOURCE_POSIX
#   include <sys/types.h>
#   include <sys/stat.h>
#   include <fcntl.h>
#   define CPPTEST_FD_TYPE int
#   define CPPTEST_CHECK_READ(FD, SIZE) (SIZE >= 0)
#   define CPPTEST_CHECK_FD(FD) (FD >= 0)
#   ifdef _MSC_VER
#    include <io.h>
#    define CPPTEST_READ(FD, BUFFER, SIZE) _read(FD, BUFFER, SIZE)
#    define CPPTEST_OPEN(FILENAME) _open(FILENAME, O_RDONLY | O_BINARY)
#    define CPPTEST_CLOSE(FD) _close(FD)
#   else
#    include <unistd.h>
#    define CPPTEST_READ(FD, BUFFER, SIZE) read(FD, BUFFER, SIZE)
#    define CPPTEST_OPEN(FILENAME) open(FILENAME, O_RDONLY | O_BINARY)
#    define CPPTEST_CLOSE(FD) close(FD)
#   endif
#  else
#   define CPPTEST_FD_TYPE CPPTEST_C_std FILE*
#   define CPPTEST_READ(FD, BUFFER, SIZE) CPPTEST_C_std fread(BUFFER, sizeof(char), SIZE, FD);
#   define CPPTEST_CHECK_READ(FD, SIZE) (CPPTEST_C_std ferror(FD) == 0)
#   define CPPTEST_OPEN(FILENAME) CPPTEST_C_std fopen(FILENAME, "rb")
#   define CPPTEST_CHECK_FD(FD) (FD != 0)
#   define CPPTEST_CLOSE(FD) CPPTEST_C_std fclose(FD)
#  endif
# endif
#endif

#if CPPTEST_CSV_DATA_SOURCE_ENABLED || CPPTEST_EXTERNAL_ASSERTS_AND_POST_CONDITIONS_ENABLED
# ifndef errno /* errno is a macro - prevent expansion in following line */
#  include CPPTEST_errno
# endif
# define CPPTEST_strerror (CPPTEST_C_std strerror(CPPTEST_C_std errno))
#endif

#ifdef CPPTEST_CYGWIN
#  include CPPTEST_signal
#endif

#ifdef __cplusplus
#  if CPPTEST_EXCEPTIONS_ENABLED && CPPTEST_USE_UNCAUGHT_EXCEPTION_CHECKING
#    include <exception>
#  endif
#  if CPPTEST_INIT_QT
#    include <QApplication>
#  endif
#  define CPPTEST_GLOBAL_PREFIX ::
#else
#  define CPPTEST_GLOBAL_PREFIX
#endif


#if CPPTEST_C_STREAMS_REDIRECT_ENABLED || CPPTEST_CPP_STREAMS_REDIRECT_ENABLED


#if CPPTEST_C_STREAMS_REDIRECT_ENABLED
#  include CPPTEST_stdio
#endif

#if CPPTEST_CPP_STREAMS_REDIRECT_ENABLED
#if __SUNPRO_CC_COMPAT == 4
#    include <iostream.h>
#  else
#    include <iostream>
#  endif
#endif


void CDECL_CALL CppTest_flushStdOutput()
{
#if CPPTEST_C_STREAMS_REDIRECT_ENABLED
    fflush(stdout);
#endif

#if CPPTEST_CPP_STREAMS_REDIRECT_ENABLED
#  if CPPTEST_USE_WSTREAMS
    CPPTEST_std wcout.flush();
#  else
    CPPTEST_std cout.flush();
#  endif
#endif
}

void CDECL_CALL CppTest_flushStdError()
{
#if CPPTEST_C_STREAMS_REDIRECT_ENABLED
    fflush(stderr);
#endif
    
#if CPPTEST_CPP_STREAMS_REDIRECT_ENABLED
#  if CPPTEST_USE_WSTREAMS
    CPPTEST_std wcerr.flush();
    CPPTEST_std wclog.flush();
#  else
    CPPTEST_std cerr.flush();
    CPPTEST_std clog.flush();
#  endif
#endif
}

#if defined (__CYGWIN__)
EXTERN_C_LINKAGE int cygwin_attach_handle_to_fd(char *name, int fd, long handle, int bin, int access);
EXTERN_C_LINKAGE int close(int);

int CDECL_CALL CppTest_streamDup(long tmpFd, int fd)
{
    close(fd);
    return cygwin_attach_handle_to_fd("", fd, tmpFd, 0, 0) == fd;
}
#endif

#define CLEARSTDINPUTBUFFERSIZE 512
void CDECL_CALL CppTest_clearStdInput()
{
#if CPPTEST_C_STREAMS_REDIRECT_ENABLED
    {
    char buf[CLEARSTDINPUTBUFFERSIZE];
    /* This function will *hang* if there is real terminal behind stdin */
    while (CPPTEST_C_std fread(buf, 1, CLEARSTDINPUTBUFFERSIZE, stdin)) {}
    clearerr(stdin);
    }
#endif
#if CPPTEST_CPP_STREAMS_REDIRECT_ENABLED
#  if CPPTEST_USE_WSTREAMS
    {
    CppTest_wchar_t wbuf[CLEARSTDINPUTBUFFERSIZE];
    while (CPPTEST_std wcin.read(wbuf, CLEARSTDINPUTBUFFERSIZE));
    CPPTEST_std wcin.clear();
    }
#  else
    {
    char buf[CLEARSTDINPUTBUFFERSIZE];
    while (CPPTEST_std cin.read(buf, CLEARSTDINPUTBUFFERSIZE));
    CPPTEST_std cin.clear();
    }
#  endif
#endif
}


#endif /* CPPTEST_C_STREAMS_REDIRECT_ENABLED || CPPTEST_CPP_STREAMS_REDIRECT_ENABLED */


#if CPPTEST_EXCEPTIONS_ENABLED && CPPTEST_USE_SET_TERMINATE
static void CDECL_CALL cpptestUnhandledCppExceptionInThreadHandler()
{
    cpptestUnhandledCppExceptionInThread();
}
#endif /* CPPTEST_EXCEPTIONS_ENABLED && CPPTEST_USE_SET_TERMINATE */


EXTERN_C_LINKAGE_START


#if CPPTEST_DATA_SOURCES_ENABLED


CppTest_GetDataSourceFPtr CppTest_getDSFPtr = 0;


void CDECL_CALL CppTest_DsFinalize(CppTest_DataSource* ds)
{
    CPPTEST_DS_FREE(ds->data);
    CPPTEST_DS_FREE(ds);
}

void CDECL_CALL CppTest_DsDumpInt(CppTestExpandableString* es, int i)
{
    if (i / 10) {
        CppTest_DsDumpInt(es, i / 10);
    }
    cpptestExpandableStringAppendChar(es, (i % 10) + '0');
}

const char* CDECL_CALL CppTest_DsGetMemBuffer(CppTest_DataSource* ds, const char* name, unsigned int* size)
{
    const char* value = (ds->getCStr)(ds, name);
    if (size) {
        *size = value ? cpptestStrLen(value) : 0;
    }
    return value;
}

static const char* CDECL_CALL skipIntegerSuffix(const char* str)
{
    int hasUnsignedSuffix = 0;
    
    if (!str) {
        return 0;
    }

    if (*str == 'u' || *str == 'U') {
        ++str;
        hasUnsignedSuffix = 1;
    }
    if (*str == 'l') {
        ++str;
        if (*str == 'l') {
            ++str;
        }
    } else if (*str == 'L') {
        ++str;
        if (*str == 'L') {
            ++str;
        }
    }
    if (!hasUnsignedSuffix) {
        if (*str == 'u' || *str == 'U') {
            ++str;
        }
    }

    return str;
}

CPPTEST_INTEGER CDECL_CALL CppTest_DsGetInteger(struct CppTest_DataSource* ds, const char* name)
{
    const char* value = (ds->getMemBuffer)(ds, name, 0);

    if (value) {
        CPPTEST_INTEGER ret = 0;
        const char* end = skipIntegerSuffix(cpptestStrToInteger(value, &ret));

        if ((end != 0) && (*end == '\0')) {
            return ret;
        }
    }

    cpptestDsErrorInValue(value);

    /* Not reached */
    return 0;
}

CPPTEST_UINTEGER CDECL_CALL CppTest_DsGetUInteger(struct CppTest_DataSource* ds, const char* name)
{
    const char* value = (ds->getMemBuffer)(ds, name, 0);

    if (value) {
        CPPTEST_UINTEGER ret = 0;
        const char* end = skipIntegerSuffix(cpptestStrToUInteger(value, &ret));
        
        if ((end != 0) && (*end == '\0')) {
            return ret;
        }
    }
 
    cpptestDsErrorInValue(value);
 
    /* Not reached */
    return 0;
}
#ifndef __KERNEL__

#if defined(CPPTEST_STRING_TO_FLOAT)

#  ifdef CPPTEST_STRING_TO_FLOAT_DECL
CPPTEST_STRING_TO_FLOAT_DECL
#  endif

static CPPTEST_FLOAT CDECL_CALL dsStrToFloat(const char* str, int* err)
{
    *err = 0;
    return CPPTEST_STRING_TO_FLOAT(str);
}

#elif CPPTEST_HAS_STRTOLD || CPPTEST_HAS_STRTOD || defined(CPPTEST_STR_TO_FLOAT)

static CPPTEST_FLOAT CDECL_CALL dsStrToFloat(const char* str, int* err)
{
    CPPTEST_FLOAT result = 0;
    char* end = (char*)str;

#  if CPPTEST_HAS_STRTOLD
    result = CPPTEST_C_std strtold(str, &end);
#  elif CPPTEST_HAS_STRTOD
    result = CPPTEST_C_std strtod(str, &end);
#else
    /* deprecated */
    CPPTEST_STR_TO_FLOAT(result, str, end);
#  endif
    if (*end == 'f' || *end == 'F' || *end == 'l' || *end == 'L') {
        ++end;
    }
    *err = ((*end == '\0') ? 0 : 1);

    return result;
}

#elif defined(CPPTEST_SCANF_FLOAT) && defined(CPPTEST_SCANF_FLOAT_FMT)

static CPPTEST_FLOAT CDECL_CALL dsStrToFloat(const char* str, int* err)
{
    CPPTEST_SCANF_FLOAT result = 0;
    char guard1 = '\0';
    char guard2 = '\0';
    int number = CPPTEST_C_std sscanf(str, CPPTEST_SCANF_FLOAT_FMT "%c%c", &result, &guard1, &guard2);
    if (guard1 == 'f' || guard1 == 'F' || guard1 == 'l' || guard1 == 'L') {
        --number;
    }
    *err = ((number == 1) ? 0 : 1);

    return result;
}

#else

static CPPTEST_FLOAT CDECL_CALL dsStrToFloat(const char* str, int* err)
{
    *err = 1;
    return 0;
}

#endif


CPPTEST_FLOAT CDECL_CALL CppTest_DsGetFloat(struct CppTest_DataSource* ds, const char* name)
{
    const char* value = (ds->getMemBuffer)(ds, name, 0);
    
    if (value) {
        int err = 0;
        CPPTEST_FLOAT ret = dsStrToFloat(value, &err);
        if (!err) {
            return ret;
        }
    }

    cpptestDsErrorInValue(value);
 
    /* Not reached */
    return 0;
}

#endif /* !__KERNEL__ */

int CDECL_CALL CppTest_DsGetBool(struct CppTest_DataSource* ds, const char* name)
{
    const char* value = (ds->getMemBuffer)(ds, name, 0);

    if (value) {
        int ret = 0;
        const char* end = cpptestStrToBool(value, &ret);
        if (end && *end == '\0') {
            return ret;
        }
    }

    cpptestDsErrorInValue(value);
 
    /* Not reached */
    return 0;
}

char CDECL_CALL CppTest_DsGetChar(struct CppTest_DataSource* ds, const char* name)
{
    const char* value = (ds->getMemBuffer)(ds, name, 0);

    if (value) {
        return *value;
    }

    cpptestDsErrorInValue(value);
 
    /* Not reached */
    return 0;
}

int CDECL_CALL CppTest_DsErrorHasMoreData(CppTest_DataSource* ds)
{
    return 0;
}

const char* CDECL_CALL CppTest_DsErrorGetDataSignature(CppTest_DataSource* ds)
{
    return "";
}

unsigned int CDECL_CALL CppTest_DsErrorGetIteration(CppTest_DataSource* ds)
{
    return 0;
}

static CppTest_DataSource* CDECL_CALL CppTest_DsError(void)
{
    CppTest_DataSource* ds = CPPTEST_DS_MALLOC(CppTest_DataSource);
 
    ds->data = 0;
    
    ds->hasColumn    = 0;
    ds->getCStr      = 0;
#if CPPTEST_WCHAR_ENABLED
    ds->getWCStr     = 0;
#endif
    ds->getMemBuffer = 0;
    ds->getInteger   = 0;
    ds->getUInteger  = 0;
    ds->getFloat     = 0;
    ds->getBool      = 0;

    ds->loadNextData     = &CppTest_DsErrorHasMoreData;
    ds->hasMoreData      = &CppTest_DsErrorHasMoreData;
#if CPPTEST_USE_EXTENDED_DATA_SOURCE_ROW_SIGNATURE
    ds->getDataSignature = &CppTest_DsErrorGetDataSignature;
#endif
    ds->getIteration     = &CppTest_DsErrorGetIteration;
    ds->finalize         = &CppTest_DsFinalize;
   
    return ds;
}


typedef struct CppTest_DsRepeatData
{
    int count;
    int _cpptest_current;
    
#if CPPTEST_USE_EXTENDED_DATA_SOURCE_ROW_SIGNATURE
    char* dataSignature;
#endif
} CppTest_DsRepeatData;

int CDECL_CALL CppTest_DsRepeatLoadNextData(CppTest_DataSource* ds)
{
    CppTest_DsRepeatData* dsR = (CppTest_DsRepeatData*)ds->data;
    ++dsR->_cpptest_current;
#if CPPTEST_USE_EXTENDED_DATA_SOURCE_ROW_SIGNATURE
    if (dsR->dataSignature) {
        CPPTEST_DS_FREE(dsR->dataSignature);
        dsR->dataSignature = 0;
    }
#endif
    return 1;
}

unsigned int CDECL_CALL CppTest_DsRepeatGetIteration(CppTest_DataSource* ds)
{
    CppTest_DsRepeatData* dsR = (CppTest_DsRepeatData*)ds->data;
    return dsR->_cpptest_current;
}

int CDECL_CALL CppTest_DsRepeatHasMoreData(CppTest_DataSource* ds)
{
    CppTest_DsRepeatData* dsR = (CppTest_DsRepeatData*)ds->data;
    return (dsR->_cpptest_current < dsR->count);
}
 
#if CPPTEST_USE_EXTENDED_DATA_SOURCE_ROW_SIGNATURE
const char* CDECL_CALL CppTest_DsRepeatGetDataSignature(CppTest_DataSource* ds)
{
    const char it[] = "Iteration: ";

    CppTest_DsRepeatData* dsR = (CppTest_DsRepeatData*)ds->data;
    if (dsR->dataSignature == 0) {
        CppTestExpandableString es;

        cpptestExpandableStringInit(&es, 10);

        cpptestExpandableStringAppend(&es, it, sizeof(it) - 1);
        CppTest_DsDumpInt(&es, dsR->_cpptest_current);

        dsR->dataSignature = cpptestStrDup(es.ptr);

        cpptestExpandableStringDispose(&es);
    }

    return dsR->dataSignature;
}
#endif

void CDECL_CALL CppTest_DsRepeatFinalize(CppTest_DataSource* ds)
{
#if CPPTEST_USE_EXTENDED_DATA_SOURCE_ROW_SIGNATURE
    CppTest_DsRepeatData* dsR = (CppTest_DsRepeatData*)ds->data;
    if (dsR->dataSignature) {
        CPPTEST_DS_FREE(dsR->dataSignature);
    }
#endif
    CppTest_DsFinalize(ds);
}

CppTest_DataSource* CDECL_CALL CppTest_DsRepeat(int count)
{
    CppTest_DataSource* ds = CPPTEST_DS_MALLOC(CppTest_DataSource);
    CppTest_DsRepeatData* repeat = CPPTEST_DS_MALLOC(CppTest_DsRepeatData);
    
    repeat->count = count;
    repeat->_cpptest_current = 0;
#if CPPTEST_USE_EXTENDED_DATA_SOURCE_ROW_SIGNATURE
    repeat->dataSignature = 0;
#endif

    ds->data = repeat;
    
    ds->hasColumn    = 0;
    ds->getCStr      = 0;
#if CPPTEST_WCHAR_ENABLED
    ds->getWCStr     = 0;
#endif
    ds->getMemBuffer = 0;
    ds->getInteger   = 0;
    ds->getUInteger  = 0;
    ds->getFloat     = 0;
    ds->getBool      = 0;
    ds->getChar      = 0;
    
    ds->loadNextData = &CppTest_DsRepeatLoadNextData;
    ds->hasMoreData  = &CppTest_DsRepeatHasMoreData;
#if CPPTEST_USE_EXTENDED_DATA_SOURCE_ROW_SIGNATURE
    ds->getDataSignature = &CppTest_DsRepeatGetDataSignature;
#endif
    ds->getIteration = &CppTest_DsRepeatGetIteration;
    ds->finalize = &CppTest_DsRepeatFinalize;
    
    return ds;
}

typedef struct CppTest_DsArrayData
{
    const char** data;
    int rows;
    int columns;
    int _cpptest_current;
    
#if CPPTEST_USE_EXTENDED_DATA_SOURCE_ROW_SIGNATURE
    char* dataSignature;
#endif
} CppTest_DsArrayData;

int CDECL_CALL CppTest_DsArrayLoadNextData(CppTest_DataSource* ds)
{
    CppTest_DsArrayData* dsA = (CppTest_DsArrayData*)ds->data;
    ++dsA->_cpptest_current;
#if CPPTEST_USE_EXTENDED_DATA_SOURCE_ROW_SIGNATURE
    if (dsA->dataSignature) {
        CPPTEST_DS_FREE(dsA->dataSignature);
        dsA->dataSignature = 0;
    }
#endif
    return 1;
}

unsigned int CDECL_CALL CppTest_DsArrayGetIteration(CppTest_DataSource* ds)
{
    CppTest_DsArrayData* dsA = (CppTest_DsArrayData*)ds->data;
    return dsA->_cpptest_current;
}

int CDECL_CALL CppTest_DsArrayHasMoreData(CppTest_DataSource* ds)
{
    CppTest_DsArrayData* dsA = (CppTest_DsArrayData*)ds->data;
    return (dsA->_cpptest_current + 1 < dsA->rows);
}

int CDECL_CALL CppTest_DsArrayGetColumn(CppTest_DataSource* ds, const char* name)
{
    CppTest_DsArrayData* dsA = (CppTest_DsArrayData*)ds->data;
    int i = 0;
    for (; i < dsA->columns; ++i) {
        if (cpptestStrCmp(name, dsA->data[i]) == 0) { 
            return i;
        }
    }
    return -1;
}

int CDECL_CALL CppTest_DsArrayHasColumn(CppTest_DataSource* ds, const char* name)
{
    return CppTest_DsArrayGetColumn(ds, name) >= 0;
}

const char* CDECL_CALL CppTest_DsArrayGetCStr(CppTest_DataSource* ds, const char* name)
{
    CppTest_DsArrayData* dsA = (CppTest_DsArrayData*)ds->data;
    int collumn = CppTest_DsArrayGetColumn(ds, name);
    if (collumn >= 0) {
        return dsA->data[dsA->_cpptest_current * dsA->columns + collumn];
    }
    
    cpptestDsUnknownValue(name);
    /* Not reached */
    return 0;
}

#if CPPTEST_WCHAR_ENABLED
const CppTest_wchar_t* CDECL_CALL CppTest_DsArrayGetWCStr(CppTest_DataSource* ds, const char* name)
{
    CppTest_DsArrayData* dsA = (CppTest_DsArrayData*)ds->data;
    int collumn = CppTest_DsArrayGetColumn(ds, name);
    if (collumn >= 0) {
        return (const CppTest_wchar_t*)(dsA->data[dsA->_cpptest_current * dsA->columns + collumn]);
    }
    
    cpptestDsUnknownValue(name);
    /* Not reached */
    return 0;
}
#endif

#if CPPTEST_USE_EXTENDED_DATA_SOURCE_ROW_SIGNATURE
const char* CDECL_CALL CppTest_DsArrayGetDataSignature(CppTest_DataSource* ds)
{
    CppTest_DsArrayData* dsA = (CppTest_DsArrayData*)ds->data;
    
    if (dsA->dataSignature == 0) {
        const char DATA_MSG[] = "Data source row: ";
        const char VALUES_MSG[] = ", Values: ";
        const char NULL_MSG[] = "<NULL PTR>";
        int i = 0;
        CppTestExpandableString es;
        cpptestExpandableStringInit(&es, 10);
            
        cpptestExpandableStringAppend(&es, DATA_MSG, sizeof(DATA_MSG) - 1);
        CppTest_DsDumpInt(&es, dsA->_cpptest_current);
        cpptestExpandableStringAppend(&es, VALUES_MSG, sizeof(VALUES_MSG) - 1);

        for (; i < dsA->columns; ++i) {
            const char* collumn = dsA->data[i];
            const char* data = dsA->data[dsA->_cpptest_current * dsA->columns + i];

            cpptestExpandableStringAppend(&es, collumn, cpptestStrLen(collumn));
            cpptestExpandableStringAppend(&es, "=", 1);
            if (data) {
                cpptestExpandableStringAppend(&es, data, cpptestStrLen(data));
            } else {
                cpptestExpandableStringAppend(&es, NULL_MSG, cpptestStrLen(NULL_MSG));
            }
            cpptestExpandableStringAppend(&es, ", ", 2);
        }

        dsA->dataSignature = cpptestStrDup(es.ptr);

        cpptestExpandableStringDispose(&es);
    }
    return dsA->dataSignature;
}
#endif

void CDECL_CALL CppTest_DsArrayFinalize(CppTest_DataSource* ds)
{
#if CPPTEST_USE_EXTENDED_DATA_SOURCE_ROW_SIGNATURE
    CppTest_DsArrayData* dsA = (CppTest_DsArrayData*)ds->data;
    if (dsA->dataSignature) {
        CPPTEST_DS_FREE(dsA->dataSignature);
    }
#endif
    CppTest_DsFinalize(ds);
}

CppTest_DataSource* CDECL_CALL CppTest_DsArray(const char** data, int rows, int columns)
{
    CppTest_DataSource* ds = CPPTEST_DS_MALLOC(CppTest_DataSource);
    CppTest_DsArrayData* ar = CPPTEST_DS_MALLOC(CppTest_DsArrayData);
    
    ar->data = data;
    ar->rows = rows;
    ar->columns = columns;
    ar->_cpptest_current = 0;
#if CPPTEST_USE_EXTENDED_DATA_SOURCE_ROW_SIGNATURE
    ar->dataSignature = 0;
#endif

    ds->data = ar;
    
    ds->hasColumn    = &CppTest_DsArrayHasColumn;
    ds->getCStr      = &CppTest_DsArrayGetCStr;
#if CPPTEST_WCHAR_ENABLED
    ds->getWCStr     = &CppTest_DsArrayGetWCStr;
#endif
    ds->getMemBuffer = &CppTest_DsGetMemBuffer;
    ds->getInteger   = &CppTest_DsGetInteger;
    ds->getUInteger  = &CppTest_DsGetUInteger;
#ifndef __KERNEL__
    ds->getFloat     = &CppTest_DsGetFloat;
#endif
    ds->getBool      = &CppTest_DsGetBool;
    ds->getChar      = &CppTest_DsGetChar;
    
    ds->loadNextData = &CppTest_DsArrayLoadNextData;
    ds->hasMoreData = &CppTest_DsArrayHasMoreData;
#if CPPTEST_USE_EXTENDED_DATA_SOURCE_ROW_SIGNATURE
    ds->getDataSignature = &CppTest_DsArrayGetDataSignature;
#endif
    ds->getIteration = &CppTest_DsArrayGetIteration;
    
    ds->finalize = &CppTest_DsArrayFinalize;
    
    return ds;
}


#if CPPTEST_CSV_DATA_SOURCE_ENABLED


#ifndef CPPTEST_CSV_SPECIAL_NULL
#define CPPTEST_CSV_SPECIAL_NULL "cpptest$NULL"
#endif

typedef struct CppTest_DsCsvData
{ 
    CppTestList* columns;
    CppTestList* data;

    const char* fileName;
#if CPPTEST_USE_EXTENDED_DATA_SOURCE_ROW_SIGNATURE
    char* dataSignature;
#endif
    char* buffer;
    int bufferSize;
    int bufferMax;
    int iBuffer;

    int trimValues;
    CPPTEST_FD_TYPE fd;
    int error;
    int _cpptest_current;

    unsigned int lineNo;
    unsigned int collumnNo;
    
    char separator;
} CppTest_DsCsvData;


static void CDECL_CALL dsCsvError(CppTest_DsCsvData* dsCsv, CppTest_ErrorCode errorCode, const char * err)
{
    dsCsv->error = 1;
    cpptestDsCsvError(errorCode, err, dsCsv->fileName, dsCsv->lineNo, dsCsv->collumnNo);
}

int CDECL_CALL CppTest_DsCsvPeekChar(CppTest_DsCsvData* dsCsv)
{
    if (dsCsv->iBuffer >= dsCsv->bufferMax) {
        dsCsv->bufferMax = CPPTEST_READ(dsCsv->fd, dsCsv->buffer, dsCsv->bufferSize);
        dsCsv->iBuffer = 0;

        if (!CPPTEST_CHECK_READ(dsCsv->fd, dsCsv->bufferMax)) {
            dsCsvError(dsCsv, CPPTEST_DS_CSV_ERROR_READING_FILE, CPPTEST_strerror);
            return EOF;
        }
    
        if (dsCsv->bufferMax == 0) {
            return EOF;
        }
    }

    return dsCsv->buffer[dsCsv->iBuffer];
} 

int CDECL_CALL CppTest_DsCsvReadChar(CppTest_DsCsvData* dsCsv)
{
    int ret = CppTest_DsCsvPeekChar(dsCsv);

    if (ret == '\n') {
        dsCsv->lineNo++;
        dsCsv->collumnNo = 0;
    } else {
        dsCsv->collumnNo++;
    }

    dsCsv->iBuffer++;
    return ret;
}

void CDECL_CALL CppTest_DsCsvGetSkipWS(CppTest_DsCsvData* dsCsv)
{
    while (CppTest_DsCsvPeekChar(dsCsv) == ' ') {
        CppTest_DsCsvReadChar(dsCsv);
    }
}

int CDECL_CALL CppTest_DsCsvIsEndOfField(CppTest_DsCsvData* dsCsv, int* pC)
{
    if (*pC == '\r') {
        if (CppTest_DsCsvPeekChar(dsCsv) == '\n') {
            *pC = CppTest_DsCsvReadChar(dsCsv);
        }
    }

    return *pC == EOF || *pC == dsCsv->separator || *pC == '\n';
}

int CDECL_CALL CppTest_DsCsvLoadQuotedField(CppTest_DsCsvData* dsCsv, 
        CppTestExpandableString* pEs, int* pC)
{
    /* Remove " from data source */
    int c = CppTest_DsCsvReadChar(dsCsv);

    unsigned int lineNo    = dsCsv->lineNo;
    unsigned int collumnNo = dsCsv->collumnNo;

    while (1) {
        c = CppTest_DsCsvReadChar(dsCsv);
        if (c == '"') {
            if (CppTest_DsCsvPeekChar(dsCsv) != '"') {
                break;
            }
            c = CppTest_DsCsvReadChar(dsCsv);
        } else if (c == EOF) {
            break;
        }
        cpptestExpandableStringAppendChar(pEs, (char)c);
    }

    if (c != '"') {
        dsCsv->lineNo    = lineNo;
        dsCsv->collumnNo = collumnNo;

        dsCsvError(dsCsv, CPPTEST_DS_CSV_UNMATCHED_QUOTE, 0);
        return 0;
    }

    if (dsCsv->trimValues) {
        CppTest_DsCsvGetSkipWS(dsCsv);
    }

    *pC = CppTest_DsCsvReadChar(dsCsv);
    if (!CppTest_DsCsvIsEndOfField(dsCsv, pC)) {
        dsCsvError(dsCsv, CPPTEST_DS_CSV_FORBIDDEN_CHAR, 0);
        return 0;
    }
    return 1;
}
 
int CDECL_CALL CppTest_DsCsvLoadField(CppTest_DsCsvData* dsCsv, 
        CppTestExpandableString* pEs, int* pC)
{
    int c = '\0';
    while (1) {
        c = CppTest_DsCsvReadChar(dsCsv);

        if (CppTest_DsCsvIsEndOfField(dsCsv, &c)) {
            break;
        }
        if (c == '"') {
            dsCsvError(dsCsv, CPPTEST_DS_CSV_INVALID_QUOTES, 0);
            return 0;
        }

        cpptestExpandableStringAppendChar(pEs, (char)c);
    }
    if (dsCsv->trimValues) {
        char* it = pEs->ptr + pEs->len - 1;
        while (pEs->ptr <= it && *it == ' ') {
            --it;
        }
        it[1] = '\0';
    }

    *pC = c;
    return 1;
}

#if CPPTEST_USE_EXTENDED_DATA_SOURCE_ROW_SIGNATURE
char* CDECL_CALL dsCsvCreateDataSignature(CppTest_DsCsvData* dsCsv)
{   
    const char DATA_MSG[] = "Data source row: ";
    const char VALUES_MSG[] = ", Values: ";
    const char NULL_MSG[] = "<NULL PTR>";
    int i = 0;
    char* signature = 0;

    CppTestListIterator cIt = cpptestListGetIterator(dsCsv->columns);
    CppTestListIterator dIt = cpptestListGetIterator(dsCsv->data);
 
    CppTestExpandableString es;
    cpptestExpandableStringInit(&es, 10);
            
    cpptestExpandableStringAppend(&es, DATA_MSG, sizeof(DATA_MSG) - 1);
    CppTest_DsDumpInt(&es, dsCsv->_cpptest_current);
    cpptestExpandableStringAppend(&es, VALUES_MSG, sizeof(VALUES_MSG) - 1);


    while (cIt || dIt) {
        if (cIt) {
            const char* collumn = cpptestListIteratorElement(cIt);
            cpptestExpandableStringAppend(&es, collumn, cpptestStrLen(collumn));
            cIt = cpptestListIteratorNext(cIt);
        } else if (dsCsv->columns) {
            cpptestExpandableStringAppend(&es, "<NO VALUE>", 10);
        } else {
            CppTest_DsDumpInt(&es, i);
        }

        cpptestExpandableStringAppend(&es, "=", 1);
            
        if (dIt) {
            const char* data = cpptestListIteratorElement(dIt);
            if (data) {
                cpptestExpandableStringAppend(&es, data, cpptestStrLen(data));
            } else {
                cpptestExpandableStringAppend(&es, NULL_MSG, cpptestStrLen(NULL_MSG));
            }
            cpptestExpandableStringAppend(&es, ", ", 2);
            dIt = cpptestListIteratorNext(dIt);
        } else {
            cpptestExpandableStringAppend(&es, "<NO VALUE>", 10);
        }

        ++i;
    }
 
    signature = cpptestStrDup(es.ptr);
    cpptestExpandableStringDispose(&es);

    return signature;
}
#endif

static char* CDECL_CALL dsCsvCreateElement(CppTestExpandableString* es)
{
    char* ptr = (char*)cpptestDsMalloc(sizeof(unsigned int) + es->len + 1);
    int i = 0;

    *((unsigned int*)ptr) = es->len;
    ptr += sizeof(unsigned int);
    for (; i < es->len; ++i) {
        ptr[i] = es->ptr[i];
    }
    ptr[es->len] = 0;
    return ptr;
}

static void CDECL_CALL dsCsvFreeElement(char * str, void * data)
{
    if (str != 0) {
        cpptestDsFree(str - sizeof(unsigned int));
    }
}

static int CDECL_CALL dsCsvLoadData(CppTest_DsCsvData* dsCsv, int columnNames)
{
    CppTestExpandableString es;
    CppTestList* list = columnNames ? dsCsv->columns : dsCsv->data;

    cpptestExpandableStringInit(&es, 10);
    while (1) {
        int c = '\0';

        cpptestExpandableStringClear(&es);

        if (dsCsv->trimValues) {
            CppTest_DsCsvGetSkipWS(dsCsv);
        }

        if (CppTest_DsCsvPeekChar(dsCsv) == '"') {
            if (CppTest_DsCsvLoadQuotedField(dsCsv, &es, &c) == 0) {
                cpptestExpandableStringDispose(&es);
                return 0;
            }
        } else {
            if (CppTest_DsCsvLoadField(dsCsv, &es, &c) == 0) {
                cpptestExpandableStringDispose(&es);
                return 0;
            }
        }

        cpptestListPushBack(list, dsCsvCreateElement(&es));
        if (c != dsCsv->separator) {
            break;
        }
    }
    cpptestExpandableStringDispose(&es);

    return 1;
}

static int CDECL_CALL isHexadecimalChar(char c)
{
    return (((c >= '0') && (c <= '9')) ||
            ((c >= 'a') && (c <= 'f')) ||
            ((c >= 'A') && (c <= 'F')));
}

static void CDECL_CALL replaceEscapeSequences(char* inOutCsvString)
{
    unsigned int inputLength = 0U;
    unsigned int inputIt = 0U;
    unsigned int outputIt = 0U;

    if (inOutCsvString == 0) {
        return;
    }

    inputLength = *((unsigned int*)(inOutCsvString - sizeof(unsigned int)));

    for (inputIt = 0U; inputIt < inputLength; ) {
        char c = inOutCsvString[inputIt];
        inputIt++;
        if ((c == '\\') && (inputIt < inputLength)) {
            char c2 = inOutCsvString[inputIt];
            if (c2 == 'n') {
                c = '\n';
                inputIt++;
            } else if (c2 == 't') {
                c = '\t';
                inputIt++;
            } else if (c2 == 'v') {
                c = '\v';
                inputIt++;
            } else if (c2 == 'b') {
                c = '\b';
                inputIt++;
            } else if (c2 == 'r') {
                c = '\r';
                inputIt++;
            } else if (c2 == 'f') {
                c = '\f';
                inputIt++;
            } else if (c2 == 'a') {
                c = '\a';
                inputIt++;
            } else if (c2 == '\\') {
                c = '\\';
                inputIt++;
            } else if (c2 == '?') {
                c = '\?';
                inputIt++;
            } else if (c2 == '\'') {
                c = '\'';
                inputIt++;
            } else if (c2 == '\"') {
                c = '\"';
                inputIt++;
            } else if (c2 == 'x') {
                unsigned int value = 0U;
                unsigned int digits = 0;
                inputIt++;
                while (digits < 2U && inputIt < inputLength && isHexadecimalChar(inOutCsvString[inputIt])) {
                    value = value * 16U + cpptestCharToDigit(inOutCsvString[inputIt]);
                    ++digits;
                    ++inputIt;
                }
                if (digits > 0U) {
                    c = (char)value;
                }
            } else if (c2 == 'u') {
                /* Unicode sequence. Just ignore it. */
            } else {
                unsigned int value = 0;
                unsigned int digits = 0;
                while (
                    digits < 3 && 
                    inputIt < inputLength && 
                    inOutCsvString[inputIt] <= '7' && 
                    inOutCsvString[inputIt] >= '0')
                {
                    value = value * 8U + cpptestCharToDigit(inOutCsvString[inputIt]);
                    ++digits;
                    ++inputIt;
                }
                if (digits > 0U) {
                    c = (char)value;
                }
            }
        }
        inOutCsvString[outputIt] = c;
        outputIt++;
    }
    inOutCsvString[outputIt] = 0;
    *((unsigned int*)(inOutCsvString - sizeof(unsigned int))) = outputIt;
}


int CDECL_CALL CppTest_DsCsvLoadNextData(CppTest_DataSource* ds)
{
    CppTest_DsCsvData* dsCsv = (CppTest_DsCsvData*)ds->data;

    cpptestListClear(dsCsv->data, &dsCsvFreeElement, 0);
#if CPPTEST_USE_EXTENDED_DATA_SOURCE_ROW_SIGNATURE
    if (dsCsv->dataSignature) {
        CPPTEST_DS_FREE(dsCsv->dataSignature);
        dsCsv->dataSignature = 0;
    }
#endif

    if (dsCsvLoadData(dsCsv, /*columnNames=*/0)) {
         CppTestListIterator dIt;
 
        ++dsCsv->_cpptest_current;
#if CPPTEST_USE_EXTENDED_DATA_SOURCE_ROW_SIGNATURE
        dsCsv->dataSignature = dsCsvCreateDataSignature(dsCsv);
#endif

        dIt = cpptestListGetIterator(dsCsv->data);
        while (dIt) {
            char* data = cpptestListIteratorElement(dIt);

            if (data) {
                if (cpptestStrCmp(CPPTEST_CSV_SPECIAL_NULL, data) == 0) {
                    /* Special NULL handler */
                    CPPTEST_DS_FREE(data);
                    dIt->data = 0;
                } else {
                    replaceEscapeSequences(data);
                }
            }
            dIt = cpptestListIteratorNext(dIt);
        }
        return 1;
    }
    return 0;
}

unsigned int CDECL_CALL CppTest_DsCsvGetIteration(CppTest_DataSource* ds)
{
    CppTest_DsCsvData* dsCsv = (CppTest_DsCsvData*)ds->data;
    return dsCsv->_cpptest_current;
}

int CDECL_CALL CppTest_DsCsvHasMoreData(CppTest_DataSource* ds)
{
    CppTest_DsCsvData* dsCsv = (CppTest_DsCsvData*)ds->data;
    return dsCsv->error == 0 && CppTest_DsCsvPeekChar(dsCsv) != EOF;
}

CppTestListIterator CDECL_CALL CppTest_DsCsvGetColumn(CppTest_DataSource* ds, const char* name)
{
    CppTest_DsCsvData* dsCsv = (CppTest_DsCsvData*)ds->data;

    CppTestListIterator it = 0;
    if (dsCsv->columns) {
        CppTestListIterator cIt = cpptestListGetIterator(dsCsv->columns);
        it = cpptestListGetIterator(dsCsv->data);
        while (cIt) {
            if (cpptestStrCmp(name, cpptestListIteratorElement(cIt)) == 0) {
                break;
            }
            cIt = cpptestListIteratorNext(cIt);
            it = cpptestListIteratorNext(it);
        }
    } else {
        CPPTEST_INTEGER collumn;
        if (cpptestStrToInteger(name, &collumn)) {
            CPPTEST_INTEGER i = 0;
            it = cpptestListGetIterator(dsCsv->data);
            while (i < collumn && it) {
                it = cpptestListIteratorNext(it);
                ++i;
            }
        }
    }

    return it;
}

int CDECL_CALL CppTest_DsCsvHasColumn(CppTest_DataSource* ds, const char* name)
{
    return CppTest_DsCsvGetColumn(ds, name) != 0;
}

const char* CDECL_CALL CppTest_DsCsvGetCStr(CppTest_DataSource* ds, const char* name)
{
    CppTestListIterator it = CppTest_DsCsvGetColumn(ds, name);

    if (it != 0) {
        return cpptestListIteratorElement(it);
    }

    cpptestDsUnknownValue(name);
    
    /* Not reached */
    return 0;
}

#if CPPTEST_WCHAR_ENABLED
const CppTest_wchar_t* CDECL_CALL CppTest_DsCsvGetWCStr(CppTest_DataSource* ds, const char* name)
{
    const char* inputString = CppTest_DsCsvGetCStr(ds, name);
    
    unsigned int inputLength = 0U;
    unsigned int i = 0U;

    CppTest_wchar_t* result = 0;
    unsigned int resultI = 0U;
 
    if (inputString == 0) {
        return 0;
    }

    inputLength = CPPTEST_C_std strlen(inputString);
    result = (CppTest_wchar_t*)cpptestDsMalloc((inputLength + 1U) * sizeof(CppTest_wchar_t));

    for (i = 0U; i < inputLength; ) {
        CppTest_wchar_t c = (CppTest_wchar_t)inputString[i];
        i++;
        if ((c == L'\\') && (i < inputLength)) {
            char c2 = inputString[i];
            if (c2 == 'u') {
                unsigned int value = 0U;
                unsigned int digits = 0;
                i++;
                while (digits < 4 && i < inputLength && isHexadecimalChar(inputString[i])) {
                    value = value * 16U + cpptestCharToDigit(inputString[i]);
                    ++digits;
                    ++i;
                }
                if (digits > 0U) {
                    c = (CppTest_wchar_t)value;
                }   
            }
        }
        result[resultI] = c;
        ++resultI;
    }
    result[resultI] = L'\0';

    return result;
}
#endif

const char* CDECL_CALL CppTest_DsCsvGetMemBuffer(CppTest_DataSource* ds, const char* name, unsigned int* size)
{
    CppTestListIterator it = CppTest_DsCsvGetColumn(ds, name);

    if (it != 0) {
        const char* str = cpptestListIteratorElement(it);
        if (size) {
            *size = *((unsigned int*)(str - sizeof(unsigned int)));
        }
        return str;
    }

    cpptestDsUnknownValue(name);
    
    /* Not reached */
    return 0;
}

#if CPPTEST_USE_EXTENDED_DATA_SOURCE_ROW_SIGNATURE
const char* CDECL_CALL CppTest_DsCsvGetDataSignature(CppTest_DataSource* ds)
{
    return ((CppTest_DsCsvData*)ds->data)->dataSignature;
}
#endif

void CDECL_CALL CppTest_DsCsvFinalize(CppTest_DataSource* ds)
{
    CppTest_DsCsvData* dsCsv = (CppTest_DsCsvData*)ds->data;
#if CPPTEST_USE_EXTENDED_DATA_SOURCE_ROW_SIGNATURE
    if (dsCsv->dataSignature) {
        CPPTEST_DS_FREE(dsCsv->dataSignature);
    }
#endif
    
    if (CPPTEST_CHECK_FD(dsCsv->fd)) {
        CPPTEST_CLOSE(dsCsv->fd);
    }

    cpptestListClear(dsCsv->data, &dsCsvFreeElement, 0);
    CPPTEST_DS_FREE(dsCsv->data);
    if (dsCsv->columns) {
        cpptestListClear(dsCsv->columns, &dsCsvFreeElement, 0);
        CPPTEST_DS_FREE(dsCsv->columns);
    }
 
    CPPTEST_DS_FREE(dsCsv->buffer);
 
    CppTest_DsFinalize(ds);
}

CppTest_DataSource* CDECL_CALL CppTest_DsCsv(const char* fileName, 
        char separator, int useCollumns, int trimValues)
{
    CPPTEST_FD_TYPE fd =  CPPTEST_OPEN(fileName);

    if (!CPPTEST_CHECK_FD(fd)) {
        cpptestDsCsvErrorOpeningFile(CPPTEST_strerror, fileName);
        return CppTest_DsError();
    } else {
        CppTest_DataSource* ds = CPPTEST_DS_MALLOC(CppTest_DataSource);
        CppTest_DsCsvData* dsCsv = CPPTEST_DS_MALLOC(CppTest_DsCsvData);
    
        dsCsv->fileName = fileName;
        dsCsv->fd       = fd;
 
        dsCsv->columns = 0;
        dsCsv->data = CPPTEST_DS_MALLOC(CppTestList);
        cpptestListInit(dsCsv->data);
 
        dsCsv->bufferSize = 256;
        dsCsv->iBuffer    = 0;
        dsCsv->bufferMax  = 0;
        dsCsv->buffer     = (char*)cpptestDsMalloc(sizeof(char) * dsCsv->bufferSize);
        dsCsv->separator  = separator;
        dsCsv->trimValues = trimValues;
        dsCsv->_cpptest_current    = 0;
        dsCsv->error      = 0;
#if CPPTEST_USE_EXTENDED_DATA_SOURCE_ROW_SIGNATURE
        dsCsv->dataSignature = 0;
#endif
        dsCsv->lineNo     = 1;
        dsCsv->collumnNo  = 0;
    
        ds->data = dsCsv;
    
        ds->hasColumn    = &CppTest_DsCsvHasColumn;
        ds->getCStr      = &CppTest_DsCsvGetCStr;
#if CPPTEST_WCHAR_ENABLED
        ds->getWCStr     = &CppTest_DsCsvGetWCStr;
#endif
        ds->getMemBuffer = &CppTest_DsCsvGetMemBuffer;
        ds->getInteger   = &CppTest_DsGetInteger;
        ds->getUInteger  = &CppTest_DsGetUInteger;
#ifndef __KERNEL__
        ds->getFloat     = &CppTest_DsGetFloat;
#endif
        ds->getBool      = &CppTest_DsGetBool;
        ds->getChar      = &CppTest_DsGetChar;
    
        ds->loadNextData = &CppTest_DsCsvLoadNextData;
        ds->hasMoreData = &CppTest_DsCsvHasMoreData;
#if CPPTEST_USE_EXTENDED_DATA_SOURCE_ROW_SIGNATURE
        ds->getDataSignature = &CppTest_DsCsvGetDataSignature;
#endif
        ds->getIteration = &CppTest_DsCsvGetIteration;
    
        ds->finalize = &CppTest_DsCsvFinalize;

        if (useCollumns) {
            dsCsv->columns = CPPTEST_DS_MALLOC(CppTestList);
            cpptestListInit(dsCsv->columns);

            dsCsvLoadData(dsCsv, /*columnNames=*/1);
        }

        return ds;
    }
}

#endif /* CPPTEST_CSV_DATA_SOURCE_ENABLED */

CppTest_DataSource* CDECL_CALL CppTest_Ds(const char* dataSourceName)
{
    CppTest_DataSource* ds = 0;
    
    if (CppTest_getDSFPtr) {
        ds = CppTest_getDSFPtr(dataSourceName, cpptestGetCurrentTestSuiteName());
    }
    if (!ds) {
        cpptestNoDsError(dataSourceName);
        ds = CppTest_DsError();
    }
    return ds;
}


#endif /* CPPTEST_DATA_SOURCES_ENABLED */

#if CPPTEST_EXTERNAL_ASSERTS_AND_POST_CONDITIONS_ENABLED


# if defined(_MSC_VER) || defined(__MINGW32__)

static void CDECL_CALL cpptestSendFullErrorMessage(CppTest_ErrorCode errorCode, 
        const char* program, const char* param1, const char* param2, const char* param3)
{
    LPSTR buf;
    const char* ret = 0;
    if (FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
                NULL,
                GetLastError(),
                0,
                (LPSTR)&buf,
                0,
                NULL) == 0)
    {
        ret = "Unknown Error";
        buf = 0;
    } else {
        ret = buf;
    }

    cpptestExternalProcessErrorFull(errorCode, ret, program, param1, param2, param3);

    if (buf) {
        LocalFree(buf);
    }
}

static void CDECL_CALL cpptestSendErrorMessage(CppTest_ErrorCode errorCode) 
{
    cpptestSendFullErrorMessage(errorCode, 0, 0, 0, 0);
}

static void CDECL_CALL cpptestQuoteParam(CppTestExpandableString* es, const char* param)
{
    unsigned int slashesInRow = 0;
    unsigned int i = 0;
    unsigned int length = CPPTEST_C_std strlen(param);
    for (; i < length; ++i) {
        /* " occured print slashes (double its amount in parameter) */
        if (param[i] == '"') {
            ++slashesInRow;
            while (slashesInRow--) {
                cpptestExpandableStringAppendChar(es, '\\');
            }
        }

        /* Count slashes in row */
        if (param[i] != '\\') {
            slashesInRow = 0;
        } else {
            ++slashesInRow;
        }

        cpptestExpandableStringAppendChar(es, param[i]);
    }

    while (slashesInRow--) {
        cpptestExpandableStringAppendChar(es, '\\');
    }
}

static char* CDECL_CALL cpptestCreateCommandLine(const char* program, const char* param1, const char* param2, const char* param3)
{
    CppTestExpandableString es;
    const char* ret = 0;
    
    cpptestExpandableStringInit(&es, 16);
    
    cpptestExpandableStringAppendChar(&es, '"');
    cpptestQuoteParam(&es, program);
    cpptestExpandableStringAppendChar(&es, '"');

    if (param1) {
        cpptestExpandableStringAppendChar(&es, ' ');
        cpptestExpandableStringAppendChar(&es, '"');
        cpptestQuoteParam(&es, param1);
        cpptestExpandableStringAppendChar(&es, '"');
    }

    if (param2) {
        cpptestExpandableStringAppendChar(&es, ' ');
        cpptestExpandableStringAppendChar(&es, '"');
        cpptestQuoteParam(&es, param2);
        cpptestExpandableStringAppendChar(&es, '"');
    }

    if (param3) {
        cpptestExpandableStringAppendChar(&es, ' ');
        cpptestExpandableStringAppendChar(&es, '"');
        cpptestQuoteParam(&es, param3);
        cpptestExpandableStringAppendChar(&es, '"');
    }

    ret = cpptestFormat("%s", es.ptr);

    cpptestExpandableStringDispose(&es);

    return (char*)ret;
}

static int CDECL_CALL cpptestWait(PROCESS_INFORMATION* processInfo, int* result)
{
    DWORD wRet = WaitForSingleObject(processInfo->hProcess, INFINITE);
    switch (wRet) {
        case WAIT_OBJECT_0:
            if (result) {
                DWORD dwResult;
                if (!GetExitCodeProcess(processInfo->hProcess, &dwResult)) {
                    cpptestSendErrorMessage(CPPTEST_EPC_CANNOT_GET_PROCESS_EXIT_CODE);
                    return 1;
                }
                *result = dwResult;
            }
            break;
        case WAIT_FAILED:
            cpptestSendErrorMessage(CPPTEST_EPC_WAITING_FOR_PROCESS_TO_FINISH_FAILED);
            break;
        case WAIT_TIMEOUT:
        default:
            /* Ignoring */
            break;
    }

    return 0;
}

static int CDECL_CALL cpptestReadStreamFromProcess(HANDLE fd, PROCESS_INFORMATION* processInfo, CppTestExpandableString* es, int* result)
{
    char buf[128];
    DWORD size = 0;
    do {
        if (ReadFile( fd, buf, 128, &size, 0) == 0) {
            break;
        }
 
        if (size) {
            cpptestExpandableStringAppend(es, buf, size);
        }
 
    } while (1);

    return cpptestWait(processInfo, result);
}

static int CDECL_CALL cpptestCallProgram(const char* program, CppTestExpandableString* es, int* returnValue, const char* param1,
        const char* param2, const char* param3)
{
    /* Based on AlexProcess code */
    
    /* Prevent children from displaying "application error" message boxes. */
    UINT oldErrorMode = SetErrorMode(SEM_FAILCRITICALERRORS |
                                     SEM_NOGPFAULTERRORBOX |
                                     SEM_NOOPENFILEERRORBOX);
    PROCESS_INFORMATION processInfo;
    
    DWORD creationFlags = CREATE_NO_WINDOW;
    
    STARTUPINFOA startupInfo;

    BOOL result = 0;
    int ret;
    char* commandLine = cpptestCreateCommandLine(program, param1, param2, param3);

    HANDLE readPipe;
    HANDLE writePipe;
    SECURITY_ATTRIBUTES securityAttributes;

    securityAttributes.nLength = sizeof(SECURITY_ATTRIBUTES);
    securityAttributes.lpSecurityDescriptor = 0;
    securityAttributes.bInheritHandle = 1;

    if (CreatePipe(&readPipe, &writePipe, &securityAttributes, 0) == 0) {
        cpptestSendErrorMessage(CPPTEST_EPC_CANNOT_CREATE_PIPE);
        return 1;
    }

    ZeroMemory(&processInfo, sizeof(processInfo));
    ZeroMemory(&startupInfo, sizeof(startupInfo));
    startupInfo.cb = sizeof(startupInfo);
    startupInfo.dwFlags = STARTF_USESTDHANDLES;
    startupInfo.hStdOutput = writePipe;
    startupInfo.hStdError = GetStdHandle(STD_ERROR_HANDLE);
    
    result = CreateProcessA(
        NULL,
        commandLine,
        NULL,
        NULL,
        TRUE,
        0,
        NULL,
        NULL,
        &startupInfo,
        &processInfo);

    SetErrorMode(oldErrorMode);
    CloseHandle(writePipe);
    
    if (result) {
        ret = cpptestReadStreamFromProcess(readPipe, &processInfo, es, returnValue);
    } else {
        cpptestSendFullErrorMessage(CPPTEST_EPC_CANNOT_CREATE_PROCESS, program, param1, param2, param3);
        ret = 2;
    }
    
    CloseHandle(readPipe);

    CloseHandle(processInfo.hProcess);
    CloseHandle(processInfo.hThread);

    return ret;
}

# else /* !(defined(_MSC_VER) || defined(__MINGW32__)) */

static void CDECL_CALL cpptestReadAndSendErrorMessage(int fd, CppTest_ErrorCode errorCode, const char* program, const char* param1,
        const char* param2, const char* param3)
{
    size_t size = 0; 
    CppTestExpandableString es;
    
    cpptestExpandableStringInit(&es, 16);
 
    do {
        char buf[128];
        size = read(fd, buf, 128);
        if (size) {
            cpptestExpandableStringAppend(&es, buf, size);
        }
    } while (size > 0);

    cpptestExternalProcessErrorFull(errorCode, es.ptr, program, param1, param2, param3);
    
    cpptestExpandableStringDispose(&es);
}

static void CDECL_CALL cpptestWriteErrorMessage(int fd)
{
    const char* error = CPPTEST_strerror;
    size_t size = CPPTEST_C_std strlen(error);
 
    if (write(fd, error, size) == -1) {
        perror("Error during write: ");
    }
}

static int CDECL_CALL cpptestReadStreamFromProcess(int fd, int pid, CppTestExpandableString* es)
{
    int status = 0;
    size_t size = 0;
    do {
        char buf[128];
        size = read(fd, buf, 128);
        if (size) {
            cpptestExpandableStringAppend(es, buf, size);
        }
    } while (size > 0);

    if (waitpid(pid, &status, 0) == -1) {
        cpptestExternalProcessError(CPPTEST_EPC_WAITING_FOR_PROCESS_TO_FINISH_FAILED, CPPTEST_strerror);
    }
    return status;
}

static int CDECL_CALL cpptestCallProgram(const char* program, CppTestExpandableString* es, int* returnValue, const char* param1,
        const char* param2, const char* param3)
{
    int ioPipe[2];
    int errorPipe[2];
    int pid = 0;
    int status;
    int retStatus;

    if (pipe(ioPipe)) {
        cpptestExternalProcessError(CPPTEST_EPC_CANNOT_CREATE_PIPE, CPPTEST_strerror);
        return 1;
    }

    if (pipe(errorPipe)) {
        cpptestExternalProcessError(CPPTEST_EPC_CANNOT_CREATE_PIPE, CPPTEST_strerror);
        return 1;
    }

    pid = fork();
    if (pid == 0) {
        const char* error = 0;
        close(ioPipe[0]);
        close(errorPipe[0]);

        if (dup2(ioPipe[1], 1) == -1) {
            cpptestWriteErrorMessage(errorPipe[1]);
            CPPTEST_EXIT(92);
        }
        
        execlp(program, program, param1, param2, param3, 0);
        cpptestWriteErrorMessage(errorPipe[1]);
        CPPTEST_EXIT(93);
    } else if (pid == -1) {
        cpptestExternalProcessErrorFull(CPPTEST_EPC_CANNOT_CREATE_PROCESS, CPPTEST_strerror, program, param1, param2, param3);
        close(ioPipe[0]);
        close(ioPipe[1]);
        return 2;
    }

    close(ioPipe[1]);
    close(errorPipe[1]);

    status = cpptestReadStreamFromProcess(ioPipe[0], pid, es);

    close(ioPipe[0]);

    if (WIFEXITED(status) == 0) {
        close(errorPipe[0]);
        return 3;
    }

    retStatus = WEXITSTATUS(status);
    if (returnValue) {
        *returnValue = retStatus;
    }

    if (retStatus == 93) {
        cpptestReadAndSendErrorMessage(errorPipe[0], CPPTEST_EPC_CANNOT_CREATE_PROCESS, program, param1, param2, param3);
        close(errorPipe[0]);
        return 4;
    } else if (retStatus == 92) {
        cpptestReadAndSendErrorMessage(errorPipe[0], CPPTEST_EPC_CANNOT_DUPLICATE_DESCRIPTOR, program, param1, param2, param3);
        close(errorPipe[0]);
        return 5;
    }

    return 0;
}

# endif /* defined(_MSC_VER) || defined(__MINGW32__) */

const char* CDECL_CALL CppTestCallProgramWrapper(const char* program, int* pResult, const char* param1,
        const char* param2, const char* param3)
{
    CppTestExpandableString es;
    const char* ret = 0;
    
    cpptestExpandableStringInit(&es, 16);
    
    if (cpptestCallProgram(program, &es, pResult, param1, param2, param3) == 0) {
        ret = cpptestFormat("%s", es.ptr);
    }

    cpptestExpandableStringDispose(&es);

    return ret;
}


#endif /* CPPTEST_EXTERNAL_ASSERTS_AND_POST_CONDITIONS_ENABLED */


int CDECL_CALL CppTest_uncaughtException(void)
{
#if CPPTEST_EXCEPTIONS_ENABLED && CPPTEST_USE_UNCAUGHT_EXCEPTION_CHECKING
    return CPPTEST_std uncaught_exception();
#else    
    return 0;
#endif    
}

/* Windows Structured Exception Handling (SEH) */
#if defined(CPPTEST_WIN32) && CPPTEST_SEH_EXCEPTIONS_ENABLED && !defined(CPPTEST_EVC)
static LONG WINAPI cpptestUnhandledStructuredExceptionInThreadFilter(struct _EXCEPTION_POINTERS *exceptionInfo)
{
    cpptestUnhandledStructuredExceptionInThread(exceptionInfo->ExceptionRecord->ExceptionCode);
    /* not reached */
    return 0;
};
#endif

static void CDECL_CALL threadInitializer(void)
{
#if CPPTEST_SIGNALS_HANDLING_ENABLED && defined(CPPTEST_CYGWIN)
    int i = 0;
#endif    
#if CPPTEST_EXCEPTIONS_ENABLED && CPPTEST_USE_SET_TERMINATE
    CPPTEST_std set_terminate(&cpptestUnhandledCppExceptionInThreadHandler);
#endif
#if defined(CPPTEST_WIN32) && CPPTEST_SEH_EXCEPTIONS_ENABLED && !defined(CPPTEST_EVC)
#  if defined(__x86_64) && !defined(CPPTEST_MSVC) && !defined(__MINGW32__)
    AddVectoredExceptionHandler(0, &cpptestUnhandledStructuredExceptionInThreadFilter);
#  else
    SetUnhandledExceptionFilter(&cpptestUnhandledStructuredExceptionInThreadFilter);
#  endif
#endif
#if CPPTEST_SIGNALS_HANDLING_ENABLED && defined(CPPTEST_CYGWIN)
    for (; i < 32; ++i) {
        signal(i, cpptestSignalFound);
    }
#endif
}

EXTERN_C_LINKAGE_END

#if CPPTEST_INIT_QT
# ifdef __cplusplus
static int _qapp_argc = 1;
static char* _qapp_argv[] = {"myapp"};
#if CPPTEST_INIT_QT_CONSOLE
static bool _guiEnabled = false;
#else
static bool _guiEnabled = true;
#endif
static QApplication _qapp(/*argc=*/_qapp_argc, /*argv=*/_qapp_argv, /*GUIenabled=*/_guiEnabled);
# endif
#endif

#if CPPTEST_DEFINE_INVALID_SIZE_ARGUMENT_FOR_IOC
unsigned int __invalid_size_argument_for_IOC;
#endif

static const char* cpptestCurrentFileContext = 0;

int CDECL_CALL CppTest_IsCurrentContext(const char* context)
{
    return cpptestStrCmp(cpptestCurrentFileContext, context) == 0;
}


#if defined(CPPTEST_WIN32) && CPPTEST_SEH_EXCEPTIONS_ENABLED

#if !defined(CPPTEST_MSVC)
EXTERN_C_LINKAGE int CDECL_CALL cpptestUnhandledStructuredExceptionInTestCaseHandler(
        struct _EXCEPTION_RECORD* exception, void* b , struct _CONTEXT* c, void* d)
{
#if defined(CPPTEST_GHS_MSVC)
    if (*(unsigned int*)exception == 0xC0000027L) {
        return 1;
    }
#elif defined(__x86_64)
    if (**(unsigned long long**)exception == 0x20474343L) {
        return 0;
    }
#endif

#if defined(__x86_64)
    cpptestUnhandledStructuredExceptionInTestCase(**(unsigned long long**)exception);
#else
    cpptestUnhandledStructuredExceptionInTestCase(*(unsigned int*)exception);
#endif
#if CPPTEST_SETJMP_ENABLED
    if (cpptestGetTestCaseBreakFuncPtr()) {
        __CPTR_TestCaseBreakFuncPtr testCaseBreakFuncPtr = cpptestSetTestCaseBreakFuncPtr(0);
        testCaseBreakFuncPtr();
    }
#endif
    return 1;
}
#endif

/* SEH */
#  if defined(CPPTEST_GHS_MSVC)
/* Use GHS asm macros to register SE handler */
#    define CPPTEST_TRY \
        asm( "\t" "pushl $_cpptestUnhandledStructuredExceptionInTestCaseHandler" "\n"); \
        asm( "\t" "pushl %fs:0" "\n"); \
        asm( "\t" "movl %esp, %fs:0" "\n"); \
        asm( "\t" "pushl 12(%esp)" "\n"); \
        asm( "\t" "pushl 12(%esp)" "\n");
#    define CPPTEST_EXCEPT \
        asm( "\t" "addl $8, %esp" "\n" ); \
        asm( "\t" "popl %fs:0" "\n" ); \
        asm( "\t" "addl $4, %esp" "\n" ); 

#  elif defined(CPPTEST_MSVC)
/* Microsoft structured exception handling */

EXTERN_C_LINKAGE unsigned long __cdecl _exception_code(void);

#    define CPPTEST_TRY \
        __try {
#    define CPPTEST_EXCEPT \
        } __except(CPPTEST_GLOBAL_PREFIX _exception_code() == 0xe06d7363 ? \
                0 : cpptestUnhandledStructuredExceptionInTestCase(CPPTEST_GLOBAL_PREFIX _exception_code())) {}

#  elif defined(__try1) && defined(__except1)

#    define CPPTEST_TRY __try1(cpptestUnhandledStructuredExceptionInTestCaseHandler)
#    define CPPTEST_EXCEPT __except1 

#  elif !defined(__x86_64)

/* Use inline asm to register SE handler */
#    define CPPTEST_TRY \
        __asm__ ("pushl %0;pushl %%fs:0;movl %%esp,%%fs:0;" : : "g" (cpptestUnhandledStructuredExceptionInTestCaseHandler));
#    define CPPTEST_EXCEPT \
        __asm__ ("popl %fs:0;addl $4,%esp;");

#  else

#    error "No available implementation of SE handler registration. Use -DCPPTEST_SEH_EXCEPTIONS_ENABLED=0"
#    define CPPTEST_TRY
#    define CPPTEST_EXCEPT

#  endif
#else /* !CPPTEST_WIN32 */
/* No SEH available */
#  define CPPTEST_TRY
#  define CPPTEST_EXCEPT
#endif /* CPPTEST_WIN32 */

/* Assure full stack frame for SEH un-/registration routine */
#if defined(CPPTEST_GCC) && defined(CPPTEST_WIN32) && CPPTEST_SEH_EXCEPTIONS_ENABLED
#  define CPPTEST_FORCE_FULL_FRAME __attribute__((optimize ("-fno-omit-frame-pointer")))
#else
#  define CPPTEST_FORCE_FULL_FRAME
#endif

#if CPPTEST_SETJMP_ENABLED
/* Jmpbuf for test/stub flow control */
static CPPTEST_C_std sigjmp_buf testJmpBuf;
static int setJmpReturn = 0;

/* Flag for detecting incorrect usage of jmp macros */
static int setJmpCalled = 0;

CPPTEST_C_std sigjmp_buf* CDECL_CALL cpptestGetJmpBuf(void)
{
    return &testJmpBuf;
}

int CDECL_CALL cpptestSetJmpReturn(int jmpReturn)
{
    setJmpReturn = jmpReturn;
    return setJmpReturn;
}

int CDECL_CALL cpptestGetJmpReturn(void)
{
    return setJmpReturn;
}

void CDECL_CALL cpptestSetJmpCalled(int jmpCalled)
{
    setJmpCalled = jmpCalled;
}

int CDECL_CALL cpptestGetJmpCalled(void)
{
    return setJmpCalled;
}

/* Jmpbuf for execution break */
static CPPTEST_C_std sigjmp_buf breakJmpBuf;

static void CDECL_CALL testCaseBreakFunc(void)
{
    CPPTEST_C_std siglongjmp(breakJmpBuf, 1);
}
#endif /* CPPTEST_SETJMP_ENABLED */

static void CDECL_CALL cpptestTestCaseSlave2(__CPTR_TestCasePtr testCase)
{
#if CPPTEST_SETJMP_ENABLED
        if (! CPPTEST_C_std sigsetjmp(breakJmpBuf, 1)) { \
            cpptestSetTestCaseBreakFuncPtr(&testCaseBreakFunc);
#endif /* CPPTEST_SETJMP_ENABLED */
            testCase();
            cpptestCheckTimeout();
#if CPPTEST_SETJMP_ENABLED
        }
#endif /* CPPTEST_SETJMP_ENABLED */
}

CPPTEST_FORCE_FULL_FRAME void CDECL_CALL cpptestTestCaseSlave(__CPTR_TestCasePtr testCase)
{
    CPPTEST_TRY
        cpptestTestCaseSlave2(testCase);
    CPPTEST_EXCEPT
}

/*****************************************************************************
 *
 *                               Triggers
 *
 ****************************************************************************/

#if CPPTEST_TRIGGER_ENABLED

static tgr_time testCaseTgrStartTime = 0;
static tgr_time testSuiteTgrStartTime = 0;

static void CDECL_CALL runtimeCommandTriggerHandler(unsigned int commandId)
{
    if (commandId == CPPTEST_CMD_TESTCASE_BEGIN) {
        testCaseTgrStartTime = tgr_get_curr_time();
     
    } else if (commandId == CPPTEST_CMD_TESTCASE_END) {
        tgr_remove_all_with_creation_time(testCaseTgrStartTime, tgr_get_curr_time());
   
    } else if (commandId == CPPTEST_CMD_TESTSUITE_BEGIN) {
        testSuiteTgrStartTime = tgr_get_curr_time();

    } else if (commandId == CPPTEST_CMD_TESTSUITE_END) {
        tgr_remove_all_with_creation_time(testSuiteTgrStartTime, tgr_get_curr_time());
    }
}

static CppTestRuntimeCommandHandlerNode runtimeCommandTriggerHandlerNode = 
{
    /*prev=*/0,
    /*handler=*/&runtimeCommandTriggerHandler
};

#endif /* CPPTEST_TRIGGER_ENABLED */

/*****************************************************************************
 *
 *                               Expectations
 *
 ****************************************************************************/

EXTERN_C_LINKAGE_START
static void CDECL_CALL checkCallExpectation(CppTest_StubInfo* stubInfo, void* data)
{
    CppTestExpectation* expectation = (CppTestExpectation*)data;

    if (!expectation->reported) {
        int calls = ++expectation->variant.expectedCalls.calls;
        if (expectation->variant.expectedCalls.max >= 0) {
            if (calls > expectation->variant.expectedCalls.max) {
                expectation->passed = 0;
            } else if (calls >= expectation->variant.expectedCalls.min) {
                expectation->passed = 1;
            }
        } else if (calls >= expectation->variant.expectedCalls.min) {
            expectation->passed = 1;
        }
    }
}
EXTERN_C_LINKAGE_END

static void CDECL_CALL endOfCheckCallExpectation(CppTestExpectation* expectation)
{
    if (!expectation->reported) {
        cpptestCallExpectationAssertion(expectation);
        expectation->reported = 1;
    }
}


static CppTestExpectation* firstExpectation = 0;
static CppTestExpectation* lastExpectation = 0;

EXTERN_C_LINKAGE 
void CDECL_CALL cpptestCreateCallExpectation(const char* funcId, int min, int max, const char* file, unsigned int line)
{
    CppTest_StubInfo* stubInfo = cpptestGetStubInfo(
            funcId, CPPTEST_GET_STUB_INFO_CONTEXT_EXPECTATION, file, line);
    
    if (stubInfo) {
        CppTestExpectation* expectation = (CppTestExpectation*)cpptestMalloc(sizeof(CppTestExpectation));
        CppTest_Checker* checker = (CppTest_Checker*)cpptestMalloc(sizeof(CppTest_Checker));
    
        expectation->next = 0;
        expectation->file = file;
        expectation->line = line;
        expectation->reported = 0;
        expectation->passed = (min <= 0 ? 1 : 0);

        expectation->endOfCheckExpectation = &endOfCheckCallExpectation;

        expectation->variant.expectedCalls.min = min;
        expectation->variant.expectedCalls.max = max;
        expectation->variant.expectedCalls.stubInfo = stubInfo;
        expectation->variant.expectedCalls.calls = 0;

        if (lastExpectation) {
            lastExpectation->next = expectation;
            lastExpectation = expectation;
        } else {
            firstExpectation = lastExpectation = expectation;
        }

        checker->checker = &checkCallExpectation;
        checker->data = expectation;
        cpptestAddChecker(stubInfo, checker);
    }
}

static CppTestCallSequenceNode* CDECL_CALL moveCallSequenceIterator(CppTestExpectation* expectation)
{
    CppTestCallSequenceNode* it = expectation->variant.sequence.it;

    if (it) {
        if (it->next) {
            it = it->next;
        } else {
            ++expectation->variant.sequence.repeat;
            if (expectation->variant.sequence.isLoop) {
                expectation->variant.sequence.inside = 0;
                it = expectation->variant.sequence.first;
            } else {
                it = 0;
            }
        }
        expectation->variant.sequence.it = it;
        if (it) {
            it->calls = 0;
        }
    }
    return it;
}

EXTERN_C_LINKAGE_START
static void CDECL_CALL checkCallSequenceExpectation(CppTest_StubInfo* stubInfo, void* data)
{
    CppTestExpectation* expectation = (CppTestExpectation*)data;

    if (!expectation->reported) {
        CppTestCallSequenceNode* it = expectation->variant.sequence.it;

        while (it && (it->stubInfo != stubInfo) && (it->times <= 0)) {
            it = moveCallSequenceIterator(expectation);
        }
        if (it) {
            if (it->times == 0 || (it->stubInfo != stubInfo)) {
                expectation->passed = 0;
                if (it->times == 0) {
                    cpptestNullCallSequenceExpectationAssertion(expectation, stubInfo->userId);
                } else {
                    cpptestWrongCallSequenceExpectationAssertion(expectation, stubInfo->userId, it->stubInfo->userId);
                }
                expectation->reported = 1;
            } else {
                expectation->variant.sequence.inside = 1;
                ++it->calls;
                if ((it->times > 0) && ((it->calls % it->times) == 0)) {
                    it = moveCallSequenceIterator(expectation);
                }
            }
        }
    }
}
EXTERN_C_LINKAGE_END

static CppTestCallSequenceExpectation* CDECL_CALL addCallSequenceNode(const char* funcId);
static CppTestCallSequenceExpectation* CDECL_CALL addNTimesCallSequenceNode(const char* funcId, int times);
static CppTestCallSequenceExpectation* CDECL_CALL addAnyTimesCallSequenceNode(const char* funcId);

static CppTestCallSequenceExpectation callSequenceExpectation = 
{
    &addCallSequenceNode,
    &addNTimesCallSequenceNode,
    addAnyTimesCallSequenceNode
};


static CppTestExpectation* sequenceExpectation = 0;


static CppTestCallSequenceNode* CDECL_CALL getFirstCallSequenceNodeFor(CppTest_StubInfo* stubInfo, CppTestExpectation* sExpectation)
{
    CppTestCallSequenceNode* it = sExpectation->variant.sequence.first;
    while (it && it->stubInfo != stubInfo) {
        it = it->next;
    }
    return it;
}

static CppTestCallSequenceExpectation* CDECL_CALL addNTimesCallSequenceNode(const char* funcId, int times)
{
    CppTest_StubInfo* stubInfo = cpptestGetStubInfo(
            funcId, CPPTEST_GET_STUB_INFO_CONTEXT_EXPECTATION, sequenceExpectation->file, sequenceExpectation->line);
    
    if (stubInfo) {
        CppTestCallSequenceNode* sequenceNode = (CppTestCallSequenceNode*)cpptestMalloc(sizeof(CppTestCallSequenceNode));

        sequenceNode->stubInfo = stubInfo;
        sequenceNode->times = times;
        sequenceNode->calls = 0;
        sequenceNode->expectation = sequenceExpectation;

        sequenceNode->prev = sequenceExpectation->variant.sequence.last;
        sequenceNode->next = 0; 
        if (sequenceExpectation->variant.sequence.last) {
            sequenceExpectation->variant.sequence.last->next = sequenceNode;
        } else {
            sequenceExpectation->variant.sequence.first = sequenceNode;
            sequenceExpectation->variant.sequence.it = sequenceNode;
        }
        sequenceExpectation->variant.sequence.last = sequenceNode;

        if (getFirstCallSequenceNodeFor(stubInfo, sequenceExpectation) == sequenceNode) {
            CppTest_Checker* checker = (CppTest_Checker*)cpptestMalloc(sizeof(CppTest_Checker));
            checker->checker = checkCallSequenceExpectation;
            checker->data = sequenceExpectation;
            cpptestAddChecker(stubInfo, checker);
        }
    }

    return &callSequenceExpectation;

}


static CppTestCallSequenceExpectation* CDECL_CALL addCallSequenceNode(const char* funcId)
{
    return addNTimesCallSequenceNode(funcId, 1);
}

static CppTestCallSequenceExpectation* CDECL_CALL addAnyTimesCallSequenceNode(const char* funcId)
{
    return addNTimesCallSequenceNode(funcId, -1);
}

static void CDECL_CALL endOfCheckCallSequenceExpectation(CppTestExpectation* expectation)
{
    if (!expectation->reported) {
        CppTestCallSequenceNode* it = expectation->variant.sequence.it;

        if (it && it == expectation->variant.sequence.first && it->times <= 0) {
            it = moveCallSequenceIterator(expectation);
        }
        while (it && it != expectation->variant.sequence.first && it->times <= 0) {
            it = moveCallSequenceIterator(expectation);
        }

        if (it && (expectation->variant.sequence.inside || (expectation->variant.sequence.repeat == 0))) {
            expectation->passed = 0;
            cpptestIncompleteCallSequenceExpectationAssertion(expectation);
        
        } else {
            expectation->passed = 1;
            cpptestCallSequenceExpectationPassed(expectation);
        }
    }
}

EXTERN_C_LINKAGE
CppTestCallSequenceExpectation* CDECL_CALL cpptestCreateCallSequenceExpectation(int isLoop, const char* file, unsigned int line)
{
    sequenceExpectation = (CppTestExpectation*)cpptestMalloc(sizeof(CppTestExpectation));
    
    sequenceExpectation->next = 0;
    sequenceExpectation->file = file;
    sequenceExpectation->line = line;
    sequenceExpectation->reported = 0;
    sequenceExpectation->passed = 0;

    sequenceExpectation->variant.sequence.first = 0;
    sequenceExpectation->variant.sequence.last = 0;
    sequenceExpectation->variant.sequence.it = 0;
    sequenceExpectation->variant.sequence.isLoop = isLoop;
    sequenceExpectation->variant.sequence.repeat = 0;
    sequenceExpectation->variant.sequence.inside = 0;

    sequenceExpectation->endOfCheckExpectation = &endOfCheckCallSequenceExpectation;

    if (lastExpectation) {
        lastExpectation->next = sequenceExpectation;
        lastExpectation = sequenceExpectation;
    } else {
        firstExpectation = lastExpectation = sequenceExpectation;
    }

    return &callSequenceExpectation;
}


static void CDECL_CALL runtimeCommandExpectationHandler(unsigned int commandId)
{
    if (commandId == CPPTEST_CMD_INITIALIZE_TESTCASE) {
        firstExpectation = 0;
        lastExpectation = 0;
    
    } else if (commandId == CPPTEST_CMD_TESTCASE_END) {
        while (firstExpectation) {
          CppTestExpectation* nextExpectation = firstExpectation->next;
          
          if (firstExpectation->endOfCheckExpectation) {
              firstExpectation->endOfCheckExpectation(firstExpectation);
          }

          if (firstExpectation->endOfCheckExpectation == &endOfCheckCallSequenceExpectation) {
              CppTestCallSequenceNode* node = firstExpectation->variant.sequence.first;

              while (node) {
                  CppTestCallSequenceNode* nextNode = node->next;
                  cpptestFree(node);
                  node = nextNode;
              }
          }

          cpptestFree(firstExpectation);
          firstExpectation = nextExpectation;
        }
        lastExpectation = 0;

        cpptestRemoveAllCheckers();
    }
}

EXTERN_C_LINKAGE_START
CppTest_StubCallInfo *stubCallInfo = 0;
EXTERN_C_LINKAGE_END

static CppTestRuntimeCommandHandlerNode runtimeCommandExpectationHandlerNode = 
{
    /*prev=*/0,
    /*handler=*/&runtimeCommandExpectationHandler
};

/*****************************************************************************
 *
 *                               Target system
 *
 ****************************************************************************/

#if CPPTEST_USE_TARGET_SYSTEM

static void * CDECL_CALL target_malloc_wrapper(CPPTEST_C_std size_t size)
{
    return CPPTEST_C_std malloc(size);
}

static void * CDECL_CALL target_calloc_wrapper(CPPTEST_C_std size_t elements, CPPTEST_C_std size_t elem_size)
{
    return CPPTEST_C_std calloc(elements, elem_size);
}

static void * CDECL_CALL target_relloc_wrapper(void* ptr, CPPTEST_C_std size_t new_size)
{
    return CPPTEST_C_std realloc(ptr, new_size);
}

static void CDECL_CALL target_free_wrapper(void* ptr)
{
    CPPTEST_C_std free(ptr);
}

static void * CDECL_CALL target_memset_wrapper(void* ptr, int value, CPPTEST_C_std size_t size)
{
    return CPPTEST_C_std memset(ptr, value, size);
}

static CppTest_System cpptestTargetSystem = 
{
    &target_malloc_wrapper,
    &target_calloc_wrapper,
    &target_relloc_wrapper,
    &target_free_wrapper,
    &target_memset_wrapper
}; 

# define CPPTEST_TARGET_SYSTEM_PTR &cpptestTargetSystem

#else /* !CPPTEST_USE_TARGET_SYSTEM */

# define CPPTEST_TARGET_SYSTEM_PTR 0

#endif /* CPPTEST_USE_TARGET_SYSTEM */

/*****************************************************************************
 *
 *                               Main routine
 *
 ****************************************************************************/

#ifndef CPPTEST_ENTRY_POINT_DEFINED
#define CPPTEST_ENTRY_POINT_DEFINED

#  ifdef CPPTEST_ENTRY_POINT_C_LINKAGE
#    define _CPPTEST_ENTRY_POINT_LINKAGE EXTERN_C_LINKAGE
#  else
#    define _CPPTEST_ENTRY_POINT_LINKAGE
#  endif


#  if defined(CPPTEST_EPT_main)

_CPPTEST_ENTRY_POINT_LINKAGE
int CDECL_CALL main(int argc, char* argv[])
{
    return CppTest_Main(argc, argv);
}

#  endif


#  if defined(CPPTEST_EPT_wmain)

_CPPTEST_ENTRY_POINT_LINKAGE
int CDECL_CALL wmain(int argc, CppTest_wchar_t* argv[])
{
    return CppTest_MainW(argc, argv);
}

#  endif


#  if defined(CPPTEST_EPT_WinMain)
#    include <windows.h>
#    include <tchar.h>

_CPPTEST_ENTRY_POINT_LINKAGE
int WINAPI _tWinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR     lpCmdLine,
                     int       nCmdShow)
{
    int result = 0;
    int argc = 0;
    char** argv = CppTest_CmdLineToArgv(GetCommandLineA(), &argc);
    
    result = CppTest_Main(argc, argv);
    CppTest_FreeArgv(argc, argv);

    return result;
}
#  endif


#  if defined(CPPTEST_EPT_wWinMain)
#    include <windows.h>
#    include <tchar.h>

_CPPTEST_ENTRY_POINT_LINKAGE
int WINAPI _tWinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPWSTR    lpCmdLine,
                     int       nCmdShow)
{
    int result = 0;
    int argc = 0;
    CppTest_wchar_t** argv = CppTest_CmdLineToArgvW(GetCommandLineW(), &argc);

    result = CppTest_MainW(argc, argv);
    CppTest_FreeArgvW(argc, argv);

    return result;
}

#  endif


#  if defined(CPPTEST_EPT_void_main)

_CPPTEST_ENTRY_POINT_LINKAGE
void CDECL_CALL main(int argc, char* argv[])
{
    CppTest_Main(argc, argv);
}

#  endif


#  if defined(CPPTEST_EPT_main_no_args)

_CPPTEST_ENTRY_POINT_LINKAGE
int CDECL_CALL main(void)
{
    return CppTest_Main(0, 0);
}

#  endif


#  if defined(CPPTEST_EPT_void_main_no_args)

_CPPTEST_ENTRY_POINT_LINKAGE
void CDECL_CALL main(void)
{
    CppTest_Main(0, 0);
}

#  endif

#  if defined(CPPTEST_EPT_void_main_no_args_with_exit)

_CPPTEST_ENTRY_POINT_LINKAGE
void CDECL_CALL main(void)
{
    CppTest_Main(0, 0);
    EXIT_STATEMENT
}

#  endif

#  if defined(CPPTEST_ENTRY_POINT)

#    ifndef CPPTEST_ENTRY_POINT_RETURN_TYPE
#      define CPPTEST_ENTRY_POINT_RETURN_TYPE int
#    endif

#    ifndef CPPTEST_ENTRY_POINT_ARGS
#      define CPPTEST_ENTRY_POINT_ARGS void
#    endif

#    ifndef CPPTEST_ENTRY_POINT_ARGC
#      define CPPTEST_ENTRY_POINT_ARGC 0
#    endif

#    ifndef CPPTEST_ENTRY_POINT_ARGV
#      define CPPTEST_ENTRY_POINT_ARGV 0
#    endif

#    ifndef CPPTEST_ENTRY_POINT_RETURN_STATEMENT
#      define CPPTEST_ENTRY_POINT_RETURN_STATEMENT return 0;
#    endif

_CPPTEST_ENTRY_POINT_LINKAGE
CPPTEST_ENTRY_POINT_RETURN_TYPE CPPTEST_ENTRY_POINT(CPPTEST_ENTRY_POINT_ARGS)
{
    CppTest_Main(CPPTEST_ENTRY_POINT_ARGC, CPPTEST_ENTRY_POINT_ARGV);
    CPPTEST_ENTRY_POINT_RETURN_STATEMENT
}
#  endif

#endif  /* CPPTEST_ENTRY_POINT_DEFINED */
