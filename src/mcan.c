#include "mcan.h"
#include "base.h"
#include "clk.h"
#include <math.h>


#define MSG_ID_DUMMY_STD11BITS  0x1FFC0000
#define MSG_MSK_ACCEPT_ALL      0u
#define MSG_LENGTH_DUMMY        0u
#define FIFO_TOTAL_SIZE         32u
#define FIFO_TX_START           1u
#define FIFO_TX_SIZE            4u
#define FIFO_RX_START           FIFO_TX_SIZE + 1u
#define FIFO_RX_SIZE            FIFO_TOTAL_SIZE - FIFO_TX_SIZE

#define MCAN_SS_BASE(mcan)          (MCAN_SS_START_ADDR + MCAN_SS_INCREMENT_ADDR * (mcan))
#define MCAN_BASE(mcan)             (MCAN_START_ADDR + MCAN_INCREMENT_ADDR * (mcan))
#define MCAN_ERR_BASE(mcan)         (MCAN_ERR_START_ADDR + MCAN_ERR_INCREMENT_ADDR * (mcan))
#define MCAN_MSG_RAM_BASE(mcan)     (MCAN_MSG_RAM_START_ADDR + MCAN_MSG_RAM_INCREMENT_ADDR * (mcan))

#define MCAN_LEGACY_BYTE_SIZE   (8)
    


//
// Register definitions
//

//
// MCAN control register
//
#define MCANSS_PID(mcan)   ATOMIC32(MCAN_SS_BASE(mcan) + 0x00) // MCAN Subsystem Revision Register
#define MCANSS_CTRL(mcan)  ATOMIC32(MCAN_SS_BASE(mcan) + 0x04) // MCAN Subsystem Control Register
#define MCANSS_STAT(mcan)  ATOMIC32(MCAN_SS_BASE(mcan) + 0x08) // MCAN Subsystem Status Register
#define MCANSS_ICS(mcan)   ATOMIC32(MCAN_SS_BASE(mcan) + 0x0C) // MCAN Subsystem Interrupt Clear Shadow Register
#define MCANSS_IRS(mcan)   ATOMIC32(MCAN_SS_BASE(mcan) + 0x10) // MCAN Subsystem Interrupt Raw Status Register
#define MCANSS_IECS(mcan)  ATOMIC32(MCAN_SS_BASE(mcan) + 0x14) // MCAN Subsystem Interrupt Enable Clear Shadow Register
#define MCANSS_IE(mcan)    ATOMIC32(MCAN_SS_BASE(mcan) + 0x18) // MCAN Subsystem Interrupt Enable Register
#define MCANSS_IES(mcan)   ATOMIC32(MCAN_SS_BASE(mcan) + 0x1C) // MCAN Subsystem Interrupt Enable Status
#define MCANSS_EOI(mcan)   ATOMIC32(MCAN_SS_BASE(mcan) + 0x20) // MCAN Subsystem End of Interrupt
#define MCANSS_EXT_TS_PRESCALER(mcan)              ATOMIC32(MCAN_BASE(mcan) + 0x24) // MCAN Subsystem External Timestamp Prescaler 0
#define MCANSS_EXT_TS_UNSERVICED_INTR_CNTR(mcan)   ATOMIC32(MCAN_BASE(mcan) + 0x28) // MCAN Subsystem External Timestamp Unserviced Interrupts Counter

