#ifndef FLASH_H
#define FLASH_H


/*  High-Level abstraction layer
    All low-level operations (bank power, timing, erase, etc) are handled internally
 */

#include "base.h"

#define FLASH_STAGING_BUFFER_BYTE       (64)

enum FLASH_Destination
{
    FLASH_Destination_AltFW,
    FLASH_Destination_Calibration,
    FLASH_Destination_Config,
    FLASH_Destination_Profile,
    FLASH_Destination_Log,
};

enum FLASH_ConfigSlot
{
    FLASH_ConfigSlot_1,
    FLASH_ConfigSlot_2,
    FLASH_ConfigSlot_None
};

// enum FLASH_Direction
// {
//     FLASH_Direction_Read,
//     FLASH_Direction_Write
// };

struct FLASH_StagingBuffer
{
    enum FLASH_Destination dest;
    // enum FLASH_Direction direction;
    uint32_t offsetByte;
    uint16_t stagedByte; 
    bool isLastChunk;
    bool ready;
    bool resume;
    uint16_t buffer[FLASH_STAGING_BUFFER_BYTE / 2];
};

struct FLASH_Status
{
    bool taskActive;
    enum FLASH_ConfigSlot activeConfig;
    uint16_t logPage;
};

void FLASH_Init(void);

void FLASH_StartWrite(enum FLASH_Destination dest);
// void FLASH_StartDmaRead(enum FLASH_Destination dest, uint32_t offsetByte, uint16_t sizeByte);
void FLASH_TerminateTask(void);
void FLASH_VerifyNewFW(void);
bool FLASH_IsBusy(void);

#endif // FLASH_H
