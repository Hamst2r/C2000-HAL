#include "mcan.h"
#include "base.h"
#include "clk.h"
#include "mcan_private.h"
#include "timer.h"
#include <math.h>



#define MCAN_SAMPLE_POINT_TARGET        (0.8f)
#define MCAN_SELFTEST_ID                (0x123U)
#define MCAN_SELFTEST_DATA_WORD0        (0x76543210UL)
#define MCAN_SELFTEST_DATA_WORD1        (0xFEDCBA98UL)


static bool MCAN_IsRxFIFOEmpty(enum MCAN mcan, enum MCAN_Rx rx);
static void MCAN_ResetMessageRAM(enum MCAN mcan);

struct MCAN_Status MCAN_status[MCAN_COUNT];

/**
 * @brief Select the source clock for an MCAN module bit timing engine.
 *
 * @param mcan Target MCAN instance.
 * @param src Bit clock source selection.
 */
static inline void MCAN_SelectBitClkSrc(enum MCAN mcan, enum MCAN_BitClkSrc src)
{
    CLK_SetCLKSRCCTL2(10+2*mcan, src);
}


/**
 * @brief Set the FCLK frequency for an MCAN module.
 *
 * @param mcan Target MCAN instance.
 * @param fclkMHz Desired FCLK frequency in MHz.
 */
static void MCAN_SetFCLK(enum MCAN mcan, float fclkMHz)
{
    uint16_t fclkDiv;
    uint16_t auxClkDivShift;

    if(fclkMHz > CLK_status.sysClkPhysicalMHz)
    {
        // FCLK cannot be faster that SYSCLK (physical). Stop initialization
        // HAL_FATAL(HAL_ErrorCode_OutOfRange, (uint32_t)fclkMHz, (uint32_t)CLK_status.sysClkPhysicalMHz);
        ESTOP0;
        while(1);
    }
    fclkDiv = roundf(CLK_status.pllOutMHz / fclkMHz);
    
    if(fclkDiv > 20)
    {
        // FCLKDIV cannot be larger than 20. Stop initialization
        // HAL_FATAL(HAL_ErrorCode_OutOfRange, (uint32_t)fclkDiv, 20U);
        ESTOP0;
        while(1);
    }

    // Bit field shift amount in AUXCLKDIV register
    auxClkDivShift = 8+mcan*5;
    
    // Write clock divider, and set clock source to PLLRAWCLK (PLL output clock)
    CLK_AUXCLKDIVSEL = CLK_AUXCLKDIVSEL 
                     & ~(0x1F << auxClkDivShift) 
                     | ((fclkDiv-1) << auxClkDivShift);
    CLK_SetCLKSRCCTL2(10+mcan*2, MCAN_BitClkSrc_PllRawClk);
    MCAN_status[mcan].clkSrc = MCAN_BitClkSrc_PllRawClk;

    MCAN_status[mcan].fclkMHz = CLK_status.pllOutMHz / fclkDiv;
}


/**
 * @brief Put an MCAN module into initialization mode.
 *
 * @param mcan Target MCAN instance.
 */
static void MCAN_EnterInitMode(enum MCAN mcan)
{
    MCAN_CCCR(mcan) |= MCAN_M_CCCR_INIT;
    while(!(MCAN_CCCR(mcan) & MCAN_M_CCCR_INIT));

    MCAN_CCCR(mcan) |= MCAN_M_CCCR_CCE;
}


/**
 * @brief Exit MCAN initialization mode to start normal operation.
 *
 * @param mcan Target MCAN instance.
 */
static void MCAN_ExitInitMode(enum MCAN mcan)
{
    MCAN_CCCR(mcan) &= ~MCAN_M_CCCR_CCE;
    
    MCAN_CCCR(mcan) &= ~MCAN_M_CCCR_INIT;
    while(MCAN_CCCR(mcan) & MCAN_M_CCCR_INIT);
}

static void MCAN_Init(enum MCAN mcan)
{
    // Wait for Message RAM to finish initialization
    while(!(MCANSS_STAT(mcan) & MCANSS_M_STAT_MEM_INIT_DONE));

    // Put MCAN in initialization mode and wait for it to take effect
    MCAN_EnterInitMode(mcan);
    
    // Enable auto-wakeup and debug suspend
    MCANSS_CTRL(mcan) = MCANSS_CTRL(mcan)
                      | MCANSS_M_CTRL_AUTOWAKEUP
                      | MCANSS_M_CTRL_WAKEUPREQEN
                      | MCANSS_M_CTRL_DBGSUSP;

    // Put settings in CCCR to default
    MCAN_CCCR(mcan) = 0x0000U | MCAN_M_CCCR_INIT | MCAN_M_CCCR_CCE;

    MCAN_ResetMessageRAM(mcan);
}


/**
 * @brief Get the element size in bytes for a given MCAN_DataSize enum.
 *
 * @param dataSize MCAN data size.
 * @return Element size in bytes.
 */
static inline uint16_t MCAN_GetElementSizeByte(enum MCAN_DataSize dataSize)
{
    switch (dataSize)
    {
        case MCAN_DataSize_8Byte:
            return  8 + 8; //  8 bytes for data, 8 bytes for metadata
        case MCAN_DataSize_12Byte:
            return 12 + 8; // 12 bytes for data, 8 bytes for metadata
        case MCAN_DataSize_16Byte:
            return 16 + 8; // 16 bytes for data, 8 bytes for metadata
        case MCAN_DataSize_20Byte:
            return 20 + 8; // 20 bytes for data, 8 bytes for metadata
        case MCAN_DataSize_24Byte:
            return 24 + 8; // 24 bytes for data, 8 bytes for metadata
        case MCAN_DataSize_32Byte:
            return 32 + 8; // 32 bytes for data, 8 bytes for metadata
        case MCAN_DataSize_48Byte:
            return 48 + 8; // 48 bytes for data, 8 bytes for metadata
        case MCAN_DataSize_64Byte:
            return 64 + 8; // 64 bytes for data, 8 bytes for metadata
    }

    return 0;
}


/**
 * @brief Get the payload length in bytes for a given CAN frame DLC.
 *
 * @param dlc Message payload DLC.
 * @return Payload length in bytes.
 */
