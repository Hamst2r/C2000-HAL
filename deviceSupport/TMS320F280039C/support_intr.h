#ifndef SUPPORT_INTR_H_
#define SUPPORT_INTR_H_

#define INTR_PIE_BASE               (0x00000CE0UL)
#define INTR_VECTOR_BASE            (0x00000D00UL)
#define INTR_Src_S_VECTOR_GROUP(x)  ((x) << 8)

enum INTR_Src
{   
    // CPU interrupt
    INTR_Src_00_00_Reset       = INTR_Src_S_VECTOR_GROUP( 0) |  0,   // CPU Reset
    INTR_Src_00_01_Group1      = INTR_Src_S_VECTOR_GROUP( 0) |  1,   // PIR Group  1
    INTR_Src_00_02_Group2      = INTR_Src_S_VECTOR_GROUP( 0) |  2,   // PIR Group  2
    INTR_Src_00_03_Group3      = INTR_Src_S_VECTOR_GROUP( 0) |  3,   // PIR Group  3
    INTR_Src_00_04_Group4      = INTR_Src_S_VECTOR_GROUP( 0) |  4,   // PIR Group  4
    INTR_Src_00_05_Group5      = INTR_Src_S_VECTOR_GROUP( 0) |  5,   // PIR Group  5
    INTR_Src_00_06_Group6      = INTR_Src_S_VECTOR_GROUP( 0) |  6,   // PIR Group  6
    INTR_Src_00_07_Group7      = INTR_Src_S_VECTOR_GROUP( 0) |  7,   // PIR Group  7
    INTR_Src_00_08_Group8      = INTR_Src_S_VECTOR_GROUP( 0) |  8,   // PIR Group  8
    INTR_Src_00_09_Group9      = INTR_Src_S_VECTOR_GROUP( 0) |  9,   // PIR Group  9
    INTR_Src_00_10_Group10     = INTR_Src_S_VECTOR_GROUP( 0) | 10,   // PIR Group 10
    INTR_Src_00_11_Group11     = INTR_Src_S_VECTOR_GROUP( 0) | 11,   // PIR Group 11
    INTR_Src_00_12_Group12     = INTR_Src_S_VECTOR_GROUP( 0) | 12,   // PIR Group 12
    INTR_Src_00_13_Timer1      = INTR_Src_S_VECTOR_GROUP( 0) | 13,   // CPU Timer 1 Interrupt
    INTR_Src_00_14_Timer2      = INTR_Src_S_VECTOR_GROUP( 0) | 14,   // CPU Timer 2 Interrupt
    INTR_Src_00_15_DataLog     = INTR_Src_S_VECTOR_GROUP( 0) | 15,   // CPU Data Logging Interrupt
    INTR_Src_00_16_RTOS        = INTR_Src_S_VECTOR_GROUP( 0) | 16,   // CPU RTOS Interrupt
    INTR_Src_00_18_NMI         = INTR_Src_S_VECTOR_GROUP( 0) | 18,   // Non-Maskable Interrupt
    INTR_Src_00_19_Illegal     = INTR_Src_S_VECTOR_GROUP( 0) | 19,   // Illegal Instruction Trap
    INTR_Src_00_20_User1       = INTR_Src_S_VECTOR_GROUP( 0) | 20,   // User-Defined Trap 1
    INTR_Src_00_21_User2       = INTR_Src_S_VECTOR_GROUP( 0) | 21,   // User-Defined Trap 2
    INTR_Src_00_22_User3       = INTR_Src_S_VECTOR_GROUP( 0) | 22,   // User-Defined Trap 3
    INTR_Src_00_23_User4       = INTR_Src_S_VECTOR_GROUP( 0) | 23,   // User-Defined Trap 4
    INTR_Src_00_24_User5       = INTR_Src_S_VECTOR_GROUP( 0) | 24,   // User-Defined Trap 5
    INTR_Src_00_25_User6       = INTR_Src_S_VECTOR_GROUP( 0) | 25,   // User-Defined Trap 6
    INTR_Src_00_26_User7       = INTR_Src_S_VECTOR_GROUP( 0) | 26,   // User-Defined Trap 7
    INTR_Src_00_27_User8       = INTR_Src_S_VECTOR_GROUP( 0) | 27,   // User-Defined Trap 8
    INTR_Src_00_28_User9       = INTR_Src_S_VECTOR_GROUP( 0) | 28,   // User-Defined Trap 9
    INTR_Src_00_29_User10      = INTR_Src_S_VECTOR_GROUP( 0) | 29,   // User-Defined Trap 10
    INTR_Src_00_30_User11      = INTR_Src_S_VECTOR_GROUP( 0) | 30,   // User-Defined Trap 11
    INTR_Src_00_31_User12      = INTR_Src_S_VECTOR_GROUP( 0) | 31,   // User-Defined Trap 12

