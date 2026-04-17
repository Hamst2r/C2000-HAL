#include "mem.h"
#include "mem_private.h"

void MEM_SetLsRamMode(enum MEM_Sect section, enum MEM_LsRamMode mode)
{
    uint16_t sectNum = MEM_GetNum(section);

    if(MEM_GetType(section) != MEM_Type_LS)
    {
        // Non-LSRAM section passed into the function. Stop execution
        ESTOP0;
        while(1);
    }

    if(sectNum == MEM_SECT_NUM_ALL)
    {
        // Apply setting to all LS RAM Sections
        switch(mode)
        {
            case MEM_LsRamMode_CPU_Only:
                MEM_LSxMSEL   = 0;
                MEM_LSxCLAPGM = 0;
                break;

            case MEM_LsRamMode_CPU_CLA_Data:
                MEM_LSxMSEL   = 0x5555;
                MEM_LSxCLAPGM = 0;
                break;

            case MEM_LsRamMode_CLA_Program:
                MEM_LSxMSEL   = 0x5555;
                MEM_LSxCLAPGM = 0x00FF;
                break;
        }
    } else {
        // Apply setting to individual LS RAM Section
        switch(mode)
        {
            case MEM_LsRamMode_CPU_Only:
                MEM_LSxMSEL   &= ~(0x3U << (sectNum * 2));
                MEM_LSxCLAPGM &= ~(0x1U << sectNum);
                break;

            case MEM_LsRamMode_CPU_CLA_Data:
                MEM_LSxMSEL    = MEM_LSxMSEL & ~(0x3U << (sectNum * 2)) | (0x1U << (sectNum * 2));
                MEM_LSxCLAPGM &= ~(0x1U << sectNum);
                break;

            case MEM_LsRamMode_CLA_Program:
                MEM_LSxMSEL    = MEM_LSxMSEL & ~(0x3U << (sectNum * 2)) | (0x1U << (sectNum * 2));
                MEM_LSxCLAPGM |= (0x1U << sectNum);
                break;
        }

    }

}