static inline uint16_t MCAN_GetPayloadLengthByte(enum MCAN_PayloadDLC dlc)
{
    switch(dlc)
    {
        case MCAN_PayloadDLC_0Byte:
        case MCAN_PayloadDLC_1Byte:
        case MCAN_PayloadDLC_2Byte:
        case MCAN_PayloadDLC_3Byte:
        case MCAN_PayloadDLC_4Byte:
        case MCAN_PayloadDLC_5Byte:
        case MCAN_PayloadDLC_6Byte:
        case MCAN_PayloadDLC_7Byte:
        case MCAN_PayloadDLC_8Byte:
            return (uint16_t)dlc;

        case MCAN_PayloadDLC_12Byte:
            return 12;

        case MCAN_PayloadDLC_16Byte:
            return 16;

        case MCAN_PayloadDLC_20Byte:
            return 20;

        case MCAN_PayloadDLC_24Byte:
            return 24;

        case MCAN_PayloadDLC_32Byte:
            return 32;

        case MCAN_PayloadDLC_48Byte:
            return 48;

        case MCAN_PayloadDLC_64Byte:
            return 64;
    }

    return 0;
}


/**
 * @brief Pack bytes from a sparse uint16_t array (data only stored in lower byte) into a uint32_t array for writing into MCAN Message RAM.
 *
 * @param dest Destination array.
 * @param src Source array.
 * @param lengthByte Number of bytes to pack.
 */
static void MCAN_PackBytes(uint32_t* dest, const uint16_t* src, uint16_t lengthByte)
{
    for(uint16_t i=0; i<lengthByte; i+=4)
    {
        dest[i/4] =  (uint32_t)(src[i]   & 0xFF)
                  | ((uint32_t)(src[i+1] & 0xFF) << 8)
                  | ((uint32_t)(src[i+2] & 0xFF) << 16)
                  | ((uint32_t)(src[i+3] & 0xFF) << 24);
    }
}

/**
 * @brief Unpack bytes from a uint32_t array read from MCAN Message RAM into a sparse uint16_t array (data only stored in lower byte).
 *
 * @param dest Destination array.
 * @param src Source array.
 * @param lengthByte Number of bytes to unpack.
 */
static void MCAN_UnpackBytes(uint16_t* dest, const uint32_t* src, uint16_t lengthByte)
{
    for(uint16_t i=0; i<lengthByte; i+=4)
    {
        dest[i]   = src[i/4]       & 0xFF;
        dest[i+1] = src[i/4] >>  8 & 0xFF;
        dest[i+2] = src[i/4] >> 16 & 0xFF;
        dest[i+3] = src[i/4] >> 24       ;
    }
}


/**
 * @brief Reset Message RAM allocation and clear all TX/RX/filter section setup.
 *
 * @param mcan Target MCAN instance.
 */
static void MCAN_ResetMessageRAM(enum MCAN mcan)
{
    MCAN_status[mcan].msgRam.allocated = 0;
    MCAN_status[mcan].msgRam.available = MCAN_MSG_RAM_DEPTH_BYTE;
    MCAN_SetupTx(mcan, MCAN_TxQueueMode_FIFO, MCAN_DataSize_8Byte, 0, 0);
    MCAN_SetupRx(mcan, MCAN_Rx_Buffer, MCAN_DataSize_8Byte, 0);
    MCAN_SetupRx(mcan, MCAN_Rx_FIFO0, MCAN_DataSize_8Byte, 0);
    MCAN_SetupRx(mcan, MCAN_Rx_FIFO1, MCAN_DataSize_8Byte, 0);
    MCAN_SetupFilter(mcan, MCAN_ID_Standard, MCAN_FilterNonMatch_Discard, false);
    MCAN_SetupFilter(mcan, MCAN_ID_Extended, MCAN_FilterNonMatch_Discard, false);
}


/**
 * @brief Allocate a section of Message RAM for TX/RX buffers or filters.
 *
 * @param mcan Target MCAN instance.
 * @param sizeByte Size of the section to allocate in bytes.
 * @return Starting byte offset of the allocated section from the beginning of Message RAM, or -1 if allocation fails due to insufficient Message RAM.
 */
static int16_t MCAN_AllocateMsgRam(enum MCAN mcan, uint16_t sizeByte)
{
    uint16_t sectionOffset;
    
    uint16_t sizeByteAligned = (sizeByte + 3) & ~0x3; // Align to 4 bytes since smallest addressable unit in Message RAM is 4 bytes

    if((MCAN_status[mcan].msgRam.allocated + sizeByteAligned) > MCAN_MSG_RAM_DEPTH_BYTE)
    {
        // Message RAM size exceed. Return -1 to indicate error
        return -1;
    }

    sectionOffset = MCAN_status[mcan].msgRam.allocated;
    MCAN_status[mcan].msgRam.allocated += sizeByteAligned;
    MCAN_status[mcan].msgRam.available = MCAN_MSG_RAM_DEPTH_BYTE - MCAN_status[mcan].msgRam.allocated;

    return sectionOffset;
}


/**
 * @brief Set arbitration phase timing parameters.
 *
 * @param mcan Target MCAN instance.
 * @param arbiPrescale Arbitration phase prescaler.
 * @param arbiKbps Arbitration phase bit rate in kbps.
 */
static void MCAN_SetArbiTiming(enum MCAN mcan, uint16_t arbiPrescale, float arbiKbps)
{
    float arbiClkKHz;
    uint16_t arbiQuanta;
    uint16_t sjw, tseg1, tseg2;

    if(arbiKbps > 1000)
    {
        // Arbitration phase bit rate cannot exceed 1Mbps. Stop initialization
        // HAL_FATAL(HAL_ErrorCode_OutOfRange, (uint32_t)arbiKbps, 1000U);
        ESTOP0;
        while(1);
    }

    arbiClkKHz = MCAN_status[mcan].fclkMHz * 1000 / arbiPrescale;
    arbiQuanta = roundf(arbiClkKHz / arbiKbps);

    if((arbiQuanta < 5) || (arbiQuanta > 280))
    {
        // Number of time quanta is outside permissable range, consider adjusting prescale ratio. Stop initialization
        // HAL_FATAL(HAL_ErrorCode_OutOfRange, (uint32_t)arbiQuanta, (uint32_t)arbiPrescale);
        ESTOP0;
        while(1);
    }

    // Calculate TSEG2 length for achieving target Sample Point position
    // Subtracting 1 from arbiQuanta to account for Sync Segment which is always 1 TQ
    tseg2 = roundf(arbiQuanta * (1-MCAN_SAMPLE_POINT_TARGET));
    tseg1 = arbiQuanta - 1 - tseg2;
    sjw = (tseg2 > 4) ? 4 : tseg2;  // SJW cannot be larger than 4

    // Value written to register in 1 less than the desired value (eg. writing 4 means 5)
    MCAN_NBTP(mcan) = MCAN_S_NBTP_NSJW(sjw-1) | MCAN_S_NBTP_NBRP(arbiPrescale-1) | MCAN_S_NBTP_NTSEG1(tseg1-1) | MCAN_S_NBTP_NTSEG2(tseg2-1);

    MCAN_status[mcan].arbiQuanta = arbiQuanta;
    MCAN_status[mcan].arbiKbps   = arbiClkKHz / arbiQuanta;
}