    // PIE Group 1
    INTR_Src_01_01_ADCA1       = INTR_Src_S_VECTOR_GROUP( 1) |  0,   // ADC-A Interrupt 1
    INTR_Src_01_02_ADCB1       = INTR_Src_S_VECTOR_GROUP( 1) |  1,   // ADC-B Interrupt 1
    INTR_Src_01_03_ADCC1       = INTR_Src_S_VECTOR_GROUP( 1) |  2,   // ADC-C Interrupt 1
    INTR_Src_01_04_XINT1       = INTR_Src_S_VECTOR_GROUP( 1) |  3,   // External Interrupt 1
    INTR_Src_01_05_XINT2       = INTR_Src_S_VECTOR_GROUP( 1) |  4,   // External Interrupt 2
    INTR_Src_01_07_Timer0      = INTR_Src_S_VECTOR_GROUP( 1) |  6,   // CPU Timer 0 Interrupt
    INTR_Src_01_08_Wake        = INTR_Src_S_VECTOR_GROUP( 1) |  7,   // MCU Wake Interrupt
    INTR_Src_01_10_SYS_ERR     = INTR_Src_S_VECTOR_GROUP( 1) |  9,   // System Error Interrupt

    // PIE Group 2
    INTR_Src_02_01_EPWM1TZ     = INTR_Src_S_VECTOR_GROUP( 2) |  0,   // ePWM1 Trip Zone Interrupt
    INTR_Src_02_02_EPWM2TZ     = INTR_Src_S_VECTOR_GROUP( 2) |  1,   // ePWM2 Trip Zone Interrupt
    INTR_Src_02_03_EPWM3TZ     = INTR_Src_S_VECTOR_GROUP( 2) |  2,   // ePWM3 Trip Zone Interrupt
    INTR_Src_02_04_EPWM4TZ     = INTR_Src_S_VECTOR_GROUP( 2) |  3,   // ePWM4 Trip Zone Interrupt
    INTR_Src_02_05_EPWM5TZ     = INTR_Src_S_VECTOR_GROUP( 2) |  4,   // ePWM5 Trip Zone Interrupt
    INTR_Src_02_06_EPWM6TZ     = INTR_Src_S_VECTOR_GROUP( 2) |  5,   // ePWM6 Trip Zone Interrupt
    INTR_Src_02_07_EPWM7TZ     = INTR_Src_S_VECTOR_GROUP( 2) |  6,   // ePWM7 Trip Zone Interrupt
    INTR_Src_02_08_EPWM8TZ     = INTR_Src_S_VECTOR_GROUP( 2) |  7,   // ePWM8 Trip Zone Interrupt

