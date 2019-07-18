/*
 * (C) Copyright Parasoft Corporation 1998 - 2019.  All rights reserved.
 * The use of this source code is governed by the general Parasoft License Agreement.
 * Redistribution strictly prohibited.
 */
#define CPPTEST_RUNTIME_IMPLEMENTATION

#include "cpptest_runtime.h"
#include "cpptest_message.h"
#include "cpptest_transport.h"
#include "cpptest_common.h"
#include "cpptest_headers.h"

#include "CppTestInternal.h"


#define NANO_MIN -999999999L
#define NANO_MAX 999999999L
#define NANO 1000000000L

/*****************************************************************************/
#if CPPTEST_TIME_MODE == CPPTEST_TIME_NOTIME

#  define CPPTEST_TIME_CURRENT(seconds, nanoseconds) \
    seconds = 0L; \
    nanoseconds = 0L;

#  define CPPTEST_HAS_NANOSECONDS 0
#  define CPPTEST_TIME_PRECISION "0"

/*****************************************************************************/
#elif CPPTEST_TIME_MODE == CPPTEST_TIME_ANSI

#  include CPPTEST_time
#  define CPPTEST_TIME_CURRENT(seconds, nanoseconds) \
    seconds = (CPPTEST_INTEGER) CPPTEST_C_std time(0); \
    nanoseconds = 0L;

#  define CPPTEST_HAS_NANOSECONDS 0
#  define CPPTEST_TIME_PRECISION "0"

/*****************************************************************************/
#elif CPPTEST_TIME_MODE == CPPTEST_TIME_GETTIMEOFDAY

#  include <sys/time.h>
#  include CPPTEST_time
#  define CPPTEST_TIME_CURRENT(seconds, nanoseconds) \
    do { \
        struct timeval tv; \
        gettimeofday(&tv, 0); \
        seconds = tv.tv_sec; \
        nanoseconds = (tv.tv_usec) * (1000L); \
    } while(0);

#  define CPPTEST_HAS_NANOSECONDS 1
#  define CPPTEST_TIME_PRECISION "6"

/*****************************************************************************/
#elif CPPTEST_TIME_MODE == CPPTEST_TIME_TIMEB

#  include <sys/timeb.h>
#  include CPPTEST_time
#  define CPPTEST_TIME_CURRENT(seconds, nanoseconds) \
    do { \
        struct timeb tb; \
        ftime(&tb); \
        seconds = (CPPTEST_INTEGER)tb.time; \
        nanoseconds = ((CPPTEST_INTEGER)tb.millitm) * (1000000L); \
    } while(0);

#  define CPPTEST_HAS_NANOSECONDS 1
#  define CPPTEST_TIME_PRECISION "6"

/*****************************************************************************/
#elif CPPTEST_TIME_MODE == CPPTEST_TIME_TICKLIB

#  include <drv/timer/timerDev.h>
#  include <tickLib.h>
#  define CPPTEST_TIME_CURRENT(seconds, nanoseconds) \
    do { \
        CPPTEST_UINTEGER tick = tickGet(); \
        CPPTEST_UINTEGER tickRate = sysClkRateGet(); \
        seconds = (tick) / (tickRate); \
        nanoseconds = ((double)(tick % tickRate) / (tickRate)) * 1000000000L; \
    } while(0);

#  define CPPTEST_HAS_NANOSECONDS 1
#  define CPPTEST_TIME_PRECISION "2"

/*****************************************************************************/
#elif CPPTEST_TIME_MODE == CPPTEST_TIME_NXT_SYSTICK

#  include "systick.h"
#  define CPPTEST_TIME_CURRENT(seconds, nanoseconds) \
    do { \
        const U32 ms = systick_get_ms(); \
        seconds = (ms) / (1000L); \
        nanoseconds = (ms % 1000L) * 1000000L; \
    } while(0);

#  define CPPTEST_HAS_NANOSECONDS 1
#  define CPPTEST_TIME_PRECISION "3"

/*****************************************************************************/
#endif /* CPPTEST_TIME_MODE */


/* Currently executed test case start time */
static CPPTEST_UINTEGER startTime = 0U;
/* Time out per test case (in miliseconds) */
static CPPTEST_UINTEGER timeout = 0U;
/* Timeout tracking switch */
static int trackTimeout = 0;

static CPPTEST_UINTEGER getTime(void)
{
    CppTest_Time t;
    CPPTEST_TIME_CURRENT(t.seconds, t.nanoseconds);
#if CPPTEST_HAS_NANOSECONDS
    return (CPPTEST_UINTEGER)t.seconds * 1000UL + (CPPTEST_UINTEGER)t.nanoseconds / 1000000UL;
#else
    return (CPPTEST_UINTEGER)t.seconds * 1000UL;
#endif
}

/*--------------------------------------------------------------------------------------------
 * Internal runtime API (function used in other runtime source files, local*)
 *------------------------------------------------------------------------------------------*/

void localSetTimeout(CPPTEST_UINTEGER aTimeout)
{
    timeout = aTimeout;
}

void localTestCaseStartTime()
{
    startTime = getTime();
    trackTimeout = 1;
}

void localTestCaseEndTime()
{
    trackTimeout = 0;
}

int localTimeToString(char* buffer, CppTest_Time t)
{
    return cpptestSPrintF(buffer, "d", t.seconds + t.nanoseconds/1000000000);
}

/*--------------------------------------------------------------------------------------------
 * Functions from runtime API (cpptest*)
 *------------------------------------------------------------------------------------------*/

/**
 * Checks test case timeout.
 */
void CDECL_CALL cpptestCheckTimeout(void)
{
    if (trackTimeout && timeout && ((getTime() - startTime) >= timeout)) {
        trackTimeout = 0;
        cpptestTimeout(timeout);
    }
}

/*--------------------------------------------------------------------------------------------
 * Functions from C++Test Runtime API (CppTest_*)
 *------------------------------------------------------------------------------------------*/

CppTest_Time CDECL_CALL CppTest_TimeInit(CPPTEST_INTEGER seconds, CPPTEST_INTEGER nanoseconds)
{
    CppTest_Time buf;
    buf.seconds = seconds;
    buf.nanoseconds = nanoseconds;

    return buf;
}

CppTest_Time CDECL_CALL CppTest_TimeCurrent()
{
    CppTest_Time buf;
    CPPTEST_TIME_CURRENT(buf.seconds, buf.nanoseconds);
    return buf;
}

CppTest_Time CDECL_CALL CppTest_TimeDiff(CppTest_Time t1, CppTest_Time  t2)
{
    CppTest_Time buf;

    buf.seconds = t1.seconds - t2.seconds;
    buf.nanoseconds = t1.nanoseconds - t2.nanoseconds;

    if (buf.nanoseconds < NANO_MIN) {
        --buf.seconds;
        buf.nanoseconds += NANO;
    }
    if (buf.nanoseconds > NANO_MAX) {
        ++buf.seconds;
        buf.nanoseconds -= NANO;
    }

    return buf;
}

int CDECL_CALL CppTest_TimeCompare(CppTest_Time t1, CppTest_Time t2)
{
    if (t1.seconds == t2.seconds) {
        if (t1.nanoseconds == t2.nanoseconds) {
            return 0;
        }
        return (t1.nanoseconds < t2.nanoseconds) ? -1 : 1;
    }
    return (t1.seconds < t2.seconds) ? -1 : 1;
}