/**
 * @brief Set data phase timing parameters.
 *
 * @param mcan Target MCAN instance.
 * @param dataKbps Data phase bit rate in kbps.
 */
static void MCAN_SetDataTiming(enum MCAN mcan, float dataKbps)
{
    uint16_t dataQuanta;
    uint16_t sjw, tseg1, tseg2;

    if(dataKbps > 5000)
    {
        // Data phase bit rate cannot exceed 5Mbps. Stop initialization
        // HAL_FATAL(HAL_ErrorCode_OutOfRange, (uint32_t)dataKbps, 5000U);
        ESTOP0;
        while(1);
    }

    // In this library, data bit rate prescale is not used
    dataQuanta = roundf(MCAN_status[mcan].fclkMHz * 1000 / dataKbps);

    if((dataQuanta < 5) || (dataQuanta > 40))
    {
        // Number of time quanta is outside permissable range. Stop initialization
        // HAL_FATAL(HAL_ErrorCode_OutOfRange, (uint32_t)dataQuanta, 40U);
        ESTOP0;
        while(1);
    }

    // Calculate TSEG2 length for achieving target Sample Point position. 
    // Subtracting 1 from dataQuanta to account for Sync Segment which is always 1 TQ
    tseg2 = roundf(dataQuanta * (1-MCAN_SAMPLE_POINT_TARGET));
    tseg1 = dataQuanta - 1 - tseg2;
    sjw = (tseg2 > 4) ? 4 : tseg2;  // SJW is equal to or less than TSEG2, and cannot be larger than 4

    // Write timing information and enable Transmitter Delay Compensation (TDC)
    // Value written to register in 1 less than the desired value (eg. writing 4 means 5)
    MCAN_DBTP(mcan) = MCAN_M_DBTP_DSJW_TDC | MCAN_S_DBTP_DSJW(sjw-1) | MCAN_S_DBTP_DTSEG1(tseg1-1) | MCAN_S_DBTP_DTSEG2(tseg2-1);

    // Configure TDC. According to E2E, both values should be equal to DTSEG1 as starting point;
    MCAN_TDCR(mcan) = MCAN_S_TDCR_TDCO(tseg1) | MCAN_S_TDCR_TDCF(tseg1);
    
    MCAN_status[mcan].dataQuanta = dataQuanta;
    MCAN_status[mcan].dataKbps   = MCAN_status[mcan].fclkMHz * 1000 / dataQuanta;
}

uint32_t txTimeUs;

/**
 * @brief Perform a self-test of the MCAN module.
 *
 * @param mcan Target MCAN instance.
 */
static void MCAN_SelfTest(enum MCAN mcan)
{
    struct MCAN_Message txMessage = {0};
    struct MCAN_Message rxMessage = {0};
    uint32_t savedCCCR;
    uint32_t arbiTxTimeUs;
    uint32_t dataTxTimeUs;
    int32_t timeout;

    savedCCCR = MCAN_CCCR(mcan);

    MCAN_ResetMessageRAM(mcan);
    MCAN_SetupTx(mcan, MCAN_TxQueueMode_FIFO, MCAN_DataSize_8Byte, 0, 1);
    MCAN_SetupRx(mcan, MCAN_Rx_FIFO0, MCAN_DataSize_8Byte, 1);
    MCAN_SetupFilter(mcan, MCAN_ID_Standard, MCAN_FilterNonMatch_FIFO0, false);

    txMessage.isRemote = false;
    txMessage.mode = MCAN_status[mcan].mode;
    txMessage.idType = MCAN_ID_Standard;
    txMessage.id = MCAN_SELFTEST_ID;
    txMessage.dlc = MCAN_PayloadDLC_8Byte;
    txMessage.data[0] = MCAN_SELFTEST_DATA_WORD0;
    txMessage.data[1] = MCAN_SELFTEST_DATA_WORD1;

    // Enter internal loopback mode (Bus monitor + Loopback) and test mode for self-test.
    // Keep TEST.TX at normal value (0) so the internal TX state machine can run.
    MCAN_CCCR(mcan) |= MCAN_M_CCCR_MON | MCAN_M_CCCR_TEST;
    MCAN_TEST(mcan) = MCAN_M_TEST_LBCK;

    MCAN_ExitInitMode(mcan);

    if(!MCAN_SendQueue(mcan, &txMessage))
    {
        // HAL_FATAL(HAL_ErrorCode_HardwareFault, (uint32_t)mcan, 1U);
        ESTOP0;
        while(1);
    }

    arbiTxTimeUs = 47.0 / MCAN_status[mcan].arbiKbps * 1000.0 + 10;// Add 10us margin for processing delay
    dataTxTimeUs = 64.0 / (MCAN_status[mcan].mode == MCAN_Mode_CANFD_BRS ? MCAN_status[mcan].dataKbps : MCAN_status[mcan].arbiKbps) * 1000;

    txTimeUs = arbiTxTimeUs + dataTxTimeUs;

    // timeout for waiting for message to be received. Set to 3 times of the expected transmission time.
    timeout = TIMER_GetExpiryTime(3*txTimeUs);

    while(MCAN_IsRxFIFOEmpty(mcan, MCAN_Rx_FIFO0))
    {
       if (TIMER_HasExpired(timeout)) {
           // Do not receive message within expected timeframe, self-test failed
           // HAL_FATAL(HAL_ErrorCode_Timeout, (uint32_t)mcan, 2U);
           ESTOP0;
           while(1);
       }
    }

    MCAN_ReadRxFIFO(mcan, MCAN_Rx_FIFO0, &rxMessage);

    if((rxMessage.id  != txMessage.id )
    || (rxMessage.dlc != txMessage.dlc)
    || (rxMessage.idType  != txMessage.idType)
    || (rxMessage.data[0] != txMessage.data[0])
    || (rxMessage.data[1] != txMessage.data[1]))
    {
        // Received message does not match transmitted message, self-test failed
        // HAL_FATAL(HAL_ErrorCode_HardwareFault, (uint32_t)mcan, 3U);
        ESTOP0;
        while(1);
    }

    MCAN_EnterInitMode(mcan);
    MCAN_CCCR(mcan) = savedCCCR;

    MCAN_ResetMessageRAM(mcan);
}

