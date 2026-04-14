#ifndef SUPPORT_SYS_H_
#define SUPPORT_SYS_H_

#define CPU_BASE    (0x0005D300UL)
#define STATUS_BASE (0x0005D400UL)
#define UID_BASE    (0x00070200UL)

#define SYS_S_ClkGate_Bit(x)    ((uint32_t)(x) << 16)
#define SYS_S_ClkGate_Reg(x)    ()

enum SYS_ClkGate
{
    SYS_ClkGate_CLA1        = 0x0000,   // CLA clock
    SYS_ClkGate_DMA         = 0x0200,   // DMA clock
    SYS_ClkGate_TIMER0      = 0x0300,   // Timer 0 clock
    SYS_ClkGate_TIMER1      = 0x0400,   // Timer 1 clock
    SYS_ClkGate_TIMER2      = 0x0500,   // Timer 2 clock
    SYS_ClkGate_CPUBGCRC    = 0x0D00,   // CPU Background CRC clock
    SYS_ClkGate_CLA1BGCRC   = 0x0E00,   // CLA Background CRC clock
    SYS_ClkGate_HRCAL       = 0x1000,   // HRCAL clock
    SYS_ClkGate_TBCLKSYNC   = 0x1200,   // ePWM Time-Base clock sync
    SYS_ClkGate_ERAD        = 0x1800,   // ERAD clock

    SYS_ClkGate_EPWM1       = 0x0002,   // ePWM1 clock
    SYS_ClkGate_EPWM2       = 0x0102,   // ePWM2 clock
    SYS_ClkGate_EPWM3       = 0x0202,   // ePWM3 clock
    SYS_ClkGate_EPWM4       = 0x0302,   // ePWM4 clock
    SYS_ClkGate_EPWM5       = 0x0402,   // ePWM5 clock
    SYS_ClkGate_EPWM6       = 0x0502,   // ePWM6 clock
    SYS_ClkGate_EPWM7       = 0x0602,   // ePWM7 clock
    SYS_ClkGate_EPWM8       = 0x0702,   // ePWM8 clock

    SYS_ClkGate_ECAP1       = 0x0003,   // eCAP1 clock
    SYS_ClkGate_ECAP2       = 0x0103,   // eCAP2 clock
    SYS_ClkGate_ECAP3       = 0x0203,   // eCAP3 clock

    SYS_ClkGate_EQEP1       = 0x0004,   // eQEP1 clock
    SYS_ClkGate_EQEP2       = 0x0104,   // eQEP2 clock

    SYS_ClkGate_SD1         = 0x0006,   // SD1 clock
    SYS_ClkGate_SD2         = 0x0106,   // SD2 clock

    SYS_ClkGate_SCIA        = 0x0007,   // SCI-A clock
    SYS_ClkGate_SCIB        = 0x0107,   // SCI-B clock

    SYS_ClkGate_SPIA        = 0x0008,   // SPI-A clock
    SYS_ClkGate_SPIB        = 0x0108,   // SPI-B clock

    SYS_ClkGate_I2CA        = 0x0009,   // I2C-A clock
    SYS_ClkGate_I2CB        = 0x0109,   // I2C-B clock

    SYS_ClkGate_CANA        = 0x000A,   // CAN-A clock
    SYS_ClkGate_MCANA       = 0x040A,   // MCAN-A clock

    SYS_ClkGate_ADCA        = 0x000D,   // ADC-A clock
    SYS_ClkGate_ADCB        = 0x010D,   // ADC-B clock
    SYS_ClkGate_ADCC        = 0x020D,   // ADC-C clock

    SYS_ClkGate_CMPSS1      = 0x000E,   // CMPSS1 clock
    SYS_ClkGate_CMPSS2      = 0x010E,   // CMPSS2 clock
    SYS_ClkGate_CMPSS3      = 0x020E,   // CMPSS3 clock
    SYS_ClkGate_CMPSS4      = 0x030E,   // CMPSS4 clock

    SYS_ClkGate_DACA        = 0x1010,   // DAC-A clock
    SYS_ClkGate_DACB        = 0x1110,   // DAC-B clock

    SYS_ClkGate_CLB1        = 0x0011,   // CLB1 clock
    SYS_ClkGate_CLB2        = 0x0111,   // CLB2 clock
    SYS_ClkGate_CLB3        = 0x0211,   // CLB3 clock
    SYS_ClkGate_CLB4        = 0x0311,   // CLB4 clock

    SYS_ClkGate_FSITXA      = 0x0012,   // FSITX-A clock
    SYS_ClkGate_FSIRXA      = 0x0112,   // FSIRX-A clock

    SYS_ClkGate_LINA        = 0x0013,   // LIN-A clock
    SYS_ClkGate_LINB        = 0x0113,   // LIN-B clock

    SYS_ClkGate_PMBUSA      = 0x0014,   // PMBus-A clock

    SYS_ClkGate_DCC0        = 0x0015,   // DCC0 clock
    SYS_ClkGate_DCC1        = 0x0115,   // DCC1 clock

    SYS_ClkGate_HICA        = 0x0019,   // HIC-A clock

    SYS_ClkGate_AESA        = 0x001A,   // AES-A clock

    SYS_ClkGate_EPG1        = 0x001B    // EPG1 clock
};

#endif /* SUPPORT_SYS_H_ */
