#ifndef MCAN_PRIVATE_H_
#define MCAN_PRIVATE_H_

#include "base.h"

// Submodule base addresses
#define MCAN_SS_BASE(mcan)          (MCAN_SS_START_ADDR + MCAN_SS_INCREMENT_ADDR * (mcan))
#define MCAN_BASE(mcan)             (MCAN_START_ADDR + MCAN_INCREMENT_ADDR * (mcan))
#define MCAN_ERR_BASE(mcan)         (MCAN_ERR_START_ADDR + MCAN_ERR_INCREMENT_ADDR * (mcan))
#define MCAN_MSG_RAM_BASE(mcan)     (MCAN_MSG_RAM_START_ADDR + MCAN_MSG_RAM_INCREMENT_ADDR * (mcan))

#define MCAN_LEGACY_BYTE_SIZE   (8)
    
//
// Register definitions
//
#define MCANSS_PID(mcan)   ATOMIC32(MCAN_SS_BASE(mcan) + 0x00)  // MCAN Subsystem Revision Register
#define MCANSS_CTRL(mcan)  ATOMIC32(MCAN_SS_BASE(mcan) + 0x04)  // MCAN Subsystem Control Register
#define MCANSS_STAT(mcan)  ATOMIC32(MCAN_SS_BASE(mcan) + 0x08)  // MCAN Subsystem Status Register
#define MCANSS_ICS(mcan)   ATOMIC32(MCAN_SS_BASE(mcan) + 0x0C)  // MCAN Subsystem Interrupt Clear Shadow Register
#define MCANSS_IRS(mcan)   ATOMIC32(MCAN_SS_BASE(mcan) + 0x10)  // MCAN Subsystem Interrupt Raw Status Register
#define MCANSS_IECS(mcan)  ATOMIC32(MCAN_SS_BASE(mcan) + 0x14)  // MCAN Subsystem Interrupt Enable Clear Shadow Register
#define MCANSS_IE(mcan)    ATOMIC32(MCAN_SS_BASE(mcan) + 0x18)  // MCAN Subsystem Interrupt Enable Register
#define MCANSS_IES(mcan)   ATOMIC32(MCAN_SS_BASE(mcan) + 0x1C)  // MCAN Subsystem Interrupt Enable Status
#define MCANSS_EOI(mcan)   ATOMIC32(MCAN_SS_BASE(mcan) + 0x20)  // MCAN Subsystem End of Interrupt
#define MCANSS_EXT_TS_PRESCALER(mcan)              ATOMIC32(MCAN_SS_BASE(mcan) + 0x24) // MCAN Subsystem External Timestamp Prescaler 0
#define MCANSS_EXT_TS_UNSERVICED_INTR_CNTR(mcan)   ATOMIC32(MCAN_SS_BASE(mcan) + 0x28) // MCAN Subsystem External Timestamp Unserviced Interrupts Counter

