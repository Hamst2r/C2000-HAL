#include "analog.h"
#include "cmpss.h"
#include "analog_private.h"

struct ANALOG_Status ANALOG_status;


void ANALOG_SetupReference(enum ANALOG_Ref ref)
{
    ANALOG_ANAREFCTL = (ANALOG_ANAREFCTL & 0xFEFE) | ref;
    ANALOG_status.refMode = ref;
}

void ANALOG_SetCmpPMux(enum AnalogPin pin, enum CMPSS_CMP cmp)
{
    enum CMPSS cmpss = CMPSS_CMP_GetCMPSS(cmp);
    ANALOG_CMPPMUXSEL(cmp) = ANALOG_CMPPMUXSEL(cmp) & ~ANALOG_M_CMPPMUXSEL(cmpss)
                           | ANALOG_S_CMPPMUXSEL(cmpss,ANALOG_GetPMux(pin));
}
