#ifndef MCAN_H_
#define MCAN_H_

// Change this to modify data exchange buffer size
// 64 bytes is the maximum data payload for CAN FD frames. Adjust as needed for smaller classic CAN frames to save memory, but should not exceed 64 bytes.
#define MCAN_MAX_DATA_BYTES     (64)

#define MCAN_LEGACY_MODE_SUPPORT

#include "base.h"
#include "support_mcan.h"

enum MCAN_ID
{
    MCAN_ID_Standard = 0,   // 11-bit identifier
    MCAN_ID_Extended = 1    // 29-bit identifier
};

// Message Data Length Code (DLC) encoding as defined in CAN FD specification
enum MCAN_PayloadDLC
{
    MCAN_PayloadDLC_0Byte  =  0U,
    MCAN_PayloadDLC_1Byte  =  1U,
    MCAN_PayloadDLC_2Byte  =  2U,
    MCAN_PayloadDLC_3Byte  =  3U,
    MCAN_PayloadDLC_4Byte  =  4U,
    MCAN_PayloadDLC_5Byte  =  5U,
    MCAN_PayloadDLC_6Byte  =  6U,
    MCAN_PayloadDLC_7Byte  =  7U,
    MCAN_PayloadDLC_8Byte  =  8U,
    MCAN_PayloadDLC_12Byte =  9U,
    MCAN_PayloadDLC_16Byte = 10U,
    MCAN_PayloadDLC_20Byte = 11U,
    MCAN_PayloadDLC_24Byte = 12U,
    MCAN_PayloadDLC_32Byte = 13U,
    MCAN_PayloadDLC_48Byte = 14U,
    MCAN_PayloadDLC_64Byte = 15U
};

// CAN-FD operation mode
enum MCAN_Mode
{
    MCAN_Mode_Classic   = 0b00U,
    MCAN_Mode_CANFD     = 0b10U,
    MCAN_Mode_CANFD_BRS = 0b11U,
    MCAN_Mode_Unspecified = 0b01U   // Not a valid value for module configuration. Only for send functions: Driver decides message mode based on module configuration.
};

// Bit clock source for MCAN module
enum MCAN_BitClkSrc
{
    MCAN_BitClkSrc_SysClk    = 0,
    MCAN_BitClkSrc_AuxClkIn  = 2,
    MCAN_BitClkSrc_PllRawClk = 3
};

// Element data section size in bytes for Tx/Rx buffers and FIFOs. 
// This is a separate enum from DataLength, which represents the actual message length
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
    MCAN_FilterType_Range   = 0,    // Match frames with ID: ID1 <= ID <= ID2
    MCAN_FilterType_DualID  = 1,    // Match frames with ID: ID = ID1 || ID = ID2
    MCAN_FilterType_ID_Mask = 2,    // Match frames with ID: (ID ^ ID1) & ID2 == 0
    MCAN_FilterType_Range_NoMask = 3 // Match frames with ID: ID1 <= ID <= ID2, without XIDAM mask. Only available for Extended ID filters
};

enum MCAN_FilterDest
{
    MCAN_FilterDest_Disable  = 0,       // Filter is not active
    MCAN_FilterDest_FIFO0    = 1,       // Matching frames are delivered to RX FIFO 0
    MCAN_FilterDest_FIFO1    = 2,       // Matching frames are delivered to RX FIFO 1
    MCAN_FilterDest_Reject   = 3,       // Matching frames are rejected
    MCAN_FilterDest_Priority = 4,       // Matching frames will set MCAN_IR.HPM interrupt flag
    MCAN_FilterDest_Priority_FIFO0 = 5, // Matching frames are delivered to RX FIFO 0, and MCAN_IR.HPM interrupt flag is set.
    MCAN_FilterDest_Priority_FIFO1 = 6, // Matching frames are delivered to RX FIFO 1, and MCAN_IR.HPM interrupt flag is set.
    MCAN_FilterDest_Buffer   = 7,       // Matching frames are delivered to dedicated RX buffer. ID1 = ID, ID2 = RX buffer INDEX. MCAN_FilterType field is ignored
};

enum MCAN_TxQueueMode
{
    MCAN_TxQueueMode_FIFO  = 0, // Message is sent in the same order it is written
    MCAN_TxQueueMode_Queue = 1  // Message with lowest Message ID is sent first
};

enum MCAN_Rx
{
    MCAN_Rx_FIFO0  = 0,
    MCAN_Rx_FIFO1  = 1,
    MCAN_Rx_Buffer = 2
};


enum MCAN_FilterNonMatch
{
    MCAN_FilterNonMatch_FIFO0   = 0,    // Non-matching frames are delivered to RX FIFO 0
    MCAN_FilterNonMatch_FIFO1   = 1,    // Non-matching frames are delivered to RX FIFO 1
    MCAN_FilterNonMatch_Discard = 2     // Non-matching frames are discarded
};


// Message structure designed for MCAN messages
struct MCAN_Message
{
    bool isRemote;
    enum MCAN_Mode mode;
    enum MCAN_ID idType;
    uint32_t id;
    uint16_t timestamp;
    enum MCAN_PayloadDLC dlc;
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
    uint16_t available; // Only used for internal calculation and debugging, not actual hardware register value
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

// void MCAN_SelectBitClkSrc(enum MCAN mcan, enum MCAN_BitClkSrc src);
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


#ifdef MCAN_LEGACY_MODE_SUPPORT
void MCAN_Legacy_Setup(void);
void MCAN_Legacy_AddFilter(uint16_t address, uint16_t dontCares, uint16_t simultaneousMsgs);
bool MCAN_Legacy_Send(struct MCAN_Legacy_canMsg *msg);
bool MCAN_Legacy_Receive(struct MCAN_Legacy_canMsg *msg);
void MCAN_Legacy_FilterAdd(uint16_t address, uint16_t dontCares, uint16_t simultaneousMsgs);
#endif /* MCAN_LEGACY_MODE_SUPPORT */

#endif /* MCAN_H */
