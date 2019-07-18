/*
 * (C) Copyright Parasoft Corporation 1998 - 2019.  All rights reserved.
 * The use of this source code is governed by the general Parasoft License Agreement.
 * Redistribution strictly prohibited.
 */
#define CPPTEST_RUNTIME_IMPLEMENTATION

#include "cpptest_runtime.h"
#include "cpptest_headers.h"

#include "CppTestInternal.h"

#include CPPTEST_string
#include CPPTEST_stdio

CPPTEST_using_namespace_std


#if CPPTEST_PRINTF_IMPL

/**
 * Formats printf-like integer
 *
 * Formats the contents of a number buffer obtained with
 * localUL2NB to printf-like integer number format.
 *
 * @param op [inout] output buffer pptr
 * @param nb number buffer ptr
 * @param l number of ciphers (ul2nb return value)
 * @param ml minimal number of ciphers, if ml>l && j<>0 zeros are prepended
 * @param w width - no. of chars reserved for resulting string
 * @param s sign: 0 - absent, other - inserted as is
 * @param j justification: 0 - right/zeros, 1 - right/spaces, -1 - left
 */
static void printi(char** op, const char* nb, unsigned l, unsigned ml,
    unsigned w, char s, int j)
{
    const unsigned cl = (ml > l) ? ml : l; /* ciphers length */
    const unsigned tl = (j ? cl : l) + (s && 1); /* total length */
    const unsigned rw = (w > tl) ? (w - tl) : 0; /* rem. reserv. space */
    const unsigned tz = j ? (cl - l) : rw; /* total zeros */

    if (j > 0) {
        localInternalMemset(*op, ' ', rw);
        *op += rw;
    }

    if (s) {
        *(*op)++ = s;
    }

    localInternalMemset(*op, '0', tz);
    *op += tz;

    memcpy(*op, nb, l);
    *op += l;

    if (j < 0) {
        localInternalMemset(*op, ' ', rw);
        *op += rw;
    }
}


/**
 * Formats printf-like string
 *
 * @param op [inout] output buffer pptr
 * @param s string to format
 * @param ml maximal number of s chars to print
 * @param w width - no. of chars reserved for resulting string
 * @param j justification: 0 - right/zeros, 1 - right/spaces, -1 - left
 */
static void prints(char** op, const char* s, unsigned ml, unsigned w, int j)
{
    const unsigned l = strlen(s);
    const unsigned tl = (ml > l) ? l : ml; /* total length */
    const unsigned rw = (w > tl) ? (w - tl) : 0; /* rem. reserv. space */

    if (j >= 0) {
        localInternalMemset(*op, j ? ' ' : '0', rw);
        *op += rw;
    }

    memcpy(*op, s, tl);
    *op += tl;

    if (j < 0) {
        localInternalMemset(*op, ' ', rw);
        *op += rw;
    }
}


/**
 * Converts unsigned decimal string to number
 *
 * @param s [inout] input string ptr
 *
 * @return the number
 */
static unsigned read_no(const char** s)
{
    unsigned r = 0U;

    for (; **s >= '0' && **s <= '9'; ++*s) {
        r *= 10U;
        r += (unsigned)(**s - '0');
    }

    return r;
}


/**
 * Scan printf-like format spec
 *
 * @param f [inout] the format spec pptr (to next char after '%')
 * @param w [out] width
 * @param p [out] precision
 * @param ps [out] precision specified? 0/1
 * @param j [out] justification: 0 - right/zeros, 1 - right/spaces, -1 - left
 * @param m [out] type modificator: like in format, except 'L' for ints means "ll"
 */
static void scan_fmt(const char** f, unsigned* w, unsigned* p, int* ps, /* parasoft-suppress METRICS-18 "Universal number<->string conversion algorithm." */ 
    int* j, char* m)
{
    *j = 1;

    switch (**f) {
        case 0:
        case '%': return;
        case '0': ++*f; *j = 0; break;
        case '-': ++*f; *j = -1; break;
        default: break;
    };

    *w = read_no(f);

    if (**f == '.') {
        ++*f;
        *ps = 1;
        *p = read_no(f);
    } else {
        *ps = 0;
    }

    *m = 0;

    switch (**f)
    {
        case 'h':
        case 'L':
            *m = *(*f)++;
            break;

        case 'l':
            if (*(*f + 1) == 'l') {
                *m = 'L';
                *f += 2;
                break;
            }
            *m = 'l';
            ++*f;
            break;

        case 'I':
            if ((*(*f + 1) == '6') && (*(*f + 2) == '4')) {
                *m = 'L';
                *f += 3;
            }
            break;

        default: break;
    };
}