#define MCAN_CREL(mcan)     ATOMIC32(MCAN_BASE(mcan) + 0x00)    // MCAN Core Release Register
#define MCAN_ENDN(mcan)     ATOMIC32(MCAN_BASE(mcan) + 0x04)    // MCAN Endian Register
#define MCAN_DBTP(mcan)     ATOMIC32(MCAN_BASE(mcan) + 0x0C)    // MCAN Data Bit Timing and Prescaler Register
#define MCAN_TEST(mcan)     ATOMIC32(MCAN_BASE(mcan) + 0x10)    // MCAN Test Register
#define MCAN_RWD(mcan)      ATOMIC32(MCAN_BASE(mcan) + 0x14)    // MCAN RAM Watchdog
#define MCAN_CCCR(mcan)     ATOMIC32(MCAN_BASE(mcan) + 0x18)    // MCAN CC Control Register
#define MCAN_NBTP(mcan)     ATOMIC32(MCAN_BASE(mcan) + 0x1C)    // MCAN Nominal Bit Timing and Prescaler Register
#define MCAN_TSCC(mcan)     ATOMIC32(MCAN_BASE(mcan) + 0x20)    // MCAN Timestamp Counter Configuration
#define MCAN_TSCV(mcan)     ATOMIC32(MCAN_BASE(mcan) + 0x24)    // MCAN Timestamp Counter Value
#define MCAN_TOCC(mcan)     ATOMIC32(MCAN_BASE(mcan) + 0x28)    // MCAN Timeout Counter Configuration
#define MCAN_TOCV(mcan)     ATOMIC32(MCAN_BASE(mcan) + 0x2C)    // MCAN Timeout Counter Value
#define MCAN_ECR(mcan)      ATOMIC32(MCAN_BASE(mcan) + 0x40)    // MCAN Error Counter Register
#define MCAN_PSR(mcan)      ATOMIC32(MCAN_BASE(mcan) + 0x44)    // MCAN Protocol Status Register
#define MCAN_TDCR(mcan)     ATOMIC32(MCAN_BASE(mcan) + 0x48)    // MCAN Transmitter Delay Compensation Register
#define MCAN_IR(mcan)       ATOMIC32(MCAN_BASE(mcan) + 0x50)    // MCAN Interrupt Register
#define MCAN_IE(mcan)       ATOMIC32(MCAN_BASE(mcan) + 0x54)    // MCAN Interrupt Enable
#define MCAN_ILS(mcan)      ATOMIC32(MCAN_BASE(mcan) + 0x58)    // MCAN Interrupt Line Select
#define MCAN_ILE(mcan)      ATOMIC32(MCAN_BASE(mcan) + 0x5C)    // MCAN Interrupt Line Enable
#define MCAN_GFC(mcan)      ATOMIC32(MCAN_BASE(mcan) + 0x80)    // MCAN Global Filter Configuration
#define MCAN_SIDFC(mcan)    ATOMIC32(MCAN_BASE(mcan) + 0x84)    // MCAN Standard ID Filter Configuration
#define MCAN_XIDFC(mcan)    ATOMIC32(MCAN_BASE(mcan) + 0x88)    // MCAN Extended ID Filter Configuration
#define MCAN_XIDAM(mcan)    ATOMIC32(MCAN_BASE(mcan) + 0x90)    // MCAN Extended ID and Mask
#define MCAN_HPMS(mcan)     ATOMIC32(MCAN_BASE(mcan) + 0x94)    // MCAN High Priority Message Status
#define MCAN_NDAT(mcan,buffer)  ATOMIC32(MCAN_BASE(mcan) + 0x98 + (((buffer) >> 5) * 4))    // MCAN New Data 1/2
#define MCAN_RXF0C(mcan)    ATOMIC32(MCAN_BASE(mcan) + 0xA0)    // MCAN Rx FIFO 0 Configuration
#define MCAN_RXF0S(mcan)    ATOMIC32(MCAN_BASE(mcan) + 0xA4)    // MCAN Rx FIFO 0 Status
#define MCAN_RXF0A(mcan)    ATOMIC32(MCAN_BASE(mcan) + 0xA8)    // MCAN Rx FIFO 0 Acknowledge
#define MCAN_RXF1C(mcan)    ATOMIC32(MCAN_BASE(mcan) + 0xB0)    // MCAN Rx FIFO 1 Configuration
#define MCAN_RXF1S(mcan)    ATOMIC32(MCAN_BASE(mcan) + 0xB4)    // MCAN Rx FIFO 1 Status
#define MCAN_RXF1A(mcan)    ATOMIC32(MCAN_BASE(mcan) + 0xB8)    // MCAN Rx FIFO 1 Acknowledge
#define MCAN_RXBC(mcan)     ATOMIC32(MCAN_BASE(mcan) + 0xAC)    // MCAN Rx Buffer Configuration
#define MCAN_RXESC(mcan)    ATOMIC32(MCAN_BASE(mcan) + 0xBC)    // MCAN Rx Buffer / FIFO Element Size Configuration
#define MCAN_TXBC(mcan)     ATOMIC32(MCAN_BASE(mcan) + 0xC0)    // MCAN Tx Buffer Configuration
#define MCAN_TXFQS(mcan)    ATOMIC32(MCAN_BASE(mcan) + 0xC4)    // MCAN Tx FIFO / Queue Status
#define MCAN_TXESC(mcan)    ATOMIC32(MCAN_BASE(mcan) + 0xC8)    // MCAN Tx Buffer Element Size Configuration
#define MCAN_TXBRP(mcan)    ATOMIC32(MCAN_BASE(mcan) + 0xCC)    // MCAN Tx Buffer Request Pending
#define MCAN_TXBAR(mcan)    ATOMIC32(MCAN_BASE(mcan) + 0xD0)    // MCAN Tx Buffer Add Request
#define MCAN_TXBCR(mcan)    ATOMIC32(MCAN_BASE(mcan) + 0xD4)    // MCAN Tx Buffer Cancellation Request
#define MCAN_TXBTO(mcan)    ATOMIC32(MCAN_BASE(mcan) + 0xD8)    // MCAN Tx Buffer Transmission Occurred
#define MCAN_TXBCF(mcan)    ATOMIC32(MCAN_BASE(mcan) + 0xDC)    // MCAN Tx Buffer Cancellation Finished
#define MCAN_TXBTIE(mcan)   ATOMIC32(MCAN_BASE(mcan) + 0xE0)    // MCAN Tx Buffer Transmission Interrupt Enable
#define MCAN_TXBCIE(mcan)   ATOMIC32(MCAN_BASE(mcan) + 0xE4)    // MCAN Tx Buffer Cancellation Finished Interrupt Enable
#define MCAN_TXEFC(mcan)    ATOMIC32(MCAN_BASE(mcan) + 0xF0)    // MCAN Tx Event FIFO Configuration
#define MCAN_TXEFS(mcan)    ATOMIC32(MCAN_BASE(mcan) + 0xF4)    // MCAN Tx Event FIFO Status
#define MCAN_TXEFA(mcan)    ATOMIC32(MCAN_BASE(mcan) + 0xF8)    // MCAN Tx Event FIFO Acknowledge

