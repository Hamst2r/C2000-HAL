#ifndef SUPPORT_DAC_H_
#define SUPPORT_DAC_H_

#define DAC_START_ADDR          (0x00005C00UL)
#define DAC_INCREMENT_ADDR      (0x00000010UL)

#define DAC_STEPS               (4096U)

enum DAC
{
    DAC_A = 0,
    DAC_B = 1
};


#endif
