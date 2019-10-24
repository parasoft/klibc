/*
 * (C) Copyright Parasoft Corporation 1998 - 2019.  All rights reserved.
 * The use of this source code is governed by the general Parasoft License Agreement.
 * Redistribution strictly prohibited.
 */
#define CPPTEST_RUNTIME_IMPLEMENTATION

#include "cpptest_runtime.h"
#include "cpptest_message.h"
#include "cpptest_transport.h"
#include "cpptest_headers.h"

#include "CppTestInternal.h"

#include CPPTEST_stdio
#include CPPTEST_string

CPPTEST_using_namespace_std


/* SYNCHRONIZED: start */
    /* Access to testlog stream should be synchronized on testLogLock variable */
    CPPTEST_LOCK_DECLARE(static, testLogLock)
/* SYNCHRONIZED: end */

#define CPPTEST_HAS_ARGUMENTS_FLAG                  0x80U

#define CPPTEST_HAS_NESTED_ARGUMENTS_FLAG           0x80U
#define CPPTEST_LAST_ARG_FLAG                       0x40U
#define CPPTEST_SHORT_DATA_FLAG                     0x20U
#define CPPTEST_LONG_LENGTH_FLAG                    0x10U

#define CPPTEST_SHORT_DATA_MASK                     0x1FU
#define CPPTEST_SHORT_LENGTH_MASK                   0x0FU
#define CPPTEST_LONG_LENGTH_TYPE_MASK               0x0EU

#define CPPTEST_1BYTE_LENGTH_TYPE                   0x00U
#define CPPTEST_2BYTE_LENGTH_TYPE                   0x02U
#define CPPTEST_ESCAPED_LENGTH_TYPE                 0x08U
#define CPPTEST_CSTRING_LENGTH_TYPE                 0x0AU
#define CPPTEST_WCSTRING_LENGTH_TYPE                0x0CU

#define CPPTEST_SPECIAL_CASE_1                      0x1EU
#define CPPTEST_SPECIAL_CASE_2                      0x1FU

#define CPPTEST_ARG_CONTROL_NATIVE_ID               0x00U
#define CPPTEST_CUSTOM_MIN_ALLOWED_ARG_NATIVE_ID    0x10U


/*--------------------------------------------------------------------------------------------
 * Internal runtime API (function used in other runtime source files)
 *------------------------------------------------------------------------------------------*/

#define SEND_BUFFER_SIZE 256U

static CPPTEST_UINT8 sendBuffer[SEND_BUFFER_SIZE];
static unsigned int sendBufferPos = 0U;

static void cpptestSendBuffer(void)
{
    if (sendBufferPos > 0U) {
         cpptestSendData(cpptestGetTestLogStream(), (char*)sendBuffer, sendBufferPos);
         sendBufferPos = 0U;
    }
}

static void cpptestFlushBuffer(void)
{
    cpptestSendBuffer();
    cpptestFlushData(cpptestGetTestLogStream());
}


static void cpptestSendUint16(CPPTEST_UINT16 value)
{
    CPPTEST_UINT8* ptr = sendBuffer + sendBufferPos;
    *ptr++ = (CPPTEST_UINT8)(value & 0x00FFU);
    *ptr = (CPPTEST_UINT8)((value & 0xFF00U) >> 8);
    sendBufferPos += 2U;
}

static void cpptestSendUint32(CPPTEST_UINT32 value)
{
    CPPTEST_UINT8* ptr = sendBuffer + sendBufferPos;
    *ptr++ = (CPPTEST_UINT8)(value & 0x000000FFU);
    *ptr++ = (CPPTEST_UINT8)((value & 0x0000FF00U) >> 8);
    *ptr++ = (CPPTEST_UINT8)((value & 0x00FF0000U) >> 16);
    *ptr = (CPPTEST_UINT8)((value & 0xFF000000U) >> 24);
    sendBufferPos += 4U;
}

static void cpptestSend(const void* ptr, unsigned int size)
{
    if (SEND_BUFFER_SIZE - sendBufferPos < size) {
        cpptestSendBuffer();
    }

    if (size > SEND_BUFFER_SIZE) {
        cpptestSendData(cpptestGetTestLogStream(), (const char*)ptr, size);
    } else if (size > 0U) {
        memcpy(sendBuffer + sendBufferPos, ptr, size);
        sendBufferPos += size;
    }
}

#define CPPTEST_ENSURE_BUFFER_SPACE(SIZE) \
    if (SEND_BUFFER_SIZE - sendBufferPos < (SIZE)) { \
        cpptestSendBuffer(); \
    }