#define MCANERR_REV(mcan)               ATOMIC32(MCAN_ERR_BASE(mcan) + 0x000)   // MCAN Error Aggregator Revision Register
#define MCANERR_VECTOR(mcan)            ATOMIC32(MCAN_ERR_BASE(mcan) + 0x008)   // MCAN ECC Vector Register
#define MCANERR_STAT(mcan)              ATOMIC32(MCAN_ERR_BASE(mcan) + 0x00C)   // MCAN Error Misc Status
#define MCANERR_WRAP_REV(mcan)          ATOMIC32(MCAN_ERR_BASE(mcan) + 0x010)   // MCAN ECC Wrapper Revision Register
#define MCANERR_CTRL(mcan)              ATOMIC32(MCAN_ERR_BASE(mcan) + 0x014)   // MCAN ECC Control
#define MCANERR_CTRL1(mcan)             ATOMIC32(MCAN_ERR_BASE(mcan) + 0x018)   // MCAN ECC Error Control 1 Register
#define MCANERR_CTRL2(mcan)             ATOMIC32(MCAN_ERR_BASE(mcan) + 0x01C)   // MCAN ECC Error Control 2 Register
#define MCANERR_STAT1(mcan)             ATOMIC32(MCAN_ERR_BASE(mcan) + 0x020)   // MCAN ECC Error Status 1 Register
#define MCANERR_STAT2(mcan)             ATOMIC32(MCAN_ERR_BASE(mcan) + 0x024)   // MCAN ECC Error Status 2 Register
#define MCANERR_STAT3(mcan)             ATOMIC32(MCAN_ERR_BASE(mcan) + 0x028)   // MCAN ECC Error Status 3 Register
#define MCANERR_SEC_EOI(mcan)           ATOMIC32(MCAN_ERR_BASE(mcan) + 0x03C)   // MCAN Single Error Corrected End of Interrupt Register
#define MCANERR_SEC_STATUS(mcan)        ATOMIC32(MCAN_ERR_BASE(mcan) + 0x040)   // MCAN Single Error Corrected Interrupt Status Register
#define MCANERR_SEC_ENABLE_SET(mcan)    ATOMIC32(MCAN_ERR_BASE(mcan) + 0x080)   // MCAN Single Error Corrected Interrupt Enable Set Register
#define MCANERR_SEC_ENABLE_CLR(mcan)    ATOMIC32(MCAN_ERR_BASE(mcan) + 0x0C0)   // MCAN Single Error Corrected Interrupt Enable Clear Register
#define MCANERR_DED_EOI(mcan)           ATOMIC32(MCAN_ERR_BASE(mcan) + 0x13C)   // MCAN Double Error Detected End of Interrupt Register
#define MCANERR_DED_STATUS(mcan)        ATOMIC32(MCAN_ERR_BASE(mcan) + 0x140)   // MCAN Double Error Detected Interrupt Status Register
#define MCANERR_DED_ENABLE_SET(mcan)    ATOMIC32(MCAN_ERR_BASE(mcan) + 0x180)   // MCAN Double Error Detected Interrupt Enable Set Register
#define MCANERR_DED_ENABLE_CLR(mcan)    ATOMIC32(MCAN_ERR_BASE(mcan) + 0x1C0)   // MCAN Double Error Detected Interrupt Enable Clear Register
#define MCANERR_AGGR_ENABLE_SET(mcan)   ATOMIC32(MCAN_ERR_BASE(mcan) + 0x200)   // MCAN Error Aggregator Enable Set Register
#define MCANERR_AGGR_ENABLE_CLR(mcan)   ATOMIC32(MCAN_ERR_BASE(mcan) + 0x204)   // MCAN Error Aggregator Enable Clear Register
#define MCANERR_AGGR_STATUS_SET(mcan)   ATOMIC32(MCAN_ERR_BASE(mcan) + 0x208)   // MCAN Error Aggregator Status Set Register
#define MCANERR_AGGR_STATUS_CLR(mcan)   ATOMIC32(MCAN_ERR_BASE(mcan) + 0x20C)   // MCAN Error Aggregator Status Clear Register