    // PIE Group 3
    INTR_Src_03_01_EPWM1       = INTR_Src_S_VECTOR_GROUP( 3) |  0,   // ePWM1 Interrupt
    INTR_Src_03_02_EPWM2       = INTR_Src_S_VECTOR_GROUP( 3) |  1,   // ePWM2 Interrupt
    INTR_Src_03_03_EPWM3       = INTR_Src_S_VECTOR_GROUP( 3) |  2,   // ePWM3 Interrupt
    INTR_Src_03_04_EPWM4       = INTR_Src_S_VECTOR_GROUP( 3) |  3,   // ePWM4 Interrupt
    INTR_Src_03_05_EPWM5       = INTR_Src_S_VECTOR_GROUP( 3) |  4,   // ePWM5 Interrupt
    INTR_Src_03_06_EPWM6       = INTR_Src_S_VECTOR_GROUP( 3) |  5,   // ePWM6 Interrupt
    INTR_Src_03_07_EPWM7       = INTR_Src_S_VECTOR_GROUP( 3) |  6,   // ePWM7 Interrupt
    INTR_Src_03_08_EPWM8       = INTR_Src_S_VECTOR_GROUP( 3) |  7,   // ePWM8 Interrupt

    // PIE Group 4
    INTR_Src_04_01_ECAP1       = INTR_Src_S_VECTOR_GROUP( 4) |  0,   // eCAP1 Interrupt
    INTR_Src_04_02_ECAP2       = INTR_Src_S_VECTOR_GROUP( 4) |  1,   // eCAP2 Interrupt
    INTR_Src_04_03_ECAP3       = INTR_Src_S_VECTOR_GROUP( 4) |  2,   // eCAP3 Interrupt
    INTR_Src_04_11_ECAP3INT2   = INTR_Src_S_VECTOR_GROUP( 4) | 10,   // eCAP3 Interrupt 2

    // PIE Group 5
    INTR_Src_05_01_EQEP1       = INTR_Src_S_VECTOR_GROUP( 5) |  0,   // eQEP1 Interrupt
    INTR_Src_05_02_EQEP2       = INTR_Src_S_VECTOR_GROUP( 5) |  1,   // eQEP2 Interrupt
    INTR_Src_05_05_CLB1        = INTR_Src_S_VECTOR_GROUP( 5) |  4,   // CLB1 Interrupt
    INTR_Src_05_06_CLB2        = INTR_Src_S_VECTOR_GROUP( 5) |  5,   // CLB2 Interrupt
    INTR_Src_05_07_CLB3        = INTR_Src_S_VECTOR_GROUP( 5) |  6,   // CLB3 Interrupt
    INTR_Src_05_08_CLB4        = INTR_Src_S_VECTOR_GROUP( 5) |  7,   // CLB4 Interrupt
    INTR_Src_05_09_SDFM1       = INTR_Src_S_VECTOR_GROUP( 5) |  8,   // SDFM1 Interrupt
    INTR_Src_05_10_SDFM2       = INTR_Src_S_VECTOR_GROUP( 5) |  9,   // SDFM2 Interrupt
    INTR_Src_05_13_SDFM1DR1    = INTR_Src_S_VECTOR_GROUP( 5) | 12,   // SDFM1 Data Ready Interrupt 1
    INTR_Src_05_14_SDFM1DR2    = INTR_Src_S_VECTOR_GROUP( 5) | 13,   // SDFM1 Data Ready Interrupt 2
    INTR_Src_05_15_SDFM1DR3    = INTR_Src_S_VECTOR_GROUP( 5) | 14,   // SDFM1 Data Ready Interrupt 3
    INTR_Src_05_16_SDFM1DR4    = INTR_Src_S_VECTOR_GROUP( 5) | 15,   // SDFM1 Data Ready Interrupt 4

