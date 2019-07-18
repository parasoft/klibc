/*
 * (C) Copyright Parasoft Corporation 1998 - 2019.  All rights reserved.
 * The use of this source code is governed by the general Parasoft License Agreement.
 * Redistribution strictly prohibited.
 */

#define CPPTEST_RUNTIME_IMPLEMENTATION

#include "cpptest_runtime.h"
#include "cpptest_headers.h"

#include "CppTestInternal.h"

#include CPPTEST_float


/*--------------------------------------------------------------------------------------------
 * Internal runtime API (function used in other runtime source files, local*)
 *------------------------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------------------------
 * Functions from runtime API (cpptest*)
 *------------------------------------------------------------------------------------------*/

char CDECL_CALL cpptestLimitsGetMaxChar(void)
{
    return CHAR_MAX;
}

char CDECL_CALL cpptestLimitsGetMinChar(void)
{
    return CHAR_MIN;
}

signed char CDECL_CALL cpptestLimitsGetMaxSignedChar(void)
{
    return SCHAR_MAX;
}

signed char CDECL_CALL cpptestLimitsGetMinSignedChar(void)
{
    return SCHAR_MIN;
}

unsigned char CDECL_CALL cpptestLimitsGetMaxUnsignedChar(void)
{
    return UCHAR_MAX;
}


short CDECL_CALL cpptestLimitsGetMaxShort(void)
{
    return SHRT_MAX;
}

short CDECL_CALL cpptestLimitsGetMinShort(void)
{
    return SHRT_MIN;
}

unsigned short CDECL_CALL cpptestLimitsGetMaxUnsignedShort(void)
{
    return USHRT_MAX;
}


int CDECL_CALL cpptestLimitsGetMaxInt(void)
{
    return INT_MAX;
}

int CDECL_CALL cpptestLimitsGetMinInt(void)
{
    return INT_MIN;
}

unsigned int CDECL_CALL cpptestLimitsGetMaxUnsignedInt(void)
{
    return UINT_MAX;
}


long CDECL_CALL cpptestLimitsGetMaxLong(void)
{
    return LONG_MAX;
}

long CDECL_CALL cpptestLimitsGetMinLong(void)
{
    return LONG_MIN;
}

unsigned long CDECL_CALL cpptestLimitsGetMaxUnsignedLong(void)
{
    return ULONG_MAX;
}

#ifndef __KERNEL__
float CDECL_CALL cpptestLimitsGetMaxPosFloat(void)
{
    return FLT_MAX;
}

float CDECL_CALL cpptestLimitsGetMinNegFloat(void)
{
    return (- FLT_MAX);
}

float CDECL_CALL cpptestLimitsGetMaxNegFloat(void)
{
    return (- FLT_MIN);
}

float CDECL_CALL cpptestLimitsGetMinPosFloat(void)
{
    return FLT_MIN;
}


double CDECL_CALL cpptestLimitsGetMaxPosDouble(void)
{
    return DBL_MAX;
}

double CDECL_CALL cpptestLimitsGetMinNegDouble(void)
{
    return (- DBL_MAX);
}

double CDECL_CALL cpptestLimitsGetMaxNegDouble(void)
{
    return (- DBL_MIN);
}

double CDECL_CALL cpptestLimitsGetMinPosDouble(void)
{
    return DBL_MIN;
}


long double CDECL_CALL cpptestLimitsGetMaxPosLongDouble(void)
{
    return LDBL_MAX;
}

long double CDECL_CALL cpptestLimitsGetMinNegLongDouble(void)
{
    return (- LDBL_MAX);
}

long double CDECL_CALL cpptestLimitsGetMaxNegLongDouble(void)
{
    return (- LDBL_MIN);
}

long double CDECL_CALL cpptestLimitsGetMinPosLongDouble(void)
{
    return LDBL_MIN;
}
#endif

/*--------------------------------------------------------------------------------------------
 * Functions from C++Test Runtime API (CppTest_*)
 *------------------------------------------------------------------------------------------*/

