#ifndef ERROR_H_
#define ERROR_H_

#include <stdint.h>
#include <stdbool.h>

#ifndef HAL_ERROR_CAPACITY
#define HAL_ERROR_CAPACITY (16U)
#endif

#ifndef HAL_ERROR_TIMESTAMP_NOW
#define HAL_ERROR_TIMESTAMP_NOW() (0U)
#endif

enum HAL_ErrorSeverity
{
    HAL_ErrorSeverity_Info = 0,
    HAL_ErrorSeverity_Warning,
    HAL_ErrorSeverity_Error,
    HAL_ErrorSeverity_Fatal,
};

enum HAL_ErrorAction
{
    HAL_ErrorAction_None = 0,
    HAL_ErrorAction_Halt,
    HAL_ErrorAction_ResetSoft,
    HAL_ErrorAction_ResetWatchdog,
};

enum HAL_ErrorCode
{
    HAL_ErrorCode_None = 0,
    HAL_ErrorCode_Assertion,
    HAL_ErrorCode_InvalidArgument,
    HAL_ErrorCode_OutOfRange,
    HAL_ErrorCode_ResourceConflict,
    HAL_ErrorCode_Timeout,
    HAL_ErrorCode_HardwareFault,
    HAL_ErrorCode_UnexpectedState,
    HAL_ErrorCode_UserDefined = 0x1000,
};

struct HAL_ErrorRecord
{
    uint32_t sequence;
    uint32_t timestamp;
    uint32_t file;
    uint32_t line;
    uint32_t data0;
    uint32_t data1;
    uint16_t code;
    uint16_t severity;
    uint16_t action;
    uint16_t reserved;
};

struct HAL_ErrorSnapshot
{
    uint32_t bootCount;
    uint32_t totalErrors;
    uint16_t head;
    uint16_t count;
    struct HAL_ErrorRecord records[HAL_ERROR_CAPACITY];
};

typedef void (*HAL_ErrorReporter)(const struct HAL_ErrorRecord* record);

void HAL_ERROR_Init(void);
void HAL_ERROR_SetReporter(HAL_ErrorReporter reporter);
void HAL_ERROR_SetResetEnabled(bool enabled);

void HAL_ERROR_RaiseImpl(
    enum HAL_ErrorCode code,
    enum HAL_ErrorSeverity severity,
    enum HAL_ErrorAction action,
    uint32_t data0,
    uint32_t data1,
    const char* file,
    uint32_t line);

const struct HAL_ErrorSnapshot* HAL_ERROR_GetSnapshot(void);
void HAL_ERROR_Clear(void);

#define HAL_ERROR_RAISE(code, severity, action, data0, data1) \
    HAL_ERROR_RaiseImpl((code), (severity), (action), (uint32_t)(data0), (uint32_t)(data1), __FILE__, (uint32_t)(__LINE__))

#define HAL_ERROR_ASSERT(condition, code, data0, data1) do { \
    if(!(condition)) { \
        HAL_ERROR_RAISE((code), HAL_ErrorSeverity_Fatal, HAL_ErrorAction_ResetWatchdog, (data0), (data1)); \
    } \
} while(0)

#define HAL_FATAL(code, data0, data1) \
    HAL_ERROR_RAISE((code), HAL_ErrorSeverity_Fatal, HAL_ErrorAction_ResetWatchdog, (data0), (data1))

#endif /* ERROR_H_ */