    // PIE Group 6
    INTR_Src_06_01_SPIA_RX     = INTR_Src_S_VECTOR_GROUP( 6) |  0,   // SPI-A Receive Interrupt
    INTR_Src_06_02_SPIA_TX     = INTR_Src_S_VECTOR_GROUP( 6) |  1,   // SPI-A Transmit Interrupt
    INTR_Src_06_03_SPIB_RX     = INTR_Src_S_VECTOR_GROUP( 6) |  2,   // SPI-B Receive Interrupt
    INTR_Src_06_04_SPIB_TX     = INTR_Src_S_VECTOR_GROUP( 6) |  3,   // SPI-B Transmit Interrupt
    INTR_Src_06_13_SDFM2DR1    = INTR_Src_S_VECTOR_GROUP( 6) | 12,   // SDFM1 Data Ready Interrupt 1
    INTR_Src_06_14_SDFM2DR2    = INTR_Src_S_VECTOR_GROUP( 6) | 13,   // SDFM1 Data Ready Interrupt 2
    INTR_Src_06_15_SDFM2DR3    = INTR_Src_S_VECTOR_GROUP( 6) | 14,   // SDFM1 Data Ready Interrupt 3
    INTR_Src_06_16_SDFM2DR4    = INTR_Src_S_VECTOR_GROUP( 6) | 15,   // SDFM1 Data Ready Interrupt 4

    // PIE Group 7
    INTR_Src_07_01_DMA_CH1     = INTR_Src_S_VECTOR_GROUP( 7) |  0,   // DMA Channel 1 Interrupt
    INTR_Src_07_02_DMA_CH2     = INTR_Src_S_VECTOR_GROUP( 7) |  1,   // DMA Channel 2 Interrupt
    INTR_Src_07_03_DMA_CH3     = INTR_Src_S_VECTOR_GROUP( 7) |  2,   // DMA Channel 3 Interrupt
    INTR_Src_07_04_DMA_CH4     = INTR_Src_S_VECTOR_GROUP( 7) |  3,   // DMA Channel 4 Interrupt
    INTR_Src_07_05_DMA_CH5     = INTR_Src_S_VECTOR_GROUP( 7) |  4,   // DMA Channel 5 Interrupt
    INTR_Src_07_06_DMA_CH6     = INTR_Src_S_VECTOR_GROUP( 7) |  5,   // DMA Channel 6 Interrupt
    INTR_Src_07_11_FSI_TX1     = INTR_Src_S_VECTOR_GROUP( 7) | 10,   // FSI Transmit Interrupt 1
    INTR_Src_07_12_FSI_TX2     = INTR_Src_S_VECTOR_GROUP( 7) | 11,   // FSI Transmit Interrupt 2
    INTR_Src_07_13_FSI_RX1     = INTR_Src_S_VECTOR_GROUP( 7) | 12,   // FSI Receive Interrupt 1
    INTR_Src_07_14_FSI_RX2     = INTR_Src_S_VECTOR_GROUP( 7) | 13,   // FSI Receive Interrupt 2
    INTR_Src_07_16_DCC0        = INTR_Src_S_VECTOR_GROUP( 7) | 15,   // Dual-Clock Comparator 0 Interrupt

    // PIE Group 8
    INTR_Src_08_01_I2CA        = INTR_Src_S_VECTOR_GROUP( 8) |  0,   // I2C-A Interrupt
    INTR_Src_08_02_I2CA_FIFO   = INTR_Src_S_VECTOR_GROUP( 8) |  1,   // I2C-A FIFO Interrupt
    INTR_Src_08_03_I2CB        = INTR_Src_S_VECTOR_GROUP( 8) |  2,   // I2C-B Interrupt
    INTR_Src_08_04_I2CB_FIFO   = INTR_Src_S_VECTOR_GROUP( 8) |  3,   // I2C-B FIFO Interrupt
    INTR_Src_08_09_LINA0       = INTR_Src_S_VECTOR_GROUP( 8) |  8,   // LIN-A Interrupt 0
    INTR_Src_08_10_LINA1       = INTR_Src_S_VECTOR_GROUP( 8) |  9,   // LIN-A Interrupt 1
    INTR_Src_08_11_LINB0       = INTR_Src_S_VECTOR_GROUP( 8) | 10,   // LIN-B Interrupt 0
    INTR_Src_08_12_LINB1       = INTR_Src_S_VECTOR_GROUP( 8) | 11,   // LIN-B Interrupt 1
    INTR_Src_08_13_PMBusA      = INTR_Src_S_VECTOR_GROUP( 8) | 12,   // PMBus-A Interrupt
    INTR_Src_08_16_DCC1        = INTR_Src_S_VECTOR_GROUP( 8) | 15,   // Dual-Clock Comparator 1 Interrupt

