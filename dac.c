#include "dac.h"
#include <math.h>

#define DAC_REV(dac)        ( REG16( DAC_BASE(dac) + 0x00U ) )  // DAC Revision Register
#define DAC_CTL(dac)        ( REG16( DAC_BASE(dac) + 0x01U ) )  // DAC Control Register EALLOW
#define DAC_VALA(dac)       ( REG16( DAC_BASE(dac) + 0x02U ) )  // DAC Value Register - Active
#define DAC_OUTEN(dac)      ( REG16( DAC_BASE(dac) + 0x04U ) )  // DAC Output Enable Register
#define DAC_LOCK(dac)       ( REG16( DAC_BASE(dac) + 0x05U ) )  // DAC Lock Register
#define DAC_TRIM(dac)       ( REG16( DAC_BASE(dac) + 0x06U ) )  // DAC Trim Register

#define DAC_S_CTL_MODE(x)   ( (x) << 1 )

struct DAC_Status DAC_status;

void DAC_Setup(enum DAC dac, float vrefhi)
{
    bool gain_2x;
    switch(ANALOG_status.refMode)
    {
        case ANALOG_Ref_Ext:
            DAC_status.vMax = vrefhi;
            // gain_2x = false;
            break;
        case ANALOG_Ref_Int_2V5:
            DAC_status.vMax = 2.5;
            // gain_2x = false;
            break;
        case ANALOG_Ref_Int_3V3:
            DAC_status.vMax = 1.65;
            // gain_2x = true;
            break;
    }
    gain_2x = false;

    DAC_CTL(dac) = 0x0001 | DAC_S_CTL_MODE(gain_2x);
    DAC_OUTEN(dac) = 1;
}

void DAC_SetVoltage(enum DAC dac, float voltage)
{
    float output;
    // Clamp output between 0 and vMax
    output = fminf(voltage,DAC_status.vMax);
    output = fmaxf(output,0);

    DAC_VALS(dac) = output / DAC_status.vMax * DAC_STEPS;
}

void DAC_SetDebugFloat(struct DAC_DebugFloat debug)
{
    if(debug.enable)
    {
        DAC_SetVoltage(debug.dac, *(debug.pointer) * debug.gain + debug.offset);
    }
}

void DAC_SetDebugInt16(struct DAC_DebugInt16 debug)
{
    if(debug.enable)
    {
        DAC_SetVoltage(debug.dac, *(debug.pointer) * debug.gain + debug.offset);
    }
}

void DAC_SetDebugUInt16(struct DAC_DebugUInt16 debug)
{
    if(debug.enable)
    {
        DAC_SetVoltage(debug.dac, *(debug.pointer) * debug.gain + debug.offset);
    }
}

// void DAC_SetDebugAny(struct DAC_DebugAny debug)
// {

// }
