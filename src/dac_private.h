#ifndef DAC_PRIVATE_H_
#define DAC_PRIVATE_H_

#define DAC_REV(dac)        ( REG16( DAC_BASE(dac) + 0x00U ) )  // DAC Revision Register
#define DAC_CTL(dac)        ( REG16( DAC_BASE(dac) + 0x01U ) )  // DAC Control Register EALLOW
#define DAC_VALA(dac)       ( REG16( DAC_BASE(dac) + 0x02U ) )  // DAC Value Register - Active
#define DAC_OUTEN(dac)      ( REG16( DAC_BASE(dac) + 0x04U ) )  // DAC Output Enable Register
#define DAC_LOCK(dac)       ( REG16( DAC_BASE(dac) + 0x05U ) )  // DAC Lock Register
#define DAC_TRIM(dac)       ( REG16( DAC_BASE(dac) + 0x06U ) )  // DAC Trim Register

#define DAC_S_CTL_MODE(x)   ( (x) << 1 )


#endif /* DAC_PRIVATE_H_ */