#define MCAN_CREL(mcan)     ATOMIC32(MCAN_BASE(mcan) + 0x00) // MCAN Core Release Register
#define MCAN_ENDN(mcan)     ATOMIC32(MCAN_BASE(mcan) + 0x04) // MCAN Endian Register
#define MCAN_DBTP(mcan)     ATOMIC32(MCAN_BASE(mcan) + 0x0C) // MCAN Data Bit Timing and Prescaler Register
#define MCAN_TEST(mcan)     ATOMIC32(MCAN_BASE(mcan) + 0x10) // MCAN Test Register
#define MCAN_RWD(mcan)      ATOMIC32(MCAN_BASE(mcan) + 0x14) // MCAN RAM Watchdog
#define MCAN_CCCR(mcan)     ATOMIC32(MCAN_BASE(mcan) + 0x18) // MCAN CC Control Register
#define MCAN_NBTP(mcan)     ATOMIC32(MCAN_BASE(mcan) + 0x1C) // MCAN Nominal Bit Timing and Prescaler Register
#define MCAN_TSCC(mcan)     ATOMIC32(MCAN_BASE(mcan) + 0x20) // MCAN Timestamp Counter Configuration
#define MCAN_TSCV(mcan)     ATOMIC32(MCAN_BASE(mcan) + 0x24) // MCAN Timestamp Counter Value
#define MCAN_TOCC(mcan)     ATOMIC32(MCAN_BASE(mcan) + 0x28) // MCAN Timeout Counter Configuration
#define MCAN_TOCV(mcan)     ATOMIC32(MCAN_BASE(mcan) + 0x2C) // MCAN Timeout Counter Value
#define MCAN_ECR(mcan)      ATOMIC32(MCAN_BASE(mcan) + 0x40) // MCAN Error Counter Register
#define MCAN_PSR(mcan)      ATOMIC32(MCAN_BASE(mcan) + 0x44) // MCAN Protocol Status Register
#define MCAN_TDCR(mcan)     ATOMIC32(MCAN_BASE(mcan) + 0x48) // MCAN Transmitter Delay Compensation Register
#define MCAN_IR(mcan)       ATOMIC32(MCAN_BASE(mcan) + 0x50) // MCAN Interrupt Register
#define MCAN_IE(mcan)       ATOMIC32(MCAN_BASE(mcan) + 0x54) // MCAN Interrupt Enable
#define MCAN_ILS(mcan)      ATOMIC32(MCAN_BASE(mcan) + 0x58) // MCAN Interrupt Line Select
#define MCAN_ILE(mcan)      ATOMIC32(MCAN_BASE(mcan) + 0x5C) // MCAN Interrupt Line Enable
#define MCAN_GFC(mcan)      ATOMIC32(MCAN_BASE(mcan) + 0x80) // MCAN Global Filter Configuration
#define MCAN_SIDFC(mcan)    ATOMIC32(MCAN_BASE(mcan) + 0x84) // MCAN Standard ID Filter Configuration
#define MCAN_XIDFC(mcan)    ATOMIC32(MCAN_BASE(mcan) + 0x88) // MCAN Extended ID Filter Configuration
#define MCAN_XIDAM(mcan)    ATOMIC32(MCAN_BASE(mcan) + 0x90) // MCAN Extended ID and Mask
#define MCAN_HPMS(mcan)     ATOMIC32(MCAN_BASE(mcan) + 0x94) // MCAN High Priority Message Status
#define MCAN_NDAT1(mcan)    ATOMIC32(MCAN_BASE(mcan) + 0x98) // MCAN New Data 1
#define MCAN_NDAT2(mcan)    ATOMIC32(MCAN_BASE(mcan) + 0x9C) // MCAN New Data 2
#define MCAN_RXF0C(mcan)    ATOMIC32(MCAN_BASE(mcan) + 0xA0) // MCAN Rx FIFO 0 Configuration
#define MCAN_RXF0S(mcan)    ATOMIC32(MCAN_BASE(mcan) + 0xA4) // MCAN Rx FIFO 0 Status
#define MCAN_RXF0A(mcan)    ATOMIC32(MCAN_BASE(mcan) + 0xA8) // MCAN Rx FIFO 0 Acknowledge
#define MCAN_RXBC(mcan)     ATOMIC32(MCAN_BASE(mcan) + 0xAC) // MCAN Rx Buffer Configuration
#define MCAN_RXF1C(mcan)    ATOMIC32(MCAN_BASE(mcan) + 0xB0) // MCAN Rx FIFO 1 Configuration
#define MCAN_RXF1S(mcan)    ATOMIC32(MCAN_BASE(mcan) + 0xB4) // MCAN Rx FIFO 1 Status
#define MCAN_RXF1A(mcan)    ATOMIC32(MCAN_BASE(mcan) + 0xB8) // MCAN Rx FIFO 1 Acknowledge
#define MCAN_RXESC(mcan)    ATOMIC32(MCAN_BASE(mcan) + 0xBC) // MCAN Rx Buffer / FIFO Element Size Configuration
#define MCAN_TXBC(mcan)     ATOMIC32(MCAN_BASE(mcan) + 0xC0) // MCAN Tx Buffer Configuration
#define MCAN_TXFQS(mcan)    ATOMIC32(MCAN_BASE(mcan) + 0xC4) // MCAN Tx FIFO / Queue Status
#define MCAN_TXESC(mcan)    ATOMIC32(MCAN_BASE(mcan) + 0xC8) // MCAN Tx Buffer Element Size Configuration
#define MCAN_TXBRP(mcan)    ATOMIC32(MCAN_BASE(mcan) + 0xCC) // MCAN Tx Buffer Request Pending
#define MCAN_TXBAR(mcan)    ATOMIC32(MCAN_BASE(mcan) + 0xD0) // MCAN Tx Buffer Add Request
#define MCAN_TXBCR(mcan)    ATOMIC32(MCAN_BASE(mcan) + 0xD4) // MCAN Tx Buffer Cancellation Request
#define MCAN_TXBTO(mcan)    ATOMIC32(MCAN_BASE(mcan) + 0xD8) // MCAN Tx Buffer Transmission Occurred
#define MCAN_TXBCF(mcan)    ATOMIC32(MCAN_BASE(mcan) + 0xDC) // MCAN Tx Buffer Cancellation Finished
#define MCAN_TXBTIE(mcan)   ATOMIC32(MCAN_BASE(mcan) + 0xE0) // MCAN Tx Buffer Transmission Interrupt Enable
#define MCAN_TXBCIE(mcan)   ATOMIC32(MCAN_BASE(mcan) + 0xE4) // MCAN Tx Buffer Cancellation Finished Interrupt Enable
#define MCAN_TXEFC(mcan)    ATOMIC32(MCAN_BASE(mcan) + 0xF0) // MCAN Tx Event FIFO Configuration
#define MCAN_TXEFS(mcan)    ATOMIC32(MCAN_BASE(mcan) + 0xF4) // MCAN Tx Event FIFO Status
#define MCAN_TXEFA(mcan)    ATOMIC32(MCAN_BASE(mcan) + 0xF8) // MCAN Tx Event FIFO Acknowledge

#define MCANERR_REV(mcan)               ATOMIC32(MCAN_ERR_BASE(mcan) + 0x00) // MCAN Error Aggregator Revision Register
#define MCANERR_VECTOR(mcan)            ATOMIC32(MCAN_ERR_BASE(mcan) + 0x04) // MCAN ECC Vector Register
#define MCANERR_STAT(mcan)              ATOMIC32(MCAN_ERR_BASE(mcan) + 0x06) // MCAN Error Misc Status
#define MCANERR_WRAP_REV(mcan)          ATOMIC32(MCAN_ERR_BASE(mcan) + 0x08) // MCAN ECC Wrapper Revision Register
#define MCANERR_CTRL(mcan)              ATOMIC32(MCAN_ERR_BASE(mcan) + 0x0A) // MCAN ECC Control
#define MCANERR_CTRL1(mcan)             ATOMIC32(MCAN_ERR_BASE(mcan) + 0x0C) // MCAN ECC Error Control 1 Register
#define MCANERR_CTRL2(mcan)             ATOMIC32(MCAN_ERR_BASE(mcan) + 0x0E) // MCAN ECC Error Control 2 Register
#define MCANERR_STAT1(mcan)             ATOMIC32(MCAN_ERR_BASE(mcan) + 0x10) // MCAN ECC Error Status 1 Register
#define MCANERR_STAT2(mcan)             ATOMIC32(MCAN_ERR_BASE(mcan) + 0x12) // MCAN ECC Error Status 2 Register
#define MCANERR_STAT3(mcan)             ATOMIC32(MCAN_ERR_BASE(mcan) + 0x14) // MCAN ECC Error Status 3 Register
#define MCANERR_SEC_EOI(mcan)           ATOMIC32(MCAN_ERR_BASE(mcan) + 0x1E) // MCAN Single Error Corrected End of Interrupt Register
#define MCANERR_SEC_STATUS(mcan)        ATOMIC32(MCAN_ERR_BASE(mcan) + 0x20) // MCAN Single Error Corrected Interrupt Status Register
#define MCANERR_SEC_ENABLE_SET(mcan)    ATOMIC32(MCAN_ERR_BASE(mcan) + 0x40) // MCAN Single Error Corrected Interrupt Enable Set Register
#define MCANERR_SEC_ENABLE_CLR(mcan)    ATOMIC32(MCAN_ERR_BASE(mcan) + 0x60) // MCAN Single Error Corrected Interrupt Enable Clear Register
#define MCANERR_DED_EOI(mcan)           ATOMIC32(MCAN_ERR_BASE(mcan) + 0x9E) // MCAN Double Error Detected End of Interrupt Register
#define MCANERR_DED_STATUS(mcan)        ATOMIC32(MCAN_ERR_BASE(mcan) + 0xA0) // MCAN Double Error Detected Interrupt Status Register
#define MCANERR_DED_ENABLE_SET(mcan)    ATOMIC32(MCAN_ERR_BASE(mcan) + 0xC0) // MCAN Double Error Detected Interrupt Enable Set Register
#define MCANERR_DED_ENABLE_CLR(mcan)    ATOMIC32(MCAN_ERR_BASE(mcan) + 0xE0) // MCAN Double Error Detected Interrupt Enable Clear Register
#define MCANERR_AGGR_ENABLE_SET(mcan)   ATOMIC32(MCAN_ERR_BASE(mcan) + 0x00) // MCAN Error Aggregator Enable Set Register
#define MCANERR_AGGR_ENABLE_CLR(mcan)   ATOMIC32(MCAN_ERR_BASE(mcan) + 0x02) // MCAN Error Aggregator Enable Clear Register
#define MCANERR_AGGR_STATUS_SET(mcan)   ATOMIC32(MCAN_ERR_BASE(mcan) + 0x04) // MCAN Error Aggregator Status Set Register
#define MCANERR_AGGR_STATUS_CLR(mcan)   ATOMIC32(MCAN_ERR_BASE(mcan) + 0x06) // MCAN Error Aggregator Status Clear Register