#define CPPTEST_SEND_UINT8_VALUE(VALUE) \
    sendBuffer[sendBufferPos] = (CPPTEST_UINT8)(VALUE); \
    ++sendBufferPos;

#define CPPTEST_SEND_UINT16_VALUE(VALUE) \
    cpptestSendUint16((CPPTEST_UINT16)(VALUE));

#define CPPTEST_SEND_INT32_VALUE(VALUE) \
    cpptestSendUint32((CPPTEST_UINT32)(VALUE));

#define CPPTEST_SEND_UINT32_VALUE(VALUE) \
    cpptestSendUint32((CPPTEST_UINT32)(VALUE));

#define CPPTEST_SEND_DATA(PTR, SIZE) \
    cpptestSend((PTR), (unsigned int)(SIZE));

#define CPPTEST_FLUSH() cpptestFlushBuffer()



static void cpptestSendArgumentHeader(unsigned int kind, size_t size, unsigned int flags)
{
    CPPTEST_ENSURE_BUFFER_SPACE(4U);
    CPPTEST_SEND_UINT8_VALUE(kind);
    
    if (size < 16U) {
        CPPTEST_SEND_UINT8_VALUE(flags | size);
    } else if (size <= 0xFFU) {
        CPPTEST_SEND_UINT8_VALUE(flags | CPPTEST_LONG_LENGTH_FLAG | 1U);
        CPPTEST_SEND_UINT8_VALUE(size);
    } else {
        if ((size >> 15) > 1) {
            size = 0xFFFFU;
        }
        CPPTEST_SEND_UINT8_VALUE(flags | CPPTEST_LONG_LENGTH_FLAG | 2U);
        CPPTEST_SEND_UINT16_VALUE(size);
    }
}

static void sendCurrentTimeArgument(void)
{
    char timeStr[30];
    CppTest_Time t = CppTest_TimeCurrent();

    localTimeToString(timeStr, t);
    cpptestSendStringArgument(CPPTEST_ARG_EVENT_TIME_STAMP, timeStr);
}

void cpptestSendStringArgument(unsigned int kind, const char* str)
{
    if (str != 0) {
        size_t size = (unsigned int)strlen(str);
        
        cpptestSendArgumentHeader(kind, size, 0U);
        CPPTEST_SEND_DATA(str, size);
    }
}

void cpptestSendCStringArgument(unsigned int kind, const char* str)
{
    size_t size = (str != 0 ? strlen(str) : 0U);
    
    cpptestSendArgumentHeader(kind, size, str == 0 ? CPPTEST_SPECIAL_CASE_1 : 0U);
    if (str != 0) {
         CPPTEST_SEND_DATA(str, size);
    }
}

void cpptestSendCStringMaxArgument(unsigned int kind, const char* str, unsigned int maxSize)
{
    size_t size = (str != 0 ? strlen(str) : 0U);

    if (size > maxSize) {
        size = maxSize;
    }
    cpptestSendArgumentHeader(kind, size, str == 0 ? CPPTEST_SPECIAL_CASE_1 : 0U);
    if (str != 0) {
         CPPTEST_SEND_DATA(str, size);
    }
}

void cpptestSendDataArgument(unsigned int kind, const void* data, unsigned int size)
{
    cpptestSendArgumentHeader(kind, size, data == 0 ? CPPTEST_SPECIAL_CASE_1 : 0U);
    if (data != 0) {
        CPPTEST_SEND_DATA(data, size);
    }
}

void cpptestSendPointerArgument(unsigned int kind, const void* ptr)
{
    char valueStr[24];
    
    cpptestSPrintF(valueStr, "%p", ptr);
    cpptestSendStringArgument(kind, valueStr);
}

void cpptestSendIntArgument(unsigned int kind, int value)
{
    if (value >= 0) {
        cpptestSendUIntArgument(kind, (unsigned int)value);
    } else {
        CPPTEST_ENSURE_BUFFER_SPACE(6U);
        CPPTEST_SEND_UINT8_VALUE(kind);
        CPPTEST_SEND_UINT8_VALUE(4);
        CPPTEST_SEND_INT32_VALUE(value);
    }
}

