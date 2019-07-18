/*
 * (C) Copyright Parasoft Corporation 1998 - 2019.  All rights reserved.
 * The use of this source code is governed by the general Parasoft License Agreement.
 * Redistribution strictly prohibited.
 */

#ifndef __Parasoft_CppTest_h__
#define __Parasoft_CppTest_h__

#include "cpptest_runtime_u.h"
#include "cpptest_data_source.h"

#if CPPTEST_TRIGGER_ENABLED
#include "trigger/api.h"
#endif /* CPPTEST_TRIGGER_ENABLED */

#ifdef __cplusplus

#if CPPTEST_EXCEPTIONS_ENABLED && CPPTEST_SPECIAL_STD_EXCEPTIONS_HANDLING_ENABLED
#  include <exception>
#endif

struct __CPTR_AddressOfType {};
#define CPPTEST_GET_ADDRESS_OF(object_name) \
    (void*)&(__CPTR_AddressOfType&)(object_name)

#endif /* __cplusplus */

#define CPTSP(STR) cpptestStringPool(0, STR)

#ifdef __cplusplus
#  define CPPTEST_IS_CPP 1
#else
#  define CPPTEST_IS_CPP 0
#endif

/* Runtime functions declarations */

/*****************************************************************************
 *
 * Report Macros
 *
 *****************************************************************************/

#define CPPTEST_REPORT(txt) \
    cpptestReport(txt, stubCallInfo, CPTSP(__FILE__), __LINE__, 0);

#define CPPTEST_REPORT_BOOL(txt, value) \
    cpptestReportBool(txt, value ? CPPTEST_INT_TRUE : 0, stubCallInfo, CPTSP(__FILE__), __LINE__);

#define CPPTEST_REPORT_INTEGER(txt, value) \
    cpptestReportInteger(txt, value, stubCallInfo, CPTSP(__FILE__), __LINE__);

#define CPPTEST_REPORT_UINTEGER(txt, value) \
    cpptestReportUInteger(txt, value, stubCallInfo, CPTSP(__FILE__), __LINE__);
 
#define CPPTEST_REPORT_FLOAT(txt, value) \
    cpptestReportFloat(txt, value, stubCallInfo, CPTSP(__FILE__), __LINE__);

#define CPPTEST_REPORT_CSTR(txt, value) \
    cpptestReportCStr(txt, (char*)value, stubCallInfo, CPTSP(__FILE__), __LINE__);

#define CPPTEST_REPORT_CSTR_N(txt, value, max_size) \
    cpptestReportCStrN(txt, (char*)value, max_size, stubCallInfo, CPTSP(__FILE__), __LINE__);

#define CPPTEST_REPORT_MEM_BUFFER(txt, value, size) \
    cpptestReportMemBuffer(txt, (void*)value, size, stubCallInfo, CPTSP(__FILE__), __LINE__);

#define CPPTEST_REPORT_PTR(txt, value) \
    cpptestReportPtr(txt, (value == 0) ? CPPTEST_INT_TRUE : 0, stubCallInfo, CPTSP(__FILE__), __LINE__);

