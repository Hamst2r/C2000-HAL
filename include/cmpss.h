#ifndef CMPSS_H_
#define CMPSS_H_

#include "base.h"
#include "support_cmpss.h"
#include "analog.h"
#include "epwm.h"

#define CMPSS_BASE(cmpss)       (CMPSS_START_ADDR + CMPSS_INCREMENT_ADDR * (cmpss))


#define CMPSS_DACVALS(cmp)              ( REG16( CMPSS_BASE(CMPSS_CMP_GetCMPSS(cmp)) + 0x1E - (CMPSS_CMP_GetHL(cmp)*12U) ) )    // Translates enum CMPSS_CMP to its corresponding DAC Value Shadow Register. Check offsets above to verify math
#define CMPSS_DACVALA(cmp)              ( REG16( CMPSS_BASE(CMPSS_CMP_GetCMPSS(cmp)) + 0x1F - (CMPSS_CMP_GetHL(cmp)*12U) ) )    // Translates enum CMPSS_CMP to its corresponding DAC Value Active Register. Check offsets above to verify math

// CMP accessor
// Multiply mask by accessor to access CMP bitfield
// CMPxH:  0x0001, access CMPH
// CMPxL:  0x0100, access CMPL
// CMPxHL: 0x0101, access both CMP (H/L) simultaneously
#define CMPSS_Accessor(cmp)         ( ((cmp >> 1) | (cmp << 8)) & 0x0101 )

#define CMPSS_M_Flag
#define CMPSS_M_LatchFlag

#define CMPSS_CMP_FromPin(pin,hl)   ( (enum CMPSS_CMP)(CMPSS_S_CMP_Module(ANALOG_GetCMPSS(pin)) | hl) )
#define CMPSS_CMP_GetCMPSS(cmp)     ( (enum CMPSS)( (cmp) >> 2 ) )
#define CMPSS_CMP_GetHL(cmp)        ( (cmp) & 0b11 )
#define CMPSS_CMP_GetHiCMP(cmp)     ( (enum CMPSS_CMP)((cmp) & 0xFE) )
#define CMPSS_CMP_GetLoCMP(cmp)     ( (enum CMPSS_CMP)((cmp) & 0xFD) )

// Condition that triggers a CMP output
enum CMPSS_Trig
{
    CMPSS_Trig_LargerThan   = 0U,   // Trigger when P input is larger than threshold/N-input
    CMPSS_Trig_SmallerThan  = 1U    // Trigger when P input is smaller than threshold/N-input
};

enum CMPSS_NegSrc
{
    CMPSS_NegSrc_DAC = 0,   // N-input is from a DAC
    CMPSS_NegSrc_Ext = 1    // N-input is from an external pin
};

enum CMPSS_Output
{
    CMPSS_Output_Async               = 0x00,    // CMP output is asynchronous
    CMPSS_Output_Sync                = 0x01,    // CMP output is synchronous to SYSCLK
    CMPSS_Output_Filter              = 0x02,    // CMP output goes through digital filter
    CMPSS_Output_LatchedFilter       = 0x03,    // CMP output is digital filtered and latched
    CMPSS_Output_Async_LatchedFilter = 0x13     // CMP output is filtered and latched, with async bypass
};

enum CMPSS_DacMode
{
    CMPSS_DacMode_SW    = 0,    // CMP DAC is controlled by software only
    CMPSS_DacMode_Ramp  = 1     // CMP DAC is controlled by ramp generator
};

enum CMPSS_DacLoad
{
    CMPSS_DacLoad_SysClk        = 0,    // DAC load at SYSCLK
    CMPSS_DacLoad_EPWMSYNCPER   = 1     // DAC load at EPWMSYNCPER pulse
};

enum CMPSS_CMP CMPSS_SetupSingle(enum AnalogPin pin, enum CMPSS_CMP cmp, enum CMPSS_Trig trig, uint16_t threshold, bool isOst);

enum CMPSS_CMP CMPSS_SetupWindow(enum AnalogPin pin, uint16_t upperThreshold, uint16_t lowerThreshold, bool isOst);

enum CMPSS_CMP CMPSS_SetupTwoPin(enum AnalogPin pinP, enum AnalogPin pinN, enum CMPSS_CMP cmp, enum CMPSS_Trig trig, bool isOst);

void CMPSS_SetupHysteresis(enum CMPSS_CMP cmp, enum CMPSS_Hysteresis hyst);
void CMPSS_SetupBlanking(enum CMPSS_CMP cmp, enum EPWM epwm);
void CMPSS_DisableBlanking(enum CMPSS_CMP cmp);
void CMPSS_SetupFilter(enum CMPSS_CMP cmp, uint16_t window, uint16_t threshold, uint16_t clkPrescale);
void CMPSS_SetupOutput(enum CMPSS_CMP cmp, enum CMPSS_Output trip, enum CMPSS_Output tripOut);


static inline bool CMPSS_CheckFlag(enum CMPSS_CMP cmp);
static inline bool CMPSS_CheckLatchFlag(enum CMPSS_CMP cmp);
static inline void CMPSS_ClearLatchFlag(enum CMPSS_CMP cmp);


static inline void CMPSS_SetThreshold(enum CMPSS_CMP cmp, uint16_t value)
{
    CMPSS_DACVALS(cmp) = value;
}

#endif /* CMPSS_H_ */