#define MCANSS_M_STAT_MEM_INIT_DONE     ( 0x01U <<  1 )

// MCANSS_CTRL
#define MCANSS_M_CTRL_DBGSUSP       (1UL << 0)
#define MCANSS_M_CTRL_WAKEUPREQEN   (1UL << 4)
#define MCANSS_M_CTRL_AUTOWAKEUP    (1UL << 5)

// MCAN_CCCR
#define MCAN_M_CCCR_INIT            (1UL << 0)
#define MCAN_M_CCCR_CCE             (1UL << 1)
#define MCAN_M_CCCR_MON             (1UL << 5)
#define MCAN_M_CCCR_TEST            (1UL << 7)
#define MCAN_M_CCCR_FDOE            (1UL << 8)
#define MCAN_M_CCCR_BRSE            (1UL << 9)

// MCAN_TEST
#define MCAN_S_TEST_TX(x)           ((uint32_t)(x) << 5)
#define MCAN_M_TEST_LBCK            (1UL << 4)

// MCAN_NBTP
#define MCAN_S_NBTP_NTSEG2(x)       ((uint32_t)(x) << 0)
#define MCAN_S_NBTP_NTSEG1(x)       ((uint32_t)(x) << 8)
#define MCAN_S_NBTP_NBRP(x)         ((uint32_t)(x) << 16)
#define MCAN_S_NBTP_NSJW(x)         ((uint32_t)(x) << 25)

// MCAN_DBTP
#define MCAN_S_DBTP_DTSEG2(x)       ((uint32_t)(x) <<  0U)
#define MCAN_S_DBTP_DTSEG1(x)       ((uint32_t)(x) <<  8U)
#define MCAN_S_DBTP_DBRP(x)         ((uint32_t)(x) << 16U)
#define MCAN_S_DBTP_DSJW(x)         ((uint32_t)(x) << 20U)

#define MCAN_M_DBTP_DSJW_TDC        (1UL << 23)

// MCAN_TDCR
#define MCAN_S_TDCR_TDCF(x)         ((uint32_t)(x) << 0)
#define MCAN_S_TDCR_TDCO(x)         ((uint32_t)(x) << 8)

#define MCAN_M_TDCR_TDCF            MCAN_S_TDCR_TDCF(MASK(7))
#define MCAN_M_TDCR_TDCO            MCAN_S_TDCR_TDCO(MASK(7))

// MCAN_GFC
#define MCAN_S_GFC_RRFE(x)          ((uint32_t)(x) << 0U)
#define MCAN_S_GFC_RRFS(x)          ((uint32_t)(x) << 1U)
#define MCAN_S_GFC_ANFE(x)          ((uint32_t)(x) << 2U)
#define MCAN_S_GFC_ANFS(x)          ((uint32_t)(x) << 4U)

#define MCAN_M_GFC_RRFE             MCAN_S_GFC_RRFE(MASK(1))
#define MCAN_M_GFC_RRFS             MCAN_S_GFC_RRFS(MASK(1))
#define MCAN_M_GFC_ANFE             MCAN_S_GFC_ANFE(MASK(2))
#define MCAN_M_GFC_ANFS             MCAN_S_GFC_ANFS(MASK(2))

// Common message RAM start-address field for SIDFC/XIDFC/RXFxC/RXBC/TXBC
#define MCAN_S_SA(x)                ((uint32_t)(x) << 2U)
#define MCAN_M_SA                   MCAN_S_SA(MASK(14))

// MCAN_SIDFC
#define MCAN_S_SIDFC_LSS(x)         ((uint32_t)(x) << 16U)
#define MCAN_M_SIDFC_LSS            MCAN_S_SIDFC_LSS(MASK(8))

// MCAN_XIDFC
#define MCAN_S_XIDFC_LSE(x)         ((uint32_t)(x) << 16U)
#define MCAN_M_XIDFC_LSE            MCAN_S_XIDFC_LSE(MASK(7))

// MCAN_RXESC
#define MCAN_S_RXESC_F0DS(x)        ((uint32_t)(x) << 0U)
#define MCAN_S_RXESC_F1DS(x)        ((uint32_t)(x) << 4U)
#define MCAN_S_RXESC_RBDS(x)        ((uint32_t)(x) << 8U)

#define MCAN_M_RXESC_F0DS           MCAN_S_RXESC_F0DS(MASK(3))
#define MCAN_M_RXESC_F1DS           MCAN_S_RXESC_F1DS(MASK(3))
#define MCAN_M_RXESC_RBDS           MCAN_S_RXESC_RBDS(MASK(3))

// MCAN_RXF0C / MCAN_RXF1C
#define MCAN_S_RXF0C_F0S(x)         ((uint32_t)(x) << 16U)
#define MCAN_S_RXF1C_F1S(x)         ((uint32_t)(x) << 16U)

#define MCAN_M_RXF0C_F0S            MCAN_S_RXF0C_F0S(MASK(7))
#define MCAN_M_RXF1C_F1S            MCAN_S_RXF1C_F1S(MASK(7))

// MCAN_RXF0S / MCAN_RXF1S
#define MCAN_M_RXF0S_F0FL           MCAN_S_RXF0S_F0FL(MASK(7))
#define MCAN_M_RXF0S_F0GI           MCAN_S_RXF0S_F0GI(MASK(6))
#define MCAN_M_RXF1S_F1FL           MCAN_S_RXF1S_F1FL(MASK(7))
#define MCAN_M_RXF1S_F1GI           MCAN_S_RXF1S_F1GI(MASK(6))