/**
 * @brief Configure an MCAN module for CAN-FD operation.
 *
 * The function initializes MCAN, configures arbitration timing, 
 * optionally enables bit-rate switching (BRS) when arbitration and data bitrates differ,
 * then runs internal loopback self-test.
 *
 * @param mcan Target MCAN instance.
 * @param fclkMHz MCAN functional clock in MHz.
 * @param arbiPrescale Arbitration clock prescaler.
 * @param arbiKbps Arbitration phase bitrate in kbps.
 * @param dataKbps Data phase bitrate in kbps.
 */
void MCAN_SetupCANFD(enum MCAN mcan, float fclkMHz, uint16_t arbiPrescale, float arbiKbps, float dataKbps)
{
    MCAN_SetFCLK(mcan, fclkMHz);
    MCAN_Init(mcan);
    
    MCAN_status[mcan].mode = MCAN_Mode_CANFD;

    // Set MCAN module to CAN-FD mode
    MCAN_CCCR(mcan) |= MCAN_M_CCCR_FDOE;

    // Configure bit timing
    MCAN_SetArbiTiming(mcan, arbiPrescale, arbiKbps);
    
    // Enable BRS (bit rate switching) if Data phase bit rate is different from Arbitration phase
    if(arbiKbps != dataKbps)
    {
        MCAN_CCCR(mcan) |= MCAN_M_CCCR_BRSE;
        MCAN_SetDataTiming(mcan, dataKbps);
        MCAN_status[mcan].mode = MCAN_Mode_CANFD_BRS;
    }

    MCAN_SelfTest(mcan);
}


/**
 * @brief Configure an MCAN module for Classical CAN operation.
 *
 * @param mcan Target MCAN instance.
 * @param fclkMHz MCAN functional clock in MHz.
 * @param arbiPrescale Arbitration clock prescaler.
 * @param bitrateKbps Classical CAN bitrate in kbps.
 */
void MCAN_SetupClassic(enum MCAN mcan, float fclkMHz, uint16_t arbiPrescale, float bitrateKbps)
{
    MCAN_SetFCLK(mcan, fclkMHz);
    MCAN_Init(mcan);

    MCAN_status[mcan].mode = MCAN_Mode_Classic;

    // Configure bit timing
    MCAN_SetArbiTiming(mcan, arbiPrescale, bitrateKbps);

    MCAN_SelfTest(mcan);
}

/**
 * @brief Start normal MCAN operation.
 *
 * Leaves initialization mode so the configured MCAN module can participate
 * on the bus.
 *
 * @param mcan Target MCAN instance.
 */
void MCAN_Start(enum MCAN mcan)
{
    MCAN_ExitInitMode(mcan);
}



/**
 * @brief Configure TX message RAM section and queue mode.
 *
 * @param mcan Target MCAN instance.
 * @param mode TX FIFO or queue arbitration mode.
 * @param dataSize Element payload capacity for TX objects.
 * @param dBufferDepth Number of dedicated TX buffers.
 * @param queueDepth Number of TX queue/FIFO elements.
 */
void MCAN_SetupTx(enum MCAN mcan, enum MCAN_TxQueueMode mode, enum MCAN_DataSize dataSize, uint16_t dBufferDepth, uint16_t queueDepth)
{
    int16_t startAddr;
    if((dBufferDepth + queueDepth) > 32)
    {
        // Sum of dedicated Tx Buffer and Tx Queue cannot exceed 32. Stop initialization
        // HAL_FATAL(HAL_ErrorCode_InvalidArgument, (uint32_t)dBufferDepth, (uint32_t)queueDepth);
        ESTOP0;
        while(1);
    }


    // Allocate Message RAM for TX buffer
    startAddr = MCAN_AllocateMsgRam(mcan, MCAN_GetElementSizeByte(dataSize) * (dBufferDepth + queueDepth));

    if(startAddr == -1)
    {
        // Unable to allocate Message RAM for TX Buffer. Stop initialization
        // HAL_FATAL(HAL_ErrorCode_ResourceConflict, (uint32_t)dataSize, (uint32_t)(dBufferDepth + queueDepth));
        ESTOP0;
        while(1);
    }

    MCAN_status[mcan].msgRam.startAddr.txBuffer = startAddr;
    MCAN_status[mcan].msgRam.elementSizeByte.txBuffer = MCAN_GetElementSizeByte(dataSize);
    MCAN_status[mcan].msgRam.nElement.txBuffer = dBufferDepth + queueDepth;

    MCAN_TXESC(mcan) = dataSize;
    MCAN_TXBC(mcan) = MCAN_S_TXBC_TFQM(mode)
                    | MCAN_S_TXBC_TFQS(queueDepth)
                    | MCAN_S_TXBC_NDTB(dBufferDepth)
                    | MCAN_S_SA(startAddr >> 2);
}

// void MCAN_AssignTxBuffer(enum MCAN mcan, uint16_t bufferNum, uint32_t messageID, enum MCAN_ID idType, bool isRemote)
// {
    
// }


/**
 * @brief Configure RX buffer/FIFO message RAM section.
 *
 * @param mcan Target MCAN instance.
 * @param rx RX destination section to configure.
 * @param dataSize Element payload capacity for RX objects.
 * @param depth Number of elements in the selected section.
 */
