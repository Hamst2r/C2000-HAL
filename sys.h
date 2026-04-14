#ifndef SYS_H_
#define SYS_H_

#include "base.h"
#include "support_sys.h"


#define PCLKCR(x)   ( REG32( CPU_BASE + 0x22 + 2U * (x) ) )

// Generate n NOP instructions. Limit: Up to 255 NOP
#define SYS_NOP(n)  asm(" RPT #" #n " || NOP")



// struct SYS_Status
// {

// };

static inline void SYS_EnablePeripheral(enum SYS_ClkGate peripheral)
{
    PCLKCR(peripheral & 0x1F) |= 1UL << (peripheral >> 8);
}

static inline void SYS_DisablePeripheral(enum SYS_ClkGate peripheral)
{
    PCLKCR(peripheral & 0x1F) |= 1UL << (peripheral >> 8);
}


void SYS_CopyRamFunctions(void);
void SYS_Reset(void);


#endif /* SYS_H_ */