void cpptestSendUIntArgument(unsigned int kind, unsigned int value)
{
    CPPTEST_ENSURE_BUFFER_SPACE(6U);
    CPPTEST_SEND_UINT8_VALUE(kind);
    if (value < 32U) {
        CPPTEST_SEND_UINT8_VALUE(CPPTEST_SHORT_DATA_FLAG | value);
    } else if (value <= 0xFFU) {
        CPPTEST_SEND_UINT8_VALUE(1U);
        CPPTEST_SEND_UINT8_VALUE(value);
    } else if ((value >> 15) <= 1) {
        CPPTEST_SEND_UINT8_VALUE(2U);
        CPPTEST_SEND_UINT16_VALUE(value);
    } else {
        CPPTEST_SEND_UINT8_VALUE(4U);
        CPPTEST_SEND_UINT32_VALUE(value);
    }
}

void cpptestSendUIntArgumentWithNestedArgs(unsigned int kind, unsigned int value)
{
    CPPTEST_ENSURE_BUFFER_SPACE(6U);
    CPPTEST_SEND_UINT8_VALUE(kind);
    if (value < 32U) {
        CPPTEST_SEND_UINT8_VALUE(CPPTEST_HAS_NESTED_ARGUMENTS_FLAG | CPPTEST_SHORT_DATA_FLAG | value);
    } else if (value <= 0xFFU) {
        CPPTEST_SEND_UINT8_VALUE(CPPTEST_HAS_NESTED_ARGUMENTS_FLAG | 1U);
        CPPTEST_SEND_UINT8_VALUE(value);
    } else if ((value >> 15) <= 1) {
        CPPTEST_SEND_UINT8_VALUE(CPPTEST_HAS_NESTED_ARGUMENTS_FLAG | 2U);
        CPPTEST_SEND_UINT16_VALUE(value);
    } else {
        CPPTEST_SEND_UINT8_VALUE(CPPTEST_HAS_NESTED_ARGUMENTS_FLAG | 4U);
        CPPTEST_SEND_UINT32_VALUE(value);
    }
}

void cpptestSendArgumentWithNestedArgs(unsigned int kind)
{
    CPPTEST_ENSURE_BUFFER_SPACE(2U);
    CPPTEST_SEND_UINT8_VALUE(kind);
    CPPTEST_SEND_UINT8_VALUE(CPPTEST_HAS_NESTED_ARGUMENTS_FLAG | CPPTEST_SHORT_DATA_FLAG);
}

void cpptestSendBoolArgument(unsigned int kind, int value)
{    
    CPPTEST_ENSURE_BUFFER_SPACE(2U);
    CPPTEST_SEND_UINT8_VALUE(kind);
    CPPTEST_SEND_UINT8_VALUE(CPPTEST_SHORT_DATA_FLAG | (value ? 1U : 0U));
}

#if !CPPTEST_DISABLE_ALL_FLOATING_POINT
void cpptestSendFloatArgument(unsigned int kind, CPPTEST_FLOAT value)
{
    char valueStr[24];
    
    cpptestSPrintF(valueStr, CPPTEST_PRINTF_FLOAT_FMT, (CPPTEST_PRINTF_FLOAT)value);
    cpptestSendStringArgument(kind, valueStr);
}
#endif

void cpptestSendIntegerArgument(unsigned int kind, CPPTEST_INTEGER value)
{
    char valueStr[24];

    cpptestSPrintF(valueStr, CPPTEST_PRINTF_INTEGER_FMT, (CPPTEST_PRINTF_INTEGER)value);
    cpptestSendStringArgument(kind, valueStr);
}

void cpptestSendUIntegerArgument(unsigned int kind, CPPTEST_UINTEGER value)
{
    char valueStr[24];

    cpptestSPrintF(valueStr, CPPTEST_PRINTF_UINTEGER_FMT, (CPPTEST_PRINTF_UINTEGER)value);
    cpptestSendStringArgument(kind, valueStr);
}

void cpptestSendEventPositionArgument(const char* defaultFile, const char* file, unsigned int line)
{
    if (!defaultFile || (file && (defaultFile != file) && cpptestStrCmp(defaultFile, file) != 0)) {
        cpptestSendStringArgument(CPPTEST_ARG_EVENT_FILE, file);
    }
    cpptestSendUIntArgument(CPPTEST_ARG_EVENT_LINE, line);
}

void cpptestDsPositionArgument(const char* file, unsigned int line, unsigned int column)
{
    cpptestSendStringArgument(CPPTEST_ARG_DS_FILE, file);
    cpptestSendUIntArgument(CPPTEST_ARG_DS_LINE, line);
    cpptestSendUIntArgument(CPPTEST_ARG_DS_COLUMN, column);
}