#define MCANSS_M_STAT_MEM_INIT_DONE     ( 0x01U <<  1 )

// MCANSS_CTRL
#define MCANSS_M_CTRL_DBGSUSP       (1UL << 3)
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
#define MCAN_S_DBTP_DTSEG2(x)       ((uint32_t)(x) <<  4U)
#define MCAN_S_DBTP_DTSEG1(x)       ((uint32_t)(x) <<  8U)
#define MCAN_S_DBTP_DBRP(x)         ((uint32_t)(x) << 16U)
#define MCAN_S_DBTP_DSJW(x)         ((uint32_t)(x) <<  0U)

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

// Common message RAM start-address field for SIDFC/XIDFC/RXFxC/RXBC/TXBC
#define MCAN_S_SA(x)                ((uint32_t)(x) << 2U)
#define MCAN_M_SA                   MCAN_S_SA(MASK(14))

// MCAN_RXF0C / MCAN_RXF1C
#define MCAN_S_RXFxC_FxS(x)         ((uint32_t)(x) << 16U)

#define MCAN_M_RXFxC_FxS            MCAN_S_RXFxC_FxS(MASK(7))

// MCAN_RXF0S / MCAN_RXF1S
#define MCAN_S_RXFxS_FxFL(x)        ((uint32_t)(x) << 0U)
#define MCAN_S_RXFxS_FxGI(x)        ((uint32_t)(x) << 8U)

#define MCAN_M_RXFxS_FxFL           MCAN_S_RXFxS_FxFL(MASK(7))
#define MCAN_M_RXFxS_FxGI           MCAN_S_RXFxS_FxGI(MASK(6))

// MCAN_RXF0A / MCAN_RXF1A
#define MCAN_S_RXFxA_FxAI(x)        ((uint32_t)(x) << 0U)

// MCAN_TXBC
#define MCAN_S_TXBC_NDTB(x)         ((uint32_t)(x) << 16U)
#define MCAN_S_TXBC_TFQS(x)         ((uint32_t)(x) << 24U)
#define MCAN_S_TXBC_TFQM(x)         ((uint32_t)(x) << 30U)

#define MCAN_M_TXBC_NDTB            MCAN_S_TXBC_NDTB(MASK(6))
#define MCAN_M_TXBC_TFQS            MCAN_S_TXBC_TFQS(MASK(6))
#define MCAN_M_TXBC_TFQM            MCAN_S_TXBC_TFQM(MASK(1))

// MCAN_TXFQS
#define MCAN_M_TXFQS_TFFL           (MASK(6) <<  0)
#define MCAN_M_TXFQS_TFQF           (MASK(1) << 21)
#define MCAN_M_TXFQS_TFQP           (MASK(5) << 16)

#endif /* MCAN_PRIVATE_H_ */