/*--------------------------------------------------------------------------------------------
 * Internal runtime API (function used in other runtime source files, local*)
 *------------------------------------------------------------------------------------------*/

/**
 * Unsigned value to number buffer.
 *
 * Converts unsigned number to string representation
 * (doesn't care of the terminating null).
 * !Function decrements the buffer pointer parameter!
 *
 * @param v the number
 * @param b [in] buffer mid-pptr or end-pptr, [out] pptr to 1st char
 * @param r radix
 * @param a base char for ciphers above 9
 */
void localU2NB(CPPTEST_UINTEGER v, char** b, unsigned char r, char a)
{
    do {
        const unsigned char c = (unsigned char)(v % r); /* cipher */
        --(*b);
        **b = (c < 10U) ? ((unsigned char)'0' + c) : ((unsigned char)a + (c - 10U));
        v /= r;
    } while (v);
}

/**
 * Local implementation of vsprintf
 *
 * @param ob output buffer
 * @param f format string
 * @param vl arguments
 *
 * @return number of chars stored
*/
unsigned localVSPrintF(char* ob, const char* f, va_list vl) /* parasoft-suppress METRICS-18 "Universal number<->string conversion algorithm." */ 
{
    const char* const obs = ob; /* output buffer start */
    while (*f) {
        const char* const fs = f; /* saved format position */
        unsigned l; /* format-spec-less string length */

        for (; *f && (*f != '%'); ++f);
        l = f - fs;

        memcpy(ob, fs, l);
        ob += l;

        if (*f) {
            /* format-spec parameters: */
            unsigned w, p; /* width, precision */
            int ps, j; /* precision_specified, justification */
            char t, m; /* type, modificator */

            ++f;

            scan_fmt(&f, &w, &p, &ps, &j, &m);

            t = *f;

            switch (t)
            {
                case '%':
                    *ob++ = '%';
                    break;

                case 'c':
                    {
                        /* one-char string */
                        char b[2];
                        b[0] = (char) va_arg(vl, int);
                        b[1] = 0;

                        prints(&ob, b, 1U, w, j);
                    }
                    break;

                case 's':
                    {
                        const char* const s = va_arg(vl, char*);

                        prints(&ob, s ? s : "(null)", ps ? p : UINT_MAX,
                            w, j);
                    }
                    break;

                case 'd':
                case 'i':
                    {
                        CPPTEST_INTEGER v = 0;
                        char b[19]; /* buffer - max unsigned long long decimal has 19 chars */
                        char* const be = b + 19; /* buffer end */
                        char* bp = be; /* buffer ptr */
                        int n; /* is number negative */
                        char hp; /* help variable */


                        switch (m)
                        {
                            case 0:
                                v = va_arg(vl, int);
                                break;

                            case 'h':
                                v = (short) va_arg(vl, int);
                                break;

                            case 'l':
                                v = va_arg(vl, long);
                                break;

                            case 'L':
                                v = va_arg(vl, CPPTEST_INTEGER);
                                break;

                            default: break;
                        };

                        n = v < 0;

                        localU2NB(n ? -v : v, &bp, 10U, 0);
                        hp = n ? '-' : 0;
                        printi(&ob, bp, be - bp, ps ? p : 0U, w,
                            hp, j);
                    }
                    break;

                case 'o':
                case 'u':
                case 'x':
                case 'X':
                    {
                        CPPTEST_UINTEGER v = 0U;
                        char b[22]; /* buffer - max unsigned long long octal has 22 chars*/
                        char* const be = b + 22; /* buffer end */
                        char* bp = be; /* buffer ptr */
                        unsigned rx = 10U; /* radix */
                        char bc = 'a'; /* base char digit */

                        switch (m)
                        {
                            case 0:
                                v = va_arg(vl, unsigned);
                                break;

                            case 'h':
                                v = (unsigned short) va_arg(vl, unsigned);
                                break;

                            case 'l':
                                v = va_arg(vl, unsigned long);
                                break;

                            case 'L':
                                v = va_arg(vl, CPPTEST_UINTEGER);
                                break;

                            default: break;
                        };

                        switch (t)
                        {
                            case 'o':
                                rx = 8U;
                                break;

                            case 'u':
                                rx = 10U;
                                break;

                            case 'x':
                                rx = 16U;
                                bc = 'a';
                                break;

                            case 'X':
                                rx = 16U;
                                bc = 'A';
                                break;

                            default: break;
                        };

                        localU2NB(v, &bp, (unsigned char)rx, bc);
                        printi(&ob, bp, be - bp, ps ? p : 0U, w, 0, j);
                    }
                    break;

                    default: break;
            };

            if (t) {
                ++f;
            }
        }
    }

    *ob = 0;

    return (unsigned)(ob - obs);
}

