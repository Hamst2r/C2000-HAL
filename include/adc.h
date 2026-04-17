#ifndef ADC_H_
#define ADC_H_

#define ADC_TYPE_LIBRARY     5

#include "base.h"
#include "support_adc.h"
#include "analog.h"
#include "intr.h"

#if(ADC_TYPE_DEVICE != ADC_TYPE_LIBRARY)
#error "ADC: Incompatible driver, check ADC Type of the library and device"
#endif

#define ADC_GetADC(x)               ((enum ADC)(x & 0x07))
#define ADC_BASE(adc)               (ADC_START_ADDR + ADC_INCREMENT_ADDR * (adc))
#define ADC_RESULT_BASE(adc)        (ADC_RESULT_START_ADDR + ADC_RESULT_INCREMENT_ADDR * (adc))


#define ADC_INTFLGCLR(adc)          ( REG16( ADC_BASE(adc) + 0x04U ) )   // ADC Interrupt Flag Clear Register
#define ADC_INTOVFCLR(adc)          ( REG16( ADC_BASE(adc) + 0x06U ) )   // ADC Interrupt Overflow Clear Register

#define ADC_Trig_EPWM(epwm,soc)     ((enum ADC_Trig)(0x05 + (uint16_t)(epwm)*2 + (uint16_t)(soc)))  // Used by ePWM library
#define ADC_GetSocToken(pin,soc)    ((enum ADC_SocToken)( ANALOG_GetADC(pin) | ((soc) << 3)))
#define ADC_GetADCInt(soc,num)         ((enum ADC_Int)( ANALOG_GetADC(soc) | ((num) << 3)))


enum ADC_IntPos
{
    ADC_IntPos_EndOfSample      = 0x00, // Send interrupt pulse when sampling has finished
    ADC_IntPos_EndOfConversion  = 0x04  // Send interrupt pulse when A/D conversion is finished
};

enum ADC_SOC
{
    ADC_SOC_0  = 0,
    ADC_SOC_1  = 1,
    ADC_SOC_2  = 2,
    ADC_SOC_3  = 3,
    ADC_SOC_4  = 4,
    ADC_SOC_5  = 5,
    ADC_SOC_6  = 6,
    ADC_SOC_7  = 7,
    ADC_SOC_8  = 8,
    ADC_SOC_9  = 9,
    ADC_SOC_10 = 10,
    ADC_SOC_11 = 11,
    ADC_SOC_12 = 12,
    ADC_SOC_13 = 13,
    ADC_SOC_14 = 14,
    ADC_SOC_15 = 15
};

enum ADC_IntNum
{
    ADC_IntNum_1 = 0,
    ADC_IntNum_2,
    ADC_IntNum_3,
    ADC_IntNum_4
};


void ADC_SetupReference(enum ANALOG_Ref ref);
void ADC_SetupClockDiv(enum ADC adc, uint16_t div);
void ADC_SetupDefaults(uint16_t sampleNs, enum ADC_Trig trigger);
enum ADC_SocToken   ADC_SetupSOC(enum AnalogPin pin, enum ADC_SOC soc);
enum ADC_SocToken   ADC_Advanced_SetupSOC(enum AnalogPin pin, enum ADC_SOC soc, uint16_t sampleNs, enum ADC_Trig trigger);
enum ADC_Int        ADC_SetupInterrupt(enum ADC_SocToken soc, enum ADC_IntNum intNum, enum ADC_IntPos pos);
void ADC_SetupRegisterISR(enum ADC adc, void* isr);
int32_t ADC_SetupPowerUp(enum ADC adc);
void  ADC_SetupWaitADCReady(int32_t readyTime);

void ADC_SetupSimple(enum ANALOG_Ref ref, uint16_t sampleNs, enum ADC_Trig trigger, enum ADC_SocToken intTrigger, enum ADC_IntPos pos, void* isr);



static inline uint16_t ADC_ReadResult(enum AnalogPin pin, enum ADC_SOC soc)
{
    return REG16(ADC_RESULT_BASE(ANALOG_GetADC(pin)) + soc);
}

// static inline void ADC_ClearInterrupt(enum ADC_Int interrupt)
// {
//     ADC_INTFLGCLR(ADC_GetADC(interrupt)) = 0x1 << (interrupt >> 3);
// }
// uint16_t ADC_ReadPPBResult(enum AnalogPin pin, enum ADC_PPB ppb);

static inline void ADC_ClearISR(enum ADC adc)
{
    ADC_INTFLGCLR(adc) = 0x1;
    ADC_INTOVFCLR(adc) = 0x1;
    INTR_AckGroup(INTR_Grp_1);
}

#endif /* ADC_H_ */