    // PIE Group 9
    INTR_Src_09_01_SCIA_RX     = INTR_Src_S_VECTOR_GROUP( 9) |  0,   // SPI-A Receive Interrupt
    INTR_Src_09_02_SCIA_TX     = INTR_Src_S_VECTOR_GROUP( 9) |  1,   // SPI-A Transmit Interrupt
    INTR_Src_09_03_SCIB_RX     = INTR_Src_S_VECTOR_GROUP( 9) |  2,   // SPI-B Receive Interrupt
    INTR_Src_09_04_SCIB_TX     = INTR_Src_S_VECTOR_GROUP( 9) |  3,   // SPI-B Transmit Interrupt
    INTR_Src_09_05_DCANA0      = INTR_Src_S_VECTOR_GROUP( 9) |  4,   // DCAN-A Interrupt 0
    INTR_Src_09_06_DCANA1      = INTR_Src_S_VECTOR_GROUP( 9) |  5,   // DCAN-A Interrupt 1
    INTR_Src_09_09_MCAN_0      = INTR_Src_S_VECTOR_GROUP( 9) |  8,   // MCAN-A Interrupt 0
    INTR_Src_09_10_MCAN_1      = INTR_Src_S_VECTOR_GROUP( 9) |  9,   // MCAN-A Interrupt 1
    INTR_Src_09_11_MCAN_ECC    = INTR_Src_S_VECTOR_GROUP( 9) | 10,   // MCAN-A ECC Interrupt
    INTR_Src_09_12_MCAN_Wake   = INTR_Src_S_VECTOR_GROUP( 9) | 11,   // MCAN-A Wake Interrupt
    INTR_Src_09_13_BGCRC_CPU   = INTR_Src_S_VECTOR_GROUP( 9) | 12,   // CPU Background CRC Interrupt
    INTR_Src_09_16_HICA        = INTR_Src_S_VECTOR_GROUP( 9) | 15,   // HIC-A Interrupt

    // PIE Group 10
    INTR_Src_10_01_ADCA_EVT    = INTR_Src_S_VECTOR_GROUP(10) |  0,   // ADC-A Event Interrupt
    INTR_Src_10_02_ADCA2       = INTR_Src_S_VECTOR_GROUP(10) |  1,   // ADC-A Interrupt 2
    INTR_Src_10_03_ADCA3       = INTR_Src_S_VECTOR_GROUP(10) |  2,   // ADC-A Interrupt 3
    INTR_Src_10_04_ADCA4       = INTR_Src_S_VECTOR_GROUP(10) |  3,   // ADC-A Interrupt 4
    INTR_Src_10_05_ADCB_EVT    = INTR_Src_S_VECTOR_GROUP(10) |  4,   // ADC-B Event Interrupt
    INTR_Src_10_06_ADCB2       = INTR_Src_S_VECTOR_GROUP(10) |  5,   // ADC-B Interrupt 2
    INTR_Src_10_07_ADCB3       = INTR_Src_S_VECTOR_GROUP(10) |  6,   // ADC-B Interrupt 3
    INTR_Src_10_08_ADCB4       = INTR_Src_S_VECTOR_GROUP(10) |  7,   // ADC-B Interrupt 4
    INTR_Src_10_09_ADCC_EVT    = INTR_Src_S_VECTOR_GROUP(10) |  8,   // ADC-C Event Interrupt
    INTR_Src_10_10_ADCC2       = INTR_Src_S_VECTOR_GROUP(10) |  9,   // ADC-C Interrupt 2
    INTR_Src_10_11_ADCC3       = INTR_Src_S_VECTOR_GROUP(10) | 10,   // ADC-C Interrupt 3
    INTR_Src_10_12_ADCC4       = INTR_Src_S_VECTOR_GROUP(10) | 11,   // ADC-C Interrupt 4