// MCAN_RXF0A / MCAN_RXF1A
#define MCAN_S_RXF0A_F0AI(x)        ((uint32_t)(x) << 0U)
#define MCAN_S_RXF1A_F1AI(x)        ((uint32_t)(x) << 0U)

// MCAN_TXBC
#define MCAN_S_TXBC_TBSA(x)         ((uint32_t)(x) << 2U)
#define MCAN_S_TXBC_NDTB(x)         ((uint32_t)(x) << 16U)
#define MCAN_S_TXBC_TFQS(x)         ((uint32_t)(x) << 24U)
#define MCAN_S_TXBC_TFQM(x)         ((uint32_t)(x) << 30U)

#define MCAN_M_TXBC_TBSA            MCAN_S_TXBC_TBSA(MASK(14))
#define MCAN_M_TXBC_NDTB            MCAN_S_TXBC_NDTB(MASK(6))
#define MCAN_M_TXBC_TFQS            MCAN_S_TXBC_TFQS(MASK(6))
#define MCAN_M_TXBC_TFQM            MCAN_S_TXBC_TFQM(MASK(1))

// MCAN_TXFQS
#define MCAN_M_TXFQS_TFQF           (0x01UL << 21)
#define MCAN_M_TXFQS_TFQP           (0x1FUL << 16)



#define MCAN_SAMPLE_POINT_TARGET        (0.8f)
#define MCAN_SELFTEST_TIMEOUT           (1000U)
#define MCAN_SELFTEST_ID                (0x123U)
#define MCAN_SELFTEST_DATA_WORD0        (0x76543210UL)
#define MCAN_SELFTEST_DATA_WORD1        (0xFEDCBA98UL)

static bool MCAN_IsRxFIFOEmpty(enum MCAN mcan, enum MCAN_Rx rx);
static void MCAN_ResetMessageRAM(enum MCAN mcan);

struct MCAN_Status MCAN_status[MCAN_COUNT];

void MCAN_SelectBitClkSrc(enum MCAN mcan, enum MCAN_BitClkSrc src)
{
    CLK_SetCLKSRCCTL2(10+2*mcan, src);
}



static void MCAN_SetFCLK(enum MCAN mcan, float fclkMHz)
{
    uint16_t fclkDiv;
    uint16_t auxClkDivShift;

    if(fclkMHz > CLK_status.sysClkPhysicalMHz)
    {
        // FCLK cannot be faster that SYSCLK (physical). Stop initialization
        HAL_FATAL(HAL_ErrorCode_OutOfRange, (uint32_t)fclkMHz, (uint32_t)CLK_status.sysClkPhysicalMHz);
    }
    fclkDiv = roundf(CLK_status.pllOutMHz / fclkMHz);
    
    if(fclkDiv > 20)
    {
        // FCLKDIV cannot be larger than 20. Stop initialization
        HAL_FATAL(HAL_ErrorCode_OutOfRange, (uint32_t)fclkDiv, 20U);
    }

    // Bit field shift amount in AUXCLKDIV register
    auxClkDivShift = 8+mcan*5;
    
    // Write clock divider, and set clock source to PLLRAWCLK (PLL output clock)
    CLK_AUXCLKDIVSEL = CLK_AUXCLKDIVSEL & ~(0x1F << auxClkDivShift) | (fclkDiv << auxClkDivShift);
    CLK_SetCLKSRCCTL2(10+mcan*2, MCAN_BitClkSrc_PllRawClk);

    MCAN_status[mcan].fclkMHz = CLK_status.pllOutMHz / fclkDiv;
}

static void MCAN_EnterInitMode(enum MCAN mcan)
{
    MCAN_CCCR(mcan) |= MCAN_M_CCCR_INIT;
    while(!(MCAN_CCCR(mcan) & MCAN_M_CCCR_INIT));

    MCAN_CCCR(mcan) |= MCAN_M_CCCR_CCE;
}

static void MCAN_ExitInitMode(enum MCAN mcan)
{
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
}

static void MCAN_SetArbiTiming(enum MCAN mcan, uint16_t arbiPrescale, float arbiKbps)
{
    float arbiClkKHz;
    uint16_t arbiQuanta;
    uint16_t sjw, tseg1, tseg2;

    if(arbiKbps > 1000)
    {
        // Arbitration phase bit rate cannot exceed 1Mbps. Stop initialization
        HAL_FATAL(HAL_ErrorCode_OutOfRange, (uint32_t)arbiKbps, 1000U);
    }

    arbiClkKHz = MCAN_status[mcan].fclkMHz * 1000 / arbiPrescale;
    arbiQuanta = roundf(arbiClkKHz / arbiKbps);

    if((arbiQuanta < 5) || (arbiQuanta > 280))
    {
        // Number of time quanta is outside permissable range, consider adjusting prescale ratio. Stop initialization
        HAL_FATAL(HAL_ErrorCode_OutOfRange, (uint32_t)arbiQuanta, (uint32_t)arbiPrescale);
    }

    // Calculate TSEG2 length for achieving target Sample Point position
    tseg2 = roundf(arbiQuanta * (1-MCAN_SAMPLE_POINT_TARGET));
    tseg1 = arbiQuanta - 1 - tseg2;
    sjw = (tseg2 > 4) ? 4 : tseg2;  // SJW cannot be larger than 4

    // Value written to register in 1 less than the desired value (eg. writing 4 means 5)
    MCAN_NBTP(mcan) = MCAN_S_NBTP_NSJW(sjw-1) | MCAN_S_NBTP_NBRP(arbiPrescale-1) | MCAN_S_NBTP_NTSEG1(tseg1-1) | MCAN_S_NBTP_NTSEG2(tseg2-1);

    MCAN_status[mcan].arbiQuanta = arbiQuanta;
    MCAN_status[mcan].arbiKbps   = arbiClkKHz / arbiQuanta;
}

