#ifndef SUPPORT_ADC_H_
#define SUPPORT_ADC_H_

#include "support_intr.h"

#define ADC_AVAILABILITY        (0b0111U)
#define ADC_START_ADDR          (0x00007400UL)      // Starting address of ADC registers
#define ADC_INCREMENT_ADDR      (0x00000080UL)      // Address increment between ADC modules
#define ADC_RESULT_START_ADDR       (0x00000B00UL)  // Starting address of ADC result registers
#define ADC_RESULT_INCREMENT_ADDR   (0x00000020UL)        // Address increment between ADC modules' result

#define ADC_OTP_OFFSET_TRIM(adc)    ( REG16(0x7016CUL + (adc)) )

#define ADC_STEPS               (4096)
#define ADC_FMAX_MHZ            (60)

#define ADC_S_SocToken_SOC(adc) ((adc) << 8)
#define ADC_S_Int_Num(num)      ((num) << 3)        // ADC Interrupt number
//#define ADC_S_Int_Grp(grp)      ((grp) << 8)        // ADC Interrupt PIE group
//#define ADC_S_Int_Ch(ch)        ((ch) << 12)        // ADC Interrupt PIE channel inside group

//#define ADC_Int_FromIntNum(adc,num)     ((!(num) ? (adc | ADC_S_Int_Num(num) | ADC_S_Int_Grp(1) | ADC_S_Int_Ch(adc)) : (adc | ADC_S_Int_Num(num) | ADC_S_Int_Grp(10) | ADC_S_Int_Ch(adc * 4 + num)) ))


enum ADC
{
    ADC_A = 0,
    ADC_B = 1,
    ADC_C = 2
};

enum ADC_SocToken
{
    ADC_SocToken_A0  = ADC_A |  0 << 3,
    ADC_SocToken_A1  = ADC_A |  1 << 3,
    ADC_SocToken_A2  = ADC_A |  2 << 3,
    ADC_SocToken_A3  = ADC_A |  3 << 3,
    ADC_SocToken_A4  = ADC_A |  4 << 3,
    ADC_SocToken_A5  = ADC_A |  5 << 3,
    ADC_SocToken_A6  = ADC_A |  6 << 3,
    ADC_SocToken_A7  = ADC_A |  7 << 3,
    ADC_SocToken_A8  = ADC_A |  8 << 3,
    ADC_SocToken_A9  = ADC_A |  9 << 3,
    ADC_SocToken_A10 = ADC_A | 10 << 3,
    ADC_SocToken_A11 = ADC_A | 11 << 3,
    ADC_SocToken_A12 = ADC_A | 12 << 3,
    ADC_SocToken_A13 = ADC_A | 13 << 3,
    ADC_SocToken_A14 = ADC_A | 14 << 3,
    ADC_SocToken_A15 = ADC_A | 15 << 3,

    ADC_SocToken_B0  = ADC_B |  0 << 3,
    ADC_SocToken_B1  = ADC_B |  1 << 3,
    ADC_SocToken_B2  = ADC_B |  2 << 3,
    ADC_SocToken_B3  = ADC_B |  3 << 3,
    ADC_SocToken_B4  = ADC_B |  4 << 3,
    ADC_SocToken_B5  = ADC_B |  5 << 3,
    ADC_SocToken_B6  = ADC_B |  6 << 3,
    ADC_SocToken_B7  = ADC_B |  7 << 3,
    ADC_SocToken_B8  = ADC_B |  8 << 3,
    ADC_SocToken_B9  = ADC_B |  9 << 3,
    ADC_SocToken_B10 = ADC_B | 10 << 3,
    ADC_SocToken_B11 = ADC_B | 11 << 3,
    ADC_SocToken_B12 = ADC_B | 12 << 3,
    ADC_SocToken_B13 = ADC_B | 13 << 3,
    ADC_SocToken_B14 = ADC_B | 14 << 3,
    ADC_SocToken_B15 = ADC_B | 15 << 3,

    ADC_SocToken_C0  = ADC_C |  0 << 3,
    ADC_SocToken_C1  = ADC_C |  1 << 3,
    ADC_SocToken_C2  = ADC_C |  2 << 3,
    ADC_SocToken_C3  = ADC_C |  3 << 3,
    ADC_SocToken_C4  = ADC_C |  4 << 3,
    ADC_SocToken_C5  = ADC_C |  5 << 3,
    ADC_SocToken_C6  = ADC_C |  6 << 3,
    ADC_SocToken_C7  = ADC_C |  7 << 3,
    ADC_SocToken_C8  = ADC_C |  8 << 3,
    ADC_SocToken_C9  = ADC_C |  9 << 3,
    ADC_SocToken_C10 = ADC_C | 10 << 3,
    ADC_SocToken_C11 = ADC_C | 11 << 3,
    ADC_SocToken_C12 = ADC_C | 12 << 3,
    ADC_SocToken_C13 = ADC_C | 13 << 3,
    ADC_SocToken_C14 = ADC_C | 14 << 3,
    ADC_SocToken_C15 = ADC_C | 15 << 3
};