int localSPrintF(char* s, const char* format, ...)
{
    int r;
    va_list vl;
    va_start(vl, format);
    r = localVSPrintF(s, format, vl);
    va_end(vl);
    return r;
}

#endif /* CPPTEST_PRINTF_IMPL */

/*--------------------------------------------------------------------------------------------
 * Functions from runtime API (cpptest*)
 *------------------------------------------------------------------------------------------*/

int CDECL_CALL cpptestCharPtrsEqual(const char* expected, const char* actual)
{
    if (expected == actual) {
        return 1;
    } else if ((0 == expected) || (0 == actual)) {
        return 0;
    }

    return (0 == strcmp(expected, actual));
}

int CDECL_CALL cpptestCharPtrsNEqual(const char* expected, const char* actual, unsigned int maxSize)
{
    if (expected == actual) {
        return 1;
    } else if ((0 == expected) || (0 == actual)) {
        return 0;
    } else if (maxSize == 0U) {
        return 1;
    }
    while (*expected && *actual && *expected == *actual) {
        if (--maxSize == 0U) {
            break;
        }
        ++expected;
        ++actual;
    }
    return (maxSize == 0U) || (*expected == *actual);
}

#if CPPTEST_WCHAR_ENABLED
int cpptestWCharPtrsEqual(const void* expected, const void* actual)
{
    const wchar_t* expectedWCharPtr = (const wchar_t*)expected;
    const wchar_t* actualWCharPtr = (const wchar_t*)actual;

    if (expectedWCharPtr == actualWCharPtr) {
        return 1;
    } else if ((0 == expectedWCharPtr) || (0 == actualWCharPtr)) {
        return 0;
    }
    while (*expectedWCharPtr && *actualWCharPtr && *expectedWCharPtr == *actualWCharPtr) {
        ++expectedWCharPtr;
        ++actualWCharPtr;
    }
    return *expectedWCharPtr == *actualWCharPtr;
}

int cpptestWCharPtrsNEqual(const void* expected, const void* actual, unsigned int maxSize)
{
    const wchar_t* expectedWCharPtr = (const wchar_t*)expected;
    const wchar_t* actualWCharPtr = (const wchar_t*)actual;

    if (expectedWCharPtr == actual) {
        return 1;
    } else if ((0 == expectedWCharPtr) || (0 == actualWCharPtr)) {
        return 0;
    } else if (maxSize == 0U) {
        return 1;
    }
    while (*expectedWCharPtr && *actualWCharPtr && *expectedWCharPtr == *actualWCharPtr) {
        if (--maxSize == 0U) {
            break;
        }
        ++expectedWCharPtr;
        ++actualWCharPtr;
    }
    return (maxSize == 0U) || (*expectedWCharPtr == *actualWCharPtr);
}
#endif

int CDECL_CALL cpptestMemBuffersEqual(const void* expected, const void* actual, unsigned int size)
{
    const char* expectedStr = (const char*)expected;
    const char* actualStr = (const char*)actual;

    if (expectedStr == actualStr) {
        return 1;
    } else if ((0 == expectedStr) || (0 == actualStr)) {
        return 0;
    } else if (size == 0) {
        return 1;
    }
    while (*expectedStr == *actualStr) {
        if (--size == 0U) {
            break;
        }
        ++expectedStr;
        ++actualStr;
    }
    return size == 0U;
}

#ifndef __KERNEL__
int CDECL_CALL cpptestFloatsFmtEqual(const char* expected, CPPTEST_FLOAT actual)
{
    char buf[24];
    
    cpptestSPrintF(buf, CPPTEST_PRINTF_FLOAT_FMT, (CPPTEST_PRINTF_FLOAT)actual);
    return cpptestCharPtrsEqual(expected, buf);
}
#endif



static char** cpptestCmdLineToArgvR(const char* cmdLine, int* readedArgs)
{
    int argNumber = *readedArgs;
    const char* beginArg = cmdLine;
    const char* endArg = cmdLine;
    int insideStr = 0;

    while (*beginArg == ' ' || *beginArg == '\t') {
        ++beginArg;
    }

    if (!*beginArg) {
        char** argv = (char**)localInternalMalloc(sizeof(char*) * (*readedArgs + 1));
        argv[*readedArgs] = 0;
        return argv;
    }

    endArg = beginArg;
    while (*endArg) {
        if (insideStr) {
            if (*endArg == '"') {
                ++endArg;
                insideStr = 0;
            } else {
                if (*endArg == '\\') {
                    endArg += 2;
                } else {
                    ++endArg;
                }
            }
        } else {
            if (*endArg == ' ' || *endArg == '\t') {
                break;
            } else if (*endArg == '\\') {
                endArg += 2;
            } else if (*endArg == '"') {
                insideStr = 1;
                ++endArg;
            } else {
                ++endArg;
            }
        }
    }

    {
    unsigned int argSize = (unsigned int)(endArg - beginArg);
    char* arg = 0;
    char** argv = 0;

    if ((*beginArg == '"') && (*(endArg - 1) == '"')) {
        ++beginArg;
        argSize -= 2U;
    }
    arg = (char*)localInternalMalloc(argSize + 1U);

    ++(*readedArgs);
    memcpy(arg, beginArg, argSize);
    arg[argSize] = 0;

    argv = cpptestCmdLineToArgvR(endArg, readedArgs);
    argv[argNumber] = arg;
    return argv;
    }
}