static void MCAN_SetDataTiming(enum MCAN mcan, float dataKbps)
{
    uint16_t dataQuanta;
    uint16_t sjw, tseg1, tseg2;

    if(dataKbps > 5000)
    {
        // Data phase bit rate cannot exceed 5Mbps. Stop initialization
        HAL_FATAL(HAL_ErrorCode_OutOfRange, (uint32_t)dataKbps, 5000U);
    }

    // In this library, data bit rate prescale is not used
    dataQuanta = roundf(MCAN_status[mcan].fclkMHz * 1000 / dataKbps);

    if((dataQuanta < 5) || (dataQuanta > 40))
    {
        // Number of time quanta is outside permissable range. Stop initialization
        HAL_FATAL(HAL_ErrorCode_OutOfRange, (uint32_t)dataQuanta, 40U);
    }

    // Calculate TSEG2 length for achieving target Sample Point position
    tseg2 = roundf(dataQuanta * (1-MCAN_SAMPLE_POINT_TARGET));
    tseg1 = dataQuanta - 1 - tseg2;
    sjw = tseg2;

    // Write timing information and enable Transmitter Delay Compensation (TDC)
    // Value written to register in 1 less than the desired value (eg. writing 4 means 5)
    MCAN_DBTP(mcan) = MCAN_M_DBTP_DSJW_TDC | MCAN_S_DBTP_DSJW(sjw-1) | MCAN_S_DBTP_DTSEG1(tseg1-1) | MCAN_S_DBTP_DTSEG2(tseg2-1);

    // Configure TDC. According to E2E, both values should be equal to DTSEG1 as starting point;
    MCAN_TDCR(mcan) = MCAN_S_TDCR_TDCO(tseg1) | MCAN_S_TDCR_TDCF(tseg1);

    MCAN_status[mcan].dataQuanta = dataQuanta;
    MCAN_status[mcan].dataKbps   = MCAN_status[mcan].fclkMHz * 1000 / dataQuanta;
}


static void MCAN_SelfTest(enum MCAN mcan)
{
    struct MCAN_Message txMessage = {0};
    struct MCAN_Message rxMessage = {0};
    uint32_t savedCCCR;
    uint32_t savedTEST;
    uint16_t timeout;

    savedCCCR = MCAN_CCCR(mcan);
    savedTEST = MCAN_TEST(mcan);

    MCAN_ResetMessageRAM(mcan);
    MCAN_SetupTx(mcan, MCAN_TxQueueMode_FIFO, MCAN_DataSize_8Byte, 0, 1);
    MCAN_SetupRx(mcan, MCAN_Rx_FIFO0, MCAN_DataSize_8Byte, 1);
    MCAN_SetupFilter(mcan, MCAN_ID_Standard, MCAN_FilterNonMatch_FIFO0, false);

    txMessage.isRemote = false;
    txMessage.mode = MCAN_status[mcan].mode;
    txMessage.idType = MCAN_ID_Standard;
    txMessage.id = MCAN_SELFTEST_ID;
    txMessage.length = MCAN_DataLength_8Byte;
    txMessage.data[0] = MCAN_SELFTEST_DATA_WORD0;
    txMessage.data[1] = MCAN_SELFTEST_DATA_WORD1;

    MCAN_CCCR(mcan) |= MCAN_M_CCCR_MON | MCAN_M_CCCR_TEST;
    MCAN_TEST(mcan) = MCAN_S_TEST_TX(3U) | MCAN_M_TEST_LBCK;

    MCAN_ExitInitMode(mcan);

    if(!MCAN_SendQueue(mcan, &txMessage))
    {
        HAL_FATAL(HAL_ErrorCode_HardwareFault, (uint32_t)mcan, 1U);
    }

    timeout = MCAN_SELFTEST_TIMEOUT;
    while(MCAN_IsRxFIFOEmpty(mcan, MCAN_Rx_FIFO0) && timeout)
    {
        timeout--;
    }

    if(timeout == 0U)
    {
        HAL_FATAL(HAL_ErrorCode_Timeout, (uint32_t)mcan, 2U);
    }

    MCAN_ReadRxFIFO(mcan, MCAN_Rx_FIFO0, &rxMessage);

    if((rxMessage.id != txMessage.id)
    || (rxMessage.length != txMessage.length)
    || (rxMessage.idType != txMessage.idType)
    || (rxMessage.data[0] != txMessage.data[0])
    || (rxMessage.data[1] != txMessage.data[1]))
    {
        HAL_FATAL(HAL_ErrorCode_HardwareFault, (uint32_t)mcan, 3U);
    }

    MCAN_EnterInitMode(mcan);
    MCAN_TEST(mcan) = savedTEST;
    MCAN_CCCR(mcan) = savedCCCR;

    MCAN_ResetMessageRAM(mcan);

}


static void MCAN_ResetMessageRAM(enum MCAN mcan)
{
    MCAN_status[mcan].msgRam.allocated = 0;
    MCAN_SetupTx(mcan, MCAN_TxQueueMode_FIFO, MCAN_DataSize_8Byte, 0, 0);
    MCAN_SetupRx(mcan, MCAN_Rx_Buffer, MCAN_DataSize_8Byte, 0);
    MCAN_SetupRx(mcan, MCAN_Rx_FIFO0, MCAN_DataSize_8Byte, 0);
    MCAN_SetupRx(mcan, MCAN_Rx_FIFO1, MCAN_DataSize_8Byte, 0);
    MCAN_SetupFilter(mcan, MCAN_ID_Standard, MCAN_FilterNonMatch_Discard, false);
    MCAN_SetupFilter(mcan, MCAN_ID_Extended, MCAN_FilterNonMatch_Discard, false);
}

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

    // initParams.wdcPreload        = 0xFFU; // Start value of the Message RAM
    //                                       // Watchdog Counter preload.

    // //
    // // Initialize MCAN Config parameters.
    // //
    // configParams.monEnable         = monEn; // Bus Monitoring Mode is disabled
    // configParams.asmEnable         = 0x0U;  // Normal CAN operation.
    // configParams.tsPrescalar       = 0xFU;  // Prescaler Value.
    // configParams.tsSelect          = 0x0U;  // Timestamp counter value.
    // configParams.timeoutSelect     = MCAN_TIMEOUT_SELECT_CONT;
    // // Time-out counter source select.
    // configParams.timeoutPreload    = 0xFFFFU; // Start value of the Timeout
    //                                           // Counter.
    // configParams.timeoutCntEnable  = 0x0U; // Time-out Counter is disabled.
    // configParams.filterConfig.rrfs = 0x1U; // Reject all remote frames with
    //                                        // 29-bit extended IDs.
    // configParams.filterConfig.rrfe = 0x1U; // Reject all remote frames with
    //                                        // 11-bit standard IDs.
    // configParams.filterConfig.anfe = 0x1U; // Accept in Rx FIFO 1.
    // configParams.filterConfig.anfs = 0x1U; // Accept in Rx FIFO 1.

}

void MCAN_SetupClassic(enum MCAN mcan, float fclkMHz, uint16_t arbiPrescale, float bitrateKbps)
{
    MCAN_SetFCLK(mcan, fclkMHz);
    MCAN_Init(mcan);

    MCAN_status[mcan].mode = MCAN_Mode_Classic;

    // Configure bit timing
    MCAN_SetArbiTiming(mcan, arbiPrescale, bitrateKbps);

    MCAN_SelfTest(mcan);
}

