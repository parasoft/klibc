/*
 * (C) Copyright Parasoft Corporation 1998 - 2019.  All rights reserved.
 * The use of this source code is governed by the general Parasoft License Agreement.
 * Redistribution strictly prohibited.
 */
#define CPPTEST_RUNTIME_IMPLEMENTATION

#include "cpptest_runtime.h"

#include "cpptest_system.h"

#include "cpptest_headers.h"

#include "CppTestInternal.h"

#include CPPTEST_string

#if defined (CPPTEST_WIN32)
#   include <windows.h>
#else
#  if CPPTEST_SIGNALS_HANDLING_ENABLED
#    include CPPTEST_signal
#  endif
#endif

CPPTEST_using_namespace_std

CPPTEST_EXIT_DECL



/* Maximum length of stack trace. */
#define CPPTEST_MAX_STACK_TRACE_DATA_LENGTH 256

#define CPPTEST_INT_TO_STRING_BUFFER_SIZE 33

#define CPPTEST_EVENT_HANDLER
#define CPPTEST_STATE_INFO

#if CPPTEST_HAS_GETENV
/* True if autoinitialization is aloowed */
static int autoInitializationAllowed = CPPTEST_DEFAULT_RUNTIME_AUTO_INIT;
/* True if runtime is initialized */
static int initialized = 0;
#endif

/* Tells whether current test case should fail */
static CppTest_ErrorCode currentTestCaseExpectedErrorCode = CPPTEST_NO_ERROR;
static unsigned int currentTestCaseExpectedErrorCodeMask = CPPTEST_FULL_MASK;
/* Name of current test case */
static const char* currentTestCaseName = 0;
/* Name of current test suite */
static const char* currentTestSuiteName = 0;
/* Path to the file with currently executed test suite */
static const char* currentTestSuiteFile = 0;
/* Current test case iteration */
static unsigned int currentTestCaseIteration = 0U;
#if CPPTEST_USE_EXTENDED_DATA_SOURCE_ROW_SIGNATURE
/* Name of current test case parameters */
static const char* currentTestCaseParameters = 0;
#endif /* CPPTEST_USE_EXTENDED_DATA_SOURCE_ROW_SIGNATURE */
/* Path to the file with currently executed test case */
static const char* currentTestCaseFile = 0;
/* Line of currently executed test case */
static unsigned int currentTestCaseLine = 0U;
/* 1 for c++ test case, 0 for c testcase */
static int currentTestCaseIsCpp = 0;
/* Local id of testcase. Incremented after each testcase execution. */
static unsigned int currentTestCaseId = 0U;
/* Pointer to current data source (may be null) */
struct CppTest_DataSource* currentDataSource = 0;
/* Current left number of messages per test case */
static int leftTestCaseMessages = INT_MAX;
/* Maximum number of messages per test case */
static unsigned int maxNumberOfTestCaseMessages = 100U;

/* Call back called at runtime exit */   
static CppTestAtExitCallback atExitCallback = 0;

/* Tells if repors are enabled */
static int _cpptestSendReports = 0;
/* Tells if passed assertions are enabled */
static int _cpptestSendPassedAsserts = 0;
/* Tells if post conditions are enabled */
static int _cpptestSendPostConditions = 1;

/* SYNCHRONIZED: start */
    /* Access to following variables should be synchronized on runtimeLock variable */
    CPPTEST_LOCK_DECLARE(static, runtimeLock)
    /* Number of errors during test case execution. The expected errors are not 
       considered here. */
    static unsigned int errors = 0U;
    /* Number of expected errors during test case execution. */
    static unsigned int expectedErrors = 0U;
    /* Id of thread that lock runtimeLock. */
    static unsigned int currRuntimeThreadId = 0U;
    /* Status of runtime */
    static enum { OUTSIDE_TEST_CASE, INSIDE_TEST_CASE } runtimeStatus = OUTSIDE_TEST_CASE;
/* SYNCHRONIZED: end */

/* SYNCHRONIZED: start */
    /* Access to following variables should be synchronized on formattedDataLock variable */
    CPPTEST_LOCK_DECLARE(static, formattedDataLock)
    /* Pointer to the last allocation made for formatted data */
    static char* formattedData = 0;
/* SYNCHRONIZED: end */

/* SYNCHRONIZED: start */
    /* Access to following variables should be synchronized on trunslationUnitLock variable */
    CPPTEST_LOCK_DECLARE(static, trunslationUnitLock)
    /* Next translation unit id. */
    static unsigned int nextTranslationUnitId = 1U;
/* SYNCHRONIZED: end */

#if CPPTEST_SETJMP_ENABLED
/* This should be used only in main thread: start */
static __CPTR_TestCaseBreakFuncPtr cpptestTestCaseBreakFuncPtr = 0;
#endif /* CPPTEST_SETJMP_ENABLED */
/* This should be used only in main thread: end */

/* Last runtime command handler */
static CppTestRuntimeCommandHandlerNode* runtimeCommandHandlers = 0;

/* Pointer to auto generated array with enum infos */
static CppTestEnumTypeString* cpptestEnumTypeStrings = 0;

/* List of all stub infos */
static CppTest_StubInfo** _stubInfos = 0;

/* Synchronize processing checkers */
CPPTEST_LOCK_DECLARE(static, checkersLock)

#if !defined (CPPTEST_WIN32) && CPPTEST_SIGNALS_HANDLING_ENABLED
/* List of handled signals */
static int handledSignals[] = {
#ifdef SIGHUP
                            SIGHUP,
#endif
#ifdef SIGINT
                            SIGINT,
#endif
#ifdef SIGQUIT
                            SIGQUIT,
#endif
#ifdef SIGILL
                            SIGILL,
#endif
#ifdef SIGABRT
                            SIGABRT,
#endif
#ifdef SIGIOT
                            SIGIOT,
#endif
#ifdef SIGBUS
                            SIGBUS,
#endif
#ifdef SIGFPE
                            SIGFPE,
#endif
#ifdef SIGUSR1
                            SIGUSR1,
#endif
#ifdef SIGSEGV
                            SIGSEGV,
#endif
#ifdef SIGUSR2
                            SIGUSR2,
#endif
#ifdef SIGPIPE
                            SIGPIPE,
#endif
#ifdef SIGALRM
                            SIGALRM,
#endif
/*
   Do not intercept SIGTERM signal.
   It is used by the driver to signal execution stop.
#ifdef SIGTERM
                            SIGTERM,
#endif
*/
#ifdef SIGSTKFLT
                            SIGSTKFLT,
#endif
#ifdef SIGTSTP
                            SIGTSTP,
#endif
#ifdef SIGTTIN
                            SIGTTIN,
#endif
#ifdef SIGTTOU
                            SIGTTOU,
#endif
#ifdef SIGXCPU
                            SIGXCPU,
#endif
#ifdef SIGXFSZ
                            SIGXFSZ,
#endif
#ifdef SIGVTALRM
                            SIGVTALRM,
#endif
#ifdef SIGPROF
                            SIGPROF,
#endif
#ifdef SIGPOLL
                            SIGPOLL,
#endif
#ifdef SIGTRAP
                            SIGTRAP,
#endif
#ifdef SIGLOST
                            SIGLOST,
#endif
#ifdef SIGEMT
                            SIGEMT,
#endif
#ifdef SIGSYS
                            SIGSYS
#endif
};
#endif /* CPPTEST_SIGNALS_HANDLING_ENABLED */

static CppTest_TestCaseResult getTestCaseResult(void)
{   
    if (errors) {
        return CPPTEST_RESULT_FAILED;
    } else if (currentTestCaseExpectedErrorCode == CPPTEST_NO_ERROR || expectedErrors) {
        return CPPTEST_RESULT_OK;
    } else {
        /* No expected result. */
        return CPPTEST_RESULT_FAILED;
    }
}

#if CPPTEST_SIGNALS_HANDLING_ENABLED
#if !defined(CPPTEST_WIN32)

typedef void (*SignalHandler)(int);

/**
 * Sets signal handler.
 */
static void setTranslator(SignalHandler handler)
{
    const unsigned int count = sizeof(handledSignals) / sizeof(int);
    unsigned int num = 0;
    for (num = 0; num < count; ++num) {
        signal(handledSignals[num], handler);
    }
}
#endif /* !defined(CPPTEST_WIN32) */
#endif /* CPPTEST_SIGNALS_HANDLING_ENABLED */

/*--------------------------------------------------------------------------------------------
 * Internal runtime API (function used in other runtime source files, local*)
 *------------------------------------------------------------------------------------------*/

int localRuntimeInitialized = 0;
int localCommunicationInitialized = 0;

static CppTest_System *targetSystem = 0;

void* localTargetMalloc(size_t size)
{
    return targetSystem ? targetSystem->mallocFunc(size) : localInternalMalloc(size);
}

void* localTargetCalloc(size_t elements, size_t elemSize)
{
    return targetSystem ? targetSystem->callocFunc(elements, elemSize) : localInternalAlloc(elements * elemSize);
}

void* localTargetRealloc(void* ptr, size_t newSize)
{
    return targetSystem ? targetSystem->reallocFunc(ptr, newSize) : localInternalRealloc(ptr, newSize);
}

void localTargetFree(void* ptr)
{
    targetSystem ? targetSystem->freeFunc(ptr) : localInternalFree(ptr);
}

void* localTargetMemset(void* ptr, int value, size_t size)
{
    return targetSystem ? targetSystem->memsetFunc(ptr, value, size) : localInternalMemset(ptr, value, size);
}



void* localInternalMalloc(size_t size)
{
    void* ptr = 
#if CPPTEST_ALLOC_IMPL
        localAllocImpl(size);
#else
        CPPTEST_C_std malloc(size);
#endif

    if (!ptr) {
	/** 
	 * Add platform dependent code here for handling malloc failure 
	 */
        CPPTEST_EXIT(CPPTEST_MALLOC_FAILED);
    }
    CPPTEST_TRACE_FMT(CPPTEST_FMT_2("localInternalMalloc: %x [%u]", ptr, size));
    CPPTEST_ANALYZE_MALLOC(ptr, size);
    return ptr;
}

void* localInternalAlloc(size_t size)
{
    void* ptr = localInternalMalloc(size);
    localInternalMemset(ptr, 0, size);
    CPPTEST_TRACE_FMT(CPPTEST_FMT_2("localInternalAlloc: %x [%u]", ptr, size));
    return ptr;
}

void* localInternalRealloc(void* ptr, size_t newSize)
{
#if CPPTEST_ALLOC_IMPL
        return 0;
#else
    void* newPtr = CPPTEST_C_std realloc(ptr, newSize);
    if (!newPtr) {
        CPPTEST_EXIT(CPPTEST_MALLOC_FAILED);
    }
    CPPTEST_TRACE_FMT(CPPTEST_FMT_2("localInternalRealloc: %x [%u]", newPtr, newSize));
    CPPTEST_ANALYZE_REALLOC(ptr, newPtr, newSize);
    return newPtr;
#endif
}

void localInternalFree(void* ptr)
{
    CPPTEST_TRACE_FMT(CPPTEST_FMT_1("localInternalFree: %x", ptr));

#if CPPTEST_ALLOC_IMPL
    localFreeImpl(ptr);
#else
    CPPTEST_C_std free(ptr);
#endif
    CPPTEST_ANALYZE_FREE(ptr);
}

void* localInternalMemset(void* ptr, int value, size_t size)
{
    if (ptr != 0) {
        CPPTEST_C_std memset(ptr, value, size);
    }
    return ptr;
}


static void lockRuntime(void)
{
    CPPTEST_LOCK(runtimeLock);
    currRuntimeThreadId = localCurrentData()->threadId;
}

static void unlockRuntime(void)
{
    currRuntimeThreadId = 0U;
    CPPTEST_UNLOCK(runtimeLock);
}

static void initializeRuntimeBeforeTestCaseExecution(void)
{
    localTestCaseStartTime();   /* startTime = getTime(); trackTimeout = 1; */ 
    localThreadTestCaseStart();    /* Removing finished threads */
    localCoverageTestCaseStart();   /* Cleaning CppTestCoverageInfos; _collectCoverage = 1; */
    localMemoryAnalysisTestCaseStart(); /* allocation_id_before_test_case = last_allocation_id;  */
    localRuntimeThreadInit();   /* setTranslator(&cpptestSignalFound);  */
    localStackTraceTestCaseStart(); /* Cleaning;  _stackTraceEnabled = 1; */

    if (maxNumberOfTestCaseMessages) {
        leftTestCaseMessages = (int)maxNumberOfTestCaseMessages;
    } else {
        leftTestCaseMessages = INT_MAX;
    }
    

}

static void finalizeRuntimeAfterTestCaseExecution(void)
{
    localStackTraceTestCaseEnd();   /* _stackTraceEnabled = 0;  */
    localMemoryAnalysisTestCaseEnd();   /* allocation_id_before_test_case = INVALID_ALLOC_ID;  */
    localCoverageTestCaseEnd(); /* _collectCoverage = 0; */
    localTestCaseEndTime();  /* trackTimeout = 0; */

    CPPTEST_LOCK(formattedDataLock);
    while (formattedData) {
        char* formattedDataPref = *(char**)formattedData;
        localInternalFree(formattedData);
        formattedData = formattedDataPref;
    }
    CPPTEST_UNLOCK(formattedDataLock);

#if CPPTEST_C_STREAMS_REDIRECT_ENABLED || CPPTEST_CPP_STREAMS_REDIRECT_ENABLED
    CppTest_ClearRedirects();
#endif
    errors = 0U;
    expectedErrors = 0U;

    cpptestUnregisterAllStubCallbacks();

    CPPTEST_LOG("Test case end");
}


static int beginErrorProcessing(CppTest_ErrorCode code)
{
    INITIALIZED_RUNTIME_REQUIRED(0)

    if ((runtimeStatus == OUTSIDE_TEST_CASE) && CPPTEST_IS_TESTRUNNER_CURR_THREAD()) {
        /* runtimeLock is already blocked by main thread */
        /* TODO:  localCurrentData - may be a recursion. */
        return 0;
    } else {
        unsigned int testCaseId = currentTestCaseId;

        lockRuntime();
        /* We are inside testcase. */

        if (testCaseId < currentTestCaseId) {
            /* We are in thread that starts error processing before current testcase has been started. 
               The cpptestTestCaseBegin was already called, so cancel current testcase and exit or
               just ignore this error. */
            unlockRuntime();
            return 0;
        } else if (currentTestCaseId == 0U) {
            /* The cpptestTestBegin is not yet called or cpptestTestEnd was already called. */
            /* Just ignore this error. */
            unlockRuntime();
            return 0;
        }
        if ((code & currentTestCaseExpectedErrorCodeMask) == currentTestCaseExpectedErrorCode) {
            ++expectedErrors;
        } else {
            ++errors;
        }
    }

    localSendAllStackTraces();

    return 1;
}

