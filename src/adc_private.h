
// Register access

#ifndef ADC_PRIVATE_H_
#define ADC_PRIVATE_H_
#define ADC_CTL1(adc)               ( REG16( ADC_BASE(adc) + 0x00U ) )   // ADC Control 1 Register
#define ADC_CTL2(adc)               ( REG16( ADC_BASE(adc) + 0x01U ) )   // ADC Control 2 Register
#define ADC_BURSTCTL(adc)           ( REG16( ADC_BASE(adc) + 0x02U ) )   // ADC Burst Control Register
#define ADC_INTFLG(adc)             ( REG16( ADC_BASE(adc) + 0x03U ) )   // ADC Interrupt Flag Register
#define ADC_INTOVF(adc)             ( REG16( ADC_BASE(adc) + 0x05U ) )   // ADC Interrupt Overflow Register
#define ADC_INTSEL(adc,num)         ( REG16( ADC_BASE(adc) + 0x07U + (uint32_t)((num)>1)) )  // ADC Interrupt Selection Register
#define ADC_SOCPRICTL(adc)          ( REG16( ADC_BASE(adc) + 0x09U ) )   // ADC SOC Priority Control Register
#define ADC_INTSOCSEL1(adc)         ( REG16( ADC_BASE(adc) + 0x0AU ) )   // ADC Interrupt SOC Selection 1 Register
#define ADC_INTSOCSEL2(adc)         ( REG16( ADC_BASE(adc) + 0x0BU ) )   // ADC Interrupt SOC Selection 2 Register
#define ADC_SOCFLG1(adc)            ( REG16( ADC_BASE(adc) + 0x0CU ) )   // ADC SOC Flag 1 Register
#define ADC_SOCFRC1(adc)            ( REG16( ADC_BASE(adc) + 0x0DU ) )   // ADC SOC Force 1 Register
#define ADC_SOCOVF1(adc)            ( REG16( ADC_BASE(adc) + 0x0EU ) )   // ADC SOC Overflow 1 Register
#define ADC_SOCOVF1CLR(adc)         ( REG16( ADC_BASE(adc) + 0x0FU ) )   // ADC SOC Overflow Clear 1 Register

#define ADC_SOCCTL(adc,soc)         ( REG32( ADC_BASE(adc) + 0x10U + (uint32_t)((soc)*2) ) )

#define ADC_EVTSTAT(adc)            ( REG32( ADC_BASE(adc) + 0x30U ) )
#define ADC_EVTCLR(adc)             ( REG32( ADC_BASE(adc) + 0x32U ) )
#define ADC_EVTSEL(adc)             ( REG32( ADC_BASE(adc) + 0x34U ) )
#define ADC_EVTINTSEL(adc)          ( REG32( ADC_BASE(adc) + 0x36U ) )
#define ADC_OSDETECT(adc)           ( REG16( ADC_BASE(adc) + 0x38U ) )
#define ADC_COUNTER(adc)            ( REG16( ADC_BASE(adc) + 0x39U ) )
#define ADC_REV(adc)                ( REG16( ADC_BASE(adc) + 0x3AU ) )
#define ADC_OFFTRIM(adc)            ( REG16( ADC_BASE(adc) + 0x3BU ) )

#define ADC_PPBCONFIG(adc,ppb)      ( REG16( ADC_BASE(adc) + 0x40U +  8U * (uint16_t)(ppb) ) )
#define ADC_PPBSTAMP(adc,ppb)       ( REG16( ADC_BASE(adc) + 0x41U +  8U * (uint16_t)(ppb) ) )
#define ADC_PPBOFFCAL(adc,ppb)      ( REG16( ADC_BASE(adc) + 0x42U +  8U * (uint16_t)(ppb) ) )
#define ADC_PPBOFFREF(adc,ppb)      ( REG16( ADC_BASE(adc) + 0x43U +  8U * (uint16_t)(ppb) ) )
#define ADC_PPBTRIPHI(adc,ppb)      ( REG32( ADC_BASE(adc) + 0x44U +  8U * (uint16_t)(ppb) ) )
#define ADC_PPBTRIPLO(adc,ppb)      ( REG32( ADC_BASE(adc) + 0x46U +  8U * (uint16_t)(ppb) ) )

#define ADC_INTCYCLE(adc)           ( REG16( ADC_BASE(adc) + 0x6FU ) )
#define ADC_INLTRIM2(adc)           ( REG32( ADC_BASE(adc) + 0x72U ) )
#define ADC_INLTRIM3(adc)           ( REG32( ADC_BASE(adc) + 0x74U ) )

// Bit field shift
#define ADC_S_SOCCTL_TRIGSEL(x)     (((uint32_t)(x) << 20) & 0x007F00000)
#define ADC_S_SOCCTL_CHSEL(x)       (((uint32_t)(x) << 15) & 0x0000F8000)
#define ADC_S_SOCCTL_ACQPS(x)       ( (uint16_t)(x)        & 0x0000001FF)

// Bit field mask
#define ADC_M_CTL1_INTPULSEPOS      (0x0004)
#define ADC_M_CTL1_ADCPWDNZ         (0x0080)
#define ADC_M_INTSEL_INTE           (0x0020)

// Helper macros
#define ADC_Int_GetGrp(num)         (((num) >>  8) & 0x0F)
#define ADC_Int_GetCh(num)          (((num) >> 12) & 0x0F)

#define ADC_IsAvailable(adc)        ( (ADC_AVAILABILITY >> adc) & 1U )


#endif /* ADC_PRIVATE_H_ */