#if CPPTEST_WCHAR_ENABLED

static wchar_t** cpptestCmdLineToArgvWR(const wchar_t* cmdLine, int* readedArgs)
{
    int argNumber = *readedArgs;
    const wchar_t* beginArg = cmdLine;
    const wchar_t* endArg = cmdLine;
    int insideStr = 0;

    while (*beginArg == L' ' || *beginArg == L'\t') {
        ++beginArg;
    }

    if (!*beginArg) {
        wchar_t** argv = (wchar_t**)localInternalMalloc(sizeof(wchar_t*) * (*readedArgs + 1));
        argv[*readedArgs] = 0;
        return argv;
    }

    endArg = beginArg;
    while (*endArg) {
        if (insideStr) {
            if (*endArg == L'"') {
                ++endArg;
                insideStr = 0;
            } else {
                if (*endArg == L'\\') {
                    endArg += 2;
                } else {
                    ++endArg;
                }
            }
        } else {
            if (*endArg == L' ' || *endArg == L'\t') {
                break;
            } else if (*endArg == L'\\') {
                endArg += 2;
            } else if (*endArg == L'"') {
                insideStr = 1;
                ++endArg;
            } else {
                ++endArg;
            }
        }
    }

    {
    unsigned int argSize = (unsigned int)(endArg - beginArg);
    wchar_t* arg = 0;
    wchar_t** argv = 0;

    if ((*beginArg == L'"') && (*(endArg - 1) == L'"')) {
        ++beginArg;
        argSize -= 2U;
    }
    arg = (wchar_t*)localInternalMalloc(sizeof(wchar_t) * (argSize + 1U));

    ++(*readedArgs);
    memcpy(arg, beginArg, sizeof(wchar_t) * argSize);
    arg[argSize] = 0U;

    argv = cpptestCmdLineToArgvWR(endArg, readedArgs);
    argv[argNumber] = arg;
    return argv;
    }
}

#endif /* CPPTEST_WCHAR_ENABLED */

const char* CDECL_CALL cpptestStringPool(int id, const char* str) 
{
    return str;
}

/*--------------------------------------------------------------------------------------------
 * Functions from C++Test Runtime API (CppTest_*)
 *------------------------------------------------------------------------------------------*/

char** CDECL_CALL CppTest_CmdLineToArgv(const char* cmdLine, int* argc)
{
    int readedArgs = 0;
    char** argv = cpptestCmdLineToArgvR(cmdLine, &readedArgs);
    *argc = readedArgs;
    return argv;
}

void CDECL_CALL CppTest_FreeArgv(int argc, char* argv[])
{
    int i;
    for (i = 0; i < argc; i++) {
        localInternalFree(argv[i]);
    }
    localInternalFree(argv);
}

#if CPPTEST_WCHAR_ENABLED

wchar_t** CDECL_CALL CppTest_CmdLineToArgvW(const wchar_t* cmdLine, int* argc)
{
    int readedArgs = 0;
    wchar_t** argv = cpptestCmdLineToArgvWR(cmdLine, &readedArgs);
    *argc = readedArgs;
    return argv;
}

void CDECL_CALL CppTest_FreeArgvW(int argc, wchar_t* argv[])
{
    int i;
    for (i = 0; i < argc; i++) {
        localInternalFree(argv[i]);
    }
    localInternalFree(argv);
}

#endif  /* CPPTEST_WCHAR_ENABLED */

int CDECL_CALL CppTest_AtomicIncrement(int* value)
{
#if CPPTEST_HAS_INTERLOCKED_INCREMENT
    return InterlockedIncrement((LONG*)value);
#elif CPPTEST_HAS_SYNC_FETCH_AND_ADD
    return __sync_fetch_and_add(value, 1) + 1;
#else 
    CPPTEST_LOCK_DECLARE(static, atomicLock)
    int result = 0;
    CPPTEST_LOCK(atomicLock)
    result = ++*value;
    CPPTEST_UNLOCK(atomicLock)
    return result;
#endif
}