#define CPPTEST_REPORT_ENUM(type, txt, value) \
    cpptestReportEnum(txt, cpptestGetEnumInfo((int)(type)0, #type, CPTSP(__FILE__), __LINE__), (CPPTEST_INTEGER)(value), \
            stubCallInfo, CPPTEST_IS_CPP, CPTSP(__FILE__), __LINE__);

#define CPPTEST_REPORT_STACKTRACE(txt) \
    cpptestReport(txt, stubCallInfo, CPTSP(__FILE__), __LINE__, CPPTEST_INT_TRUE);

/* Deprecated: use CPPTEST_REPORT(txt) instead. */
#define CPPTEST_MESSAGE(message) CPPTEST_REPORT(message)


/*****************************************************************************
 *
 * Assertion Macros 
 *
 *****************************************************************************/

#define CPPTEST_FAIL(message) \
    cpptestAssertion(0, message, 0, stubCallInfo, CPTSP(__FILE__), __LINE__);

/* Assertion that condition is true */
#define CPPTEST_ASSERT(condition) \
    CPPTEST_ASSERT_HELPER(condition, 0)

/* Assertion with a user specific message */
#define CPPTEST_ASSERT_MESSAGE(message, condition) \
    CPPTEST_ASSERT_HELPER(condition, message)

/* Asserts that two values are equal */
#define CPPTEST_ASSERT_EQUAL(expected, actual) \
    CPPTEST_ASSERT_EQUAL_HELPER(expected, actual, 0)

/* Asserts that two values are equal (custom message) */
#define CPPTEST_ASSERT_EQUAL_MESSAGE(message, expected, actual) \
    CPPTEST_ASSERT_EQUAL_HELPER(expected, actual, message)

/* Asserts that two boolean values are equal */
#define CPPTEST_ASSERT_BOOL_EQUAL(expected, actual) \
    CPPTEST_ASSERT_BOOL_EQUAL_HELPER(expected, actual, 0)

/* Asserts that two boolean values are equal (custom message) */
#define CPPTEST_ASSERT_BOOL_EQUAL_MESSAGE(message, expected, actual) \
    CPPTEST_ASSERT_BOOL_EQUAL_HELPER(expected, actual, message)

/* Asserts that two integer values are equal */
#define CPPTEST_ASSERT_INTEGER_EQUAL(expected, actual) \
   CPPTEST_ASSERT_NUMBER_EQUAL_HELPER((expected) == (actual), expected, actual, 0, 0, cpptestIntegerEqualAssertion, 0)

/* Asserts that two integer values are equal (custom message) */
#define CPPTEST_ASSERT_INTEGER_EQUAL_MESSAGE(message, expected, actual) \
   CPPTEST_ASSERT_NUMBER_EQUAL_HELPER((expected) == (actual), expected, actual, 0, 0, cpptestIntegerEqualAssertion, message)

/* Asserts that two integer values are equal */
#define CPPTEST_ASSERT_INTEGER_EQUAL_DELTA(expected, actual, delta) \
   CPPTEST_ASSERT_NUMBER_EQUAL_HELPER(CPPTEST_EQUAL_DELTA(expected, actual, delta), expected, actual, 1, delta, cpptestIntegerEqualAssertion, 0)

/* Asserts that two integer values are equal (custom message) */
#define CPPTEST_ASSERT_INTEGER_EQUAL_DELTA_MESSAGE(message, expected, actual, delta) \
   CPPTEST_ASSERT_NUMBER_EQUAL_HELPER(CPPTEST_EQUAL_DELTA(expected, actual, delta), expected, actual, 1, delta, cpptestIntegerEqualAssertion, message)

/* Asserts that two integer arrays are equal */
#define CPPTEST_ASSERT_INTEGER_ARRAY_EQUAL(expected, actual, size) \
    CPPTEST_ASSERT_NUMBER_ARRAY_EQUAL_HELPER(expected, actual, size, 0, 0, cpptestIntegerEqualAssertion)

/* Asserts that two integer arrays are equal */
#define CPPTEST_ASSERT_INTEGER_ARRAY_EQUAL_DELTA(expected, actual, size, delta) \
    CPPTEST_ASSERT_NUMBER_ARRAY_EQUAL_HELPER(expected, actual, size, 1, delta, cpptestIntegerEqualAssertion)

/* Asserts that two unsigned integer values are equal */
#define CPPTEST_ASSERT_UINTEGER_EQUAL(expected, actual) \
   CPPTEST_ASSERT_NUMBER_EQUAL_HELPER((expected) == (actual), expected, actual, 0, 0, cpptestUIntegerEqualAssertion, 0)

/* Asserts that two unsigned integer values are equal (custom message) */
#define CPPTEST_ASSERT_UINTEGER_EQUAL_MESSAGE(message, expected, actual) \
   CPPTEST_ASSERT_NUMBER_EQUAL_HELPER((expected) == (actual), expected, actual, 0, 0, cpptestUIntegerEqualAssertion, message)

/* Asserts that two unsigned integer values are equal */
#define CPPTEST_ASSERT_UINTEGER_EQUAL_DELTA(expected, actual, delta) \
   CPPTEST_ASSERT_NUMBER_EQUAL_HELPER(CPPTEST_EQUAL_DELTA(expected, actual, delta), expected, actual, 1, delta, cpptestUIntegerEqualAssertion, 0)

/* Asserts that two unsigned integer values are equal (custom message) */
#define CPPTEST_ASSERT_UINTEGER_EQUAL_DELTA_MESSAGE(message, expected, actual, delta) \
   CPPTEST_ASSERT_NUMBER_EQUAL_HELPER(CPPTEST_EQUAL_DELTA(expected, actual, delta), expected, actual, 1, delta, cpptestUIntegerEqualAssertion, message)

/* Asserts that two unsigned integer arrays are equal */
#define CPPTEST_ASSERT_UINTEGER_ARRAY_EQUAL(expected, actual, size) \
    CPPTEST_ASSERT_NUMBER_ARRAY_EQUAL_HELPER(expected, actual, size, 0, 0, cpptestUIntegerEqualAssertion)

/* Asserts that two unsigned integer arrays are equal */
#define CPPTEST_ASSERT_UINTEGER_ARRAY_EQUAL_DELTA(expected, actual, size, delta) \
    CPPTEST_ASSERT_NUMBER_ARRAY_EQUAL_HELPER(expected, actual, size, 1, delta, cpptestUIntegerEqualAssertion)

/* Asserts that two float values are equal */
#define CPPTEST_ASSERT_FLOAT_EQUAL_DELTA(expected, actual, delta) \
   CPPTEST_ASSERT_NUMBER_EQUAL_HELPER(CPPTEST_EQUAL_DELTA(expected, actual, delta), expected, actual, 1, delta, cpptestFloatEqualAssertion, 0)

/* Asserts that two float values are equal (custom message) */
#define CPPTEST_ASSERT_FLOAT_EQUAL_DELTA_MESSAGE(message, expected, actual, delta) \
   CPPTEST_ASSERT_NUMBER_EQUAL_HELPER(CPPTEST_EQUAL_DELTA(expected, actual, delta), expected, actual, 1, delta, cpptestFloatEqualAssertion, message)

/* Asserts that two float arrays are equal */
#define CPPTEST_ASSERT_FLOAT_ARRAY_EQUAL_DELTA(expected, actual, size, delta) \
    CPPTEST_ASSERT_NUMBER_ARRAY_EQUAL_HELPER(expected, actual, size, 1, delta, cpptestFloatEqualAssertion)

/* Deprecated - use CPPTEST_ASSERT_FLOAT_EQUAL_DELTA macro */
#define CPPTEST_ASSERT_FLOAT_EQUAL(expected, actual, delta) \
   CPPTEST_ASSERT_FLOAT_EQUAL_DELTA(expected, actual, delta)

/* Deprecated - use CPPTEST_ASSERT_FLOAT_EQUAL_DELTA macro */
#define CPPTEST_ASSERT_DOUBLES_EQUAL(expected, actual, delta) \
   CPPTEST_ASSERT_FLOAT_EQUAL_DELTA(expected, actual, delta)

/* Deprecated - use CPPTEST_ASSERT_FLOAT_EQUAL_DELTA_MESSAGE macro */
#define CPPTEST_ASSERT_DOUBLES_EQUAL_MESSAGE(message, expected, actual, delta) \
   CPPTEST_ASSERT_FLOAT_EQUAL_DELTA_MESSAGE(message, expected, actual, delta)

/* Asserts that actual double value has format specified as expected */
#define CPPTEST_ASSERT_DOUBLES_FMT_EQUAL(expected, actual) \
   CPPTEST_ASSERT_FMT_EQUAL_HELPER(CPPTEST_FLOATS_FMT_EQUAL(expected, actual), expected, actual, cpptestFloatFmtEqualAssertion, 0)

/* Asserts that actual double value has format specified as expected (custom message) */
#define CPPTEST_ASSERT_DOUBLES_FMT_EQUAL_MESSAGE(message, expected, actual) \
   CPPTEST_ASSERT_FMT_EQUAL_HELPER(CPPTEST_FLOATS_FMT_EQUAL(expected, actual), expected, actual, cpptestFloatFmtEqualAssertion, message)

/* Asserts that masked actual and expected bits are equal */
#define CPPTEST_ASSERT_BITS_MESSAGE(message, mask, expected, actual)	\
   CPPTEST_ASSERT_BITS_HELPER((mask & expected) == (mask & actual), mask, expected, mask & actual, cpptestBitAssertion, CPTSP("" CPT_TO_STRING(actual)), message, 0)

#define CPPTEST_ASSERT_BITS(mask, expected, actual)	\
   CPPTEST_ASSERT_BITS_HELPER((mask & expected) == (mask & actual), mask, expected, mask & actual, cpptestBitAssertion, CPTSP("" CPT_TO_STRING(actual)), 0, 0)

/* Asserts that masked bits are high */
#define CPPTEST_ASSERT_BITS_HIGH_MESSAGE(message, mask, actual)	\
   CPPTEST_ASSERT_BITS_HELPER((mask & actual) == mask, mask, mask, mask & actual, cpptestBitAssertion, CPTSP("" CPT_TO_STRING(actual)), message, 0)

#define CPPTEST_ASSERT_BITS_HIGH(mask, actual)	  \
   CPPTEST_ASSERT_BITS_HELPER((mask & actual) == mask, mask, mask, mask & actual, cpptestBitAssertion, CPTSP("" CPT_TO_STRING(actual)), 0, 0)

/* Asserts that mased bits are low */
#define CPPTEST_ASSERT_BITS_LOW_MESSAGE(message, mask, actual)	\
   CPPTEST_ASSERT_BITS_HELPER(!(mask & actual), mask, mask, actual, cpptestBitAssertion, CPTSP("" CPT_TO_STRING(actual)), message, ~0);

#define CPPTEST_ASSERT_BITS_LOW(mask, actual) \
   CPPTEST_ASSERT_BITS_HELPER(!(mask & actual), mask, mask, actual, cpptestBitAssertion, CPTSP("" CPT_TO_STRING(actual)), 0, ~0);

/* Asserts that n'th bit is high */
#define CPPTEST_ASSERT_BIT_HIGH_MESSAGE(message, /* n'th */bit, actual)	\
   CPPTEST_ASSERT_BITS_HELPER(actual & (1<<bit), (1<<bit), (1<<bit), actual, cpptestBitAssertion, CPTSP("" CPT_TO_STRING(actual)), message, 0)

#define CPPTEST_ASSERT_BIT_HIGH(bit, actual)	\
   CPPTEST_ASSERT_BITS_HELPER(actual & (1<<bit), (1<<(bit)), (1<<(bit)), actual, cpptestBitAssertion, CPTSP("" CPT_TO_STRING(actual)), 0, 0)

/* Asserts that n'th bit is low */
#define CPPTEST_ASSERT_BIT_LOW_MESSAGE(message, /* n'th */bit, actual)	\
   CPPTEST_ASSERT_BITS_HELPER(!(actual & (1<<bit)), (1<<bit), 0, (1<<bit) & actual, cpptestBitAssertion, CPTSP("" CPT_TO_STRING(actual)), message, 0)

#define CPPTEST_ASSERT_BIT_LOW(bit, actual)	\
   CPPTEST_ASSERT_BITS_HELPER(!(actual & (1<<bit)), (1<<bit), 0, (1<<bit) & actual, cpptestBitAssertion, CPTSP("" CPT_TO_STRING(actual)), 0, 0)

/* Asserts that two C-style strings are equal */
#define CPPTEST_ASSERT_CSTR_EQUAL(expected, actual) \
    CPPTEST_ASSERT_TYPE_EQUAL_HELPER(CPPTEST_CSTRS_EQUAL(expected, actual), expected, actual, cpptestCstrEqualAssertion, 0)

/* Asserts that two C-style strings are equal (custom message) */
#define CPPTEST_ASSERT_CSTR_EQUAL_MESSAGE(message, expected, actual) \
    CPPTEST_ASSERT_TYPE_EQUAL_HELPER(CPPTEST_CSTRS_EQUAL(expected, actual), expected, actual, cpptestCstrEqualAssertion, message)

/* Asserts that two C-style string arrays are equal */
#define CPPTEST_ASSERT_CSTR_ARRAY_EQUAL(expected, actual, size) \
    CPPTEST_ASSERT_ARRAY_EQUAL_CSTR_HELPER(expected, actual, size, cpptestCstrEqualAssertion)

/* Asserts that two C-style strings are equal (with the max number of characters to compare) */
#define CPPTEST_ASSERT_CSTR_N_EQUAL(expected, actual, max_size) \
    CPPTEST_ASSERT_MEM_EQUAL_HELPER(CPPTEST_CSTRS_N_EQUAL(expected, actual, max_size), expected, actual, max_size, cpptestCstrNEqualAssertion, 0)

/* Asserts that two C-style strings are equal (with the max number of characters to compare) (custom message)*/
#define CPPTEST_ASSERT_CSTR_N_EQUAL_MESSAGE(message, expected, actual, max_size) \
    CPPTEST_ASSERT_MEM_EQUAL_HELPER(CPPTEST_CSTRS_N_EQUAL(expected, actual, max_size), expected, actual, max_size, cpptestCstrNEqualAssertion, message)

/* Asserts that two data buffers are equal (compares size number of bytes) */
#define CPPTEST_ASSERT_MEM_BUFFER_EQUAL(expected, actual, size) \
    CPPTEST_ASSERT_MEM_EQUAL_HELPER(CPPTEST_MEM_BUFFERS_EQUAL(expected, actual, size), expected, actual, size, cpptestMemBufferEqualAssertion, 0)

/* Asserts that two data buffers are equal (compares size number of bytes) (custom message) */
#define CPPTEST_ASSERT_MEM_BUFFER_EQUAL_MESSAGE(message, expected, actual, size) \
    CPPTEST_ASSERT_MEM_EQUAL_HELPER(CPPTEST_MEM_BUFFERS_EQUAL(expected, actual, size), expected, actual, size, cpptestMemBufferEqualAssertion, message)

/* Asserts that two pointer values are equal */
#define CPPTEST_ASSERT_PTR_EQUAL(expected, actual) \
   CPPTEST_ASSERT_TYPE_EQUAL_HELPER((expected) == (actual), expected, actual, cpptestPtrEqualAssertion, 0)

/* Asserts that two pointer values are equal (custom message) */
#define CPPTEST_ASSERT_PTR_EQUAL_MESSAGE(message, expected, actual) \
   CPPTEST_ASSERT_TYPE_EQUAL_HELPER((expected) == (actual), expected, actual, cpptestPtrEqualAssertion, message)

/* Asserts that two pointer arrays are equal */
#define CPPTEST_ASSERT_PTR_ARRAY_EQUAL(expected, actual, size) \
    CPPTEST_ASSERT_TYPE_ARRAY_EQUAL_HELPER(expected, actual, size, cpptestPtrEqualAssertion)

#if CPPTEST_WCHAR_ENABLED
/* Asserts that two C-style wide character strings are equal */
#define CPPTEST_ASSERT_WCSTR_EQUAL(expected, actual) \
    CPPTEST_ASSERT_TYPE_EQUAL_HELPER(CPPTEST_WCSTRS_EQUAL(expected, actual), expected, actual, cpptestWCstrEqualAssertion, 0)

/* Asserts that two C-style wide character strings are equal (custom message) */
#define CPPTEST_ASSERT_WCSTR_EQUAL_MESSAGE(message, expected, actual) \
    CPPTEST_ASSERT_TYPE_EQUAL_HELPER(CPPTEST_WCSTRS_EQUAL(expected, actual), expected, actual, cpptestWCstrEqualAssertion, message)
#endif  /* CPPTEST_WCHAR_ENABLED */

/* Asserts that two C-style wide character strings are equal */
#define CPPTEST_ASSERT_ENUM_EQUAL(type, expected, actual) \
    CPPTEST_ASSERT_ENUM_EQUAL_HELPER((expected) == (actual), type, expected, actual, cpptestEnumEqualAssertion, 0)

/* Asserts that two C-style wide character strings are equal (custom message) */
#define CPPTEST_ASSERT_ENUM_EQUAL_MESSAGE(type, message, expected, actual) \
    CPPTEST_ASSERT_ENUM_EQUAL_HELPER((expected) == (actual), type, expected, actual, cpptestEnumEqualAssertion, message)

/* Asserts that given expression throws an exception of specified type */
#define CPPTEST_ASSERT_THROW(expression, ExceptionType) \
    CPPTEST_ASSERT_THROW_HELPER(0, expression, ExceptionType)

/* Asserts that given expression throws an exception of specified type (custom message) */
#define CPPTEST_ASSERT_THROW_MESSAGE(message, expression, ExceptionType) \
    CPPTEST_ASSERT_THROW_HELPER(message, expression, ExceptionType)

/* Asserts that given expression does not throw any exceptions */
#define CPPTEST_ASSERT_NO_THROW(expression) \
    CPPTEST_ASSERT_NO_THROW_HELPER(0, expression)

/* Asserts that given expression does not throw any exceptions (custom message) */
#define CPPTEST_ASSERT_NO_THROW_MESSAGE(message, expression) \
    CPPTEST_ASSERT_NO_THROW_HELPER(message, expression)

#if CPPTEST_EXTERNAL_ASSERTS_AND_POST_CONDITIONS_ENABLED

/** Assert that use external application to validate values:
 * program recives three parameters (param1, param2, param3) and if it returns
 * non-zero value, assert fails and program output is used as message
 */
#define CPPTEST_ASSERT_EXTERNAL(program, param1, param2, param3) \
    do { \
      int result = 0;\
      const char* ret = CppTestCallProgramWrapper(program, &result, param1, param2, param3); \
      if (result && ret) {\
        CPPTEST_FAIL(ret);\
      }\
    } while (0);

/** Assert that use external application to validate values:
 * program recives three parameters (param1, param2, param3) and if it returns
 * non-zero value, assert fails (custom message)
 */
#define CPPTEST_ASSERT_EXTERNAL_MESSAGE(message, program, param1, param2, param3) \
    do { \
      int result = 0;\
      CppTestCallProgramWrapper(program, &result, param1, param2, param3); \
      if (result) {\
        CPPTEST_FAIL(message);\
      }\
    } while (0);

#endif

#if CPPTEST_HAS_SNPRINTF
#  define CPPTEST_SNPRINTF(buffer, size, fmt, value) CPPTEST_C_std snprintf(buffer, size, fmt, value)
#else
#  define CPPTEST_SNPRINTF(buffer, size, fmt, value) (CPPTEST_C_std sprintf(buffer, fmt, value), 0)
#endif

/* Asserts that two values are equal (custom format) */
#define CPPTEST_ASSERT_EQUAL_FMT(expected, actual, fmt) \
    do { \
        char expectedBuffer[CPPTEST_FMT_BUFFER_SIZE]; \
        char actualBuffer[CPPTEST_FMT_BUFFER_SIZE]; \
        int snprintfResults[2]; \
        snprintfResults[0] = CPPTEST_SNPRINTF(expectedBuffer, CPPTEST_FMT_BUFFER_SIZE, fmt, (expected)); \
        snprintfResults[1] = CPPTEST_SNPRINTF(actualBuffer, CPPTEST_FMT_BUFFER_SIZE, fmt, (actual)); \
        cpptestEqualAssertionFmt((expected) == (actual), 0, 0, CPTSP("" CPT_TO_STRING(expected)), CPTSP("" CPT_TO_STRING(actual)), \
                expectedBuffer, actualBuffer, 0, CPPTEST_FMT_BUFFER_SIZE, snprintfResults, stubCallInfo, CPTSP(__FILE__), __LINE__); \
    } while (0);

/* Asserts that two values are equal (custom format) */
#define CPPTEST_ASSERT_EQUAL_DELTA_FMT(expected, actual, delta, fmt) \
    do { \
        char expectedBuffer[CPPTEST_FMT_BUFFER_SIZE]; \
        char actualBuffer[CPPTEST_FMT_BUFFER_SIZE]; \
        char deltaBuffer[CPPTEST_FMT_BUFFER_SIZE]; \
        int snprintfResults[3]; \
        snprintfResults[0] = CPPTEST_SNPRINTF(expectedBuffer, CPPTEST_FMT_BUFFER_SIZE, fmt, (expected)); \
        snprintfResults[1] = CPPTEST_SNPRINTF(actualBuffer, CPPTEST_FMT_BUFFER_SIZE, fmt, (actual)); \
        snprintfResults[2] = CPPTEST_SNPRINTF(deltaBuffer, CPPTEST_FMT_BUFFER_SIZE, fmt, (delta)); \
        cpptestEqualAssertionFmt(CPPTEST_EQUAL_DELTA(expected, actual, delta), 1, 0, CPTSP("" CPT_TO_STRING(expected)), CPTSP("" CPT_TO_STRING(actual)), \
                expectedBuffer, actualBuffer, deltaBuffer, CPPTEST_FMT_BUFFER_SIZE, snprintfResults, stubCallInfo, CPTSP(__FILE__), __LINE__); \
    } while (0);

/* Asserts that two arrays are equal (custom format) */
#define CPPTEST_ASSERT_ARRAY_EQUAL_FMT(expected, actual, size, fmt) \
    do { \
        char expectedBuffer[CPPTEST_FMT_BUFFER_SIZE]; \
        char actualBuffer[CPPTEST_FMT_BUFFER_SIZE]; \
        int snprintfResults[2]; \
        unsigned int counter = 0; \
        while (counter < size) { \
            snprintfResults[0] = CPPTEST_SNPRINTF(expectedBuffer, CPPTEST_FMT_BUFFER_SIZE, fmt, (expected)[counter]); \
            snprintfResults[1] = CPPTEST_SNPRINTF(actualBuffer, CPPTEST_FMT_BUFFER_SIZE, fmt, (actual)[counter]); \
            cpptestEqualAssertionFmt((expected)[counter] == (actual)[counter], 0, &counter, CPTSP("" CPT_TO_STRING(expected)), CPTSP("" CPT_TO_STRING(actual)), \
                    expectedBuffer, actualBuffer, 0, CPPTEST_FMT_BUFFER_SIZE, snprintfResults, stubCallInfo, CPTSP(__FILE__), __LINE__); \
            ++counter; \
        } \
    } while (0);

/* Asserts that two arrays are equal (custom format) */
#define CPPTEST_ASSERT_ARRAY_EQUAL_DELTA_FMT(expected, actual, size, delta, fmt) \
    do { \
        unsigned int counter = 0; \
        char expectedBuffer[CPPTEST_FMT_BUFFER_SIZE]; \
        char actualBuffer[CPPTEST_FMT_BUFFER_SIZE]; \
        char deltaBuffer[CPPTEST_FMT_BUFFER_SIZE]; \
        int snprintfResults[3]; \
        snprintfResults[2] = CPPTEST_SNPRINTF(deltaBuffer, CPPTEST_FMT_BUFFER_SIZE, fmt, (delta)); \
        while (counter < size) { \
            snprintfResults[0] = CPPTEST_SNPRINTF(expectedBuffer, CPPTEST_FMT_BUFFER_SIZE, fmt, (expected)[counter]); \
            snprintfResults[1] = CPPTEST_SNPRINTF(actualBuffer, CPPTEST_FMT_BUFFER_SIZE, fmt, (actual)[counter]); \
            cpptestEqualAssertionFmt(CPPTEST_EQUAL_DELTA((expected)[counter], (actual)[counter], delta), 1, &counter, CPTSP("" CPT_TO_STRING(expected)), CPTSP("" CPT_TO_STRING(actual)), \
                    expectedBuffer, actualBuffer, deltaBuffer, CPPTEST_FMT_BUFFER_SIZE, snprintfResults, stubCallInfo, CPTSP(__FILE__), __LINE__); \
            ++counter; \
        } \
    } while (0);

#define CPPTEST_ASSERT_TYPE_ARRAY_EQUAL_HELPER(expected, actual, size, action) \
   do { \
        unsigned int counter = 0; \
        while (counter < size) { \
            CPPTEST_ASSERT_TYPE_ARRAY_ELEM_EQUAL_HELPER((expected)[counter] == (actual)[counter], counter, expected, actual, action, 0) \
            counter++; \
        } \
   } while (0);


/* Asserts memory leaks */
#define CPPTEST_ASSERT_MEMORY_LEAKS() \
    cpptestCheckMemoryLeaks(CPTSP(__FILE__), __LINE__, 1);

/* Asserts not yet validated test case */
#define CPPTEST_NOT_VALIDATED() \
    cpptestTestCaseNotValidated(CPTSP(__FILE__), __LINE__);

/* Helper Macros *************************************************************/

#define CPPTEST_EQUAL_DELTA(v1, v2, delta) (((v1) > (v2)) ? ((v1) - (v2) <= (delta)) : ((v2) - (v1) <= (delta)))

#define CPPTEST_FLOATS_FMT_EQUAL(f1, f2) (0 != cpptestFloatsFmtEqual(CPTSP("" CPT_TO_STRING(f1)), f2))

#define CPPTEST_CSTRS_EQUAL(s1, s2) (0 != cpptestCharPtrsEqual(s1, s2))

#define CPPTEST_CSTRS_N_EQUAL(s1, s2, max_size) (0 != cpptestCharPtrsNEqual(s1, s2, max_size))

#define CPPTEST_MEM_BUFFERS_EQUAL(p1, p2, size) (0 != cpptestMemBuffersEqual(p1, p2, size))

#define CPPTEST_WCSTRS_EQUAL(s1, s2) (0 != cpptestWCharPtrsEqual(s1, s2))

#define CPPTEST_WCSTRS_N_EQUAL(s1, s2, max_size) (0 != cpptestWCharPtrsNEqual(s1, s2, max_size))

#define CPPTEST_ASSERT_HELPER(condition, message) \
    cpptestAssertion((condition), message, CPTSP("" CPT_TO_STRING(condition)), stubCallInfo, CPTSP(__FILE__), __LINE__);

#define CPPTEST_ASSERT_EQUAL_HELPER(expected, actual, message) \
    cpptestEqualAssertion(((expected) == (actual)), message, CPTSP("" CPT_TO_STRING(expected)), CPTSP("" CPT_TO_STRING(actual)), stubCallInfo, CPTSP(__FILE__), __LINE__);

#define CPPTEST_ASSERT_BOOL_EQUAL_HELPER(expected, actual, message) \
    do { int __expected = ((expected) ? CPPTEST_INT_TRUE : 0); int __actual = ((actual) ? CPPTEST_INT_TRUE : 0); \
      cpptestBoolEqualAssertion((__expected == __actual), __expected, __actual, message, \
              CPTSP("" CPT_TO_STRING(expected)), CPTSP("" CPT_TO_STRING(actual)), stubCallInfo, CPTSP(__FILE__), __LINE__); \
    } while (0);

#define CPPTEST_ASSERT_TYPE_ARRAY_EQUAL_HELPER(expected, actual, size, action) \
   do { \
        unsigned int counter = 0; \
        while (counter < size) { \
            CPPTEST_ASSERT_TYPE_ARRAY_ELEM_EQUAL_HELPER((expected)[counter] == (actual)[counter], counter, expected, actual, action, 0) \
            counter++; \
        } \
   } while (0);

#define CPPTEST_ASSERT_NUMBER_ARRAY_EQUAL_HELPER(expected, actual, size, hasDelta, delta, action) \
   do { \
        unsigned int counter = 0; \
        while (counter < size) { \
            CPPTEST_ASSERT_NUMBER_ARRAY_ELEM_EQUAL_HELPER(hasDelta ? CPPTEST_EQUAL_DELTA((expected)[counter], (actual)[counter], delta) : ((expected)[counter] == (actual)[counter]), counter, expected, actual, hasDelta, delta, action, 0) \
            counter++; \
        } \
   } while (0);

#define CPPTEST_ASSERT_BITS_HELPER(condition, mask, maskedBits /* orginal bits before masking */, actual, action, actualStr, message, isBitsLow) \
  action(condition, mask, mask & (maskedBits & ~isBitsLow) /* expected bits after masking */, actual, message, actualStr, stubCallInfo, CPTSP(__FILE__), __LINE__);

#define CPPTEST_ASSERT_ARRAY_EQUAL_CSTR_HELPER(expected, actual, size, action) \
   do { \
        unsigned int counter = 0; \
        while (counter < size) { \
            CPPTEST_ASSERT_TYPE_ARRAY_ELEM_EQUAL_HELPER(CPPTEST_CSTRS_EQUAL((expected)[counter], (actual)[counter]), counter, expected, actual, action, 0) \
            counter++; \
        } \
   } while (0);


#define CPPTEST_ASSERT_TYPE_EQUAL_HELPER(condition, expected, actual, action, message) \
    action((condition), (expected), (actual), 0, message, CPTSP("" CPT_TO_STRING(expected)), CPTSP("" CPT_TO_STRING(actual)), stubCallInfo, CPTSP(__FILE__), __LINE__);

#define CPPTEST_ASSERT_TYPE_ARRAY_ELEM_EQUAL_HELPER(condition, array_index, expected, actual, action, message) \
    action((condition), (expected)[array_index], (actual)[array_index], &array_index, message, CPTSP("" CPT_TO_STRING(expected)), CPTSP("" CPT_TO_STRING(actual)), stubCallInfo, CPTSP(__FILE__), __LINE__);

#define CPPTEST_ASSERT_NUMBER_EQUAL_HELPER(condition, expected, actual, hasDelta, delta, action, message) \
    action((condition), (expected), (actual), hasDelta, delta, 0, message, CPTSP("" CPT_TO_STRING(expected)), CPTSP("" CPT_TO_STRING(actual)), stubCallInfo, CPTSP(__FILE__), __LINE__);

#define CPPTEST_ASSERT_NUMBER_ARRAY_ELEM_EQUAL_HELPER(condition, array_index, expected, actual, hasDelta, delta, action, message) \
    action((condition), (expected)[array_index], (actual)[array_index], hasDelta, delta, &array_index, message, CPTSP("" CPT_TO_STRING(expected)), CPTSP("" CPT_TO_STRING(actual)), stubCallInfo, CPTSP(__FILE__), __LINE__);

#define CPPTEST_ASSERT_FMT_EQUAL_HELPER(condition, expected, actual, action, message) \
    action((condition), (actual), message, CPTSP("" CPT_TO_STRING(expected)), CPTSP("" CPT_TO_STRING(actual)), stubCallInfo, CPTSP(__FILE__), __LINE__);

#define CPPTEST_ASSERT_MEM_EQUAL_HELPER(condition, expected, actual, size, action, message) \
    action((condition), (expected), (actual), size, message, CPTSP("" CPT_TO_STRING(expected)), CPTSP("" CPT_TO_STRING(actual)), stubCallInfo, CPTSP(__FILE__), __LINE__);

#define CPPTEST_ASSERT_ENUM_EQUAL_HELPER(condition, type, expected, actual, action, message) \
    action((condition), cpptestGetEnumInfo((int)(type)0, #type, CPTSP(__FILE__), __LINE__), (CPPTEST_INTEGER)(expected), (CPPTEST_INTEGER)(actual), message,\
            CPTSP("" CPT_TO_STRING(expected)), CPTSP("" CPT_TO_STRING(actual)), stubCallInfo, CPPTEST_IS_CPP, CPTSP(__FILE__), __LINE__);

#define CPPTEST_ASSERT_THROW_HELPER(message, expression, ExceptionType) \
    do { \
        try { \
            expression; \
            cpptestThrowAssertion(0, message, CPTSP("" #ExceptionType), stubCallInfo, CPTSP(__FILE__), __LINE__); \
        } catch (ExceptionType) { \
            cpptestThrowAssertion(1, message, CPTSP("" #ExceptionType), stubCallInfo, CPTSP(__FILE__), __LINE__); \
        } \
    } while(0);

#define CPPTEST_ASSERT_NO_THROW_HELPER(message, expression) \
    do { \
        try { \
            expression; \
            cpptestNoThrowAssertion(1, message, stubCallInfo, CPTSP(__FILE__), __LINE__); \
        } catch (...) { \
            cpptestNoThrowAssertion(0, message, stubCallInfo, CPTSP(__FILE__), __LINE__); \
        } \
    } while(0);

/*****************************************************************************
 *
 * Utility Macros
 *
 *****************************************************************************/

/* Breaks test case execution */
#define CPPTEST_BREAK() \
    cpptestUserBreak(CPTSP(__FILE__), __LINE__);

/* Converts relative path to absolute */
#define CPPTEST_PATH_RELATIVE_TO_ABSOLUTE(FILE) \
	cpptestPathRelativeToAbsolute(FILE, __FILE__)

/* Allows for test/stub execution flow control */
#if CPPTEST_SETJMP_ENABLED
#  define CPPTEST_REGISTER_JMP(expression) \
    do { \
        if (! (cpptestSetJmpReturn(CPPTEST_C_std sigsetjmp(*cpptestGetJmpBuf(), 1)))) { \
            cpptestSetJmpCalled(1); \
            expression; \
        } \
    } while(0);
#  define CPPTEST_JMP(value) \
    do { \
        if (cpptestGetJmpCalled()) { \
            cpptestSetJmpCalled(0); \
            CPPTEST_C_std siglongjmp(*cpptestGetJmpBuf(), value); \
        } \
    } while(0);
#else /* !CPPTEST_SETJMP_ENABLED */
/* Do nothing if setjmp is not enabled */
#  define CPPTEST_REGISTER_JMP(expression)
#  define CPPTEST_JMP(value)
#endif /* CPPTEST_SETJMP_ENABLED */

/*****************************************************************************
 *
 * Post Condition Macros 
 *
 *****************************************************************************/

/* Post condition for boolean types */
#define CPPTEST_POST_CONDITION_BOOL(value_string, value) \
   cpptestPostConditionBool(value_string, value ? CPPTEST_INT_TRUE : 0, CPTSP(__FILE__), __LINE__);

/* Post condition for all integer types */
#define CPPTEST_POST_CONDITION_INTEGER(value_string, value) \
    cpptestPostConditionInteger(value_string, value, CPTSP(__FILE__), __LINE__);

/* Post condition for all unsigned integer types */
#define CPPTEST_POST_CONDITION_UINTEGER(value_string, value) \
    cpptestPostConditionUInteger(value_string, value, CPTSP(__FILE__), __LINE__);
 
/* Post condition for float, double and long double */
#define CPPTEST_POST_CONDITION_FLOAT(value_string, value) \
    cpptestPostConditionFloat(value_string, value, CPTSP(__FILE__), __LINE__);

/* Post condition for C-string types */
#define CPPTEST_POST_CONDITION_CSTR(value_string, value) \
    cpptestPostConditionCStr(value_string, (char*)value, CPTSP(__FILE__), __LINE__);

/* Post condition for C-string types with the characters limit */
#define CPPTEST_POST_CONDITION_CSTR_N(value_string, value, max_size) \
    cpptestPostConditionCStrN(value_string, (char*)value, max_size, CPTSP(__FILE__), __LINE__);

/* Post conditions for data buffers */
#define CPPTEST_POST_CONDITION_MEM_BUFFER(value_string, value, size) \
    cpptestPostConditionMemBuffer(value_string, (void*)value, size, CPTSP(__FILE__), __LINE__);

/* Post condition for pointer types */
#define CPPTEST_POST_CONDITION_PTR(value_string, value) \
    cpptestPostConditionPtr(value_string, (value == 0) ? CPPTEST_INT_TRUE : 0, CPTSP(__FILE__), __LINE__);

/* Post condition for enum types */
#define CPPTEST_POST_CONDITION_ENUM(type, value_string, value) \
    cpptestPostConditionEnum(cpptestGetEnumInfo((int)(type)0, #type, CPTSP(__FILE__), __LINE__), value_string, (CPPTEST_INTEGER)(value), \
            CPPTEST_IS_CPP, CPTSP(__FILE__), __LINE__);

#if CPPTEST_EXTERNAL_ASSERTS_AND_POST_CONDITIONS_ENABLED

/** Post condition that use external application to create message:
 * program recives three parameters (param1, param2, param3) and program output is used as message
 */
#define CPPTEST_POST_CONDITION_EXTERNAL(message, program, param1, param2, param3) \
    cpptestPostConditionExternal(message, CppTestCallProgramWrapper(program, 0, param1, param2, param3), CPTSP(__FILE__), __LINE__);

#endif

/*****************************************************************************
 *
 * Helper Macros Used in Test Suite Declaration
 *
 *****************************************************************************/

#define CPPTEST_CONTEXT(X)

#ifdef __cplusplus

struct __CPTR_TestRunner;
#   define CPPTEST_TEST_SUITE(X) \
    public: \
    typedef X ThisTestSuite; \
    static void (ThisTestSuite::*call_slave_ptr)(); \
    static void call_slave() \
    { \
        X s; \
        cpptestIgnoreCurrStackElement(); \
        s.setUp(); \
        (s.*call_slave_ptr)(); \
        s.tearDown(); \
    } \
    static void testSuiteBegin() \
    { \
        cpptestTestSuiteBegin(CPTSP("" #X), CPTSP(__FILE__), __LINE__); \
    } \
    CPPTEST_TEST_NO_EXCEPTION_FUNC \
    static int call(const char* name) \
    { \
        cpptestIgnoreCurrStackElement(); \

#else   /* __cplusplus */

#if CPPTEST_USE_SETUP_FOR_C_SUITES

#   define CPPTEST_TEST_SUITE(X) \
    static void (* X ## _call_slave_ptr)(void); \
    static void X ## _call_slave(void) \
    { \
        void X ## _setUp(void); \
        void X ## _tearDown(void); \
        cpptestIgnoreCurrStackElement(); \
        X ## _setUp(); \
        X ## _call_slave_ptr(); \
        X ## _tearDown(); \
    } \
    EXTERN_C_LINKAGE void X ## _testSuiteBegin(void) \
    { \
        cpptestTestSuiteBegin(CPTSP("" #X), CPTSP(__FILE__), __LINE__); \
    } \
    static int X ## _callTestStatic(const char* name) \
    { \
        static void (** call_slave_ptr_ptr)() = & X ## _call_slave_ptr; \
        static void (*call_slave_ptr)() = & X ## _call_slave; \
        cpptestIgnoreCurrStackElement(); \

#else   /* CPPTEST_USE_SETUP_FOR_C_SUITES */

#   define CPPTEST_TEST_SUITE(X) \
    EXTERN_C_LINKAGE void X ## _testSuiteBegin(void) \
    { \
        cpptestTestSuiteBegin(CPTSP("" #X), CPTSP(__FILE__), __LINE__); \
    } \
    static int X ## _callTestStatic(const char* name) \
    { \
        cpptestIgnoreCurrStackElement(); \

#endif  /* CPPTEST_USE_SETUP_FOR_C_SUITES */


#endif

#define CPPTEST_TEST_SUITE_END() \
        return 1; \
    }

#ifdef __cplusplus

#  define CPPTEST_TEST_SUITE_SETUP(X) \
        if (cpptestStrCmp(name, "#setUp") == 0) { \
            X(); \
            return 0; \
        } \

#  define CPPTEST_TEST_SUITE_TEARDOWN(X) \
        if (cpptestStrCmp(name, "#tearDown") == 0) { \
            X(); \
            return 0; \
        } \

#else

#  define CPPTEST_TEST_SUITE_SETUP(X) \
        if (cpptestStrCmp(name, "#setUp") == 0) { \
            void X(void); \
            X(); \
            return 0; \
        } \

#  define CPPTEST_TEST_SUITE_TEARDOWN(X) \
        if (cpptestStrCmp(name, "#tearDown") == 0) { \
            void X(void); \
            X(); \
            return 0; \
        } \

#endif


#if CPPTEST_DATA_SOURCES_ENABLED

static const char* CPPTEST_DS_GET_CSTR(const char* name)
{
    return CPPTEST_DS_METHOD(cpptestGetCurrentDataSource(), getCStr)(cpptestGetCurrentDataSource(), name);
}
#if CPPTEST_WCHAR_ENABLED
static const CppTest_wchar_t* CPPTEST_DS_GET_WCSTR(const char* name)
{
    return CPPTEST_DS_METHOD(cpptestGetCurrentDataSource(), getWCStr)(cpptestGetCurrentDataSource(), name);
}
#endif
static const char* CPPTEST_DS_GET_MEM_BUFFER(const char* name, unsigned int* sizePtr)
{
    return CPPTEST_DS_METHOD(cpptestGetCurrentDataSource(), getMemBuffer)(cpptestGetCurrentDataSource(), name, sizePtr);
}
static CPPTEST_INTEGER CPPTEST_DS_GET_INTEGER(const char* name)
{
    return CPPTEST_DS_METHOD(cpptestGetCurrentDataSource(), getInteger)(cpptestGetCurrentDataSource(), name);
}
static CPPTEST_UINTEGER CPPTEST_DS_GET_UINTEGER(const char* name)
{
    return CPPTEST_DS_METHOD(cpptestGetCurrentDataSource(), getUInteger)(cpptestGetCurrentDataSource(), name);
}
#ifndef __KERNEL__
static CPPTEST_FLOAT CPPTEST_DS_GET_FLOAT(const char* name)
{
    return CPPTEST_DS_METHOD(cpptestGetCurrentDataSource(), getFloat)(cpptestGetCurrentDataSource(), name);
}
#endif
static int CPPTEST_DS_GET_BOOL(const char* name)
{
    return CPPTEST_DS_METHOD(cpptestGetCurrentDataSource(), getBool)(cpptestGetCurrentDataSource(), name);
}
static char CPPTEST_DS_GET_CHAR(const char* name)
{
    return CPPTEST_DS_METHOD(cpptestGetCurrentDataSource(), getChar)(cpptestGetCurrentDataSource(), name);
}

#define CPPTEST_DS_GET_ENUM(type, name) \
    (type)cpptestGetEnumeratorValue((int)(type)0, #type, CPPTEST_DS_GET_CSTR(name), CPPTEST_IS_CPP, CPTSP(__FILE__), __LINE__)

static unsigned int CPPTEST_DS_GET_ITERATION(void)
{
    return CPPTEST_DS_METHOD(cpptestGetCurrentDataSource(), getIteration)(cpptestGetCurrentDataSource());
}
static int CPPTEST_DS_HAS_COLUMN(const char* name)
{
    if (cpptestGetCurrentDataSource() && (CPPTEST_DS_METHOD(cpptestGetCurrentDataSource(), hasColumn))) {
        return CPPTEST_DS_METHOD(cpptestGetCurrentDataSource(), hasColumn)(cpptestGetCurrentDataSource(), name);
    }
    return 0;
}

#define CPPTEST_DS_GET_VALUE(SOURCE) SOURCE[CPPTEST_DS_GET_ITERATION() - 1]

#define CPPTEST_DS_REPEAT(NUMBER) \
        CppTest_DsRepeat(NUMBER)

#define CPPTEST_DS_ARRAY(DATA, ROW, COLUMN) \
        CppTest_DsArray((const char**)DATA, ROW, COLUMN) 

#define CPPTEST_DS_CSV(FILE, SEPARATOR, HEADER, TRIM) \
        CppTest_DsCsv(FILE, SEPARATOR, HEADER, TRIM) 

#define CPPTEST_DS(DATA_SOURCE) \
        CppTest_Ds(DATA_SOURCE) 

#endif        

#define CPPTEST_TEST_CASE_PARAMETERS() \
    (cpptestGetCurrentDataSource() ? (CPPTEST_DS_GET_DATA_SIGNATURE()) : (const char*)0)

#define CPPTEST_TEST_SLAVE(TEST_CASE, EXPECTED_ERROR, EXPECTED_ERROR_MASK) \
    if (cpptestStrCmp(name, CPT_TO_STRING(TEST_CASE)) == 0) \
    { \
        cpptestTestCaseBegin(CPTSP("" CPT_TO_STRING(TEST_CASE)), EXPECTED_ERROR, EXPECTED_ERROR_MASK, CPPTEST_IS_CPP, CPTSP(__FILE__), __LINE__); \
        CPPTEST_TEST_NO_EXCEPTION(TEST_CASE) \
        cpptestTestCaseEnd(); \
        return 0; \
    }

#define CPPTEST_TEST_DS_SLAVE(TEST_CASE, DATA_SOURCE, EXPECTED_ERROR, EXPECTED_ERROR_MASK) \
    if (cpptestStrCmp(name, CPT_TO_STRING(TEST_CASE)) == 0) \
    { \
        cpptestDsTestCaseBegin(CPTSP("" CPT_TO_STRING(TEST_CASE)), EXPECTED_ERROR, EXPECTED_ERROR_MASK, CPPTEST_IS_CPP, CPTSP(__FILE__), __LINE__); \
        cpptestDsForCurrentTestCaseInitialized(DATA_SOURCE); \
        while (CPPTEST_DS_HAS_MORE_DATA()) \
        { \
            if (CPPTEST_DS_LOAD_NEXT_DATA()) { \
                cpptestDsTestCaseIterationBegin(CPPTEST_DS_GET_ITERATION(), CPPTEST_DS_GET_DATA_SIGNATURE()); \
                CPPTEST_TEST_NO_EXCEPTION(TEST_CASE) \
                cpptestDsTestCaseIterationEnd(); \
            } \
        } \
        CPPTEST_DS_FINALIZE(); \
        cpptestDsTestCaseEnd(); \
        return 0; \
    }

#define CPPTEST_TEST(TEST_CASE) \
    CPPTEST_TEST_SLAVE(TEST_CASE, CPPTEST_NO_ERROR, CPPTEST_FULL_MASK)

#define CPPTEST_TEST_FAIL(TEST_CASE) \
    CPPTEST_TEST_SLAVE(TEST_CASE, CPPTEST_ANY_ERROR, CPPTEST_ERROR_MASK)

#define CPPTEST_TEST_ERROR(TEST_CASE, ERROR_CODE) \
    CPPTEST_TEST_SLAVE(TEST_CASE, ERROR_CODE, CPPTEST_FULL_MASK)

#define CPPTEST_TEST_DS(TEST_CASE, DATA_SOURCE) \
    CPPTEST_TEST_DS_SLAVE(TEST_CASE, DATA_SOURCE, CPPTEST_NO_ERROR, CPPTEST_FULL_MASK)

#define CPPTEST_TEST_FAIL_DS(TEST_CASE, DATA_SOURCE) \
    CPPTEST_TEST_DS_SLAVE(TEST_CASE, DATA_SOURCE, CPPTEST_ANY_ERROR, CPPTEST_ERROR_MASK)

#define CPPTEST_TEST_ERROR_DS(TEST_CASE, DATA_SOURCE, ERROR_CODE) \
    CPPTEST_TEST_DS_SLAVE(TEST_CASE, DATA_SOURCE, ERROR_CODE, CPPTEST_FULL_MASK)

#if CPPTEST_EXCEPTIONS_ENABLED

#define CPPTEST_TEST_EXCEPTION(TEST_CASE, EXCEPTION) \
    if (cpptestStrCmp(name, CPT_TO_STRING(TEST_CASE)) == 0) \
    { \
        cpptestTestCaseBegin(CPTSP("" CPT_TO_STRING(TEST_CASE)), CPPTEST_NO_ERROR, CPPTEST_FULL_MASK, CPPTEST_IS_CPP, CPTSP(__FILE__), __LINE__); \
        CPPTEST_TEST_EXCEPTION_SLAVE(TEST_CASE, EXCEPTION) \
        cpptestTestCaseEnd(); \
        return 0; \
    }

#else

#define CPPTEST_TEST_EXCEPTION(TEST_CASE, EXCEPTION) \
    CPPTEST_TEST_FAIL(TEST_CASE)

#endif

#define CPPTEST_TEST_DISABLED(TEST_CASE) \
    if (cpptestStrCmp(name, #TEST_CASE) == 0) \
    { \
        cpptestTestCaseDisabled(CPTSP("" #TEST_CASE), CPTSP(__FILE__), __LINE__); \
        return 0; \
    }

#define CPPTEST_TEST_FAIL_DISABLED(TEST_CASE) \
    CPPTEST_TEST_DISABLED(TEST_CASE)

#define CPPTEST_TEST_ERROR_DISABLED(TEST_CASE, ERROR_CODE) \
    CPPTEST_TEST_DISABLED(TEST_CASE)

#define CPPTEST_TEST_EXCEPTION_DISABLED(TEST_CASE, EXCEPTION) \
    CPPTEST_TEST_DISABLED(TEST_CASE)

#define CPPTEST_TEST_DS_DISABLED(TEST_CASE, DATA_SOURCE) \
    CPPTEST_TEST_DISABLED(TEST_CASE)


/* Initialize testsuite */
#define CPPTEST_RUN_TEST_SUITE_BEGIN(X, FILE_CONTEXT, TEST_CASE_NUMBER) \
    if (TEST_CASE_NUMBER && (__CPTR_Test_Manager_Count + TEST_CASE_NUMBER > __CPTR_Test_Manager_Start_Id)) { \
        const char* testCaseName = 0; \
        const char* testSuiteName = #X; \
        cpptestCurrentFileContext = FILE_CONTEXT; \
        X ## _testSuiteBegin();

/* Finalize testsuite */
#define CPPTEST_RUN_TEST_SUITE_END(X, TEST_CASE_NUMBER) \
        cpptestTestSuiteEnd(); \
    } else { \
        __CPTR_Test_Manager_Count += TEST_CASE_NUMBER; \
    }

/* Run testcase */
#define CPPTEST_RUN_TEST_CASE(S, X) \
        testCaseName = #X;\
        if (__CPTR_Test_Manager_Test(testSuiteName, testCaseName)) { \
            S ## _callTest(testCaseName); \
        }

#define CPPTEST_RUN_TEST_SUITE_SETUP(S) \
        testCaseName = "#setUp";\
        S ## _callTest(testCaseName); \

#define CPPTEST_RUN_TEST_SUITE_TEARDOWN(S) \
        testCaseName = "#tearDown";\
        S ## _callTest(testCaseName); \

/*****************************************************************************
 *
 *  Helper Macros Used in C++ Test Runner
 *
 *****************************************************************************/

#ifdef __cplusplus
extern "C" {        
    typedef int (CPPTEST_CALL_ROUTINE)(const char*);
}


#if CPPTEST_EXCEPTIONS_ENABLED

#if CPPTEST_SPECIAL_STD_EXCEPTIONS_HANDLING_ENABLED
#   define CPPTEST_STD_EXCEPT_HANDLER \
    } catch (std::exception& except) { \
        cpptestIgnoreCurrStackElement(); \
        cpptestUnhandledStdExceptionInTestCase(except.what());
#else
#   define CPPTEST_STD_EXCEPT_HANDLER
#endif

#define CPPTEST_TEST_NO_EXCEPTION(X) \
        cpptest_test_no_exception(&ThisTestSuite::X);
        
#define CPPTEST_TEST_NO_EXCEPTION_FUNC \
    static void cpptest_test_no_exception(void (ThisTestSuite::*testcase_ptr)()) \
    { \
        cpptestIgnoreCurrStackElement(); \
        try { \
            try { \
                call_slave_ptr = testcase_ptr; \
                cpptestTestCaseSlave(& call_slave); \
            CPPTEST_STD_EXCEPT_HANDLER \
            } catch (...) { \
                cpptestIgnoreCurrStackElement(); \
                cpptestUnhandledCppExceptionInTestCase(); \
            } \
        } catch (...) {} \
    };

#define CPPTEST_TEST_EXCEPTION_SLAVE(X, E) \
        try { \
            int cpptestExceptionCaught = 0; \
            try { \
                try { \
                    call_slave_ptr = &ThisTestSuite::X; \
                    cpptestTestCaseSlave(& call_slave); \
                } catch (E) { \
                   cpptestExceptionCaught = 1; \
                } \
            CPPTEST_STD_EXCEPT_HANDLER \
            } catch (...) { \
                cpptestIgnoreCurrStackElement(); \
                cpptestUnhandledCppExceptionInTestCase(); \
            } \
            if (0 == cpptestExceptionCaught) { \
                cpptestNoExpectedCppExceptionInTestCase(CPTSP( "" #E)); \
            } \
        } catch (...) {} \

#else   /* CPPTEST_EXCEPTIONS_ENABLED */

#define CPPTEST_TEST_NO_EXCEPTION(X) \
        call_slave_ptr = &ThisTestSuite::X; \
        cpptestTestCaseSlave(& call_slave);

#define CPPTEST_TEST_NO_EXCEPTION_FUNC

#endif  /* CPPTEST_EXCEPTIONS_ENABLED */

#define CPPTEST_DEFINE_TEST_CASE_SLAVE(X) \
    static void cpptestCallTestCase_##X() \
    { \
        ThisTestSuite s; \
        cpptestIgnoreCurrStackElement(); \
        s.setUp(); \
        s.X(); \
        s.tearDown(); \
    }

#define CPPTEST_TEST_SUITE_REGISTRATION(X) void (X::*X::call_slave_ptr)(); \
EXTERN_C_LINKAGE void X ## _testSuiteBegin(void) \
{ \
    X::testSuiteBegin(); \
} \
EXTERN_C_LINKAGE int X ## _callTest(const char* name) \
{ \
    cpptestIgnoreCurrStackElement(); \
    return X::call(name); \
}

#else   /* __cplusplus */

typedef int (CPPTEST_CALL_ROUTINE)(const char*);

/*****************************************************************************
 *
 *  Helper Macros Used in C Test Runner
 *
 *****************************************************************************/

#if CPPTEST_USE_SETUP_FOR_C_SUITES

#define CPPTEST_TEST_NO_EXCEPTION(X) \
        { \
            void X(); \
            (*call_slave_ptr_ptr) = & X; \
            cpptestTestCaseSlave(call_slave_ptr); \
        }

#else   /* CPPTEST_USE_SETUP_FOR_C_SUITES */

#define CPPTEST_TEST_NO_EXCEPTION(X) \
        { \
            void X(); \
            cpptestTestCaseSlave(&X); \
        }

#endif  /* CPPTEST_USE_SETUP_FOR_C_SUITES */

#define CPPTEST_DEFINE_TEST_CASE_SLAVE(X) \
    void X(void); \
    static void cpptestCallTestCase_##X() \
    { \
        cpptestIgnoreCurrStackElement(); \
        X(); \
    }

#define CPPTEST_TEST_SUITE_REGISTRATION(X) \
EXTERN_C_LINKAGE int X ## _callTest(const char* name) \
{ \
    cpptestIgnoreCurrStackElement(); \
    return X ## _callTestStatic(name); \
}

#endif  /* __cplusplus */      

#define CPPTEST_TEST_SUITE_INCLUDED_TO(file)

#define CPPTEST_ADDITIONAL_STUB_FILES(files)

#ifdef __cplusplus
/**
 * Test suite base class.
 */
struct CppTest_TestSuite
{
    /**
     * Called to set up test environment.
     */
    virtual void setUp()
    {
    }

    /**
     * Called to clean up test environment.
     */
    virtual void tearDown()
    {
    }

    virtual ~CppTest_TestSuite()
    {
    }
};

#endif  /* __cplusplus */

#define CPPTEST_MACRO_TO_STR_PROXY(ARG) #ARG
#define CPPTEST_MACRO_TO_STR(ARG) CPPTEST_MACRO_TO_STR_PROXY(ARG)

#ifndef CPPTEST_PROJECT_ROOT
#define CPPTEST_PROJECT_ROOT CPPTEST_MACRO_TO_STR(CPPTEST_PROJECT_ROOT_RAW)
#endif

/*****************************************************************************
 *
 * Stub Macros
 *
 *****************************************************************************/

#define CPPTEST_CALL_STUB_CALLBACK cpptestCallStubCallback

#define CPPTEST_REGISTER_STUB_CALLBACK(ID, CALLBACK_PTR) \
    cpptestRegisterStubCallback(ID, (CppTest_Stub_Callback)(CALLBACK_PTR), CPTSP(__FILE__), __LINE__)

#define CPPTEST_UNREGISTER_STUB_CALLBACK(ID) \
    cpptestUnregisterStubCallback(ID, CPTSP(__FILE__), __LINE__)

#define CPPTEST_UNREGISTER_ALL_STUB_CALLBACKS() \
    cpptestUnregisterAllStubCallbacks()

#define CPPTEST_HAS_STUB_CALLBACK(ID) \
    cpptestHasStubCallback(ID, CPTSP(__FILE__), __LINE__)

/*****************************************************************************
 *
 * Trigger Macros
 *
 *****************************************************************************/

#ifdef __cplusplus

#define CPPTEST_ON_CALL(NAME) cpptestValidateTriggerName(NAME, CPTSP(__FILE__), __LINE__); CREATE_TRIGGER(NAME, c_tgr_run_before)

#define CPPTEST_AFTER_CALL(NAME) cpptestValidateTriggerName(NAME, CPTSP(__FILE__), __LINE__); CREATE_TRIGGER(NAME, c_tgr_run_after)

#else /* !__cplusplus */

#define CPPTEST_ON_CALL(NAME) cpptestValidateTriggerName(NAME, CPTSP(__FILE__), __LINE__); CREATE_TRIGGER_C(NAME, c_tgr_run_before)

#define CPPTEST_AFTER_CALL(NAME) cpptestValidateTriggerName(NAME, CPTSP(__FILE__), __LINE__); CREATE_TRIGGER_C(NAME, c_tgr_run_after)


#endif /* __cplusplus */


#define CPPTEST_ACTUAL_CALL(NAME) cpptestCallTriggerCallback(NAME); RUN_TRIGGER(NAME, c_tgr_run_before)

#define CPPTEST_ACTUAL_BEFORE_CALL(NAME) CPPTEST_ACTUAL_CALL(NAME)

#define CPPTEST_ACTUAL_AFTER_CALL(NAME) RUN_TRIGGER(NAME, c_tgr_run_after)


#define CPPTEST_ACTUAL_CALL_C(NAME) cpptestCallTriggerCallback(NAME); RUN_TRIGGER_C(NAME, c_tgr_run_before)

#define CPPTEST_ACTUAL_BEFORE_CALL_C(NAME) CPPTEST_ACTUAL_CALL_C(NAME)

#define CPPTEST_ACTUAL_AFTER_CALL_C(NAME) RUN_TRIGGER_C(NAME, c_tgr_run_after)



/*****************************************************************************
 *
 * Expectation macros
 *
 *****************************************************************************/

#define CPPTEST_EXPECT_NCALLS(NAME, NUMBER) \
    cpptestCreateCallExpectation(NAME, NUMBER, NUMBER, CPTSP(__FILE__), __LINE__)

#define CPPTEST_EXPECT_NCALLS_MORE_THAN(NAME, NUMBER) \
    cpptestCreateCallExpectation(NAME, (NUMBER) + 1, -1, CPTSP(__FILE__), __LINE__)

#define CPPTEST_EXPECT_NCALLS_LESS_THAN(NAME, NUMBER) \
    cpptestCreateCallExpectation(NAME, -1, (NUMBER) - 1, CPTSP(__FILE__), __LINE__)

#define CPPTEST_EXPECT_NCALLS_IN_RANGE(NAME, MINIMUM, MAXIMUM) \
    cpptestCreateCallExpectation(NAME, MINIMUM, MAXIMUM, CPTSP(__FILE__), __LINE__)


#ifdef __cplusplus
class CppTestCallSequenceExpectationCpp
{
public:
    CppTestCallSequenceExpectationCpp(CppTestCallSequenceExpectation* seq) : _seq(seq) 
    {
    }

    CppTestCallSequenceExpectationCpp Add(const char* funcId)
    {
        return _seq->Add(funcId);
    }

    CppTestCallSequenceExpectationCpp AddNTimes(const char* funcId, int times)
    { 
        return _seq->AddNTimes(funcId, times); 
    }
    
    CppTestCallSequenceExpectationCpp AddAnyTimes(const char* funcId)
    { 
        return _seq->AddAnyTimes(funcId); 
    }

    CppTestCallSequenceExpectationCpp* operator->()
    {
        return this;
    }

private:
    CppTestCallSequenceExpectation* _seq;
};

#define CPPTEST_EXPECT_CALL_SEQUENCE() \
    CppTestCallSequenceExpectationCpp(cpptestCreateCallSequenceExpectation(/*isLoop=*/1, CPTSP(__FILE__), __LINE__))

#define CPPTEST_EXPECT_CALL_ORDER() \
    CppTestCallSequenceExpectationCpp(cpptestCreateCallSequenceExpectation(/*isLoop=*/0, CPTSP(__FILE__), __LINE__))

#else /* !__cplusplus */

#define CPPTEST_EXPECT_CALL_SEQUENCE() \
    cpptestCreateCallSequenceExpectation(/*isLoop=*/1, CPTSP(__FILE__), __LINE__)

#define CPPTEST_EXPECT_CALL_ORDER() \
    cpptestCreateCallSequenceExpectation(/*isLoop=*/0, CPTSP(__FILE__), __LINE__)

#endif /* __cplusplus */


/*****************************************************************************
 *
 * Compatibility Macros
 *
 *****************************************************************************/

/* deprecated use CPPTEST_ASSERT_MESSAGE(message, condition) instead */
#define CppTest_Assert(condition, message) \
    CPPTEST_ASSERT_MESSAGE(message, condition)

/* deprecated use CPPTEST_BREAK() instead */
#define CppTest_Break() \
    CPPTEST_BREAK()

/* deprecated use CPPTEST_BREAK() instead */
#define CPPTEST_BREAK_EXECUTION(code, fileName, lineNo) \
    CPPTEST_BREAK()

/* deprecated use CPPTEST_BREAK() instead */
#define CPPTEST_BREAK_EXECUTION_MSG(code, message, fileName, lineNo) \
    CPPTEST_BREAK()

#endif