static void endErrorProcessing(int exitCode, int doJmp, int doExit)
{
    /* We are inside testcase. The beginErrorProcessing was called before. */
#if CPPTEST_SETJMP_ENABLED
    if (doJmp && cpptestTestCaseBreakFuncPtr) {
        if (CPPTEST_IS_TESTRUNNER_CURR_THREAD()) {
            __CPTR_TestCaseBreakFuncPtr testCaseBreakFuncPtr = cpptestTestCaseBreakFuncPtr;
            cpptestTestCaseBreakFuncPtr = 0;
            unlockRuntime();
            CPPTEST_LOG_FMT(CPPTEST_FMT_1("Jumping (%d)...", localThreadCurrentId()));
            testCaseBreakFuncPtr();
        }
    }
#endif /* CPPTEST_SETJMP_ENABLED */

    if (doExit) {

        runtimeStatus = OUTSIDE_TEST_CASE;
        cpptestSendChunk(CPPTEST_MSG_EXIT_TEST, /*hasArguments=*/1);
        if (!cpptestGetBoolPropertyEx("test.app_mode", CPPTEST_APPLICATION_MODE)) {
            cpptestSendUIntArgument(CPPTEST_ARG_TESTCASE_RESULT, getTestCaseResult());
        }
        cpptestSendLastArgument();
        if (!cpptestGetBoolPropertyEx("test.app_mode", CPPTEST_APPLICATION_MODE)) {
            finalizeRuntimeAfterTestCaseExecution();
            currentTestCaseIteration = 0U;
#if CPPTEST_USE_EXTENDED_DATA_SOURCE_ROW_SIGNATURE
            currentTestCaseParameters = 0;
#endif /* CPPTEST_USE_EXTENDED_DATA_SOURCE_ROW_SIGNATURE */
            currentTestCaseName = "";
        }

        if (atExitCallback) {
            atExitCallback();
        }

        cpptestFinalizeRuntime();

        unlockRuntime();
        CPPTEST_EXIT(exitCode);
    } else {
        unlockRuntime();
    }
}


void localRuntimeThreadInit(void)
{
#if CPPTEST_SIGNALS_HANDLING_ENABLED
#if !defined(CPPTEST_WIN32)
    setTranslator(&cpptestSignalFound);
#endif
#endif /* CPPTEST_SIGNALS_HANDLING_ENABLED */
}

int localAutoInitializationAllowed(void)
{
#if CPPTEST_HAS_GETENV
    if (!initialized) {
        const char* autoInitEnv = getenv("CPPTEST_RUNTIME_AUTO_INIT");
    
        if (autoInitEnv) {
            if (*autoInitEnv == '0') {
                autoInitializationAllowed = 0;
            } else {
                autoInitializationAllowed = 1;
            }
        }
        initialized = 1;
    }
    return autoInitializationAllowed;
#else /* !CPPTEST_HAS_GETENV */
    return CPPTEST_DEFAULT_RUNTIME_AUTO_INIT;
#endif  /* CPPTEST_HAS_GETENV */
}

int localIsInsideRuntime(void)
{
    return localCurrentData()->threadId == currRuntimeThreadId ? 1 : 0;
}

void localInitializeTranslationUnit(CppTestTranslationUnitInfo* tui)
{
    CPPTEST_LOCK(trunslationUnitLock)
    /* Check againg */
    if (tui->id == 0U) {
        cpptestSendEmptyChunkWithArgs(CPPTEST_MSG_TRANSLATION_UNIT_INFO);
        if (tui->ildb_file) {
            cpptestSendStringArgument(CPPTEST_ARG_ILDB_FILE_NAME, tui->ildb_file);
        }
        cpptestSendLastArgument();

        /* Do this at end */
        tui->id = nextTranslationUnitId;
        ++nextTranslationUnitId;
    }
    CPPTEST_UNLOCK(trunslationUnitLock)
}


void localInitializeTestCases(void)
{
    maxNumberOfTestCaseMessages = (unsigned int)cpptestGetUIntPropertyEx("testcases.max_messages", 100U);
    localSetTimeout((CPPTEST_UINTEGER)cpptestGetUIntPropertyEx("testcases.timeout", 5000U));
    _cpptestSendPostConditions = (int)cpptestGetIntPropertyEx("send.post.conditions", 1);
    _cpptestSendReports = (int)cpptestGetIntPropertyEx("send.reports", 0);
    _cpptestSendPassedAsserts = (int)cpptestGetIntPropertyEx("send.passed.asserts", 0);
}

void localFinalizeTestCases(void)
{
}

const char* localFormatSlave(const char* format, va_list args)
{
    char formattedTextBuf[CPPTEST_MAX_MESSAGE_SIZE];
    char* formattedText = 0;
    size_t length = 0U;

    cpptestVSPrintF(formattedTextBuf, format, args);

    length = strlen(formattedTextBuf) + 1U;
    formattedText = (char*) localInternalMalloc(sizeof(char*) + length);

    CPPTEST_LOCK(formattedDataLock);
    *(char**)formattedText = formattedData;
    formattedData = formattedText;
    CPPTEST_UNLOCK(formattedDataLock);

    formattedText += sizeof(char*);
    memcpy(formattedText, formattedTextBuf, length);

    return formattedText;
}

unsigned int localGetCurrentTestCaseIteration(void)
{
    return currentTestCaseIteration;
}

const char* localGetCurrentTestCaseFullName(void)
{
    const char* ret = 0;
 
#if CPPTEST_USE_EXTENDED_DATA_SOURCE_ROW_SIGNATURE
    if (0 == currentTestCaseParameters) {
#endif
        if (currentTestCaseIsCpp) {
            ret = cpptestFormat("%s::%s", currentTestSuiteName, currentTestCaseName);
        } else {
            ret = cpptestFormat("%s", currentTestCaseName);
        }
#if CPPTEST_USE_EXTENDED_DATA_SOURCE_ROW_SIGNATURE
    } else {
        if (currentTestCaseIsCpp) {
            ret = cpptestFormat("%s::%s;%s", currentTestSuiteName, currentTestCaseName, currentTestCaseParameters);
        } else {
            ret = cpptestFormat("%s;%s", currentTestCaseName, currentTestCaseParameters);
        }
    }
#endif
    return ret;
}


/*--------------------------------------------------------------------------------------------
 * Functions from runtime API (cpptest*)
 *------------------------------------------------------------------------------------------*/

void CDECL_CALL cpptestAddRuntimeCommandHandler(CppTestRuntimeCommandHandlerNode* runtimeCommandHandlerNode)
{
    runtimeCommandHandlerNode->prev = runtimeCommandHandlers;
    runtimeCommandHandlers = runtimeCommandHandlerNode;
}

void CDECL_CALL cpptestExecuteRuntimeCommand(unsigned int eventId)
{
    CppTestRuntimeCommandHandlerNode* runtimeCommandHandlerNode = runtimeCommandHandlers;

    while (runtimeCommandHandlerNode != 0) {
        runtimeCommandHandlerNode->handler(eventId);
        runtimeCommandHandlerNode = runtimeCommandHandlerNode->prev;
    }
}