void MCAN_SetupRx(enum MCAN mcan, enum MCAN_Rx rx, enum MCAN_DataSize dataSize, uint16_t depth)
{
    int16_t startAddr;

    if (depth > 64) 
    {
        // Each RX Section is limited to 64 elements. Stop initialization
        // HAL_FATAL(HAL_ErrorCode_OutOfRange, (uint32_t)depth, 64U);
        ESTOP0;
        while(1);

    }

    startAddr = MCAN_AllocateMsgRam(mcan, MCAN_GetElementSizeByte(dataSize) * depth);

    if(startAddr == -1)
    {
        // Unable to allocate Message RAM. Stop initialization
        // HAL_FATAL(HAL_ErrorCode_ResourceConflict, (uint32_t)rx, (uint32_t)depth);
        ESTOP0;
        while(1);
    }

    switch (rx) 
    {
        case MCAN_Rx_Buffer:
            MCAN_status[mcan].msgRam.startAddr.rxBuffer = startAddr;
            MCAN_status[mcan].msgRam.elementSizeByte.rxBuffer = MCAN_GetElementSizeByte(dataSize);
            MCAN_status[mcan].msgRam.nElement.rxBuffer = depth;
            MCAN_RXESC(mcan) =  MCAN_RXESC(mcan)
                             & ~MCAN_M_RXESC_RBDS
                             |  MCAN_S_RXESC_RBDS(dataSize);
            MCAN_RXBC(mcan) = MCAN_S_SA(startAddr >> 2);   
            break;
        
        case MCAN_Rx_FIFO0:
            MCAN_status[mcan].msgRam.startAddr.rxFIFO0 = startAddr;
            MCAN_status[mcan].msgRam.elementSizeByte.rxFIFO0 = MCAN_GetElementSizeByte(dataSize);
            MCAN_status[mcan].msgRam.nElement.rxFIFO0 = depth;
            MCAN_RXESC(mcan) =  MCAN_RXESC(mcan)
                             & ~MCAN_M_RXESC_F0DS
                             |  MCAN_S_RXESC_F0DS(dataSize);
            MCAN_RXF0C(mcan) = MCAN_S_RXFxC_FxS(depth)
                             | MCAN_S_SA(startAddr >> 2);
            break;
        
        case MCAN_Rx_FIFO1:
            MCAN_status[mcan].msgRam.startAddr.rxFIFO1 = startAddr;
            MCAN_status[mcan].msgRam.elementSizeByte.rxFIFO1 = MCAN_GetElementSizeByte(dataSize);
            MCAN_status[mcan].msgRam.nElement.rxFIFO1 = depth;
            MCAN_RXESC(mcan) =  MCAN_RXESC(mcan)
                             & ~MCAN_M_RXESC_F1DS
                             |  MCAN_S_RXESC_F1DS(dataSize);
            MCAN_RXF1C(mcan) = MCAN_S_RXFxC_FxS(depth)
                             | MCAN_S_SA(startAddr >> 2);
            break;
        
    }

}


// void MCAN_AssignRxBuffer(enum MCAN mcan, uint16_t bufferNum, uint32_t messageID)
// {
    
// }


/**
 * @brief Initialize standard or extended filter table base configuration.
 *
 * This sets filter table base address/count and global behavior for non-match
 * and remote frames.
 *
 * @param mcan Target MCAN instance.
 * @param filter Filter table type (standard or extended).
 * @param nonMatchDest Destination for frames not matching any filter.
 * @param rejectRemote Whether remote frames are rejected.
 */
void MCAN_SetupFilter(enum MCAN mcan, enum MCAN_ID filter, enum MCAN_FilterNonMatch nonMatchDest, bool rejectRemote)
{
    int16_t startAddr;
    
    startAddr = MCAN_AllocateMsgRam(mcan, 0);
    if(startAddr == -1)
    {
        // Unable to allocate Message RAM. Stop initialization
        // HAL_FATAL(HAL_ErrorCode_ResourceConflict, (uint32_t)filter, 0U);
        ESTOP0;
        while(1);
    }

    if(filter == MCAN_ID_Standard)
    {
        MCAN_status[mcan].msgRam.startAddr.filterStd = startAddr;
        MCAN_status[mcan].msgRam.nElement.filterStd = 0;
        MCAN_SIDFC(mcan) = MCAN_S_SIDFC_LSS(0) | MCAN_S_SA(startAddr >> 2);
        MCAN_GFC(mcan) = MCAN_GFC(mcan)
                       & ~(MCAN_M_GFC_ANFS | MCAN_M_GFC_RRFS)
                       | MCAN_S_GFC_ANFS(nonMatchDest)
                       | MCAN_S_GFC_RRFS(rejectRemote);
    } else {
        MCAN_status[mcan].msgRam.startAddr.filterExt = startAddr;
        MCAN_status[mcan].msgRam.nElement.filterExt = 0;
        MCAN_XIDFC(mcan) = MCAN_S_XIDFC_LSE(0) | MCAN_S_SA(startAddr >> 2);
        MCAN_GFC(mcan) = MCAN_GFC(mcan)
                       & ~(MCAN_M_GFC_ANFE | MCAN_M_GFC_RRFE)
                       | MCAN_S_GFC_ANFE(nonMatchDest)
                       | MCAN_S_GFC_RRFE(rejectRemote);
    }
}


/**
 * @brief Reset filter configuration, including standard and extended filters.
 *
 * Only available if filters are placed at the end of Message RAM allocation (ie. filters are allocated after all TX/RX buffers)
 * MCAN module must be in INIT mode before calling this function
 *
 * @param mcan Target MCAN instance.
 * @return true if filters are successfully reset, false if filters cannot be reset due to filter table not being placed at the end of Message RAM allocation.
 */