void cpptestSendStackTraceTable(CppTestStackTraceData* sData, unsigned int threadId)
{
    unsigned int numberOfElements = 0U;
    CppTestStackTraceElement* element = 0;

    cpptestSendEmptyChunkWithArgs(CPPTEST_MSG_STACK_TRACE_TABLE);
    if (threadId != CPPTEST_DEFAULT_THREAD_ID) {
        cpptestSendUIntArgument(CPPTEST_ARG_THREAD_ID, threadId);
    }
    numberOfElements = sData->numberOfElements;
    element = &sData->elements[0];
    while (numberOfElements > 0U) {
        CPPTEST_TRACE_FMT(CPPTEST_FMT_3("Sending stack trace element (%d): %s:%d", 
                    localThreadCurrentId(), element->file,element->line));
        cpptestSendArgumentWithNestedArgs(CPPTEST_ARG_STACK_TRACE_ELEMENT);
        if (element->tuId != 0U) {
            cpptestSendUIntArgument(CPPTEST_SUB_ARG_TRANSLATION_UNIT_ID, element->tuId);
        }
        if (element->stmtId != 0U) {
            cpptestSendUIntArgument(CPPTEST_SUB_ARG_STMT_ID, element->stmtId);
        }
        if (element->funcCallId != 0U) {
            cpptestSendUIntArgument(CPPTEST_SUB_ARG_VALUE, element->funcCallId);
        }
        if (element->flags != 0U) {
            cpptestSendUIntArgument(CPPTEST_SUB_ARG_FLAGS, element->flags);
        }
        cpptestSendLastNestedArgument();
        ++element;
        --numberOfElements;
    }
    sData->numberOfElements = 0U;
    sData->numberOfSentElements += sData->numberOfElements;
    cpptestSendLastArgument();
}

void cpptestSendLastArgument(void)
{
    CPPTEST_ENSURE_BUFFER_SPACE(2U);
    CPPTEST_SEND_UINT8_VALUE(CPPTEST_ARG_CONTROL);
    CPPTEST_SEND_UINT8_VALUE(CPPTEST_LAST_ARG_FLAG);
    CPPTEST_FLUSH();
    CPPTEST_UNLOCK(testLogLock);
}

void cpptestSendLastNestedArgument(void)
{
    CPPTEST_ENSURE_BUFFER_SPACE(2U);
    CPPTEST_SEND_UINT8_VALUE(CPPTEST_ARG_CONTROL);
    CPPTEST_SEND_UINT8_VALUE(CPPTEST_LAST_ARG_FLAG);
}

void cpptestSendEmptyChunkWithArgs(unsigned int kind)
{
    INITIALIZED_RUNTIME_REQUIRED(NO_RETURN_VALUE)
    INITIALIZED_COMMUNICATION_REQUIRED()

    CPPTEST_LOCK(testLogLock);

    CPPTEST_ENSURE_BUFFER_SPACE(4U);
    CPPTEST_SEND_UINT16_VALUE(kind);
    CPPTEST_SEND_UINT8_VALUE(CPPTEST_HAS_ARGUMENTS_FLAG);
    CPPTEST_SEND_UINT8_VALUE(4U);
}

void cpptestSendChunk(unsigned int kind, int hasArguments)
{
    INITIALIZED_RUNTIME_REQUIRED(NO_RETURN_VALUE)
    INITIALIZED_COMMUNICATION_REQUIRED()
    
    CPPTEST_LOCK(testLogLock);

    CPPTEST_ENSURE_BUFFER_SPACE(4U);
    CPPTEST_SEND_UINT16_VALUE(kind);
    CPPTEST_SEND_UINT8_VALUE(CPPTEST_HAS_ARGUMENTS_FLAG);
    CPPTEST_SEND_UINT8_VALUE(4U);

    {
        unsigned int threadId = localThreadCurrentId();
        if (threadId != CPPTEST_DEFAULT_THREAD_ID) {
            cpptestSendUIntArgument(CPPTEST_ARG_EVENT_THREAD_ID, threadId);
        }
    }
    sendCurrentTimeArgument();
    if (!hasArguments) {
        cpptestSendLastArgument();
    }
}

void localInitializeMessages(void)
{
    CPPTEST_LOCK_INIT(testLogLock);
}

void localFinalizeMessages(void)
{
    CPPTEST_LOCK_DESTROY(testLogLock);
}

/*--------------------------------------------------------------------------------------------
 * Functions from runtime API
 *------------------------------------------------------------------------------------------*/

const char* cpptestFormatBuffer(char* buffer, const char* format, ...)
{
    va_list args;
    va_start(args, format);
    cpptestVSPrintF(buffer, format, args);
    va_end(args);
    return buffer;
}

