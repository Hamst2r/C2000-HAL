#include "adc.h"
#include "clk.h"
#include "timer.h"
#include "adc_private.h"
#include <math.h>

#define ADC_TYPE_C_FILE     5

#if(ADC_TYPE_DEVICE != ADC_TYPE_C_FILE)
#error "ADC: Incompatible driver, check ADC Type of the library and device"
#endif

struct ADC_Defaults
{
    uint16_t sampleNs;
    enum ADC_Trig trigger;
};

struct ADC_Defaults ADC_defaults = 
{
    .sampleNs = 100,
    .trigger = ADC_Trig_SW
};


static void ADC_LoadOffsetTrim()
{
    uint16_t offsetShiftVal;

    // Offset trim for internal VREF 3.3V is unique and stored in upper byte.
    offsetShiftVal = (ANALOG_status.refMode == ANALOG_Ref_Int_3V3) ? 8U : 0U;

    for(uint16_t adc=0;adc<8;adc++)
    {
        if(ADC_IsAvailable(adc))
        {
            // Load Offset Trim
             ADC_OFFTRIM(adc) = (ADC_OTP_OFFSET_TRIM(adc) >> offsetShiftVal) & 0xFFU;
        }
    }

}

//
//  Configure VREF for all ADC and other analog modules
//
void ADC_SetupReference(enum ANALOG_Ref ref)
{
    ANALOG_SetupReference(ref);
    ADC_LoadOffsetTrim();
}

//
//  Configure clock divider for ADC module
//
void ADC_SetupClockDiv(enum ADC adc, uint16_t div)
{
    ADC_CTL2(adc) = (ADC_CTL2(adc) & 0xFFF0) | ((div-1)<<1);
}

//
//  Configures default behaviour of SOC
//
void ADC_SetupDefaults(uint16_t sampleNs, enum ADC_Trig trigger)
{
    ADC_defaults.sampleNs = sampleNs;
    ADC_defaults.trigger  = trigger;
}

//
//  Configure ADC SOC. Uses default values
//
enum ADC_SocToken ADC_SetupSOC(enum AnalogPin pin, enum ADC_SOC soc)
{
    return ADC_Advanced_SetupSOC(pin, soc, ADC_defaults.sampleNs, ADC_defaults.trigger);
}

//
//  Configure ADC SOC. All parameters need to be entered
//
enum ADC_SocToken ADC_Advanced_SetupSOC(enum AnalogPin pin, enum ADC_SOC soc, uint16_t sampleNs, enum ADC_Trig trigger)
{
    ADC_SOCCTL(ADC_GetADC(pin), soc) = ADC_S_SOCCTL_CHSEL(ANALOG_GetChNum(pin)) 
                                     | ADC_S_SOCCTL_ACQPS(roundf(sampleNs * CLK_status.sysClkLogicalMHz / 1000) - 1)
                                     | ADC_S_SOCCTL_TRIGSEL(trigger);

    return ADC_GetSocToken(pin, soc);
}

//
//  Setup ADC interrupt generation from SOC
//
enum ADC_Int ADC_SetupInterrupt(enum ADC_SocToken soc, enum ADC_IntNum intNum, enum ADC_IntPos pos)
{
    enum ADC adc = ADC_GetADC(soc);

    // Clear interrupt flag and interrupt overflow flag
    ADC_INTFLGCLR(adc) = 0x1 << intNum;
    ADC_INTOVFCLR(adc) = 0x1 << intNum;

    // Set interrupt pulse position
    ADC_CTL1(adc)  = ADC_CTL1(adc) & ~ADC_M_CTL1_INTPULSEPOS | pos;

    // Set interrupt trigger and enable interrupt
    ADC_INTSEL(adc, intNum) = ADC_INTSEL(adc, intNum)
                            & ~(  0x0F             << (((uint16_t)(intNum) & 0x1) << 3) )
                            | ( (uint16_t)(soc >> 3) << (((uint16_t)(intNum) & 0x1) << 3) )
                            | ( ADC_M_INTSEL_INTE  << (((uint16_t)(intNum) & 0x1) << 3) );

    return (enum ADC_Int)(adc | ADC_S_Int_Num(intNum));
}

//
//  Register ISR in vector table and enable interrupt in PIE
//
void ADC_SetupRegisterISR(enum ADC adc, void* isr)
{
    enum INTR_Src intSrc = ADC_GetPieIntSrc(adc);
    INTR_RegisterISR(intSrc, isr);
    INTR_Enable(intSrc);
}

//
//  Power up ADC module
//
int32_t ADC_SetupPowerUp(enum ADC adc)
{
    ADC_CTL1(adc) |= ADC_M_CTL1_ADCPWDNZ;
    return TIMER_GetExpiryTime( (ANALOG_status.refMode == ANALOG_Ref_Ext) ? 500 : 5000);
}

//
//  Blocks the code until timer expires
//
void ADC_SetupWaitADCReady(int32_t readyTime)
{
    while(!TIMER_HasExpired(readyTime));
}

//
//  Simple ADC setup. Enables all ADC modules available and only uses ADC_IntNum_1 for ISR. 
//  Blocks the code until ADCs have powered up
//
void ADC_SetupSimple(enum ANALOG_Ref ref, uint16_t sampleNs, enum ADC_Trig trigger, enum ADC_SocToken intTrigger, enum ADC_IntPos pos, void* isr)
{
    int32_t adcReadyTime;
    uint16_t clkDiv;

    ADC_SetupReference(ref);
    ADC_SetupDefaults(sampleNs, trigger);

    clkDiv = ceilf(CLK_status.sysClkPhysicalMHz / ADC_FMAX_MHZ);

    for(uint16_t adc = 0;adc<8;adc++)
    {
        // Check if ADC is available
        if (ADC_IsAvailable((enum ADC)adc) ) 
        {
            ADC_SetupClockDiv((enum ADC)adc, clkDiv);
            adcReadyTime = ADC_SetupPowerUp((enum ADC)adc);
        }
    }

    ADC_SetupInterrupt(intTrigger, ADC_IntNum_1, pos);

    ADC_SetupWaitADCReady(adcReadyTime);

    ADC_SetupRegisterISR(ADC_GetADC(intTrigger), isr);
}
