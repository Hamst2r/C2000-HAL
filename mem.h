#ifndef MEM_H_
#define MEM_H_

#include "base.h"
#include "support_mem.h"




enum MEM_Test
{
    //! Functional mode. Test mode is disabled.
    MEM_Test_None           = 0,
    //! Writes allowed to data only
    MEM_Test_WriteData      = 1,
    //! Writes allowed to ECC only (for DxRAM/MxRAM)
    MEM_Test_WriteECC       = 2,
    //! Writes allowed to parity only (for LSxRAM, GSxRAM, and MSGxRAM)
    MEM_Test_WriteParity    = 2,
    //! Diagnostic mode, similar to functional mode but NMI is not generated.
    MEM_Test_Diag           = 3
};


void MEM_SetLsRamMode(enum MEM_Sect section, enum MEM_LsRamMode mode);

#endif /* MEM_H_ */