const char* CDECL_CALL cpptestPathRelativeToAbsolute(const char* relPath, const char* relToFile)
{
    unsigned int cutOff = 0U;
    unsigned int i;
    for (i = cpptestStrLen(relToFile); i > 0U; i--) {
#if defined(FILE_SEPARATOR_CONDITION)	    
        if (FILE_SEPARATOR_CONDITION(relToFile[i])) {
#else
        if (relToFile[i] == '\\' || relToFile[i] == '/') {
#endif
            cutOff = i + 1U;
            break; /* parasoft-suppress CODSTA-08 "accepted" */
        }
    }
    if (cutOff > 0U) {
        unsigned int relDirLength = cpptestStrLen(relPath);
        unsigned int dirLength = cutOff * sizeof(char);
        char* csvFile = (char*)localInternalMalloc(dirLength + relDirLength + sizeof(char));
        memcpy(csvFile, relToFile, dirLength);
        memcpy(csvFile + dirLength, relPath, relDirLength + sizeof(char));
        return csvFile;
    }
    return relPath; 
}

#if CPPTEST_SETJMP_ENABLED
__CPTR_TestCaseBreakFuncPtr CDECL_CALL cpptestGetTestCaseBreakFuncPtr(void)
{
    return cpptestTestCaseBreakFuncPtr;
}

__CPTR_TestCaseBreakFuncPtr CDECL_CALL cpptestSetTestCaseBreakFuncPtr(__CPTR_TestCaseBreakFuncPtr funcPtr)
{
    __CPTR_TestCaseBreakFuncPtr old = cpptestTestCaseBreakFuncPtr;
    cpptestTestCaseBreakFuncPtr = funcPtr;
    return old;
}
#endif /* CPPTEST_SETJMP_ENABLED */


static int cpptestSendLimitedChunk(unsigned int kind, CppTest_StubCallInfo* sci, const char* file, unsigned int line)
{
    /* Not synchronized */
    if (leftTestCaseMessages <= 0) {
        if (leftTestCaseMessages == 0) {
            --leftTestCaseMessages;
            cpptestSendChunk(CPPTEST_MSG_TOO_MANY_MSGS, /*hasArguments=*/1);
            cpptestSendUIntArgument(CPPTEST_ARG_TC_MAX_MESSAGES, maxNumberOfTestCaseMessages);
            if (file != 0) {
                cpptestSendEventPositionArgument(currentTestSuiteFile, file, line);
            }
            cpptestSendLastArgument();
        }
        return 0;
    } else {
        --leftTestCaseMessages;

        cpptestSendChunk(kind, /*hasArguments=*/1);
        if (file != 0) {
            cpptestSendEventPositionArgument(currentTestSuiteFile, file, line);
        }
        if (sci) {
            cpptestSendStringArgument(CPPTEST_ARG_STUB_ID, sci->stubInfo->userId);
            cpptestSendUIntArgument(CPPTEST_ARG_CALL_ID, sci->callNo);
        }
        return 1;
    }
}

CPPTEST_EVENT_HANDLER
void CDECL_CALL cpptestUserBreak(const char* file, unsigned int line)
{
    if (beginErrorProcessing(CPPTEST_USER_BREAK)) {

        if (cpptestSendLimitedChunk(CPPTEST_USER_BREAK, 0, file, line)) {
            cpptestSendLastArgument();
        }

        endErrorProcessing(/*exitCode=*/33, /*dojmp=*/1, /*doExit=*/1);
    }
}

CPPTEST_EVENT_HANDLER
void CDECL_CALL cpptestTimeout(CPPTEST_UINTEGER timeout)
{
    if (beginErrorProcessing(CPPTEST_TIMEOUT)) {
        cpptestSendChunk(CPPTEST_TIMEOUT, /*hasArguments=*/1);
        cpptestSendUIntArgument(CPPTEST_ARG_TIMEOUT_VALUE, (unsigned int)(timeout / 1000U));
        cpptestSendLastArgument();

        endErrorProcessing(/*exitCode=*/33, /*dojmp=*/1, /*doExit=*/1);
    }
}

#if CPPTEST_SIGNALS_HANDLING_ENABLED
CPPTEST_EVENT_HANDLER
void CDECL_CALL cpptestSignalFound(int sigNo)
{
    CppTest_ErrorCode code = localGetHandledSignalErrorCode(sigNo);

#if !defined(CPPTEST_WIN32)
    setTranslator(SIG_DFL);
    /* set ignoring the signal */
    signal(sigNo, SIG_IGN);
#endif

    if (beginErrorProcessing(code)) {
        if (cpptestSendLimitedChunk(code, 0, 0, 0U)) {
            cpptestSendLastArgument();
        }
        endErrorProcessing(/*exitCode=*/33, /*dojmp=*/1, /*doExit=*/1);
    }
}
#endif /* CPPTEST_SIGNALS_HANDLING_ENABLED */


CPPTEST_EVENT_HANDLER
void CDECL_CALL cpptestCommunicationBegin(void)
{
    cpptestSendChunk(CPPTEST_MSG_TEST_HEADER, /*hasArguments=*/1);
    cpptestSendStringArgument(CPPTEST_ARG_TEST_NAME, cpptestGetProperty("test.name"));
    cpptestSendLastArgument();
}

void CDECL_CALL cpptestCommunicationEnd(void)
{
    cpptestSendChunk(CPPTEST_MSG_TEST_END, /*hasArguments=*/0);
}

CPPTEST_EVENT_HANDLER
void CDECL_CALL cpptestTestRunnerBegin(void)
{
    CPPTEST_SET_IS_TESTRUNNER_CURR_THREAD();
    lockRuntime();      
}

CPPTEST_EVENT_HANDLER
void CDECL_CALL cpptestTestRunnerEnd(void)
{
    currentTestCaseId = 0U;
    unlockRuntime();
}


CPPTEST_EVENT_HANDLER
void CDECL_CALL cpptestTestSuiteBegin(const char* name, const char* file, unsigned int line)
{
    currentTestSuiteName = name;
    currentTestSuiteFile = file;

    cpptestSendChunk(CPPTEST_MSG_TESTSUITE_BEGIN, /*hasArguments=*/1);
    cpptestSendStringArgument(CPPTEST_ARG_TESTSUITE_NAME, name);
    cpptestSendEventPositionArgument(0, file, line);
    cpptestSendLastArgument();

    cpptestExecuteRuntimeCommand(CPPTEST_CMD_TESTSUITE_BEGIN);
}

CPPTEST_EVENT_HANDLER
void CDECL_CALL cpptestTestSuiteEnd(void)
{
    cpptestExecuteRuntimeCommand(CPPTEST_CMD_TESTSUITE_END);
    cpptestSendChunk(CPPTEST_MSG_TESTSUITE_END, /*hasArguments=*/0);
}

CPPTEST_EVENT_HANDLER
void CDECL_CALL cpptestTestCaseBegin(
        const char* name, CppTest_ErrorCode expectedErrorCode, unsigned int expectedErrorCodeMask, int isCpp, const char* file, unsigned int line)
{
    CPPTEST_LOG("Test case start");

    currentTestCaseName = name;
    currentTestCaseFile = file;
    currentTestCaseLine = line;
    currentTestCaseIsCpp = isCpp;
    currentTestCaseExpectedErrorCode = expectedErrorCode;
    currentTestCaseExpectedErrorCodeMask = expectedErrorCodeMask;
    
    cpptestSendChunk(CPPTEST_MSG_TESTCASE_BEGIN, /*hasArguments=*/1);
    if (expectedErrorCode != CPPTEST_NO_ERROR) {
        cpptestSendUIntArgument(CPPTEST_ARG_EXPECTED_ERROR, expectedErrorCode);
    }
    cpptestSendStringArgument(CPPTEST_ARG_TESTCASE_NAME, name);
    cpptestSendEventPositionArgument(currentTestSuiteFile, file, line);
    cpptestSendLastArgument();

    initializeRuntimeBeforeTestCaseExecution();
    cpptestExecuteRuntimeCommand(CPPTEST_CMD_INITIALIZE_TESTCASE);

    ++currentTestCaseId;
    unlockRuntime();
    runtimeStatus = INSIDE_TEST_CASE;

    cpptestExecuteRuntimeCommand(CPPTEST_CMD_TESTCASE_BEGIN);
}

CPPTEST_EVENT_HANDLER
void CDECL_CALL cpptestTestCaseEnd(void)
{
    cpptestExecuteRuntimeCommand(CPPTEST_CMD_TESTCASE_END);
    runtimeStatus = OUTSIDE_TEST_CASE;
    lockRuntime();

    cpptestSendChunk(CPPTEST_MSG_TESTCASE_END, /*hasArguments=*/1);
    cpptestSendUIntArgument(CPPTEST_ARG_TESTCASE_RESULT, getTestCaseResult());
    cpptestSendLastArgument();

    cpptestExecuteRuntimeCommand(CPPTEST_CMD_FINALIZE_TESTCASE);
    finalizeRuntimeAfterTestCaseExecution();
    currentTestCaseName = "";
}

CPPTEST_EVENT_HANDLER
void CDECL_CALL cpptestDsForCurrentTestCaseInitialized(struct CppTest_DataSource* dataSource)
{
    currentDataSource = dataSource;
}

CPPTEST_EVENT_HANDLER
void CDECL_CALL cpptestDsTestCaseBegin(const char* name, CppTest_ErrorCode expectedErrorCode, unsigned int expectedErrorCodeMask,  int isCpp, const char* file, unsigned int line)
{
    CPPTEST_LOG("Data source test case start");

    currentTestCaseName = name;
    currentTestCaseFile = file;
    currentTestCaseLine = line;
    currentTestCaseIsCpp = isCpp;
    currentTestCaseExpectedErrorCode = expectedErrorCode;
    currentTestCaseExpectedErrorCodeMask = expectedErrorCodeMask;

    cpptestSendChunk(CPPTEST_MSG_DS_TESTCASE_BEGIN, /*hasArguments=*/1);
    if (expectedErrorCode != CPPTEST_NO_ERROR) {
        cpptestSendUIntArgument(CPPTEST_ARG_EXPECTED_ERROR, expectedErrorCode);
    }
    cpptestSendStringArgument(CPPTEST_ARG_TESTCASE_NAME, name);
    cpptestSendEventPositionArgument(currentTestSuiteFile, file, line);
    cpptestSendLastArgument();
}

CPPTEST_EVENT_HANDLER
void CDECL_CALL cpptestDsTestCaseEnd(void)
{
    currentTestCaseName = "";
    currentDataSource = 0;
    cpptestSendChunk(CPPTEST_MSG_DS_TESTCASE_END, /*hasArguments=*/0);
}

CPPTEST_EVENT_HANDLER
void CDECL_CALL cpptestDsTestCaseIterationBegin(unsigned int iteration, const char* signature)
{
    CPPTEST_LOG("Data source test case iteration start");

    currentTestCaseIteration = iteration;
#if CPPTEST_USE_EXTENDED_DATA_SOURCE_ROW_SIGNATURE
    currentTestCaseParameters = signature;
#endif /* CPPTEST_USE_EXTENDED_DATA_SOURCE_ROW_SIGNATURE */

    cpptestSendChunk(CPPTEST_MSG_DS_ITERATION_BEGIN, /*hasArguments=*/1);
    cpptestSendUIntArgument(CPPTEST_ARG_TESTCASE_ITERATION_ID, currentTestCaseIteration);
#if CPPTEST_USE_EXTENDED_DATA_SOURCE_ROW_SIGNATURE
    cpptestSendStringArgument(CPPTEST_ARG_TESTCASE_PARAMETERS, currentTestCaseParameters);
#endif
    cpptestSendLastArgument();

    initializeRuntimeBeforeTestCaseExecution();
    cpptestExecuteRuntimeCommand(CPPTEST_CMD_INITIALIZE_TESTCASE);

    ++currentTestCaseId;
    unlockRuntime();
    runtimeStatus = INSIDE_TEST_CASE;

    cpptestExecuteRuntimeCommand(CPPTEST_CMD_TESTCASE_BEGIN);
}


CPPTEST_EVENT_HANDLER
void CDECL_CALL cpptestDsTestCaseIterationEnd(void)
{
    cpptestExecuteRuntimeCommand(CPPTEST_CMD_TESTCASE_END);
    runtimeStatus = OUTSIDE_TEST_CASE;
    lockRuntime();

    cpptestSendChunk(CPPTEST_MSG_DS_ITERATION_END, /*hasArguments=*/1);
    cpptestSendUIntArgument(CPPTEST_ARG_TESTCASE_RESULT, getTestCaseResult());
    cpptestSendLastArgument();

    cpptestExecuteRuntimeCommand(CPPTEST_CMD_FINALIZE_TESTCASE);
    finalizeRuntimeAfterTestCaseExecution();
#if CPPTEST_USE_EXTENDED_DATA_SOURCE_ROW_SIGNATURE
    currentTestCaseParameters = 0;
#endif /* CPPTEST_USE_EXTENDED_DATA_SOURCE_ROW_SIGNATURE */
    currentTestCaseIteration = 0;
}

CPPTEST_EVENT_HANDLER
void CDECL_CALL cpptestTestCaseDisabled(
        const char* name, const char* file, unsigned int line)
{   
    currentTestCaseName = name;
    currentTestCaseFile = file;
    currentTestCaseLine = line;

    cpptestSendChunk(CPPTEST_MSG_TESTCASE_DISABLED, /*hasArguments=*/1);
    cpptestSendStringArgument(CPPTEST_ARG_TESTCASE_NAME, name);
    cpptestSendEventPositionArgument(currentTestSuiteFile, file, line);
    cpptestSendLastArgument();

    currentTestCaseName = "";
    currentTestCaseFile = 0;
    currentTestCaseLine = 0U;
}

CPPTEST_STATE_INFO
const char* CDECL_CALL cpptestGetCurrentTestSuiteName(void)
{
    return currentTestSuiteName;
}

CPPTEST_STATE_INFO
const char* CDECL_CALL cpptestGetCurrentTestCaseName(void)
{
    return currentTestCaseName;
}

CPPTEST_STATE_INFO
unsigned int CDECL_CALL cpptestGetCurrentTestCaseIteration(void)
{
    return currentTestCaseIteration;
}

#if CPPTEST_USE_EXTENDED_DATA_SOURCE_ROW_SIGNATURE
CPPTEST_STATE_INFO
const char* CDECL_CALL cpptestGetCurrentTestCaseParameters(void)
{
    return currentTestCaseParameters;
}
#endif /* CPPTEST_USE_EXTENDED_DATA_SOURCE_ROW_SIGNATURE */

CPPTEST_STATE_INFO
const char* CDECL_CALL cpptestGetCurrentTestCaseFile(void)
{
    return currentTestCaseFile;
}

CPPTEST_STATE_INFO
unsigned int CDECL_CALL cpptestGetCurrentTestCaseLine(void)
{
    return currentTestCaseLine;
}

#if CPPTEST_SEH_EXCEPTIONS_ENABLED
#if defined(CPPTEST_WIN32)

CPPTEST_EVENT_HANDLER
void CDECL_CALL cpptestUnhandledStructuredExceptionInThread(unsigned int code)
{
    CppTest_ErrorCode cpptestCode =
            localGetHandledWin32ExceptionErrorCode(code);

    if (beginErrorProcessing(cpptestCode)) {       
        
        cpptestSendChunk(cpptestCode, /*hasArguments=*/0);        
        
        endErrorProcessing(/**errorCode=*/44, /*doJmp=*/0, /*doExit=*/1);
    }
}

CPPTEST_EVENT_HANDLER
int CDECL_CALL cpptestUnhandledStructuredExceptionInTestCase(unsigned int code)
{
    CppTest_ErrorCode cpptestCode =
            localGetHandledWin32ExceptionErrorCode(code);

    if (beginErrorProcessing(cpptestCode)) {
        
        cpptestSendChunk(cpptestCode, /*hasArguments=*/0);        
        
        endErrorProcessing(/**errorCode=*/44, /*doJmp=*/0, /*doExit=*/cpptestCode == CPPTEST_STACK_OVERFLOW);
    }
    /* Return value used in __except handler in cpptest_driver.c.h */
    return 1; 
}

#endif /* defined(CPPTEST_WIN32) */
#endif /* CPPTEST_SEH_EXCEPTIONS_ENABLED */

CPPTEST_EVENT_HANDLER
void CDECL_CALL cpptestUnhandledCppExceptionInThread(void)
{
    if (beginErrorProcessing(CPPTEST_CPP_EXCEPTION)) {
        
        cpptestSendChunk(CPPTEST_CPP_EXCEPTION, /*hasArguments=*/0);
        
        endErrorProcessing(/**errorCode=*/44, /*doJmp=*/0, /*doExit=*/1);
    }
}

CPPTEST_EVENT_HANDLER
void CDECL_CALL cpptestNoExpectedCppExceptionInTestCase(const char *e)
{
    if (beginErrorProcessing(CPPTEST_MISSING_CPP_EXCEPTION)) {

        cpptestSendChunk(CPPTEST_MISSING_CPP_EXCEPTION, /*hasArguments=*/1);
        cpptestSendStringArgument(CPPTEST_ARG_EXCEPTION_NAME, e);
        cpptestSendLastArgument();

        endErrorProcessing(/**errorCode=*/0, /*doJmp=*/0, /*doExit=*/0);
    }
}

CPPTEST_EVENT_HANDLER
void CDECL_CALL cpptestUnhandledStdExceptionInTestCase(const char *what)
{
    if (beginErrorProcessing(CPPTEST_STD_EXCEPTION)) {

        cpptestSendChunk(CPPTEST_STD_EXCEPTION, /*hasArguments=*/1);
        cpptestSendStringArgument(CPPTEST_ARG_EXCEPTION_WHAT, what);
        cpptestSendLastArgument();

        endErrorProcessing(/**errorCode=*/0, /*doJmp=*/0, /*doExit=*/0);
    }
}

CPPTEST_EVENT_HANDLER
void CDECL_CALL cpptestUnhandledCppExceptionInTestCase(void)
{
    if (beginErrorProcessing(CPPTEST_CPP_EXCEPTION)) {
        
        cpptestSendChunk(CPPTEST_CPP_EXCEPTION, /*hasArguments=*/0);

        endErrorProcessing(/**errorCode=*/0, /*doJmp=*/0, /*doExit=*/0);
    }
}

CPPTEST_EVENT_HANDLER
void cpptestViolation(int vid, const char* file, unsigned int line, int doBreak)
{
    if (beginErrorProcessing(vid)) {
        
        cpptestSendChunk(vid, /*hasArguments=*/1);
        cpptestSendEventPositionArgument(0, file, line);
        cpptestSendLastArgument();

        endErrorProcessing(/*exitCode=*/33, /*dojmp=*/doBreak, /*doExit=*/doBreak);
    }
}

CPPTEST_EVENT_HANDLER
void cpptestAccessViolation(int vid, const char* file, unsigned int line, 
        unsigned int aid, CPPTEST_INTEGER ptr_offset, CPPTEST_INTEGER offset, unsigned int elemSize, int doBreak)
{
    if (beginErrorProcessing(vid)) {
        cpptestSendChunk(vid, /*hasArguments=*/1);

        cpptestSendUIntArgumentWithNestedArgs(CPPTEST_ARG_MEMORY_ACCESS_INFO, aid);
        if (ptr_offset != 0) {
            cpptestSendIntArgument(CPPTEST_SUB_ARG_PTR_OFFSET, (int)ptr_offset);
        }
        if (offset != 0) {
            cpptestSendIntArgument(CPPTEST_SUB_ARG_OFFSET, (int)offset);
        }
        if (elemSize != 1U) {
            cpptestSendUIntArgument(CPPTEST_SUB_ARG_ELEMENT_SIZE, elemSize);
        }
        cpptestSendLastNestedArgument();

        cpptestSendEventPositionArgument(0, file, line);

        cpptestSendLastArgument();

        endErrorProcessing(/*exitCode=*/33, /*dojmp=*/doBreak, /*doExit=*/doBreak);
    }
}

CPPTEST_EVENT_HANDLER
void cpptestMemoryLeak(unsigned int aid)
{
    cpptestSendChunk(CPPTEST_MEMORY_LEAK_VIOLATION, /*hasArguments=*/1);
    cpptestSendUIntArgument(CPPTEST_ARG_ALLOCATION_ID, aid);
    cpptestSendLastArgument();
}

CPPTEST_EVENT_HANDLER
void cpptestMemoryAllocCall(const char* file, unsigned int line, unsigned int aid, 
        unsigned int elements, unsigned int elemSize)
{
    localSendCurrentThreadStackTrace();

    cpptestSendChunk(CPPTEST_MSG_MEMORY_ALLOC, /*hasArguments=*/1);
    
    cpptestSendUIntArgument(CPPTEST_ARG_ALLOCATION_ID, aid);
    if (elements != 1U) {
        cpptestSendUIntArgument(CPPTEST_ARG_NUMBER_OF_ELEMENTS, elements);
    }
    if (elemSize != 1U) {
        cpptestSendUIntArgument(CPPTEST_ARG_ELEMENT_SIZE, elemSize);
    }
    cpptestSendEventPositionArgument(0, file, line);

    cpptestSendLastArgument();
}

CPPTEST_EVENT_HANDLER
void cpptestMemoryReallocCall(const char* file, unsigned int line, unsigned int oldAid,
        unsigned int newAid, unsigned int elements, unsigned int elemSize)
{
    localSendCurrentThreadStackTrace();

    cpptestSendChunk(CPPTEST_MSG_MEMORY_REALLOC, /*hasArguments=*/1);

    cpptestSendUIntArgument(CPPTEST_ARG_ALLOCATION_ID, newAid);
    if (oldAid != newAid) {
        cpptestSendUIntArgument(CPPTEST_ARG_OLD_ALLOCATION_ID, oldAid);
    }
    if (elements != 1U) {
        cpptestSendUIntArgument(CPPTEST_ARG_NUMBER_OF_ELEMENTS, elements);
    }
    if (elemSize != 1U) {
        cpptestSendUIntArgument(CPPTEST_ARG_ELEMENT_SIZE, elemSize);
    }

    cpptestSendEventPositionArgument(0, file, line);

    cpptestSendLastArgument();
}

CPPTEST_EVENT_HANDLER
void cpptestMemoryFreeCall(const char* file, unsigned int line, unsigned int aid)
{
    localSendCurrentThreadStackTrace();

    cpptestSendChunk(CPPTEST_MSG_MEMORY_FREE, /*hasArguments=*/1);
    cpptestSendEventPositionArgument(0, file, line);
    cpptestSendUIntArgument(CPPTEST_ARG_ALLOCATION_ID, aid);
    cpptestSendLastArgument();
}

CPPTEST_EVENT_HANDLER
void cpptestMonitoredMemoryOut(unsigned int aid)
{
    cpptestSendChunk(CPPTEST_MSG_MEMORY_OUT, /*hasArguments=*/1);
    cpptestSendUIntArgument(CPPTEST_ARG_ALLOCATION_ID, aid);
    cpptestSendLastArgument();
}



CPPTEST_EVENT_HANDLER
int CDECL_CALL cpptestAssertion(int value, const char* userMessage, const char* conditionStr, 
        CppTest_StubCallInfo* sci, const char* file, unsigned int line)
{
    if ((value && _cpptestSendPassedAsserts) || (!value && beginErrorProcessing(CPPTEST_GENERAL_ASSERTION_FAILED))) {
        if (cpptestSendLimitedChunk(value ? CPPTEST_GENERAL_ASSERTION : CPPTEST_GENERAL_ASSERTION_FAILED, sci, file, line)) {
            if (!value) {
                cpptestSendStringArgument(CPPTEST_ARG_USER_MESSAGE, userMessage);
            }
            cpptestSendStringArgument(CPPTEST_ARG_CONDITION, conditionStr);
            cpptestSendLastArgument();
        }
        if (!value) {
            endErrorProcessing(/**errorCode=*/0, /*doJmp=*/0, /*doExit=*/0);
        }
    }
    return value;
}

int CDECL_CALL cpptestEqualAssertion(int value, const char* userMessage, const char* expectedStr, const char* actualStr, 
        CppTest_StubCallInfo* sci, const char* file, unsigned int line)
{
    if ((value && _cpptestSendPassedAsserts) || (!value && beginErrorProcessing(CPPTEST_GENERAL_EQUAL_ASSERTION_FAILED))) {
        if (cpptestSendLimitedChunk(value ? CPPTEST_GENERAL_EQUAL_ASSERTION : CPPTEST_GENERAL_EQUAL_ASSERTION_FAILED, sci, file, line)) {
            if (!value) {
                cpptestSendStringArgument(CPPTEST_ARG_USER_MESSAGE, userMessage);
            }
            cpptestSendStringArgument(CPPTEST_ARG_EXPECTED_STR, expectedStr);
            cpptestSendStringArgument(CPPTEST_ARG_ACTUAL_STR, actualStr);
            cpptestSendLastArgument();
        }
        if (!value) {
            endErrorProcessing(/**errorCode=*/0, /*doJmp=*/0, /*doExit=*/0);
        }
    }
    return value;
}

int CDECL_CALL cpptestBoolEqualAssertion(int value, int expected, int actual, const char* userMessage, const char* expectedStr, const char* actualStr,
        CppTest_StubCallInfo* sci, const char* file, unsigned int line)
{    
    if ((value && _cpptestSendPassedAsserts) || (!value && beginErrorProcessing(CPPTEST_BOOL_EQUAL_ASSERTION_FAILED))) {
        if (cpptestSendLimitedChunk(value ? CPPTEST_BOOL_EQUAL_ASSERTION : CPPTEST_BOOL_EQUAL_ASSERTION_FAILED, sci, file, line)) {
            if (!value) {
                cpptestSendStringArgument(CPPTEST_ARG_USER_MESSAGE, userMessage);
            }
            cpptestSendStringArgument(CPPTEST_ARG_EXPECTED_STR, expectedStr);
            cpptestSendStringArgument(CPPTEST_ARG_ACTUAL_STR, actualStr);
            cpptestSendBoolArgument(CPPTEST_ARG_EXPECTED, expected);
            cpptestSendBoolArgument(CPPTEST_ARG_ACTUAL, actual);
            cpptestSendLastArgument();
        }
        if (!value) {
            endErrorProcessing(/**errorCode=*/0, /*doJmp=*/0, /*doExit=*/0);
        }
    }
    return value;
}


int CDECL_CALL cpptestBitAssertion(int value, unsigned int mask, unsigned int expected, 
			unsigned int actual, const char* userMessage, const char* actualStr,
			CppTest_StubCallInfo* sci, const char* file, unsigned int line)
{
    if ((value && _cpptestSendPassedAsserts) || (!value && beginErrorProcessing(CPPTEST_BITS_ASSERTION_FAILED))) {
        if (cpptestSendLimitedChunk(value ? CPPTEST_BITS_ASSERTION : CPPTEST_BITS_ASSERTION_FAILED, sci, file, line)) {
            char expectedString[CPPTEST_INT_TO_STRING_BUFFER_SIZE] = {0};
            char actualString[CPPTEST_INT_TO_STRING_BUFFER_SIZE] = {0};

            cpptestIntToBitStrMask(expectedString, expected, mask);
            cpptestIntToBitStrMask(actualString, actual, mask);

            if (!value) {
                cpptestSendStringArgument(CPPTEST_ARG_USER_MESSAGE, userMessage);
            }
            cpptestSendStringArgument(CPPTEST_ARG_ACTUAL_STR, actualStr);
            cpptestSendStringArgument(CPPTEST_ARG_EXPECTED_MASKED_STR, expectedString);
            cpptestSendStringArgument(CPPTEST_ARG_ACTUAL_MASKED_STR, actualString);
            cpptestSendLastArgument();
        }
        if (!value) {
            endErrorProcessing(/**errorCode=*/0, /*doJmp=*/0, /*doExit=*/0);
        }
    }
    return value;
}

int CDECL_CALL cpptestIntegerEqualAssertion(int value, CPPTEST_INTEGER expected, CPPTEST_INTEGER actual,
        int hasDelta, CPPTEST_INTEGER delta, unsigned int* arrayIndex,
        const char* userMessage, const char* expectedStr, const char* actualStr, 
        CppTest_StubCallInfo* sci, const char* file, unsigned int line)
{
    if ((value && _cpptestSendPassedAsserts) || (!value && beginErrorProcessing(CPPTEST_INTEGER_EQUAL_ASSERTION_FAILED))) {
        if (cpptestSendLimitedChunk(value ? CPPTEST_INTEGER_EQUAL_ASSERTION : CPPTEST_INTEGER_EQUAL_ASSERTION_FAILED, sci, file, line)) {
            if (!value) {
                cpptestSendStringArgument(CPPTEST_ARG_USER_MESSAGE, userMessage);
            }
            cpptestSendStringArgument(CPPTEST_ARG_EXPECTED_STR, expectedStr);
            cpptestSendStringArgument(CPPTEST_ARG_ACTUAL_STR, actualStr);
            cpptestSendIntegerArgument(CPPTEST_ARG_EXPECTED, expected);
            cpptestSendIntegerArgument(CPPTEST_ARG_ACTUAL, actual);
            if (hasDelta) {
                cpptestSendIntegerArgument(CPPTEST_ARG_DELTA, delta);
            }
            if (arrayIndex) {
                cpptestSendUIntArgument(CPPTEST_ARG_ARRAY_INDEX, *arrayIndex);
            }
            cpptestSendLastArgument();
        }
        if (!value) {
           endErrorProcessing(/**errorCode=*/0, /*doJmp=*/0, /*doExit=*/0);
        }
    }
    return value;
}

int CDECL_CALL cpptestUIntegerEqualAssertion(int value, CPPTEST_UINTEGER expected, CPPTEST_UINTEGER actual,
        int hasDelta, CPPTEST_UINTEGER delta, unsigned int* arrayIndex,
        const char* userMessage, const char* expectedStr, const char* actualStr, 
        CppTest_StubCallInfo* sci, const char* file, unsigned int line)
{
    if ((value && _cpptestSendPassedAsserts) || (!value && beginErrorProcessing(CPPTEST_UINTEGER_EQUAL_ASSERTION_FAILED))) {
        if (cpptestSendLimitedChunk(value ? CPPTEST_UINTEGER_EQUAL_ASSERTION : CPPTEST_UINTEGER_EQUAL_ASSERTION_FAILED, sci, file, line)) {
            if (!value) {
                cpptestSendStringArgument(CPPTEST_ARG_USER_MESSAGE, userMessage);
            }
            cpptestSendStringArgument(CPPTEST_ARG_EXPECTED_STR, expectedStr);
            cpptestSendStringArgument(CPPTEST_ARG_ACTUAL_STR, actualStr);
            cpptestSendUIntegerArgument(CPPTEST_ARG_EXPECTED, expected);
            cpptestSendUIntegerArgument(CPPTEST_ARG_ACTUAL, actual);
            if (hasDelta) {
                cpptestSendUIntegerArgument(CPPTEST_ARG_DELTA, delta);
            }
            if (arrayIndex) {
                cpptestSendUIntArgument(CPPTEST_ARG_ARRAY_INDEX, *arrayIndex);
            }
            cpptestSendLastArgument();
        }
        if (!value) {
            endErrorProcessing(/**errorCode=*/0, /*doJmp=*/0, /*doExit=*/0);
        }
    }
    return value;
}

#if !CPPTEST_DISABLE_ALL_FLOATING_POINT
int CDECL_CALL cpptestFloatEqualAssertion(int value, CPPTEST_FLOAT expected, CPPTEST_FLOAT actual,
        int hasDelta, CPPTEST_FLOAT delta, unsigned int* arrayIndex,
        const char* userMessage, const char* expectedStr, const char* actualStr, 
        CppTest_StubCallInfo* sci, const char* file, unsigned int line)
{
    if ((value && _cpptestSendPassedAsserts) || (!value && beginErrorProcessing(CPPTEST_FLOAT_EQUAL_ASSERTION_FAILED))) {
        if (cpptestSendLimitedChunk(value ? CPPTEST_FLOAT_EQUAL_ASSERTION : CPPTEST_FLOAT_EQUAL_ASSERTION_FAILED, sci, file, line)) {
            if (!value) {
                cpptestSendStringArgument(CPPTEST_ARG_USER_MESSAGE, userMessage);
            }
            cpptestSendStringArgument(CPPTEST_ARG_EXPECTED_STR, expectedStr);
            cpptestSendStringArgument(CPPTEST_ARG_ACTUAL_STR, actualStr);
            cpptestSendFloatArgument(CPPTEST_ARG_EXPECTED, expected);
            cpptestSendFloatArgument(CPPTEST_ARG_ACTUAL, actual);
            if (hasDelta) {
                cpptestSendFloatArgument(CPPTEST_ARG_DELTA, delta);
            }
            if (arrayIndex) {
                cpptestSendUIntArgument(CPPTEST_ARG_ARRAY_INDEX, *arrayIndex);
            }
            cpptestSendLastArgument();
        }
        if (!value) {
           endErrorProcessing(/**errorCode=*/0, /*doJmp=*/0, /*doExit=*/0);
        }
    }
    return value;
}

int CDECL_CALL cpptestFloatFmtEqualAssertion(int value, CPPTEST_FLOAT actual, const char* userMessage, 
        const char* expectedStr, const char* actualStr, 
        CppTest_StubCallInfo* sci, const char* file, unsigned int line)
{
    if ((value && _cpptestSendPassedAsserts) || (!value && beginErrorProcessing(CPPTEST_FLOAT_FMT_EQUAL_ASSERTION_FAILED))) {
        if (cpptestSendLimitedChunk(value ? CPPTEST_FLOAT_FMT_EQUAL_ASSERTION : CPPTEST_FLOAT_FMT_EQUAL_ASSERTION_FAILED, sci, file, line)) {
            if (!value) {
                cpptestSendStringArgument(CPPTEST_ARG_USER_MESSAGE, userMessage);
            }
            cpptestSendStringArgument(CPPTEST_ARG_EXPECTED_STR, expectedStr);
            cpptestSendStringArgument(CPPTEST_ARG_ACTUAL_STR, actualStr);
            cpptestSendStringArgument(CPPTEST_ARG_EXPECTED,  expectedStr);
            cpptestSendFloatArgument(CPPTEST_ARG_ACTUAL, actual);
            cpptestSendLastArgument();
        }
        if (!value) {
            endErrorProcessing(/**errorCode=*/0, /*doJmp=*/0, /*doExit=*/0);
        }
    }
    return value;
}
#endif

int CDECL_CALL cpptestCstrEqualAssertion(int value, const char* expected, const char* actual, unsigned int* arrayIndex,
        const char* userMessage, const char* expectedStr, const char* actualStr, 
        CppTest_StubCallInfo* sci, const char* file, unsigned int line)
{
    if ((value && _cpptestSendPassedAsserts) || (!value && beginErrorProcessing(CPPTEST_CSTR_EQUAL_ASSERTION_FAILED))) {
        if (cpptestSendLimitedChunk(value ? CPPTEST_CSTR_EQUAL_ASSERTION : CPPTEST_CSTR_EQUAL_ASSERTION_FAILED, sci, file, line)) {
            if (!value) {
                cpptestSendStringArgument(CPPTEST_ARG_USER_MESSAGE, userMessage);
            }
            cpptestSendStringArgument(CPPTEST_ARG_EXPECTED_STR, expectedStr);
            cpptestSendStringArgument(CPPTEST_ARG_ACTUAL_STR, actualStr);
            cpptestSendCStringArgument(CPPTEST_ARG_EXPECTED, expected);
            cpptestSendCStringArgument(CPPTEST_ARG_ACTUAL, actual);
            if (arrayIndex) {
                cpptestSendUIntArgument(CPPTEST_ARG_ARRAY_INDEX, *arrayIndex);
            }
            cpptestSendLastArgument();
        }
        if (!value) {
           endErrorProcessing(/**errorCode=*/0, /*doJmp=*/0, /*doExit=*/0);
        }
    }
    return value;
}

int CDECL_CALL cpptestCstrNEqualAssertion(int value, const char* expected, const char* actual, unsigned int maxSize, const char* userMessage, 
        const char* expectedStr, const char* actualStr, CppTest_StubCallInfo* sci, const char* file, unsigned int line)
{
    if ((value && _cpptestSendPassedAsserts) || (!value && beginErrorProcessing(CPPTEST_CSTRN_EQUAL_ASSERTION_FAILED))) {
        if (cpptestSendLimitedChunk(value ? CPPTEST_CSTRN_EQUAL_ASSERTION : CPPTEST_CSTRN_EQUAL_ASSERTION_FAILED, sci, file, line)) {
            if (!value) {
                cpptestSendStringArgument(CPPTEST_ARG_USER_MESSAGE, userMessage);
            }
            cpptestSendStringArgument(CPPTEST_ARG_EXPECTED_STR, expectedStr);
            cpptestSendStringArgument(CPPTEST_ARG_ACTUAL_STR, actualStr);
            cpptestSendCStringMaxArgument(CPPTEST_ARG_EXPECTED, expected, maxSize);
            cpptestSendCStringMaxArgument(CPPTEST_ARG_ACTUAL, actual, maxSize);
            cpptestSendLastArgument();
        }
        if (!value) {
            endErrorProcessing(/**errorCode=*/0, /*doJmp=*/0, /*doExit=*/0);
        }
    }
    return value;
}

int CDECL_CALL cpptestMemBufferEqualAssertion(int value, const void* expected, const void* actual, unsigned int size, const char* userMessage, 
        const char* expectedStr, const char* actualStr, CppTest_StubCallInfo* sci, const char* file, unsigned int line)
{
    if ((value && _cpptestSendPassedAsserts) || (!value && beginErrorProcessing(CPPTEST_MEM_EQUAL_ASSERTION_FAILED))) {
        if (cpptestSendLimitedChunk(value ? CPPTEST_MEM_EQUAL_ASSERTION : CPPTEST_MEM_EQUAL_ASSERTION_FAILED, sci, file, line)) {
            if (size > CPPTEST_MAX_MEM_EQUAL_ASSERTION_SIZE_ALLOWED) {
                unsigned int differenceIndex = 0U;
                unsigned int startIndex = 0U;
                const char* expectedC = (const char*)expected;
                const char* actualC = (const char*)actual;

                while (differenceIndex < size && *expectedC == *actualC) {
                    ++differenceIndex;
                    ++expectedC;
                    ++actualC;
                }
                if (differenceIndex > CPPTEST_MAX_MEM_EQUAL_ASSERTION_SIZE_ALLOWED/2U) {
                    startIndex = differenceIndex - CPPTEST_MAX_MEM_EQUAL_ASSERTION_SIZE_ALLOWED/2U;
                }
                if (startIndex > size - CPPTEST_MAX_MEM_EQUAL_ASSERTION_SIZE_ALLOWED) {
                    startIndex = size - CPPTEST_MAX_MEM_EQUAL_ASSERTION_SIZE_ALLOWED;
                }
                if ((startIndex & 3U) != 0U) {
                    startIndex &= ~3U;
                }
                expected = (const char*)expected + startIndex;
                actual = (const char*)actual + startIndex;
                size = CPPTEST_MAX_MEM_EQUAL_ASSERTION_SIZE_ALLOWED;
                cpptestSendUIntArgument(CPPTEST_ARG_MEM_BUFF_OFFSET, startIndex);
            }

            if (!value) {
                cpptestSendStringArgument(CPPTEST_ARG_USER_MESSAGE, userMessage);
            }
            cpptestSendStringArgument(CPPTEST_ARG_EXPECTED_STR, expectedStr);
            cpptestSendStringArgument(CPPTEST_ARG_ACTUAL_STR, actualStr);
            cpptestSendDataArgument(CPPTEST_ARG_EXPECTED, expected, size);
            cpptestSendDataArgument(CPPTEST_ARG_ACTUAL, actual, size);
            cpptestSendLastArgument();
        }
        if (!value) {
           endErrorProcessing(/**errorCode=*/0, /*doJmp=*/0, /*doExit=*/0);
        }
    }

    return value;
}

int CDECL_CALL cpptestPtrEqualAssertion(int value, const void* expected, const void* actual, unsigned int* arrayIndex,
        const char* userMessage, const char* expectedStr, const char* actualStr, 
        CppTest_StubCallInfo* sci, const char* file, unsigned int line)
{
    if ((value && _cpptestSendPassedAsserts) || (!value && beginErrorProcessing(CPPTEST_PTR_EQUAL_ASSERTION_FAILED))) {
        if (cpptestSendLimitedChunk(value ? CPPTEST_PTR_EQUAL_ASSERTION : CPPTEST_PTR_EQUAL_ASSERTION_FAILED, sci, file, line)) {
            if (!value) {
                cpptestSendStringArgument(CPPTEST_ARG_USER_MESSAGE, userMessage);
            }
            cpptestSendStringArgument(CPPTEST_ARG_EXPECTED_STR, expectedStr);
            cpptestSendStringArgument(CPPTEST_ARG_ACTUAL_STR, actualStr);
            cpptestSendPointerArgument(CPPTEST_ARG_EXPECTED, expected);
            cpptestSendPointerArgument(CPPTEST_ARG_ACTUAL, actual);
            if (arrayIndex) {
                cpptestSendUIntArgument(CPPTEST_ARG_ARRAY_INDEX, *arrayIndex);
            }
            cpptestSendLastArgument();
        }
        if (!value) {
            endErrorProcessing(/**errorCode=*/0, /*doJmp=*/0, /*doExit=*/0);
        }
    }
    return value;
}


#if CPPTEST_WCHAR_ENABLED
int CDECL_CALL cpptestWCstrEqualAssertion(int value, const wchar_t* expected, const wchar_t* actual, unsigned int* arrayIndex,
        const char* userMessage, const char* expectedStr, const char* actualStr, 
        CppTest_StubCallInfo* sci, const char* file, unsigned int line)
{
    if ((value && _cpptestSendPassedAsserts) || (!value && beginErrorProcessing(CPPTEST_WCSTR_EQUAL_ASSERTION_FAILED))) {
        if (cpptestSendLimitedChunk(value ? CPPTEST_WCSTR_EQUAL_ASSERTION : CPPTEST_WCSTR_EQUAL_ASSERTION_FAILED, sci, file, line)) {
            if (!value) {
                cpptestSendStringArgument(CPPTEST_ARG_USER_MESSAGE, userMessage);
            }
            cpptestSendStringArgument(CPPTEST_ARG_EXPECTED_STR, expectedStr);
            cpptestSendStringArgument(CPPTEST_ARG_ACTUAL_STR, actualStr);
            if (arrayIndex) {
                cpptestSendUIntArgument(CPPTEST_ARG_ARRAY_INDEX, *arrayIndex);
            }
            cpptestSendLastArgument();
        }
        if (!value) {
            endErrorProcessing(/**errorCode=*/0, /*doJmp=*/0, /*doExit=*/0);
        }
    }
    return value;
}
#endif

int CDECL_CALL cpptestEnumEqualAssertion(int value, CppTestEnumInfo* enumInfo, CPPTEST_INTEGER expected, CPPTEST_INTEGER actual,
        const char* userMessage, const char* expectedStr, const char* actualStr, 
        CppTest_StubCallInfo* sci, int isCpp, const char* file, unsigned int line)
{
    if (enumInfo != 0) {
        const char* expectedEnum = cpptestGetEnumeratorStrFromTable(enumInfo, expected, isCpp, file, line);
        const char* actualEnum = cpptestGetEnumeratorStrFromTable(enumInfo, actual, isCpp, file, line); 
        if ((value && _cpptestSendPassedAsserts) || (!value && beginErrorProcessing(CPPTEST_ENUM_EQUAL_ASSERTION_FAILED))) {
            if (cpptestSendLimitedChunk(value ? CPPTEST_ENUM_EQUAL_ASSERTION : CPPTEST_ENUM_EQUAL_ASSERTION_FAILED, sci, file, line)) {            
                if (!value) {
                    cpptestSendStringArgument(CPPTEST_ARG_USER_MESSAGE, userMessage);
                }
                cpptestSendStringArgument(CPPTEST_ARG_EXPECTED_STR, expectedStr);
                cpptestSendStringArgument(CPPTEST_ARG_ACTUAL_STR, actualStr);
                cpptestSendStringArgument(CPPTEST_ARG_EXPECTED, expectedEnum);
                cpptestSendStringArgument(CPPTEST_ARG_ACTUAL, actualEnum);
                if (isCpp) {
                    cpptestSendStringArgument(CPPTEST_ARG_ENUM_SCOPE, enumInfo->scope);
                }
                if (enumInfo->scoped) {
                    cpptestSendStringArgument(CPPTEST_ARG_ENUM_NAME, enumInfo->name);
                    cpptestSendBoolArgument(CPPTEST_ARG_ENUM_SCOPED, enumInfo->scoped);
                }
                cpptestSendLastArgument();
            }
            if (!value) {
                endErrorProcessing(/**errorCode=*/0, /*doJmp=*/0, /*doExit=*/0);
            }
        }
    }
    return value;
}

static void checkSnprintfResult(int result, char* buffer, int size, char* type, 
        CppTest_StubCallInfo* sci, const char* file, unsigned int line)
{
    if (result >= size) {
        if (beginErrorProcessing(CPPTEST_SNPRINTF_BUFFER_TOO_SMALL)) {
            if (cpptestSendLimitedChunk(CPPTEST_SNPRINTF_BUFFER_TOO_SMALL, sci, file, line)) {
                cpptestSendIntArgument(CPPTEST_ARG_BUFFER_SIZE, size);
                cpptestSendIntArgument(CPPTEST_ARG_REQUIRD_BUFFER_SIZE, result + 1);
                cpptestSendStringArgument(CPPTEST_ARG_BUFFER_TYPE, type);
                cpptestSendStringArgument(CPPTEST_ARG_BUFFER, buffer);
                cpptestSendLastArgument();
            }
            endErrorProcessing(/**errorCode=*/0, /*doJmp=*/0, /*doExit=*/0);
        }
    }
}

int CDECL_CALL cpptestEqualAssertionFmt(int value, int hasDelta, unsigned int* arrayIndex,
        const char* expectedStr, const char* actualStr, 
        char* expectedFmt, char* actualFmt, char* deltaFmt,
        int size, int* snprintfResults, CppTest_StubCallInfo* sci, const char* file, unsigned int line)
{
    checkSnprintfResult(snprintfResults[0], expectedFmt, size, "Expected", sci, file, line);
    checkSnprintfResult(snprintfResults[1], actualFmt, size, "Actual", sci, file, line);
    if (hasDelta) {
        checkSnprintfResult(snprintfResults[2], deltaFmt, size, "Delta", sci, file, line);
    }
    if ((value && _cpptestSendPassedAsserts) || (!value && beginErrorProcessing(CPPTEST_INTEGER_EQUAL_ASSERTION_FAILED))) {
        if (cpptestSendLimitedChunk(value ? CPPTEST_INTEGER_EQUAL_ASSERTION : CPPTEST_INTEGER_EQUAL_ASSERTION_FAILED, sci, file, line)) {
            cpptestSendStringArgument(CPPTEST_ARG_EXPECTED_STR, expectedStr);
            cpptestSendStringArgument(CPPTEST_ARG_ACTUAL_STR, actualStr);
            cpptestSendStringArgument(CPPTEST_ARG_EXPECTED, expectedFmt);
            cpptestSendStringArgument(CPPTEST_ARG_ACTUAL, actualFmt);
            if (hasDelta) {
                cpptestSendStringArgument(CPPTEST_ARG_DELTA, deltaFmt);
            }
            if (arrayIndex) {
                cpptestSendUIntArgument(CPPTEST_ARG_ARRAY_INDEX, *arrayIndex);
            }
            cpptestSendLastArgument();
        }
        if (!value) {
           endErrorProcessing(/**errorCode=*/0, /*doJmp=*/0, /*doExit=*/0);
        }
    }
    return value;
}


void CDECL_CALL cpptestThrowAssertion(int value, const char* userMessage, const char* exceptionTypeStr, 
        CppTest_StubCallInfo* sci, const char* file, unsigned int line)
{
    if ((value && _cpptestSendPassedAsserts) || (!value && beginErrorProcessing(CPPTEST_THROW_ASSERTION_FAILED))) {
        if (cpptestSendLimitedChunk(value ? CPPTEST_THROW_ASSERTION : CPPTEST_THROW_ASSERTION_FAILED, sci, file, line)) {
            if (!value) {
                cpptestSendStringArgument(CPPTEST_ARG_USER_MESSAGE, userMessage);
            }
            cpptestSendStringArgument(CPPTEST_ARG_EXCEPTION_NAME, exceptionTypeStr);
            cpptestSendLastArgument();
        }
        if (!value) {
            endErrorProcessing(/**errorCode=*/0, /*doJmp=*/0, /*doExit=*/0);
        }
    }
}

void CDECL_CALL cpptestNoThrowAssertion(int value, const char* userMessage, 
        CppTest_StubCallInfo* sci, const char* file, unsigned int line)
{
    if ((value && _cpptestSendPassedAsserts) || (!value && beginErrorProcessing(CPPTEST_NOTHROW_ASSERTION_FAILED))) {
        if (cpptestSendLimitedChunk(value ? CPPTEST_NOTHROW_ASSERTION : CPPTEST_NOTHROW_ASSERTION_FAILED, sci, file, line)) {
            cpptestSendStringArgument(CPPTEST_ARG_USER_MESSAGE, userMessage);
            cpptestSendLastArgument();
        }
        if (!value) {
           endErrorProcessing(/**errorCode=*/0, /*doJmp=*/0, /*doExit=*/0);
        }
    }
}

int cpptestMemoryLeakAssertionFailed(const char* file, unsigned int line)
{
    if (beginErrorProcessing(CPPTEST_MEMORY_LEAKS_ASSERTION_FAILED)) {
        if (cpptestSendLimitedChunk(CPPTEST_MEMORY_LEAKS_ASSERTION_FAILED, 0, file, line)) {
            cpptestSendLastArgument();
        }
        endErrorProcessing(/**errorCode=*/0, /*doJmp=*/0, /*doExit=*/0);
    }
    return 0;
}

#if CPPTEST_TRIGGER_ENABLED
CPPTEST_EVENT_HANDLER
void CDECL_CALL cpptestTriggerAssertion(const char* triggerName, const char* userMessage, const char* file, unsigned int line)
{
    if (beginErrorProcessing(CPPTEST_GENERAL_ASSERTION_FAILED)) {
        if (cpptestSendLimitedChunk(CPPTEST_GENERAL_ASSERTION_FAILED, 0, file, line)) {
            cpptestSendStringArgument(CPPTEST_ARG_TRIGGER_NAME, triggerName);
            cpptestSendStringArgument(CPPTEST_ARG_USER_MESSAGE, userMessage);
            cpptestSendLastArgument();
        }
        endErrorProcessing(/**errorCode=*/0, /*doJmp=*/0, /*doExit=*/0);
    }
}

CPPTEST_RUNTIME_API void CDECL_CALL cpptestCallTriggerCallback(const char* triggerName)
{
}

void CDECL_CALL cpptestValidateTriggerName(const char* triggerName, const char* file, unsigned int line)
{
    (void)cpptestGetStubInfo(triggerName, CPPTEST_GET_STUB_INFO_CONTEXT_TRIGGER, file, line);
}

#endif /* CPPTEST_TRIGGER_ENABLED */

CPPTEST_EVENT_HANDLER
void CDECL_CALL cpptestCallExpectationAssertion(CppTestExpectation* expectation)
{
    if ((expectation->passed && _cpptestSendPassedAsserts) || 
        (!expectation->passed && beginErrorProcessing(CPPTEST_CALL_EXPECTATION_FAILED)))
    {
        if (cpptestSendLimitedChunk(expectation->passed ? CPPTEST_CALL_EXPECTATION : CPPTEST_CALL_EXPECTATION_FAILED, 
                    0, expectation->file, expectation->line))
        {
            cpptestSendStringArgument(CPPTEST_ARG_FUNC_ID, expectation->variant.expectedCalls.stubInfo->userId);
            cpptestSendIntArgument(CPPTEST_ARG_MIN_CALLS, expectation->variant.expectedCalls.min);
            cpptestSendIntArgument(CPPTEST_ARG_MAX_CALLS, expectation->variant.expectedCalls.max);
            cpptestSendIntArgument(CPPTEST_ARG_CALLS, expectation->variant.expectedCalls.calls);
            cpptestSendLastArgument();
        }
        if (!expectation->passed) {
           endErrorProcessing(/**errorCode=*/0, /*doJmp=*/0, /*doExit=*/0);
        }
    }
}


CPPTEST_EVENT_HANDLER
void CDECL_CALL cpptestNullCallSequenceExpectationAssertion(CppTestExpectation* expectation, const char* funcId)
{
    if (beginErrorProcessing(CPPTEST_NULL_CALL_SEQUENCE_EXPECTATION_FAILED)) {
        if (cpptestSendLimitedChunk(CPPTEST_NULL_CALL_SEQUENCE_EXPECTATION_FAILED, 0, expectation->file, expectation->line)) {
            cpptestSendStringArgument(CPPTEST_ARG_FUNC_ID, funcId);
            cpptestSendLastArgument();
        }
        endErrorProcessing(/**errorCode=*/0, /*doJmp=*/0, /*doExit=*/0);
    }
}

CPPTEST_EVENT_HANDLER
void CDECL_CALL cpptestWrongCallSequenceExpectationAssertion(CppTestExpectation* expectation, const char* funcId, const char* expectedFuncId)
{
    if (beginErrorProcessing(CPPTEST_WRONG_CALL_SEQUENCE_EXPECTATION_FAILED)) {
        if (cpptestSendLimitedChunk(CPPTEST_WRONG_CALL_SEQUENCE_EXPECTATION_FAILED, 0, expectation->file, expectation->line)) {
            cpptestSendStringArgument(CPPTEST_ARG_FUNC_ID, funcId);
            cpptestSendStringArgument(CPPTEST_ARG_EXPECTED_FUNC_ID, expectedFuncId);
            cpptestSendLastArgument();
        }
        endErrorProcessing(/**errorCode=*/0, /*doJmp=*/0, /*doExit=*/0);
    }
}

CPPTEST_EVENT_HANDLER
void CDECL_CALL cpptestIncompleteCallSequenceExpectationAssertion(CppTestExpectation* expectation)
{
    if (beginErrorProcessing(CPPTEST_INCOMPLETE_CALL_SEQUENCE_EXPECTATION_FAILED)) {
        if (cpptestSendLimitedChunk(CPPTEST_INCOMPLETE_CALL_SEQUENCE_EXPECTATION_FAILED, 0, expectation->file, expectation->line)) {
            cpptestSendStringArgument(CPPTEST_ARG_FUNC_ID, expectation->variant.sequence.it->stubInfo->userId);
            cpptestSendLastArgument();
        }
        endErrorProcessing(/**errorCode=*/0, /*doJmp=*/0, /*doExit=*/0);
    }
}

CPPTEST_EVENT_HANDLER
void CDECL_CALL cpptestCallSequenceExpectationPassed(CppTestExpectation* expectation)
{
    if (expectation->passed && _cpptestSendPassedAsserts) {
        if (cpptestSendLimitedChunk(CPPTEST_CALL_SEQUENCE_EXPECTATION, 0, expectation->file, expectation->line)) {
            CppTestCallSequenceNode* it = expectation->variant.sequence.first;
            for (; it != 0; it = it->next) {
                cpptestSendArgumentWithNestedArgs(CPPTEST_ARG_CALL_SEQUENCE_ELEMENT);
                cpptestSendStringArgument(CPPTEST_SUB_ARG_FUNC_ID, it->stubInfo->userId);
                if (it->times != 1U) {
                    cpptestSendIntArgument(CPPTEST_SUB_ARG_FUNC_TIMES, it->times);
                }
                cpptestSendLastNestedArgument();
            }
            cpptestSendLastArgument();
        }
    }
}

CPPTEST_EVENT_HANDLER
void CDECL_CALL cpptestExternalProcessError(CppTest_ErrorCode errorCode, const char * err)
{
    if (beginErrorProcessing(errorCode)) {
        if (cpptestSendLimitedChunk(errorCode, 0, 0, 0U)) {
            cpptestSendStringArgument(CPPTEST_ARG_STRERROR, err);
            cpptestSendLastArgument();
        }
        endErrorProcessing(/**errorCode=*/0, /*doJmp=*/0, /*doExit=*/0);
    }
}

CPPTEST_EVENT_HANDLER
void CDECL_CALL cpptestExternalProcessErrorFull(CppTest_ErrorCode errorCode, const char * err, 
        const char* program, const char* param1, const char* param2, const char* param3)
{
    if (beginErrorProcessing(errorCode)) {
        if (cpptestSendLimitedChunk(errorCode, 0, 0, 0U)) {
            cpptestSendStringArgument(CPPTEST_ARG_STRERROR, err);
            cpptestSendStringArgument(CPPTEST_ARG_EXECUTABLE, program);
            cpptestSendLastArgument();
        }
        endErrorProcessing(/**errorCode=*/0, /*doJmp=*/0, /*doExit=*/0);
    }
}




struct CppTest_DataSource* CDECL_CALL cpptestGetCurrentDataSource()
{
    return currentDataSource;
}

void* CDECL_CALL cpptestDsMalloc(size_t size)
{
    return localInternalMalloc(size);
}

void CDECL_CALL cpptestDsFree(void* str)
{
    localInternalFree(str);
}


CPPTEST_EVENT_HANDLER
void CDECL_CALL cpptestDsCsvError(CppTest_ErrorCode errorCode, const char * err, 
        const char * file, unsigned int line, unsigned int column)
{
    cpptestSendChunk(errorCode, /*hasArguments=*/1);
    cpptestSendStringArgument(CPPTEST_ARG_STRERROR, err);
    cpptestDsPositionArgument(file, line, column);
    cpptestSendLastArgument();

    ++errors;
}

CPPTEST_EVENT_HANDLER
void CDECL_CALL cpptestDsCsvErrorOpeningFile(const char * err, const char * file)
{
    cpptestSendChunk(CPPTEST_DS_CSV_ERROR_OPENING_FILE, /*hasArguments=*/1);
    cpptestSendStringArgument(CPPTEST_ARG_STRERROR, err);
    cpptestSendStringArgument(CPPTEST_ARG_DS_FILE, file);
    cpptestSendLastArgument();

    ++errors;
}



CPPTEST_EVENT_HANDLER
void CDECL_CALL cpptestNoDsError(const char* dataSourceName)
{
    cpptestSendChunk(CPPTEST_DS_CSV_NO_DATA_SOURCE, /*hasArguments=*/1);
    cpptestSendStringArgument(CPPTEST_ARG_DS_NAME, dataSourceName);
    cpptestSendLastArgument();

    ++errors;
}

CPPTEST_EVENT_HANDLER
void CDECL_CALL cpptestDsErrorInValue(const char* value)
{
    if (beginErrorProcessing(CPPTEST_DS_ERROR_IN_VALUE)) {
        cpptestSendChunk(CPPTEST_DS_ERROR_IN_VALUE, /*hasArguments=*/1);
        cpptestSendStringArgument(CPPTEST_ARG_DS_VALUE, value ? value : "(null)");
        cpptestSendLastArgument();
    
        endErrorProcessing(/*exitCode=*/33, /*dojmp=*/1, /*doExit=*/1);
    }
}

CPPTEST_EVENT_HANDLER
void CDECL_CALL cpptestDsUnknownValue(const char* name)
{
    if (beginErrorProcessing(CPPTEST_DS_UNKNOWN_VALUE)) {
        cpptestSendChunk(CPPTEST_DS_UNKNOWN_VALUE, /*hasArguments=*/1);
        cpptestSendStringArgument(CPPTEST_ARG_DS_COLUMN_NAME, name ? name : "(null)");
        cpptestSendLastArgument();

        endErrorProcessing(/*exitCode=*/33, /*dojmp=*/1, /*doExit=*/1);
    }
}

CPPTEST_EVENT_HANDLER
int CDECL_CALL cpptestDsNotInitialized(void)
{
    if (beginErrorProcessing(CPPTEST_DS_NOT_INITIALIZED)) {
        
        cpptestSendChunk(CPPTEST_DS_NOT_INITIALIZED, /*hasArguments=*/0);

        endErrorProcessing(/*exitCode=*/33, /*dojmp=*/1, /*doExit=*/1);
    }
    return 0;
}

CPPTEST_EVENT_HANDLER
int CDECL_CALL cpptestDsMethodNotInitialized(const char* method)
{
    if (beginErrorProcessing(CPPTEST_DS_METHOD_NOT_INITIALIZED)) {
        cpptestSendChunk(CPPTEST_DS_METHOD_NOT_INITIALIZED, /*hasArguments=*/1);
        cpptestSendStringArgument(CPPTEST_ARG_DS_METHOD_NAME, method);
        cpptestSendLastArgument();

        endErrorProcessing(/*exitCode=*/33, /*dojmp=*/1, /*doExit=*/1);
    }
    return 0;
}


CPPTEST_EVENT_HANDLER
void CDECL_CALL cpptestAssertCall(const char *message, const char *file, unsigned line)
{   
    if (beginErrorProcessing(CPPTEST_ASSERT_CALLED)) {
        cpptestSendChunk(CPPTEST_ASSERT_CALLED, /*hasArguments=*/1);
        cpptestSendEventPositionArgument(0, file, line);
        cpptestSendStringArgument(CPPTEST_ARG_ASSERT_MESSAGE, message);
        cpptestSendLastArgument();
        
        endErrorProcessing(/*exitCode=*/33, /*dojmp=*/1, /*doExit=*/1);
    }
}

CPPTEST_EVENT_HANDLER
void CDECL_CALL cpptestExitCall(int exitCode)
{
    
    if (beginErrorProcessing(CPPTEST_EXIT_CALLED)) {
        cpptestSendChunk(CPPTEST_EXIT_CALLED, /*hasArguments=*/1);
        cpptestSendIntArgument(CPPTEST_ARG_EXIT_CODE, exitCode);
        cpptestSendLastArgument();

        endErrorProcessing(/*exitCode=*/33, /*dojmp=*/1, /*doExit=*/1);
    }
}

CPPTEST_EVENT_HANDLER
void CDECL_CALL cpptestAbortCall(void)
{
    if (beginErrorProcessing(CPPTEST_ABORT_CALLED)) {
        
        cpptestSendChunk(CPPTEST_ABORT_CALLED, /*hasArguments=*/0);

        endErrorProcessing(/*exitCode=*/33, /*dojmp=*/1, /*doExit=*/1);
    }
}

CPPTEST_EVENT_HANDLER
void CDECL_CALL cpptestRiseCall(int exitCode)
{
    if (beginErrorProcessing(CPPTEST_RAISE_CALLED)) {
        cpptestSendChunk(CPPTEST_RAISE_CALLED, /*hasArguments=*/1);
        cpptestSendIntArgument(CPPTEST_ARG_EXIT_CODE, exitCode);
        cpptestSendLastArgument();

        endErrorProcessing(/*exitCode=*/33, /*dojmp=*/1, /*doExit=*/1);
    }
}

#if CPPTEST_WCHAR_ENABLED && defined(_MSC_VER)
CPPTEST_EVENT_HANDLER
void CDECL_CALL cpptestInvalidParamCalled(const wchar_t * expression, const wchar_t * function, const wchar_t * file, unsigned int line)
{
    /* TODO: send wchar_t strings */
    if (beginErrorProcessing(CPPTEST_INVALID_PARAM_CALLED)) {
        
        cpptestSendChunk(CPPTEST_INVALID_PARAM_CALLED, /*hasArguments=*/0);

        endErrorProcessing(/**errorCode=*/33, /*doJmp=*/1, /*doExit=*/1);
    }
}
#endif

CPPTEST_EVENT_HANDLER
void CDECL_CALL cpptestTooLargeStackTrace(unsigned int maxAllowedStackTraceDepth)
{
    if (beginErrorProcessing(CPPTEST_TOO_LARGE_STACK_TRACE)) {

        cpptestSendChunk(CPPTEST_TOO_LARGE_STACK_TRACE, /*hasArguments=*/1);
        cpptestSendUIntArgument(CPPTEST_ARG_MAX_STACK_TRACE_DEPTH, maxAllowedStackTraceDepth);
        cpptestSendLastArgument();

        endErrorProcessing(/*exitCode=*/39, /*dojmp=*/1, /*doExit=*/1);
    }
}

CPPTEST_EVENT_HANDLER
void CDECL_CALL cpptestTestCaseNotValidated(const char *file, unsigned line)
{   
    if (cpptestSendLimitedChunk(CPPTEST_MSG_NOT_VALIDATED, 0, file, line)) {
        cpptestSendLastArgument();
    }
}

#if CPPTEST_TRIGGER_ENABLED
CPPTEST_EVENT_HANDLER
void CDECL_CALL cpptestTriggerError(const char *triggerName, unsigned int errorCode, const void* details, const char* file, unsigned int line)
{
    errorCode += CPPTEST_TRIGGER_ERROR_ID_START;
    if (beginErrorProcessing(errorCode)) {
        cpptestSendChunk(errorCode, /*hasArguments=*/1);
        cpptestSendEventPositionArgument(0, file, line);
        if (triggerName && *triggerName) {
            cpptestSendStringArgument(CPPTEST_ARG_TRIGGER_NAME, triggerName);
        }
        cpptestSendLastArgument();

        endErrorProcessing(/*exitCode=*/33, /*dojmp=*/1, /*doExit=*/1);
    }
}
#endif /* CPPTEST_TRIGGER_ENABLED */

CPPTEST_EVENT_HANDLER
void CDECL_CALL cpptestReport(const char* txt, CppTest_StubCallInfo* sci, const char *file, unsigned line, int reportStackTrace)
{
    if (_cpptestSendReports == 0) {
        return;
    }

    if (reportStackTrace) {
        localSendAllStackTraces();
    }
    if (cpptestSendLimitedChunk(CPPTEST_MSG_REPORT, sci, file, line)) {
        cpptestSendStringArgument(CPPTEST_ARG_REPORT_EXPR, txt);
        cpptestSendBoolArgument(CPPTEST_ARG_REPORT_ST_PRESENT, reportStackTrace);
        cpptestSendLastArgument();
    }
}

CPPTEST_EVENT_HANDLER
void CDECL_CALL cpptestReportBool(const char* txt, int value, CppTest_StubCallInfo* sci, const char *file, unsigned line)
{
    if (_cpptestSendReports == 0) {
        return;
    }

    if (cpptestSendLimitedChunk(CPPTEST_MSG_REPORT_BOOL, sci, file, line)) {
        cpptestSendStringArgument(CPPTEST_ARG_REPORT_EXPR, txt);
        cpptestSendBoolArgument(CPPTEST_ARG_REPORT_VALUE, value);
        cpptestSendLastArgument();
    }
}

CPPTEST_EVENT_HANDLER
void CDECL_CALL cpptestReportInteger(const char* txt, CPPTEST_INTEGER value, CppTest_StubCallInfo* sci, const char *file, unsigned line)
{
    if (_cpptestSendReports == 0) {
        return;
    }

    if (cpptestSendLimitedChunk(CPPTEST_MSG_REPORT_INTEGER, sci, file, line)) {
        cpptestSendStringArgument(CPPTEST_ARG_REPORT_EXPR, txt);
        cpptestSendIntegerArgument(CPPTEST_ARG_REPORT_VALUE, value);
        cpptestSendLastArgument();
    }
}

CPPTEST_EVENT_HANDLER
void CDECL_CALL cpptestReportUInteger(const char* txt, CPPTEST_UINTEGER value, CppTest_StubCallInfo* sci, const char *file, unsigned line)
{
    if (_cpptestSendReports == 0) {
        return;
    }

    if (cpptestSendLimitedChunk(CPPTEST_MSG_REPORT_UINTEGER, sci, file, line)) {
        cpptestSendStringArgument(CPPTEST_ARG_REPORT_EXPR, txt);
        cpptestSendUIntegerArgument(CPPTEST_ARG_REPORT_VALUE, value);
        cpptestSendLastArgument();
    }
}

#if !CPPTEST_DISABLE_ALL_FLOATING_POINT
CPPTEST_EVENT_HANDLER
void CDECL_CALL cpptestReportFloat(const char* txt, CPPTEST_FLOAT value, CppTest_StubCallInfo* sci, const char *file, unsigned line)
{
    if (_cpptestSendReports == 0) {
        return;
    }

    if (cpptestSendLimitedChunk(CPPTEST_MSG_REPORT_FLOAT, sci, file, line)) {
        cpptestSendStringArgument(CPPTEST_ARG_REPORT_EXPR, txt);
        cpptestSendFloatArgument(CPPTEST_ARG_REPORT_VALUE, value);
        cpptestSendLastArgument();
    }
}
#endif

CPPTEST_EVENT_HANDLER
void CDECL_CALL cpptestReportCStr(const char* txt, const char* value, CppTest_StubCallInfo* sci, const char *file, unsigned line)
{
    if (_cpptestSendReports == 0) {
        return;
    }

    if (cpptestSendLimitedChunk(CPPTEST_MSG_REPORT_CSTR, sci, file, line)) {        
        cpptestSendStringArgument(CPPTEST_ARG_REPORT_EXPR, txt);
        cpptestSendCStringArgument(CPPTEST_ARG_REPORT_VALUE, value);
        cpptestSendLastArgument();
    }
}

CPPTEST_EVENT_HANDLER
void CDECL_CALL cpptestReportCStrN(const char* txt, const char* value, unsigned int maxSize, CppTest_StubCallInfo* sci, const char *file, unsigned line)
{
    if (_cpptestSendReports == 0) {
        return;
    }

    if (cpptestSendLimitedChunk(CPPTEST_MSG_REPORT_CSTR_N, sci, file, line)) {
        cpptestSendStringArgument(CPPTEST_ARG_REPORT_EXPR, txt);
        cpptestSendCStringMaxArgument(CPPTEST_ARG_REPORT_VALUE, value, maxSize);
        cpptestSendLastArgument();
    }
}

CPPTEST_EVENT_HANDLER
void CDECL_CALL cpptestReportMemBuffer(const char* txt, const void* value, unsigned int size, CppTest_StubCallInfo* sci, const char *file, unsigned line)
{
    if (_cpptestSendReports == 0) {
        return;
    }

    if (cpptestSendLimitedChunk(CPPTEST_MSG_REPORT_MEM_BUFFER, sci, file, line)) {
        if (size > CPPTEST_MAX_POST_CONDITION_MEM_BUFF_SIZE_ALLOWED) {
            size = CPPTEST_MAX_POST_CONDITION_MEM_BUFF_SIZE_ALLOWED;
        }
        cpptestSendStringArgument(CPPTEST_ARG_REPORT_EXPR, txt);
        cpptestSendDataArgument(CPPTEST_ARG_REPORT_VALUE, value, size);
        cpptestSendLastArgument();
    }
}

CPPTEST_EVENT_HANDLER
void CDECL_CALL cpptestReportPtr(const char* txt, int isNull, CppTest_StubCallInfo* sci, const char *file, unsigned line)
{
    if (_cpptestSendReports == 0) {
        return;
    }

    if (cpptestSendLimitedChunk(CPPTEST_MSG_REPORT_PTR, sci, file, line)) {
        cpptestSendStringArgument(CPPTEST_ARG_REPORT_EXPR, txt);
        cpptestSendStringArgument(CPPTEST_ARG_REPORT_VALUE, isNull ? "NULL" : "NOT_NULL");
        cpptestSendLastArgument();
    }
}

CPPTEST_EVENT_HANDLER
void CDECL_CALL cpptestReportEnum(const char* txt, CppTestEnumInfo* enumInfo, CPPTEST_INTEGER value, 
        CppTest_StubCallInfo* sci, int isCpp, const char *file, unsigned line)
{
    if ((enumInfo == 0) || (_cpptestSendReports == 0)) {
        return;
    } else {
        const char* valueEnum = cpptestGetEnumeratorStrFromTable(enumInfo, value, isCpp, file, line);
        if (cpptestSendLimitedChunk(CPPTEST_MSG_REPORT_ENUM, sci, file, line)) {
            cpptestSendStringArgument(CPPTEST_ARG_REPORT_EXPR, txt);
            cpptestSendStringArgument(CPPTEST_ARG_REPORT_VALUE, valueEnum);
            if (isCpp) {
                cpptestSendStringArgument(CPPTEST_ARG_ENUM_SCOPE, enumInfo->scope);
            }
            if (enumInfo->scoped) {
                cpptestSendStringArgument(CPPTEST_ARG_ENUM_NAME, enumInfo->name);
                cpptestSendBoolArgument(CPPTEST_ARG_ENUM_SCOPED, enumInfo->scoped);
            }
            if (enumInfo->scoped) {
                cpptestSendStringArgument(CPPTEST_ARG_ENUM_NAME, enumInfo->name);
                cpptestSendBoolArgument(CPPTEST_ARG_ENUM_SCOPED, enumInfo->scoped);
            }
            cpptestSendLastArgument();
        }
    }
}

#if CPPTEST_TRIGGER_ENABLED
CPPTEST_RUNTIME_API
void CDECL_CALL cpptestTriggerReport(const char* triggerName, const char* txt, const char* file, unsigned int line)
{
    if (_cpptestSendReports == 0) {
        return;
    }

    if (cpptestSendLimitedChunk(CPPTEST_MSG_REPORT, 0, file, line)) {
        cpptestSendStringArgument(CPPTEST_ARG_TRIGGER_NAME, triggerName);
        cpptestSendStringArgument(CPPTEST_ARG_REPORT_EXPR, txt);
        cpptestSendLastArgument();
    }
}
#endif /* CPPTEST_TRIGGER_ENABLED */

CPPTEST_EVENT_HANDLER
void CDECL_CALL cpptestPostConditionBool(const char* valueString, int value, const char *file, unsigned line)
{
    if (_cpptestSendPostConditions == 0) {
        return;
    }

    if (cpptestSendLimitedChunk(CPPTEST_MSG_POST_CONDITION_BOOL, 0, file, line)) {
        cpptestSendStringArgument(CPPTEST_ARG_POST_CONDITION_EXPR, valueString);
        cpptestSendBoolArgument(CPPTEST_ARG_POST_CONDITION_VALUE, value);
        cpptestSendLastArgument();
    }
}

CPPTEST_EVENT_HANDLER
void CDECL_CALL cpptestPostConditionInteger(const char* valueString, CPPTEST_INTEGER value, const char *file, unsigned line)
{
    if (_cpptestSendPostConditions == 0) {
        return;
    }

    if (cpptestSendLimitedChunk(CPPTEST_MSG_POST_CONDITION_INTEGER, 0, file, line)) {
        cpptestSendStringArgument(CPPTEST_ARG_POST_CONDITION_EXPR, valueString);
        cpptestSendIntegerArgument(CPPTEST_ARG_POST_CONDITION_VALUE, value);
        cpptestSendLastArgument();
    }
}

CPPTEST_EVENT_HANDLER
void CDECL_CALL cpptestPostConditionUInteger(const char* valueString, CPPTEST_UINTEGER value, const char *file, unsigned line)
{
    if (_cpptestSendPostConditions == 0) {
        return;
    }

    if (cpptestSendLimitedChunk(CPPTEST_MSG_POST_CONDITION_UINTEGER, 0, file, line)) {
        cpptestSendStringArgument(CPPTEST_ARG_POST_CONDITION_EXPR, valueString);
        cpptestSendUIntegerArgument(CPPTEST_ARG_POST_CONDITION_VALUE, value);
        cpptestSendLastArgument();
    }
}

#if !CPPTEST_DISABLE_ALL_FLOATING_POINT
CPPTEST_EVENT_HANDLER
void CDECL_CALL cpptestPostConditionFloat(const char* valueString, CPPTEST_FLOAT value, const char *file, unsigned line)
{
    if (_cpptestSendPostConditions == 0) {
        return;
    }

    if (cpptestSendLimitedChunk(CPPTEST_MSG_POST_CONDITION_FLOAT, 0, file, line)) {
        cpptestSendStringArgument(CPPTEST_ARG_POST_CONDITION_EXPR, valueString);
        cpptestSendFloatArgument(CPPTEST_ARG_POST_CONDITION_VALUE, value);
        cpptestSendLastArgument();
    }
}
#endif

CPPTEST_EVENT_HANDLER
void CDECL_CALL cpptestPostConditionCStr(const char* valueString, const char* value, const char *file, unsigned line)
{
    if (_cpptestSendPostConditions == 0) {
        return;
    }

    if (cpptestSendLimitedChunk(CPPTEST_MSG_POST_CONDITION_CSTR, 0, file, line)) {        
        cpptestSendStringArgument(CPPTEST_ARG_POST_CONDITION_EXPR, valueString);
        cpptestSendCStringArgument(CPPTEST_ARG_POST_CONDITION_VALUE, value);
        cpptestSendLastArgument();
    }
}

CPPTEST_EVENT_HANDLER
void CDECL_CALL cpptestPostConditionCStrN(const char* valueString, const char* value, unsigned int maxSize, const char *file, unsigned line)
{
    if (_cpptestSendPostConditions == 0) {
        return;
    }

    if (cpptestSendLimitedChunk(CPPTEST_MSG_POST_CONDITION_CSTR_N, 0, file, line)) {
        cpptestSendStringArgument(CPPTEST_ARG_POST_CONDITION_EXPR, valueString);
        cpptestSendCStringMaxArgument(CPPTEST_ARG_POST_CONDITION_VALUE, value, maxSize);
        cpptestSendLastArgument();
    }
}

CPPTEST_EVENT_HANDLER
void CDECL_CALL cpptestPostConditionMemBuffer(const char* valueString, const void* value, unsigned int size, const char *file, unsigned line)
{
    if (_cpptestSendPostConditions == 0) {
        return;
    }

    if (cpptestSendLimitedChunk(CPPTEST_MSG_POST_CONDITION_MEM_BUFFER, 0, file, line)) {
        if (size > CPPTEST_MAX_POST_CONDITION_MEM_BUFF_SIZE_ALLOWED) {
            size = CPPTEST_MAX_POST_CONDITION_MEM_BUFF_SIZE_ALLOWED;
        }
        cpptestSendStringArgument(CPPTEST_ARG_POST_CONDITION_EXPR, valueString);
        cpptestSendDataArgument(CPPTEST_ARG_POST_CONDITION_VALUE, value, size);
        cpptestSendLastArgument();
    }
}

CPPTEST_EVENT_HANDLER
void CDECL_CALL cpptestPostConditionPtr(const char* valueString, int isNull, const char *file, unsigned line)
{
    if (_cpptestSendPostConditions == 0) {
        return;
    }

    if (cpptestSendLimitedChunk(CPPTEST_MSG_POST_CONDITION_PTR, 0, file, line)) {
        cpptestSendStringArgument(CPPTEST_ARG_POST_CONDITION_EXPR, valueString);
        cpptestSendStringArgument(CPPTEST_ARG_POST_CONDITION_VALUE, isNull ? "NULL" : "NOT_NULL");
        cpptestSendLastArgument();
    }
}

CPPTEST_EVENT_HANDLER
void CDECL_CALL cpptestPostConditionExternal(const char* userMessage, const char* ret, const char *file, unsigned line)
{
    if (_cpptestSendPostConditions == 0) {
        return;
    }

    if (ret) {
        if (cpptestSendLimitedChunk(CPPTEST_MSG_POST_CONDITION_EXTERNAL, 0, file, line)) {
            cpptestSendStringArgument(CPPTEST_ARG_POST_CONDITION_EXPR, userMessage);
            cpptestSendStringArgument(CPPTEST_ARG_POST_CONDITION_VALUE, ret);
            cpptestSendLastArgument();
        }
    }
}

CPPTEST_EVENT_HANDLER
void CDECL_CALL cpptestPostConditionEnum(CppTestEnumInfo* enumInfo, const char* valueString, CPPTEST_INTEGER value, 
                                         int isCpp, const char *file, unsigned line)
{
    if ((enumInfo == 0) || (_cpptestSendPostConditions == 0)) {
        return;
    } else {
        const char* valueEnum = cpptestGetEnumeratorStrFromTable(enumInfo, value, isCpp, file, line);
        if (cpptestSendLimitedChunk(CPPTEST_MSG_POST_CONDITION_UINTEGER, 0, file, line)) {
            cpptestSendStringArgument(CPPTEST_ARG_POST_CONDITION_EXPR, valueString);
            cpptestSendStringArgument(CPPTEST_ARG_POST_CONDITION_VALUE, valueEnum);
            if (isCpp) {
                cpptestSendStringArgument(CPPTEST_ARG_ENUM_SCOPE, enumInfo->scope);
            }
            if (enumInfo->scoped) {
                cpptestSendStringArgument(CPPTEST_ARG_ENUM_NAME, enumInfo->name);
                cpptestSendBoolArgument(CPPTEST_ARG_ENUM_SCOPED, enumInfo->scoped);
            }
            if (enumInfo->scoped) {
                cpptestSendStringArgument(CPPTEST_ARG_ENUM_NAME, enumInfo->name);
                cpptestSendBoolArgument(CPPTEST_ARG_ENUM_SCOPED, enumInfo->scoped);
            }
            cpptestSendLastArgument();
        }
    }
}

void CDECL_CALL cpptestAtExit(CppTestAtExitCallback callback)
{
    atExitCallback = callback;
}

static int CDECL_CALL cpptestInvalidEnumTypeName(const char* enumTypeStr, const char* file, unsigned int line)
{
    if (beginErrorProcessing(CPPTEST_ECE_INVALID_ENUM_TYPE_NAME)) {
        cpptestSendChunk(CPPTEST_ECE_INVALID_ENUM_TYPE_NAME, /*hasArguments=*/1);
        cpptestSendEventPositionArgument(currentTestSuiteFile, file, line);
        cpptestSendStringArgument(CPPTEST_ARG_ENUM_TYPE_NAME, enumTypeStr);
        cpptestSendLastArgument();

        endErrorProcessing(/*exitCode=*/33, /*dojmp=*/1, /*doExit=*/1);
    }
    return 0;
}

static int CDECL_CALL cpptestInvalidEnumeratorName(CppTestEnumInfo* enumInfo, const char* enumeratorStr, 
        int isCpp, const char* file, unsigned int line)
{
    if (beginErrorProcessing(CPPTEST_ECE_INVALID_ENUMERATOR_NAME)) {
        cpptestSendChunk(CPPTEST_ECE_INVALID_ENUMERATOR_NAME, /*hasArguments=*/1);
        cpptestSendEventPositionArgument(currentTestSuiteFile, file, line);
        cpptestSendStringArgument(CPPTEST_ARG_ENUM_NAME, enumInfo->name);
        if (isCpp) {
            cpptestSendStringArgument(CPPTEST_ARG_ENUM_SCOPE, enumInfo->scope);
        }
        cpptestSendStringArgument(CPPTEST_ARG_ENUMERATOR_NAME, enumeratorStr);
        cpptestSendLastArgument();

        endErrorProcessing(/*exitCode=*/33, /*dojmp=*/1, /*doExit=*/1);
    }
    return 0;
}

static int CDECL_CALL cpptestInvalidEnumeratorValue(CppTestEnumInfo* enumInfo, CPPTEST_INTEGER value, 
        int isCpp, const char* file, unsigned int line)
{
    if (beginErrorProcessing(CPPTEST_ECE_INVALID_ENUMERATOR_VALUE)) {
        cpptestSendChunk(CPPTEST_ECE_INVALID_ENUMERATOR_VALUE, /*hasArguments=*/1);
        cpptestSendEventPositionArgument(currentTestSuiteFile, file, line);
        cpptestSendStringArgument(CPPTEST_ARG_ENUM_NAME, enumInfo->name);
        if (isCpp) {
            cpptestSendStringArgument(CPPTEST_ARG_ENUM_SCOPE, enumInfo->scope);
        }
        cpptestSendIntegerArgument(CPPTEST_ARG_ENUMERATOR_VALUE, value);
        cpptestSendLastArgument();

        endErrorProcessing(/*exitCode=*/33, /*dojmp=*/1, /*doExit=*/1);
    }
    return 0;
}

CppTestEnumInfo* CDECL_CALL cpptestGetEnumInfo(int dummy, const char* enumTypeStr, const char* file, unsigned int line)
{
    if (cpptestEnumTypeStrings) {
        CppTestEnumTypeString* enumTypeString = cpptestEnumTypeStrings;
        for (; enumTypeString->str != 0; ++enumTypeString) {
            if (cpptestStrCmp(enumTypeStr, enumTypeString->str) == 0) {
                return enumTypeString->enumInfo;
            }
        }
    }
    cpptestInvalidEnumTypeName(enumTypeStr, file, line);
    return 0;

}

CPPTEST_INTEGER CDECL_CALL cpptestGetEnumeratorValueFromTable(CppTestEnumInfo* enumInfo, const char* enumeratorStr, 
        int isCpp, const char* file, unsigned int line)
{
    CppTestEnumerator* enumerator = enumInfo->enumerators;
    unsigned int nameLen = cpptestStrLen(enumInfo->name);
    unsigned int scopeLen = cpptestStrLen(enumInfo->scope);

    /* Skip scope if any */
    if (cpptestStrNCmp(enumInfo->scope, enumeratorStr, scopeLen) == 0) {
            enumeratorStr += scopeLen;
    }
    
    /* Skip enum type name if any */
    if (cpptestStrNCmp(enumInfo->name, enumeratorStr, nameLen) == 0 && (enumeratorStr[nameLen] == ':') && (enumeratorStr[nameLen + 1] == ':')) {
        enumeratorStr += nameLen + 2;
    }

    /* Find enumerator */
    for (; enumerator->name != 0; ++enumerator) {
        if (cpptestStrCmp(enumerator->name, enumeratorStr) == 0) return enumerator->value;
    }
    cpptestInvalidEnumeratorName(enumInfo, enumeratorStr, isCpp, file, line);
    return -1;
}

const char* CDECL_CALL cpptestGetEnumeratorStrFromTable(CppTestEnumInfo* enumInfo, CPPTEST_INTEGER value, 
        int isCpp, const char* file, unsigned int line)
{
    CppTestEnumerator* enumerator = enumInfo->enumerators;
    for (; enumerator->name != 0; ++enumerator) {
        if (enumerator->value == value) return enumerator->name;
    }
    cpptestInvalidEnumeratorValue(enumInfo, value, isCpp, file, line);
    return "<unknown>";
}

CPPTEST_INTEGER CDECL_CALL cpptestGetEnumeratorValue(int dummy, const char* enumTypeStr, const char* enumeratorNameStr, 
        int isCpp, const char* file, unsigned int line)
{
    if (cpptestEnumTypeStrings) {
        CppTestEnumTypeString* enumTypeString = cpptestEnumTypeStrings;
        for (; enumTypeString->str != 0; ++enumTypeString) {
            if (cpptestStrCmp(enumTypeStr, enumTypeString->str) == 0) {
                return cpptestGetEnumeratorValueFromTable(enumTypeString->enumInfo, enumeratorNameStr, isCpp, file, line);
            }
        }
    }
    cpptestInvalidEnumTypeName(enumTypeStr, file, line);
    return -1; 
}

const char* CDECL_CALL cpptestGetEnumeratorStr(int dummy, const char* enumTypeStr, CPPTEST_INTEGER value, 
        int isCpp, const char* file, unsigned int line)
{
    if (cpptestEnumTypeStrings) {
        CppTestEnumTypeString* enumTypeString = cpptestEnumTypeStrings;
        for (; enumTypeString->str != 0; ++enumTypeString) {
            if (cpptestStrCmp(enumTypeStr, enumTypeString->str) == 0) {
                return cpptestGetEnumeratorStrFromTable(enumTypeString->enumInfo, value, isCpp, file, line);
            }
        }
    }
    cpptestInvalidEnumTypeName(enumTypeStr, file, line);
    return "<unknown>";
}

static int cpptestStubCallbackGeneratorProblemMissingStubInfo(const char* stubId, const char* file, unsigned int line)
{
    if (beginErrorProcessing(CPPTEST_STUB_CALLBACK_GENERATOR_ERROR_MISSING_STUB_INFO)) {
        cpptestSendChunk(CPPTEST_STUB_CALLBACK_GENERATOR_ERROR_MISSING_STUB_INFO, /*hasArguments=*/1);
        cpptestSendEventPositionArgument(currentTestSuiteFile, file, line);
        cpptestSendStringArgument(CPPTEST_ARG_STUB_ID, stubId);
        cpptestSendLastArgument();

        endErrorProcessing(/*exitCode=*/33, /*dojmp=*/1, /*doExit=*/1);
    }
    return 0;
}

CppTest_StubInfo* CDECL_CALL cpptestGetStubInfo(const char* userId, unsigned context, const char* file, unsigned int line)
{
    CppTest_StubInfo** sip = &_stubInfos[0];
    CppTest_StubInfo* result = 0;
    CppTest_ErrorCode errorCode = CPPTEST_NO_ERROR;

    for (; *sip; ++sip) {
        if (strcmp((*sip)->userId, userId) == 0) {
            if (result) {
                if ((result->flags & CPPTEST_STUB_HAS_BEEN_REMOVED) != 0) {
                    result = *sip;
                } else if (((*sip)->flags & CPPTEST_STUB_HAS_BEEN_REMOVED) != 0) {
                    /* ignore *sip */
                } else {
                    errorCode = CPPTEST_STUB_CALLBACK_ERROR_DUPLICATED_STUB_ID;
                    break;
                }
            } else {
                result = *sip;
            }
        }
    }
    
    if (errorCode != CPPTEST_NO_ERROR) {
        /* do nothing */
    } else if (context == CPPTEST_GET_STUB_INFO_CONTEXT_HAS_CALLBACK) {
        /* CPPTEST_HAS_STUB_CALLBACK is allowed to be used with not existed id. */
    } else if (!result) {
        errorCode = CPPTEST_STUB_CALLBACK_ERROR_INVALID_STUB_ID;
    
    } else if ((result->flags & CPPTEST_STUB_HAS_BEEN_REMOVED) != 0) {
        errorCode = CPPTEST_STUB_CALLBACK_ERROR_REMOVED_STUB_ID;
    
    } else if ((result->flags & CPPTEST_STUB_HAS_CALLBACK) == 0 && 
                (context == CPPTEST_GET_STUB_INFO_CONTEXT_CALLBACK))
    {
        errorCode = CPPTEST_STUB_CALLBACK_ERROR_MISSING_STUB_CALLBACK_MACRO;
    
    } else if ((result->flags & CPPTEST_STUB_HAS_TRIGGER) == 0 && 
                (context == CPPTEST_GET_STUB_INFO_CONTEXT_TRIGGER))
    {
        errorCode = CPPTEST_STUB_CALLBACK_ERROR_MISSING_STUB_TRIGGER_MACRO;
    }

    if (errorCode != CPPTEST_NO_ERROR) {
        if (beginErrorProcessing(errorCode)) {
            cpptestSendChunk(errorCode, /*hasArguments=*/1);
            cpptestSendEventPositionArgument(currentTestSuiteFile, file, line);
            cpptestSendStringArgument(CPPTEST_ARG_STUB_ID, userId);
            cpptestSendIntArgument(CPPTEST_ARG_STUB_CONTEXT_ID, context);
            cpptestSendLastArgument();

            endErrorProcessing(/*exitCode=*/33, /*dojmp=*/1, /*doExit=*/1);
        }
    }

    return result;
}

void CDECL_CALL cpptestRegisterStubCallback(const char* userId, CppTest_Stub_Callback callbackPtr, const char* file, unsigned int line)
{
    CppTest_StubInfo* stubInfo = cpptestGetStubInfo(userId, CPPTEST_GET_STUB_INFO_CONTEXT_CALLBACK, file, line);
    if (stubInfo) {
        stubInfo->lastCallNo = 0;
        stubInfo->callbackPtr = callbackPtr;
        if (!callbackPtr) {
            /* Assume that, the generator was not able to create a stub callback */
            cpptestStubCallbackGeneratorProblemMissingStubInfo(userId, file, line);
        }
    }
}

void CDECL_CALL cpptestUnregisterStubCallback(const char* userId, const char* file, unsigned int line)
{
    CppTest_StubInfo* stubInfo = cpptestGetStubInfo(userId, CPPTEST_GET_STUB_INFO_CONTEXT_CALLBACK, file, line);
    if (stubInfo) {
        stubInfo->callbackPtr = 0;
    }
}

void CDECL_CALL cpptestUnregisterAllStubCallbacks(void)
{
    CppTest_StubInfo** sip = 0;
    for (sip = &_stubInfos[0]; *sip; ++sip) {
        (*sip)->callbackPtr = 0;
    }
}

int CDECL_CALL cpptestHasStubCallback(const char* userId, const char* file, unsigned int line)
{
    CppTest_StubInfo* stubInfo = cpptestGetStubInfo(userId, CPPTEST_GET_STUB_INFO_CONTEXT_HAS_CALLBACK, file, line);
    return stubInfo && stubInfo->callbackPtr ? 1 : 0;
}


void CDECL_CALL cpptestAddChecker(CppTest_StubInfo* stubInfo, CppTest_Checker* checker)
{
    CPPTEST_LOCK(checkersLock);
    checker->next = 0;
    if (stubInfo->lastChecker) {
        stubInfo->lastChecker->next = checker;
    } else {
        stubInfo->firstChecker = checker;
    }
    stubInfo->lastChecker = checker;
    CPPTEST_UNLOCK(checkersLock);
}

void CDECL_CALL cpptestRemoveAllCheckers(void)
{
    CPPTEST_LOCK(checkersLock);
    {
        CppTest_StubInfo** sip = 0;
        for (sip = &_stubInfos[0]; *sip; ++sip) {
            CppTest_Checker* checker = (*sip)->firstChecker;
            while (checker) {
                (*sip)->firstChecker = checker->next;
                cpptestFree(checker);
                checker = (*sip)->firstChecker;
            }
            (*sip)->lastChecker = 0;
        }
    }
    CPPTEST_UNLOCK(checkersLock);
}

void CDECL_CALL cpptestCallCheckers(CppTest_StubInfo* stubInfo)
{
    CPPTEST_LOCK(checkersLock);
    {
        CppTest_Checker* checker = stubInfo->firstChecker;
        while (checker) {
            checker->checker(stubInfo, checker->data);
            checker = checker->next;
        }
    }
    CPPTEST_UNLOCK(checkersLock);
}


/**
 * Initialize runtime.
 */
CPPTEST_RUNTIME_API
void CDECL_CALL cpptestInitializeRuntime(
                        CppTestThreadInitCallback threadInitCallback,
                        CppTest_Property* properties,
                        CppTestEnumTypeString* enumTypeStrings,
                        CppTest_StubInfo** stubInfos,
                        CppTest_System* system)
{
    if (!localRuntimeInitialized) {

        cpptestEnumTypeStrings = enumTypeStrings;
         _stubInfos = stubInfos;
        if (system) {
            targetSystem = system;
        }

        localInitializeProperties(properties);

#if CPPTEST_THREADS_ENABLED 
        lockRuntime();
        if (localRuntimeInitialized) {
            unlockRuntime();
            return;
        }
#endif
        localRuntimeInitialized = 1;
        
        localInitializeMessages();
        if (!cpptestGetBoolPropertyEx("init.delay_comm_init", CPPTEST_DEFAULT_DELAY_COMMUNICATION_INIT)) {
            localInitializeCommunication();
        }
        localInitializeTestCases();
        localInitializeStackTrace();
        localInitializeCoverage();
        localInitializeMemoryAnalysis();
        cpptestSetThreadInitCallback(threadInitCallback);
        if (cpptestGetBoolPropertyEx("test.app_mode", CPPTEST_APPLICATION_MODE)) {
            currentTestCaseId = 1U;
            runtimeStatus = INSIDE_TEST_CASE;
        }
        cpptestExecuteRuntimeCommand(CPPTEST_CMD_INITIALIZE_RUNTIME);
        unlockRuntime();
    }
}

void CDECL_CALL cpptestFinalizeRuntime(void)
{
    INITIALIZED_RUNTIME_REQUIRED(NO_RETURN_VALUE)

    INITIALIZED_COMMUNICATION_REQUIRED()

    if (cpptestGetBoolPropertyEx("test.app_mode", CPPTEST_APPLICATION_MODE)) {
        runtimeStatus = OUTSIDE_TEST_CASE;
        currentTestCaseId = 0U;
    }

    cpptestExecuteRuntimeCommand(CPPTEST_CMD_FINALIZE_RUNTIME);
    localFinalizeMemoryAnalysis();
    localFinalizeCoverage();
    localFinalizeStackTrace();
    localFinalizeTestCases();
    localFinalizeCommunication();
    localFinalizeMessages();
    localFinalizeProperties();
    cpptestSetThreadInitCallback((CppTestThreadInitCallback)0);
    
    localRuntimeInitialized = 0;  
}


/*--------------------------------------------------------------------------------------------
 * Functions from C++Test Runtime API (CppTest_*)
 *------------------------------------------------------------------------------------------*/