bool MCAN_ResetFilters(enum MCAN mcan)
{
    uint16_t highestBufferStartAddr = MCAN_status[mcan].msgRam.startAddr.txBuffer;

    highestBufferStartAddr = (MCAN_status[mcan].msgRam.startAddr.rxBuffer > highestBufferStartAddr) ? MCAN_status[mcan].msgRam.startAddr.rxBuffer : highestBufferStartAddr;
    highestBufferStartAddr = (MCAN_status[mcan].msgRam.startAddr.rxFIFO0  > highestBufferStartAddr) ? MCAN_status[mcan].msgRam.startAddr.rxFIFO0  : highestBufferStartAddr;
    highestBufferStartAddr = (MCAN_status[mcan].msgRam.startAddr.rxFIFO1  > highestBufferStartAddr) ? MCAN_status[mcan].msgRam.startAddr.rxFIFO1  : highestBufferStartAddr;

    // Check if filters are placed at the end of Message RAM allocation. If not, new sets of filters may corrupt TX/RX buffers and lead to unexpected behavior. Stop initialization if this is the case.
    if((MCAN_status[mcan].msgRam.nElement.filterStd > 0 && MCAN_status[mcan].msgRam.startAddr.filterStd < highestBufferStartAddr)
    || (MCAN_status[mcan].msgRam.nElement.filterExt > 0 && MCAN_status[mcan].msgRam.startAddr.filterExt < highestBufferStartAddr))
    {
        return false;
    }

    // Clear filter count to 0 to logically remove all filters. 
    // The actual filter entries in Message RAM will be overwritten when new filters are added, so no need to explicitly clear them.
    MCAN_SIDFC(mcan) = MCAN_S_SIDFC_LSS(0);
    MCAN_status[mcan].msgRam.nElement.filterStd = 0;
    MCAN_XIDFC(mcan) = MCAN_S_XIDFC_LSE(0);
    MCAN_status[mcan].msgRam.nElement.filterExt = 0;

    // Update Message RAM allocation. Since filters are on top of the allocation, simply move the allocation pointer back to the starting address of filters.
    MCAN_status[mcan].msgRam.allocated = (MCAN_status[mcan].msgRam.startAddr.filterStd < MCAN_status[mcan].msgRam.startAddr.filterExt) ? MCAN_status[mcan].msgRam.startAddr.filterStd : MCAN_status[mcan].msgRam.startAddr.filterExt;
    MCAN_status[mcan].msgRam.available = MCAN_MSG_RAM_DEPTH_BYTE - MCAN_status[mcan].msgRam.allocated;

    return true;
}

/**
 * @brief Append one filter entry to the selected filter table.
 *
 * This function must be called immediately after MCAN_SetupFilter() call of the corresponding filter type
 *
 * @param mcan Target MCAN instance.
 * @param filter Filter table type (standard or extended).
 * @param type Filter matching mode.
 * @param dest Filter destination/action.
 * @param id1 First ID field, meaning depends on filter type.
 * @param id2 Second ID field, meaning depends on filter type.
 */
void MCAN_AddFilter(enum MCAN mcan, enum MCAN_ID filter, enum MCAN_FilterType type, enum MCAN_FilterDest dest, uint32_t id1, uint32_t id2)
{
    uint16_t existingCount, filterLimit;
    uint16_t filterSizeByte;
    int32_t filterAddr;

    if(filter == MCAN_ID_Standard)
    {
        filterSizeByte = 4;
        filterLimit = 128;
        existingCount = (MCAN_SIDFC(mcan) & MCAN_M_SIDFC_LSS) >> 16;
    } else {
        filterSizeByte = 8;
        filterLimit = 64;
        existingCount = (MCAN_XIDFC(mcan) & MCAN_M_XIDFC_LSE) >> 16;
    }


    if(existingCount >= filterLimit)
    {
        // Maximum filter count reached. Stop initialization
        // HAL_FATAL(HAL_ErrorCode_ResourceConflict, (uint32_t)existingCount, (uint32_t)filterLimit);
        ESTOP0;
        while(1);
    }

    filterAddr = MCAN_AllocateMsgRam(mcan, filterSizeByte);
    if(filterAddr == -1)
    {
        // Unable to allocate Message RAM. Stop initialization
        // HAL_FATAL(HAL_ErrorCode_ResourceConflict, (uint32_t)filter, (uint32_t)filterSizeByte);
        ESTOP0;
        while(1);
    }
    
    filterAddr += MCAN_MSG_RAM_BASE(mcan);

    if(filter == MCAN_ID_Standard)
    {
        ATOMIC32(filterAddr) = (uint32_t)type << 30
                             | (uint32_t)dest << 27
                             | (uint32_t)id1  << 16
                             | (uint32_t)id2;
        MCAN_SIDFC(mcan) = MCAN_SIDFC(mcan) & ~MCAN_M_SIDFC_LSS | MCAN_S_SIDFC_LSS(existingCount + 1);
        MCAN_status[mcan].msgRam.nElement.filterStd = existingCount + 1;
    } else {
        ATOMIC32(filterAddr) = (uint32_t)type << 31
                             | (uint32_t)dest << 30
                             | (uint32_t)id1;
        ATOMIC32(filterAddr + 4) = id2;
        MCAN_XIDFC(mcan) = MCAN_XIDFC(mcan) & ~MCAN_M_XIDFC_LSE | MCAN_S_XIDFC_LSE(existingCount + 1);
        MCAN_status[mcan].msgRam.nElement.filterExt = existingCount + 1;
    }

}

/**
 * @brief Write a TX message to the element in specified address.
 *
 * @param mcan Target MCAN instance.
 * @param elementAddr Address of the TX element.
 * @param message Message to write.
 */