void MCAN_Start(enum MCAN mcan)
{
    MCAN_ExitInitMode(mcan);
}

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
}

static inline uint16_t MCAN_GetDataLengthByte(enum MCAN_DataLength length)
{
    switch(length)
    {
        case MCAN_DataLength_0Byte:
        case MCAN_DataLength_1Byte:
        case MCAN_DataLength_2Byte:
        case MCAN_DataLength_3Byte:
        case MCAN_DataLength_4Byte:
        case MCAN_DataLength_5Byte:
        case MCAN_DataLength_6Byte:
        case MCAN_DataLength_7Byte:
        case MCAN_DataLength_8Byte:
            return length;

        case MCAN_DataLength_12Byte:
            return 12;

        case MCAN_DataLength_16Byte:
            return 16;

        case MCAN_DataLength_20Byte:
            return 20;

        case MCAN_DataLength_24Byte:
            return 24;

        case MCAN_DataLength_32Byte:
            return 32;

        case MCAN_DataLength_48Byte:
            return 48;

        case MCAN_DataLength_64Byte:
            return 64;
    }

    return 0;
}

static void MCAN_PackBytes(uint32_t* dest, const uint16_t* src, uint16_t lengthByte)
{
    for(uint16_t i=0; i<lengthByte; i+=4)
    {
        dest[i/4] = (uint32_t)src[i]
                  | ((uint32_t)src[i+1] << 8)
                  | ((uint32_t)src[i+2] << 16)
                  | ((uint32_t)src[i+3] << 24);
    }
}

static void MCAN_UnpackBytes(uint16_t* dest, const uint32_t* src, uint16_t lengthByte)
{
    for(uint16_t i=0; i<lengthByte; i+=4)
    {
        dest[i] = src[i/4] & 0xFF;
        dest[i+1] = src[i/4] >> 8 & 0xFF;
        dest[i+2] = src[i/4] >> 16 & 0xFF;
        dest[i+3] = src[i/4] >> 24;
    }
}


static int16_t MCAN_AllocateMsgRam(enum MCAN mcan, uint16_t sizeByte)
{
    uint16_t sectionAddr;

    if((MCAN_status[mcan].msgRam.allocated + sizeByte) > MCAN_MSG_RAM_DEPTH_BYTE)
    {
        // Message RAM size exceed. Return -1 to indicate error
        return -1;
    }

    sectionAddr = MCAN_status[mcan].msgRam.allocated;
    MCAN_status[mcan].msgRam.allocated += sizeByte;

    return sectionAddr;
}

void MCAN_SetupTx(enum MCAN mcan, enum MCAN_TxQueueMode mode, enum MCAN_DataSize dataSize, uint16_t dBufferDepth, uint16_t queueDepth)
{
    int16_t startAddr;
    if((dBufferDepth + queueDepth) > 32)
    {
        // Sum of dedicated Tx Buffer and Tx Queue cannot exceed 32. Stop initialization
        HAL_FATAL(HAL_ErrorCode_InvalidArgument, (uint32_t)dBufferDepth, (uint32_t)queueDepth);
    }


    // Allocate Message RAM for TX buffer
    startAddr = MCAN_AllocateMsgRam(mcan, MCAN_GetElementSizeByte(dataSize) * (dBufferDepth + queueDepth));

    if(startAddr == -1)
    {
        // Unable to allocate Message RAM for TX Buffer. Stop initialization
        HAL_FATAL(HAL_ErrorCode_ResourceConflict, (uint32_t)dataSize, (uint32_t)(dBufferDepth + queueDepth));
    }

    MCAN_status[mcan].msgRam.startAddr.txBuffer = startAddr;
    MCAN_status[mcan].msgRam.elementSizeByte.txBuffer = MCAN_GetElementSizeByte(dataSize);
    MCAN_status[mcan].msgRam.nElement.txBuffer = dBufferDepth + queueDepth;

    MCAN_TXESC(mcan) = dataSize;
    MCAN_TXBC(mcan) = MCAN_S_TXBC_TFQM(mode)
                    | MCAN_S_TXBC_TFQS(queueDepth)
                    | MCAN_S_TXBC_NDTB(dBufferDepth)
                    | MCAN_S_TXBC_TBSA(startAddr >> 2);
}

// void MCAN_AssignTxBuffer(enum MCAN mcan, uint16_t bufferNum, uint32_t messageID, enum MCAN_ID idType, bool isRemote)
// {
    
// }


void MCAN_SetupRx(enum MCAN mcan, enum MCAN_Rx rx, enum MCAN_DataSize dataSize, uint16_t depth)
{
    int16_t startAddr;

    if (depth > 64) 
    {
        // Each RX Section is limited to 64 elements. Stop initialization
        HAL_FATAL(HAL_ErrorCode_OutOfRange, (uint32_t)depth, 64U);
    }

    startAddr = MCAN_AllocateMsgRam(mcan, MCAN_GetElementSizeByte(dataSize) * depth);

    if(startAddr == -1)
    {
        // Unable to allocate Message RAM. Stop initialization
        HAL_FATAL(HAL_ErrorCode_ResourceConflict, (uint32_t)rx, (uint32_t)depth);
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
            MCAN_RXF0C(mcan) = MCAN_S_RXF0C_F0S(depth)
                             | MCAN_S_SA(startAddr >> 2);
            break;
        
        case MCAN_Rx_FIFO1:
            MCAN_status[mcan].msgRam.startAddr.rxFIFO1 = startAddr;
            MCAN_status[mcan].msgRam.elementSizeByte.rxFIFO1 = MCAN_GetElementSizeByte(dataSize);
            MCAN_status[mcan].msgRam.nElement.rxFIFO1 = depth;
            MCAN_RXESC(mcan) =  MCAN_RXESC(mcan)
                             & ~MCAN_M_RXESC_F1DS
                             |  MCAN_S_RXESC_F1DS(dataSize);
            MCAN_RXF1C(mcan) = MCAN_S_RXF1C_F1S(depth)
                             | MCAN_S_SA(startAddr >> 2);
            break;
        
    }

}


// void MCAN_AssignRxBuffer(enum MCAN mcan, uint16_t bufferNum, uint32_t messageID)
// {
    
// }


