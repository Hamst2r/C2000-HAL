#ifndef ANALOG_H_
#define ANALOG_H_

#include "base.h"
#include "support_analog.h"


#define ANALOG_GetADC(pin)      (pin & 0x07)
#define ANALOG_GetChNum(pin)    ((pin >> 3) & 0x1F)
#define ANALOG_GetCMPSS(pin)    ((enum CMPSS)((pin >> 8) & 0x0F))
#define ANALOG_GetPMux(pin)     ((pin >>12) & 0x0F)
#define ANALOG_GetNMux(pin)     ((pin >>16) & 0x0F)

#define ANALOG_HasCMPSS(pin)    (ANALOG_GetCMPSS(pin) != 15)
#define ANALOG_HasNMux(pin)     (ANALOG_GetNMux(pin)  != 15)


enum ANALOG_Ref
{
    ANALOG_Ref_Ext     = 0x0001,    // External analog reference
    ANALOG_Ref_Int_3V3 = 0x0000,    // Internal analog reference - 3V3 mode. (actual VREFHI is 1V65, with 2x attenuation applied to signals)
    ANALOG_Ref_Int_2V5 = 0x0100     // Internal analog reference - 2V5 mode
};

struct ANALOG_Status
{
    enum ANALOG_Ref refMode;
};

extern struct ANALOG_Status ANALOG_status;


void ANALOG_SetupReference(enum ANALOG_Ref ref);
void ANALOG_SetCmpPMux(enum AnalogPin pin, enum CMPSS_CMP cmp);

static inline void ANALOG_LoadTrim(void)
{
    // Call the built-in device calibration function
    ANALOG_DEVICE_CAL_FN();
    // The function ends with EDIS, do EALLOW again to allow access to protected registers
    EALLOW;
}

#endif /* ANALOG_H_ */
