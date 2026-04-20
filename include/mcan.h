#ifndef MCAN_H_
#define MCAN_H_

// Change this to modify data exchange buffer size
#define MCAN_MAX_DATA_BYTES     (8)

#include "base.h"
#include "support_mcan.h"

enum MCAN_ID
{
    MCAN_ID_Standard,
    MCAN_ID_Extended
};

enum MCAN_DataLength
{
    MCAN_DataLength_0Byte  =  0U,
    MCAN_DataLength_1Byte  =  1U,
    MCAN_DataLength_2Byte  =  2U,
    MCAN_DataLength_3Byte  =  3U,
    MCAN_DataLength_4Byte  =  4U,
    MCAN_DataLength_5Byte  =  5U,
    MCAN_DataLength_6Byte  =  6U,
    MCAN_DataLength_7Byte  =  7U,
    MCAN_DataLength_8Byte  =  8U,
    MCAN_DataLength_12Byte =  9U,
    MCAN_DataLength_16Byte = 10U,
    MCAN_DataLength_20Byte = 11U,
    MCAN_DataLength_24Byte = 12U,
    MCAN_DataLength_32Byte = 13U,
    MCAN_DataLength_48Byte = 14U,
    MCAN_DataLength_64Byte = 15U
};

enum MCAN_Mode
{
    MCAN_Mode_Classic   = 0b00U,
    MCAN_Mode_CANFD     = 0b10U,
    MCAN_Mode_CANFD_BRS = 0b11U
};

enum MCAN_BitClkSrc
{
    MCAN_BitClkSrc_SysClk    = 0,
    MCAN_BitClkSrc_AuxClkIn  = 2,
    MCAN_BitClkSrc_PllRawClk = 3
};


enum MCAN_DataSize
{
    MCAN_DataSize_8Byte  = 0U,
    MCAN_DataSize_12Byte = 1U,
    MCAN_DataSize_16Byte = 2U,
    MCAN_DataSize_20Byte = 3U,
    MCAN_DataSize_24Byte = 4U,
    MCAN_DataSize_32Byte = 5U,
    MCAN_DataSize_48Byte = 6U,
    MCAN_DataSize_64Byte = 7U
};

enum MCAN_FilterType
{
    MCAN_FilterType_Range,
    MCAN_FilterType_DualID,
    MCAN_FilterType_ID_Mask,
    MCAN_FilterType_Range_NoMask
};

enum MCAN_FilterDest
{
    MCAN_FilterDest_Disable,
    MCAN_FilterDest_FIFO0,
    MCAN_FilterDest_FIFO1,
    MCAN_FilterDest_Reject,
    MCAN_FilterDest_Priority,
    MCAN_FilterDest_Priority_FIFO0,
    MCAN_FilterDest_Priority_FIFO1,
    MCAN_FilterDest_Buffer,
};

enum MCAN_TxQueueMode
{
    MCAN_TxQueueMode_FIFO,  // Message is sent in the same order it is written
    MCAN_TxQueueMode_Queue  // Message with lowest Message ID is sent first
};

enum MCAN_Rx
{
    MCAN_Rx_FIFO0,
    MCAN_Rx_FIFO1,
    MCAN_Rx_Buffer
};


enum MCAN_FilterNonMatch
{
    MCAN_FilterNonMatch_FIFO0   = 0,  // Non-matching frames are delivered to Rx FIFO 0
    MCAN_FilterNonMatch_FIFO1   = 1,  // Non-matching frames are delivered to Rx FIFO 1
    MCAN_FilterNonMatch_Discard = 2 // Non-matching frames are discarded
};


// Message structure designed for MCAN messages
struct MCAN_Message
{
    bool isRemote;
    enum MCAN_Mode mode;
    enum MCAN_ID idType;
    uint32_t id;
    uint16_t timestamp;
    enum MCAN_DataLength length;
    uint32_t data[MCAN_MAX_DATA_BYTES / 4];
};

// Message structure for existing implementation
struct MCAN_Legacy_canMsg{
    uint16_t address;
    uint16_t length;
    uint16_t data[8];
};

struct MCAN_MsgRamSect
{
    uint16_t filterStd;   // Standard 11-bit Filter
    uint16_t filterExt;   // Extended 29-bit Filter
    uint16_t rxFIFO0;     // Receive FIFO 0
    uint16_t rxFIFO1;     // Receive FIFO 1
    uint16_t rxBuffer;    // Dedicated Receive Buffer
    uint16_t txEvtFIFO;   // Transmit Event FIFO
    uint16_t txBuffer;    // Transmit Buffer (dedicated buffer and FIFO/Queue)
};

struct MCAN_MsgRam
{
    uint16_t allocated;
    struct MCAN_MsgRamSect startAddr;   // Byte offset from beginning of Message RAM
    struct MCAN_MsgRamSect elementSizeByte;
    struct MCAN_MsgRamSect nElement;
};

struct MCAN_Status
{
    enum MCAN_Mode mode;
    enum MCAN_BitClkSrc clkSrc;
    float fclkMHz;
    float arbiKbps;
    float dataKbps;
    uint16_t arbiQuanta;
    uint16_t dataQuanta;
    struct MCAN_MsgRam msgRam;
};

void MCAN_SelectBitClkSrc(enum MCAN mcan, enum MCAN_BitClkSrc src);
void MCAN_SetupCANFD(enum MCAN mcan, float fclkMHz, uint16_t arbiPrescale, float arbiKbps, float dataKbps);
void MCAN_SetupClassic(enum MCAN mcan, float fclkMHz, uint16_t arbiPrescale, float bitrateKbps);
void MCAN_SetupTx(enum MCAN mcan, enum MCAN_TxQueueMode mode, enum MCAN_DataSize dataSize, uint16_t dBufferDepth, uint16_t queueDepth);
void MCAN_SetupRx(enum MCAN mcan, enum MCAN_Rx rx, enum MCAN_DataSize dataSize, uint16_t depth);
void MCAN_SetupFilter(enum MCAN mcan, enum MCAN_ID filter, enum MCAN_FilterNonMatch nonMatchDest, bool rejectRemote);
void MCAN_AddFilter(enum MCAN mcan, enum MCAN_ID filter, enum MCAN_FilterType type, enum MCAN_FilterDest dest, uint32_t id1, uint32_t id2);
void MCAN_Start(enum MCAN mcan);


void MCAN_ReadRxFIFO(enum MCAN mcan, enum MCAN_Rx rx, struct MCAN_Message *message);
void MCAN_ReadRxBuffer(enum MCAN mcan, uint16_t bufferNum, struct MCAN_Message *message);


// bool MCAN_SendBuffer(enum MCAN mcan, uint16_t index, struct MCAN_Message *message);
bool MCAN_SendQueue(enum MCAN mcan, struct MCAN_Message *message);

void MCAN_Legacy_Setup(enum MCAN mcan);
void MCAN_Legacy_AddFilter(enum MCAN mcan, uint16_t address, uint16_t dontCares, uint16_t simultaneousMsgs);
bool MCAN_Legacy_Send(enum MCAN mcan, struct MCAN_Legacy_canMsg *msg);
bool MCAN_Legacy_Receive(enum MCAN mcan, struct MCAN_Legacy_canMsg *msg);


#endif /* MCAN_H */