enum ADC_Trig
{
    ADC_Trig_SW,            // Software trigger only
    ADC_Trig_CPU1_Timer0,   // Trigger from CPU1 Timer0
    ADC_Trig_CPU1_Timer1,   // Trigger from CPU1 Timer1
    ADC_Trig_CPU1_Timer2,   // Trigger from CPU1 Timer2
    ADC_Trig_EXT,           // Trigger from external pin
    ADC_Trig_EPWM1_SOCA,    // Trigger from ePWM1 SOC-A pulse
    ADC_Trig_EPWM1_SOCB,    // Trigger from ePWM1 SOC-B pulse
    ADC_Trig_EPWM2_SOCA,    // Trigger from ePWM2 SOC-A pulse
    ADC_Trig_EPWM2_SOCB,    // Trigger from ePWM2 SOC-B pulse
    ADC_Trig_EPWM3_SOCA,    // Trigger from ePWM3 SOC-A pulse
    ADC_Trig_EPWM3_SOCB,    // Trigger from ePWM3 SOC-B pulse
    ADC_Trig_EPWM4_SOCA,    // Trigger from ePWM4 SOC-A pulse
    ADC_Trig_EPWM4_SOCB,    // Trigger from ePWM4 SOC-B pulse
    ADC_Trig_EPWM5_SOCA,    // Trigger from ePWM5 SOC-A pulse
    ADC_Trig_EPWM5_SOCB,    // Trigger from ePWM5 SOC-B pulse
    ADC_Trig_EPWM6_SOCA,    // Trigger from ePWM6 SOC-A pulse
    ADC_Trig_EPWM6_SOCB,    // Trigger from ePWM6 SOC-B pulse
    ADC_Trig_EPWM7_SOCA,    // Trigger from ePWM7 SOC-A pulse
    ADC_Trig_EPWM7_SOCB,    // Trigger from ePWM7 SOC-B pulse
    ADC_Trig_EPWM8_SOCA,    // Trigger from ePWM8 SOC-A pulse
    ADC_Trig_EPWM8_SOCB     // Trigger from ePWM8 SOC-B pulse
};

enum ADC_Int
{
    ADC_Int_A1 = ADC_A | ADC_S_Int_Num(0),     // ADC-A Interrupt 1
    ADC_Int_A2 = ADC_A | ADC_S_Int_Num(1),     // ADC-A Interrupt 2
    ADC_Int_A3 = ADC_A | ADC_S_Int_Num(2),     // ADC-A Interrupt 3
    ADC_Int_A4 = ADC_A | ADC_S_Int_Num(3),     // ADC-A Interrupt 4

    ADC_Int_B1 = ADC_B | ADC_S_Int_Num(0),     // ADC-B Interrupt 1
    ADC_Int_B2 = ADC_B | ADC_S_Int_Num(1),     // ADC-B Interrupt 2
    ADC_Int_B3 = ADC_B | ADC_S_Int_Num(2),     // ADC-B Interrupt 3
    ADC_Int_B4 = ADC_B | ADC_S_Int_Num(3),     // ADC-B Interrupt 4

    ADC_Int_C1 = ADC_C | ADC_S_Int_Num(0),     // ADC-C Interrupt 1
    ADC_Int_C2 = ADC_C | ADC_S_Int_Num(1),     // ADC-C Interrupt 2
    ADC_Int_C3 = ADC_C | ADC_S_Int_Num(2),     // ADC-C Interrupt 3
    ADC_Int_C4 = ADC_C | ADC_S_Int_Num(3)      // ADC-C Interrupt 4
};

static inline enum INTR_Src ADC_GetPieIntSrc(enum ADC adc)
{
    switch(adc)
    {
        case ADC_A:
            return INTR_Src_01_01_ADCA1;
        case ADC_B:
            return INTR_Src_01_02_ADCB1;
        case ADC_C:
            return INTR_Src_01_03_ADCC1;
        default:
            return INTR_Src_01_01_ADCA1;
    }
}

#endif /* SUPPORT_ADC_H_ */