void MCAN_SetupFilter(enum MCAN mcan, enum MCAN_ID filter, enum MCAN_FilterNonMatch nonMatchDest, bool rejectRemote)
{
    int16_t startAddr;
    uint16_t filterCount;
    
    startAddr = MCAN_AllocateMsgRam(mcan, 0);
    if(startAddr == -1)
    {
        // Unable to allocate Message RAM. Stop initialization
        HAL_FATAL(HAL_ErrorCode_ResourceConflict, (uint32_t)filter, 0U);
    }

    if(filter == MCAN_ID_Standard)
    {
        filterCount = 0;
        MCAN_status[mcan].msgRam.startAddr.filterStd = startAddr;
        MCAN_status[mcan].msgRam.nElement.filterStd = 0;
        MCAN_SIDFC(mcan) = MCAN_S_SIDFC_LSS(filterCount) | MCAN_S_SA(startAddr >> 2);
        MCAN_GFC(mcan) = MCAN_GFC(mcan)
                       & ~(MCAN_M_GFC_ANFS | MCAN_M_GFC_RRFS) // Clear ANFS and RRFS bits
                       | MCAN_S_GFC_ANFS(nonMatchDest)
                       | MCAN_S_GFC_RRFS(rejectRemote);
    } else {
        filterCount = 0;
        MCAN_status[mcan].msgRam.startAddr.filterExt = startAddr;
        MCAN_status[mcan].msgRam.nElement.filterExt = 0;
        MCAN_XIDFC(mcan) = MCAN_S_XIDFC_LSE(filterCount) | MCAN_S_SA(startAddr >> 2);
        MCAN_GFC(mcan) = MCAN_GFC(mcan)
                       & ~(MCAN_M_GFC_ANFE | MCAN_M_GFC_RRFE) // Clear ANFE and RRFE bits
                       | MCAN_S_GFC_ANFE(nonMatchDest)
                       | MCAN_S_GFC_RRFE(rejectRemote);
    }
}

void MCAN_AddFilter(enum MCAN mcan, enum MCAN_ID filter, enum MCAN_FilterType type, enum MCAN_FilterDest dest, uint32_t id1, uint32_t id2)
{
    uint16_t existingCount, filterLimit;
    uint16_t filterSizeByte;
    uint32_t filterAddr;

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
        HAL_FATAL(HAL_ErrorCode_ResourceConflict, (uint32_t)existingCount, (uint32_t)filterLimit);
    }

    filterAddr = MCAN_AllocateMsgRam(mcan, filterSizeByte);
    if(filterAddr == -1)
    {
        // Unable to allocate Message RAM. Stop initialization
        HAL_FATAL(HAL_ErrorCode_ResourceConflict, (uint32_t)filter, (uint32_t)filterSizeByte);
    }
    
    filterAddr += MCAN_MSG_RAM_BASE(mcan);

    if(filter == MCAN_ID_Standard)
    {
        ATOMIC32(filterAddr) = (type << 30) | (dest << 27) | (id1 << 16) | id2;
        MCAN_SIDFC(mcan) = MCAN_SIDFC(mcan) & ~MCAN_M_SIDFC_LSS | MCAN_S_SIDFC_LSS(existingCount + 1);
        MCAN_status[mcan].msgRam.nElement.filterStd = existingCount + 1;
    } else {
        ATOMIC32(filterAddr)     = (type << 31) | (dest << 30) | id1;
        ATOMIC32(filterAddr + 4) = id2;
        MCAN_XIDFC(mcan) = MCAN_XIDFC(mcan) & ~MCAN_M_XIDFC_LSE | MCAN_S_XIDFC_LSE(existingCount + 1);
        MCAN_status[mcan].msgRam.nElement.filterExt = existingCount + 1;
    }

}


// bool MCAN_SendBuffer(enum MCAN mcan, uint16_t index, struct MCAN_Message *message);
bool MCAN_SendQueue(enum MCAN mcan, struct MCAN_Message *message)
{
    bool txFull;
    uint16_t dataLengthByte;
    uint16_t dataLengthWord;
    uint16_t putIndex;
    uint32_t elementAddr;

    uint32_t messageID;
    uint32_t t0, t1;

    txFull = (MCAN_TXFQS(mcan) & MCAN_M_TXFQS_TFQF) != 0;

    if(txFull)
    {
        return false;
    }

    putIndex = (MCAN_TXFQS(mcan) & MCAN_M_TXFQS_TFQP) >> 16;
    elementAddr = MCAN_MSG_RAM_BASE(mcan)
                + MCAN_status[mcan].msgRam.startAddr.txBuffer
                + (uint32_t)MCAN_status[mcan].msgRam.elementSizeByte.txBuffer * putIndex;

    dataLengthByte = MCAN_GetDataLengthByte(message->length);
    dataLengthWord = (dataLengthByte + 3U) >> 2;

    if(dataLengthByte > MCAN_MAX_DATA_BYTES)
    {
        HAL_FATAL(HAL_ErrorCode_OutOfRange, (uint32_t)dataLengthByte, MCAN_MAX_DATA_BYTES);
    }

    if(message->idType == MCAN_ID_Standard && (message->id & 0x7FF) !=0)
    {
        messageID = message->id << 18;
    } else {
        messageID = message->id;
    }
    t0 = 0 << 31
       | message->idType   << 30
       | message->isRemote << 29
       | messageID;

    t1 = message->mode   << 20
       | message->length << 16;

    ATOMIC32(elementAddr) = t0;
    ATOMIC32(elementAddr + 4U) = t1;

    for(uint16_t i=0; i<dataLengthWord; i++)
    {
        ATOMIC32(elementAddr + 8U + ((uint32_t)i * 4U)) = message->data[i];
    }

    MCAN_TXBAR(mcan) = 1UL << putIndex;

    return true;

}

static bool MCAN_IsRxFIFOEmpty(enum MCAN mcan, enum MCAN_Rx rx)
{
    switch (rx)
    {
        case MCAN_Rx_FIFO0:
            return (MCAN_RXF0S(mcan) & MCAN_M_RXF0S_F0FL) == 0;

        case MCAN_Rx_FIFO1:
            return (MCAN_RXF1S(mcan) & MCAN_M_RXF1S_F1FL) == 0;

        default:
            return false;
    }
}

