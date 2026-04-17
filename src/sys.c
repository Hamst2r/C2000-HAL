#include "sys.h"
#include <string.h>


#define SIMRESET    ( REG32( CPU_BASE + 0x70 ) )

#ifndef CMDTOOL
extern uint16_t RamfuncsLoadStart;
extern uint16_t RamfuncsLoadEnd;
extern uint16_t RamfuncsLoadSize;
extern uint16_t RamfuncsRunStart;
extern uint16_t RamfuncsRunEnd;
extern uint16_t RamfuncsRunSize;
#endif



void SYS_CopyRamFunctions(void)
{
    memcpy(&RamfuncsRunStart, &RamfuncsLoadStart, (size_t)&RamfuncsLoadSize);
}

void SYS_Reset(void)
{
    SIMRESET = 0xA5A50002; // The upper 16 bits must be 0xA5A5 to allow the write. 0x0002 is the command to generate XRSn-like reset
}