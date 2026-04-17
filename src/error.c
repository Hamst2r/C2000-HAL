#include "error.h"

#include "base.h"
#include "sys.h"
#include "wdog.h"

#define HAL_ERROR_MAGIC (0x45525230UL)

struct HAL_ErrorState
{
    uint32_t magic;
    struct HAL_ErrorSnapshot snapshot;
};

static struct HAL_ErrorState HAL_errorState;
static HAL_ErrorReporter HAL_errorReporter = (HAL_ErrorReporter)0;
static bool HAL_errorResetEnabled = true;

static void HAL_ERROR_Halt(void)
{
    DINT();
    ESTOP0;
    while(1)
    {
    }
}

static void HAL_ERROR_DefaultAction(enum HAL_ErrorAction action)
{
    if((action == HAL_ErrorAction_None) || (!HAL_errorResetEnabled))
    {
        return;
    }

    DINT();

    if(action == HAL_ErrorAction_Halt)
    {
        HAL_ERROR_Halt();
    }
    else if(action == HAL_ErrorAction_ResetSoft)
    {
        SYS_Reset();
        HAL_ERROR_Halt();
    }
    else if(action == HAL_ErrorAction_ResetWatchdog)
    {
        WDOG_SystemReset();
        HAL_ERROR_Halt();
    }
}

void HAL_ERROR_Init(void)
{
    if(HAL_errorState.magic != HAL_ERROR_MAGIC)
    {
        HAL_errorState.magic = HAL_ERROR_MAGIC;
        HAL_errorState.snapshot.bootCount = 0U;
        HAL_errorState.snapshot.totalErrors = 0U;
        HAL_errorState.snapshot.head = 0U;
        HAL_errorState.snapshot.count = 0U;
    }

    HAL_errorState.snapshot.bootCount++;
}

void HAL_ERROR_SetReporter(HAL_ErrorReporter reporter)
{
    HAL_errorReporter = reporter;
}

void HAL_ERROR_SetResetEnabled(bool enabled)
{
    HAL_errorResetEnabled = enabled;
}

void HAL_ERROR_RaiseImpl(
    enum HAL_ErrorCode code,
    enum HAL_ErrorSeverity severity,
    enum HAL_ErrorAction action,
    uint32_t data0,
    uint32_t data1,
    const char* file,
    uint32_t line)
{
    struct HAL_ErrorRecord* record;

    if(HAL_errorState.magic != HAL_ERROR_MAGIC)
    {
        HAL_ERROR_Init();
    }

    record = &HAL_errorState.snapshot.records[HAL_errorState.snapshot.head];

    HAL_errorState.snapshot.totalErrors++;

    record->sequence = HAL_errorState.snapshot.totalErrors;
    record->timestamp = (uint32_t)HAL_ERROR_TIMESTAMP_NOW();
    record->file = (uint32_t)(uintptr_t)file;
    record->line = line;
    record->data0 = data0;
    record->data1 = data1;
    record->code = (uint16_t)code;
    record->severity = (uint16_t)severity;
    record->action = (uint16_t)action;
    record->reserved = 0U;

    HAL_errorState.snapshot.head++;
    if(HAL_errorState.snapshot.head >= HAL_ERROR_CAPACITY)
    {
        HAL_errorState.snapshot.head = 0U;
    }

    if(HAL_errorState.snapshot.count < HAL_ERROR_CAPACITY)
    {
        HAL_errorState.snapshot.count++;
    }

    if(HAL_errorReporter != (HAL_ErrorReporter)0)
    {
        HAL_errorReporter(record);
    }

    if(severity == HAL_ErrorSeverity_Fatal)
    {
        HAL_ERROR_DefaultAction(action);
        HAL_ERROR_Halt();
    }
}

const struct HAL_ErrorSnapshot* HAL_ERROR_GetSnapshot(void)
{
    if(HAL_errorState.magic != HAL_ERROR_MAGIC)
    {
        HAL_ERROR_Init();
    }

    return &HAL_errorState.snapshot;
}

void HAL_ERROR_Clear(void)
{
    HAL_errorState.snapshot.totalErrors = 0U;
    HAL_errorState.snapshot.head = 0U;
    HAL_errorState.snapshot.count = 0U;
}