void MCAN_ReadRxFIFO(enum MCAN mcan, enum MCAN_Rx rx, struct MCAN_Message *message)
{
    uint16_t dataLengthByte;
    uint16_t dataLengthWord;
    uint32_t elementAddr;
    uint32_t t0, t1;
    uint32_t messageID;
    uint16_t getIndex;

    switch (rx) 
    {
        case MCAN_Rx_FIFO0:
            // Get read index from RXF0S register
            getIndex = (MCAN_RXF0S(mcan) & MCAN_M_RXF0S_F0GI) >> 8;

            elementAddr = MCAN_MSG_RAM_BASE(mcan)
                        + MCAN_status[mcan].msgRam.startAddr.rxFIFO0
                        + (uint32_t)MCAN_status[mcan].msgRam.elementSizeByte.rxFIFO0 * getIndex;
            break;
        
        case MCAN_Rx_FIFO1:
            // Get read index from RXF1S register
            getIndex = (MCAN_RXF1S(mcan) & MCAN_M_RXF1S_F1GI) >> 8;

            elementAddr = MCAN_MSG_RAM_BASE(mcan)
                        + MCAN_status[mcan].msgRam.startAddr.rxFIFO1
                        + (uint32_t)MCAN_status[mcan].msgRam.elementSizeByte.rxFIFO1 * getIndex;
            break;
        
        default:
            // Invalid RX type. Stop execution
            HAL_FATAL(HAL_ErrorCode_InvalidArgument, (uint32_t)rx, 0U);
    }

    t0 = ATOMIC32(elementAddr);
    t1 = ATOMIC32(elementAddr + 4);

    message->isRemote = (t0 >> 29) & 0x1;
    message->idType   = (t0 >> 30) & 0x1;

    if(message->idType == MCAN_ID_Standard)
    {
        messageID = (t0 >> 18) & 0x7FF;
    } else {
        messageID = t0 & 0x3FFFFFFF;
    }
    
    message->id     = messageID;
    message->timestamp = t1 & 0xFFFF;
    message->mode   = (t1 >> 20) & 0x7;
    message->length = (t1 >> 16) & 0xF;

    dataLengthByte = MCAN_GetDataLengthByte(message->length);
    dataLengthWord = (dataLengthByte + 3U) >> 2;
    if(dataLengthByte > MCAN_MAX_DATA_BYTES)
    {
        HAL_FATAL(HAL_ErrorCode_OutOfRange, (uint32_t)dataLengthByte, MCAN_MAX_DATA_BYTES);
    }

    for(uint16_t i=0; i<dataLengthWord; i++)
    {
        message->data[i] = ATOMIC32(elementAddr + 8U + ((uint32_t)i * 4U));
    }

    if(rx == MCAN_Rx_FIFO0)
    {
        MCAN_RXF0A(mcan) = MCAN_S_RXF0A_F0AI(getIndex);
    } else {
        MCAN_RXF1A(mcan) = MCAN_S_RXF1A_F1AI(getIndex);
    }
}

void MCAN_ReadRxBuffer(enum MCAN mcan, uint16_t bufferNum, struct MCAN_Message *message)
{
    uint16_t dataLengthByte;
    uint16_t dataLengthWord;
    uint32_t elementAddr;
    uint32_t t0, t1;
    uint32_t messageID;

    if(bufferNum >= MCAN_status[mcan].msgRam.nElement.rxBuffer)
    {
        // Invalid buffer number. Stop execution
        HAL_FATAL(HAL_ErrorCode_InvalidArgument, (uint32_t)bufferNum, MCAN_status[mcan].msgRam.nElement.rxBuffer);
    }

    elementAddr = MCAN_MSG_RAM_BASE(mcan)
                + MCAN_status[mcan].msgRam.startAddr.rxBuffer
                + (uint32_t)MCAN_status[mcan].msgRam.elementSizeByte.rxBuffer * bufferNum;

    t0 = ATOMIC32(elementAddr);
    t1 = ATOMIC32(elementAddr + 4);

    message->isRemote = (t0 >> 29) & 0x1;
    message->idType   = (t0 >> 30) & 0x1;

    if(message->idType == MCAN_ID_Standard)
    {
        messageID = (t0 >> 18) & 0x7FF;
    } else {
        messageID = t0 & 0x3FFFFFFF;
    }
    
    message->id     = messageID;
    message->timestamp = t1 & 0xFFFF;
    message->mode   = (t1 >> 20) & 0x7;
    message->length = (t1 >> 16) & 0xF;

    dataLengthByte = MCAN_GetDataLengthByte(message->length);
    dataLengthWord = (dataLengthByte + 3U) >> 2;
    if(dataLengthByte > MCAN_MAX_DATA_BYTES)
    {
        HAL_FATAL(HAL_ErrorCode_OutOfRange, (uint32_t)dataLengthByte, MCAN_MAX_DATA_BYTES);
    }

    for(uint16_t i=0; i<dataLengthWord; i++)
    {
        message->data[i] = ATOMIC32(elementAddr + 8U + ((uint32_t)i * 4U));
    }
}


void MCAN_Legacy_Setup(enum MCAN mcan)
{
    MCAN_SetupClassic(mcan, 10, 4, 125);

   MCAN_SetupTx(mcan, MCAN_TxQueueMode_FIFO, MCAN_DataSize_8Byte, 0, 24);
   MCAN_SetupRx(mcan, MCAN_Rx_FIFO0, MCAN_DataSize_8Byte, 64);
}

void MCAN_Legacy_AddFilter(enum MCAN mcan, uint16_t address, uint16_t dontCares, uint16_t simultaneousMsgs)
{
    if(!simultaneousMsgs)
    {
        // Clear all filters - reset to accept all
        MCAN_SetupFilter(mcan, MCAN_ID_Standard, MCAN_FilterNonMatch_Discard, false);
    }
    else
    {
        // Add filter for the given address
        MCAN_AddFilter(mcan, MCAN_ID_Standard, MCAN_FilterType_Range, MCAN_FilterDest_FIFO0, address, address);
    }
}

// Sending wrapper to support existing CANO implementation
bool MCAN_Legacy_Send(enum MCAN mcan, struct MCAN_Legacy_canMsg *msg)
{
    // Convert legacy message format to new format
    struct MCAN_Message message = 
    {
        .isRemote = false,
        .mode     = MCAN_Mode_Classic,
        .idType   = MCAN_ID_Standard,
        .id       = msg->address,
        .length   = msg->length,
        .data     = {0}
    };

    MCAN_PackBytes(message.data, msg->data, msg->length);
    
    return MCAN_SendQueue(mcan, &message);
}
bool MCAN_Legacy_Receive(enum MCAN mcan, struct MCAN_Legacy_canMsg *msg)
{
    struct MCAN_Message message = {0};

    if(MCAN_IsRxFIFOEmpty(mcan, MCAN_Rx_FIFO0))
    {
        return false;
    }

    MCAN_ReadRxFIFO(mcan, MCAN_Rx_FIFO0, &message);

    msg->address = (uint16_t)message.id;
    msg->length = MCAN_GetDataLengthByte(message.length);
    MCAN_UnpackBytes(msg->data, message.data, msg->length);

    return true;
}

