#ifndef DAC_H_
#define DAC_H_

#include "base.h"
#include "support_dac.h"
#include "analog.h"

#define DAC_BASE(dac)       (DAC_START_ADDR + DAC_INCREMENT_ADDR * (dac))

#define DAC_VALS(dac)       ( REG16( DAC_BASE(dac) + 0x03U ) )  // DAC Value Register - Shadow

struct DAC_Status
{
    float vMax;
};

struct DAC_DebugFloat
{
    bool enable;
    enum DAC dac;
    float* pointer;
    float gain;
    float offset;
};

struct DAC_DebugInt16
{
    bool enable;
    enum DAC dac;
    int16_t* pointer;
    float gain;
    float offset;
};

struct DAC_DebugUInt16
{
    bool enable;
    enum DAC dac;
    uint16_t* pointer;
    float gain;
    float offset;
};


void DAC_Setup(enum DAC dac, float vrefhi);
void DAC_SetVoltage(enum DAC dac, float voltage);

static inline void DAC_SetValue(enum DAC dac, uint16_t value)
{
    DAC_VALS(dac) = value;
}

#endif /* DAC_H_ */