static bool MCAN_WriteTxElement(enum MCAN mcan, uint32_t elementAddr, struct MCAN_Message *message)
{
    uint32_t messageID;
    enum MCAN_Mode mode;
    uint16_t dataLengthByte;
    uint16_t dataLengthWord;
    uint32_t t0, t1;

    // If message ID is standard and has value in the lower 11 bits, it should be shifted to bits [28:18] of the ID field in Message RAM
    if(message->idType == MCAN_ID_Standard && (message->id & MASK(11)) !=0)
    {
        messageID = (message->id << 18) & MASK(29);
    } else {
        messageID = message->id & MASK(29);
    }

    // If message mode is unspecified or higher than current MCAN mode, use current MCAN mode as default. 
    // This allows user to set up messages without explicitly specifying mode when the message is compatible with current MCAN mode, 
    // and also provides flexibility for user to prepare messages for different MCAN modes in advance.
    if(message->mode == MCAN_Mode_Unspecified || message->mode > MCAN_status[mcan].mode)
    {
        mode = MCAN_status[mcan].mode;
    } else {
        mode = message->mode;
    }

    // Error: Classical CAN does not support payload larger than 8 bytes.
    if((message->dlc > MCAN_PayloadDLC_8Byte) && (mode == MCAN_Mode_Classic))
    {
        // HAL_FATAL(HAL_ErrorCode_OutOfRange, (uint32_t)message->dlc, (uint32_t)MCAN_PayloadDLC_8Byte);
        // ESTOP0;
        // while(1);
        return false;
    }

    dataLengthByte = MCAN_GetPayloadLengthByte(message->dlc);

    if(dataLengthByte > MCAN_status[mcan].msgRam.elementSizeByte.txBuffer - 8) // Each element has 8 bytes of metadata, so subtract 8 from total element size to get max data payload
    {
        // Message length exceeds TX element data capacity. Stop execution
        // HAL_FATAL(HAL_ErrorCode_OutOfRange, (uint32_t)dataLengthByte, MCAN_status[mcan].msgRam.elementSizeByte.txBuffer - 8);
        // ESTOP0;
        // while(1);
        return false;
    }

    t0 = (uint32_t)message->idType   << 30
       | (uint32_t)message->isRemote << 29
       | (uint32_t)messageID;

    t1 = (uint32_t)mode << 20
       | (uint32_t)message->dlc << 16;

    ATOMIC32(elementAddr) = t0;
    ATOMIC32(elementAddr + 4U) = t1;

    dataLengthWord = (dataLengthByte + 3U) >> 2;

    for(uint16_t i=0; i<dataLengthWord; i++)
    {
        ATOMIC32(elementAddr + 8U + ((uint32_t)i * 4U)) = message->data[i];
    }
    return true;
}

// bool MCAN_SendBuffer(enum MCAN mcan, uint16_t index, struct MCAN_Message *message);
/**
 * @brief Push one message to TX queue/FIFO and request transmission.
 *
 * @param mcan Target MCAN instance.
 * @param message Message to transmit.
 * @return true Message accepted for transmission.
 * @return false TX queue/FIFO is full.
 */
bool MCAN_SendQueue(enum MCAN mcan, struct MCAN_Message *message)
{
    bool txFull;
    uint16_t putIndex;
    uint32_t elementAddr;


    txFull = (MCAN_TXFQS(mcan) & MCAN_M_TXFQS_TFQF) != 0;

    if(txFull)
    {
        return false;
    }

    putIndex = (MCAN_TXFQS(mcan) & MCAN_M_TXFQS_TFQP) >> 16;
    elementAddr = MCAN_MSG_RAM_BASE(mcan)
                + MCAN_status[mcan].msgRam.startAddr.txBuffer
                + (uint32_t)MCAN_status[mcan].msgRam.elementSizeByte.txBuffer * putIndex;


    if(!MCAN_WriteTxElement(mcan, elementAddr, message))
    {
        return false;
    }

    MCAN_TXBAR(mcan) = 1UL << putIndex;

    return true;

}

/**
 * @brief Check whether the selected RX FIFO currently contains any messages.
 *
 * @param mcan Target MCAN instance.
 * @param rx RX FIFO selector.
 * @return true Selected FIFO is empty.
 * @return false Selected FIFO has at least one pending message.
 */
static inline bool MCAN_IsRxFIFOEmpty(enum MCAN mcan, enum MCAN_Rx rx)
{
    if (rx == MCAN_Rx_FIFO0) 
    {
        return (MCAN_RXF0S(mcan) & MCAN_M_RXFxS_FxFL) == 0;
    } else {
        return (MCAN_RXF1S(mcan) & MCAN_M_RXFxS_FxFL) == 0;
    }
}

/**
 * @brief Read one element from the RX FIFO or buffer.
 *
 * @param mcan Target MCAN instance.
 * @param elementAddr Absolute address of the element (MCU address space) to read.
 * @param message Output message structure.
 */
static void MCAN_ReadRxElement(uint32_t elementAddr, struct MCAN_Message *message)
{
    uint16_t dataLengthByte;
    uint16_t dataLengthWord;
    uint32_t t0, t1;
    uint32_t messageID;

    t0 = ATOMIC32(elementAddr);
    t1 = ATOMIC32(elementAddr + 4);

    message->isRemote = (bool)        ((t0 >> 29) & 0x1);
    message->idType   = (enum MCAN_ID)((t0 >> 30) & 0x1);

    if(message->idType == MCAN_ID_Standard)
    {
        messageID = (t0 >> 18) & MASK(11);
    } else {
        messageID = t0 & MASK(29);
    }
    
    message->id     = messageID;
    message->timestamp = t1 & MASK(16);
    message->mode   = (enum MCAN_Mode)      ((t1 >> 20) & MASK(2));
    message->dlc    = (enum MCAN_PayloadDLC)((t1 >> 16) & MASK(4));

    dataLengthByte = MCAN_GetPayloadLengthByte(message->dlc);
    dataLengthWord = (dataLengthByte + 3U) >> 2;
    if(dataLengthByte > MCAN_MAX_DATA_BYTES)
    {
        // Message length exceeds maximum data bytes. Stop execution
        // HAL_FATAL(HAL_ErrorCode_OutOfRange, (uint32_t)dataLengthByte, MCAN_MAX_DATA_BYTES);
        ESTOP0;
        while(1);
    }

    for(uint16_t i=0; i<dataLengthWord; i++)
    {
        message->data[i] = ATOMIC32(elementAddr + 8U + ((uint32_t)i * 4U));
    }
}

/**
 * @brief Read and acknowledge one message from RX FIFO0 or RX FIFO1.
 *
 * @param mcan Target MCAN instance.
 * @param rx RX FIFO selector.
 * @param message Output message structure.
 */