    // PIE Group 11
    INTR_Src_11_01_CLA1        = INTR_Src_S_VECTOR_GROUP(11) |  0,   // CLA Interrupt 1
    INTR_Src_11_02_CLA2        = INTR_Src_S_VECTOR_GROUP(11) |  1,   // CLA Interrupt 2
    INTR_Src_11_03_CLA3        = INTR_Src_S_VECTOR_GROUP(11) |  2,   // CLA Interrupt 3
    INTR_Src_11_04_CLA4        = INTR_Src_S_VECTOR_GROUP(11) |  3,   // CLA Interrupt 4
    INTR_Src_11_05_CLA5        = INTR_Src_S_VECTOR_GROUP(11) |  4,   // CLA Interrupt 5
    INTR_Src_11_06_CLA6        = INTR_Src_S_VECTOR_GROUP(11) |  5,   // CLA Interrupt 6
    INTR_Src_11_07_CLA7        = INTR_Src_S_VECTOR_GROUP(11) |  6,   // CLA Interrupt 7
    INTR_Src_11_08_CLA8        = INTR_Src_S_VECTOR_GROUP(11) |  7,   // CLA Interrupt 8

    // PIE Group 12
    INTR_Src_12_01_XINT3       = INTR_Src_S_VECTOR_GROUP(12) |  0,   // External Interrupt 3
    INTR_Src_12_02_XINT4       = INTR_Src_S_VECTOR_GROUP(12) |  1,   // External Interrupt 4
    INTR_Src_12_03_XINT5       = INTR_Src_S_VECTOR_GROUP(12) |  2,   // External Interrupt 5
    INTR_Src_12_04_MPOST       = INTR_Src_S_VECTOR_GROUP(12) |  3,   // Memory Power On Self-Test Interrupt
    INTR_Src_12_05_FlashDone   = INTR_Src_S_VECTOR_GROUP(12) |  4,   // Flash Wrapper Operation Done Interrupt
    INTR_Src_12_07_FPU_OVFL    = INTR_Src_S_VECTOR_GROUP(12) |  6,   // FPU Overflow Interrupt
    INTR_Src_12_08_FPU_UFL     = INTR_Src_S_VECTOR_GROUP(12) |  7,   // FPU Underflow Interrupt
    INTR_Src_12_10_RAM_ERR     = INTR_Src_S_VECTOR_GROUP(12) |  9,   // RAM Correctable Error Interrupt
    INTR_Src_12_11_Flash_ERR   = INTR_Src_S_VECTOR_GROUP(12) | 10,   // Flash Correctable Error Interrupt
    INTR_Src_12_12_RAM_Access  = INTR_Src_S_VECTOR_GROUP(12) | 11,   // RAM Access Violation Interrupt
    INTR_Src_12_13_AES_SIN_TREQ= INTR_Src_S_VECTOR_GROUP(12) | 12,   // AES SIN Transfer Request Interrupt
    INTR_Src_12_14_BGCRC_CLA   = INTR_Src_S_VECTOR_GROUP(12) | 13,   // CLA Background CRC Interrupt
    INTR_Src_12_15_CLA_OVFL    = INTR_Src_S_VECTOR_GROUP(12) | 14,   // FPU Overflow Interrupt
    INTR_Src_12_16_CLA_UFL     = INTR_Src_S_VECTOR_GROUP(12) | 15,   // FPU Underflow Interrupt
};

#endif /* SUPPORT_INTR_H_ */