void MCAN_ReadRxFIFO(enum MCAN mcan, enum MCAN_Rx rx, struct MCAN_Message *message)
{
    uint32_t elementAddr;
    uint16_t getIndex;

    switch (rx) 
    {
        case MCAN_Rx_FIFO0:
            // Get read index from RXF0S register
            getIndex = (MCAN_RXF0S(mcan) & MCAN_M_RXFxS_FxGI) >> 8;

            elementAddr = MCAN_MSG_RAM_BASE(mcan)
                        + MCAN_status[mcan].msgRam.startAddr.rxFIFO0
                        + (uint32_t)MCAN_status[mcan].msgRam.elementSizeByte.rxFIFO0 * getIndex;
                        
            MCAN_ReadRxElement(elementAddr, message);
            MCAN_RXF0A(mcan) = MCAN_S_RXFxA_FxAI(getIndex);
            break;
        
        case MCAN_Rx_FIFO1:
            // Get read index from RXF1S register
            getIndex = (MCAN_RXF1S(mcan) & MCAN_M_RXFxS_FxGI) >> 8;

            elementAddr = MCAN_MSG_RAM_BASE(mcan)
                        + MCAN_status[mcan].msgRam.startAddr.rxFIFO1
                        + (uint32_t)MCAN_status[mcan].msgRam.elementSizeByte.rxFIFO1 * getIndex;
                        
            MCAN_ReadRxElement(elementAddr, message);
            MCAN_RXF1A(mcan) = MCAN_S_RXFxA_FxAI(getIndex);
            break;
        
        default:
            // Invalid RX type. Stop execution
            // HAL_FATAL(HAL_ErrorCode_InvalidArgument, (uint32_t)rx, 0U);
            ESTOP0;
            while(1);
    }

}

/**
 * @brief Read one message from a dedicated RX buffer.
 *
 * @param mcan Target MCAN instance.
 * @param bufferNum RX buffer index.
 * @param message Output message structure.
 */
void MCAN_ReadRxBuffer(enum MCAN mcan, uint16_t bufferNum, struct MCAN_Message *message)
{
    uint32_t elementAddr;

    if(bufferNum >= MCAN_status[mcan].msgRam.nElement.rxBuffer)
    {
        // Invalid buffer number. Stop execution
        // HAL_FATAL(HAL_ErrorCode_InvalidArgument, (uint32_t)bufferNum, MCAN_status[mcan].msgRam.nElement.rxBuffer);
        ESTOP0;
        while(1);
    }

    elementAddr = MCAN_MSG_RAM_BASE(mcan)
                + MCAN_status[mcan].msgRam.startAddr.rxBuffer
                + (uint32_t)MCAN_status[mcan].msgRam.elementSizeByte.rxBuffer * bufferNum;

    MCAN_ReadRxElement(elementAddr, message);

    MCAN_NDAT(mcan, bufferNum) = 1U << (bufferNum & MASK(5)); // Acknowledge the message by setting the corresponding bit in NDAT1/NDAT2 register

}



#ifdef MCAN_LEGACY_MODE_SUPPORT

#define MCAN_LEGACY_MODULE  MCAN_A

/**
 * @brief Configure legacy compatibility mode defaults.
 *
 * Sets classical CAN timing and default FIFO depths used by legacy wrappers.
 *
 * @param mcan Target MCAN instance.
 */
void MCAN_Legacy_Setup(void)
{
    enum MCAN mcan = MCAN_LEGACY_MODULE;

    MCAN_SetupClassic(mcan, 10, 4, 125);

    MCAN_SetupTx(mcan, MCAN_TxQueueMode_FIFO, MCAN_DataSize_8Byte, 0, 32);
    MCAN_SetupRx(mcan, MCAN_Rx_FIFO0, MCAN_DataSize_8Byte, 64);

    MCAN_ExitInitMode(mcan);
}


/**
 * @brief Configure legacy standard-ID acceptance filter.
 *
 * If simultaneousMsgs = 0, clear all standard filters and discard non-matching frames.
 *
 * @param address Standard ID to accept.
 * @param dontCares Bits that the filter should ignore
 * @param simultaneousMsgs Nonzero configures a filter, zero resets all filters.
 */
void MCAN_Legacy_FilterAdd(uint16_t address, uint16_t dontCares, uint16_t simultaneousMsgs)
{
    uint16_t filterID;
    uint16_t filterMask;

    enum MCAN mcan = MCAN_LEGACY_MODULE;

    filterID = address & MASK(11);
    filterMask = (~dontCares) & MASK(11);

    MCAN_EnterInitMode(mcan);

    if(!simultaneousMsgs)
    {
        // Reset standard filter table and discard non-matching frames.
        MCAN_ResetFilters(mcan);
        MCAN_SetupFilter(mcan, MCAN_ID_Standard, MCAN_FilterNonMatch_Discard, false);
    }
    else
    {
        // Since we use FIFO with 64 slots, number of simultaneous messages no longer matter for the Legacy support (legacy only need maximum of 16)
        MCAN_AddFilter(mcan, MCAN_ID_Standard, MCAN_FilterType_ID_Mask, MCAN_FilterDest_FIFO0, filterID, filterMask);
    }

    MCAN_ExitInitMode(mcan);
}


/**
 * @brief Send one legacy CAN message using the MCAN queue API.
 *
 * @param msg Legacy message container.
 * @return true Message accepted for transmission.
 * @return false TX queue/FIFO is full.
 */
bool MCAN_Legacy_Send(struct MCAN_Legacy_canMsg *msg)
{
    enum MCAN mcan = MCAN_LEGACY_MODULE;

    // Convert legacy message format to new format
    struct MCAN_Message message = 
    {
        .isRemote = false,
        .mode     = MCAN_Mode_Classic,
        .idType   = MCAN_ID_Standard,
        .id       = msg->address,
        .dlc      = (enum MCAN_PayloadDLC)msg->length,
        .data     = {0}
    };

    MCAN_PackBytes(message.data, msg->data, msg->length);
    
    return MCAN_SendQueue(mcan, &message);
}

/**
 * @brief Receive one legacy CAN message from RX FIFO0.
 *
 * @param msg Output legacy message container.
 * @return true One message was read and unpacked.
 * @return false No message available.
 */
bool MCAN_Legacy_Receive(struct MCAN_Legacy_canMsg *msg)
{
    enum MCAN mcan = MCAN_LEGACY_MODULE;

    struct MCAN_Message message = {0};

    if(MCAN_IsRxFIFOEmpty(mcan, MCAN_Rx_FIFO0))
    {
        return false;
    }

    MCAN_ReadRxFIFO(mcan, MCAN_Rx_FIFO0, &message);

    msg->address = (uint16_t)message.id;
    msg->length = MCAN_GetPayloadLengthByte(message.dlc);
    MCAN_UnpackBytes(msg->data, message.data, msg->length);

    return true;
}

#endif /* MCAN_LEGACY_MODE_SUPPORT */

