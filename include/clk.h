#ifndef CLK_H_
#define CLK_H_

#include "base.h"
#include "support_clk.h"


#define CLK_DCC_BASE(dcc)   (CLK_DCC_START_ADDR + CLK_DCC_INCREMENT_ADDR * (dcc))


#define CLK_AUXCLKDIVSEL        ( REG16( CLK_BASE + 0x24 ) )  // Auxillary Clock Divider Select register

#define CLK_DCC_GCTRL(dcc)      ( REG16( CLK_DCC_BASE(dcc) + 0x00 ) )  // Start/Stop DCC counters. Enabld/Disable Done/Error signals
#define CLK_DCC_STATUS(dcc)     ( REG16( CLK_DCC_BASE(dcc) + 0x14 ) )  // Done/Error flags


#define CLK_DCC_S_FLAG(flag,en)     ( (en) << ((flag)*8-4) )  // DCC Flag (Done/Error) Enable, check enum CLK_DCC_Flag and TRM bitfields for the math
#define CLK_DCC_S_SINGLESHOT(en)    ( (en) << 8 )  // DCC Single-Shot mode
#define CLK_DCC_S_DCCENA(en)        ( (en) << 0 )  // DCC Enable
#define CLK_DCC_M_GCTRL             ( 0x0F )    // Mask for DCGCTRL fields, need to shift

#define CLKCFG_SRC(src)         ((uint32_t)((src)     & 0x07) << 29)  // Clock Source, 3 bits
#define CLKCFG_REFDIV(div)      ((uint32_t)(((div)-1) & 0x1F) << 24)  // PLL Reference Divider, 5 bits
#define CLKCFG_ODIV(div)        ((uint32_t)(((div)-1) & 0x1F) << 16)  // PLL Output Divider, 5 bits
#define CLKCFG_IMULT(mult)      ((uint32_t)((mult)    & 0xFF) <<  0)  // PLL Multiplier, 8 bits

// #define CLKCFG_SYSDIV(div)  (((div) & 0x3F) <<  8)  // SYSCLK Divider, 6 bits

// Clock Source: [CLKSRCCTL1.OSCCLKSRCSEL]:[XTALCR.SE]
enum CLK_Src
{
    CLK_Src_XTAL    = 0b001,    // Crystal connected to X1/X2 pins
    CLK_Src_EXT     = 0b101,    // External oscillator connected to X1 pin
    CLK_Src_INTOSC1 = 0b010,    // Internal oscillator 1 (10MHz)
    CLK_Src_INTOSC2 = 0b000     // Internal oscillator 2 (10MHz)
};

enum CLK_CFG
{
    CLK_CFG_XTAL20_SYS150 = CLKCFG_SRC(CLK_Src_XTAL) | CLKCFG_REFDIV(2) | CLKCFG_ODIV(2) | CLKCFG_IMULT(30),
    CLK_CFG_XTAL20_SYS120 = CLKCFG_SRC(CLK_Src_XTAL) | CLKCFG_REFDIV(2) | CLKCFG_ODIV(4) | CLKCFG_IMULT(48),
    CLK_CFG_XTAL20_SYS100 = CLKCFG_SRC(CLK_Src_XTAL) | CLKCFG_REFDIV(2) | CLKCFG_ODIV(4) | CLKCFG_IMULT(40)
};

enum CLK_OutSrc
{
    CLK_OutSrc_PLLSYSCLK    = 0b0000,
    CLK_OutSrc_PLLCLK       = 0b0001,
    CLK_OutSrc_SYSCLK       = 0b0010,
    CLK_OutSrc_INTOSC1      = 0b0101,
    CLK_OutSrc_INTOSC2      = 0b0110,
    CLK_OutSrc_XTAL         = 0b0111
};

enum CLK_OutDiv
{
    CLK_OutDiv_1 = 0,
    CLK_OutDiv_2 = 1,
    CLK_OutDiv_4 = 2,
    CLK_OutDiv_8 = 3
};

enum CLK_DCC_Flag
{
    CLK_DCC_Flag_Error  = 0b0001,
    CLK_DCC_Flag_Done   = 0b0010,
    CLK_DCC_Flag_Ended  = CLK_DCC_Flag_Error | CLK_DCC_Flag_Done
};

struct CLK_Status
{
    enum CLK_Src source;
    uint16_t downClock;
    float extMHz;
    float sourceMHz;
    float pllInMHz;
    float pllVCOMHz;
    float pllOutMHz;
    float sysClkLogicalMHz;
    float sysClkPhysicalMHz;
};

extern struct CLK_Status CLK_status;

void CLK_EnterExtFreqMHz(float freqMHz);
bool CLK_SetupClkConfig(enum CLK_CFG config);
void CLK_SetupDownclock(uint16_t ratio);
void CLK_SetCLKSRCCTL2(uint16_t shift, uint16_t value);
void CLK_SetupClkOut(enum CLK_OutSrc source, enum CLK_OutDiv div);



// Start/Stop DCC module
static inline void CLK_DCC_Enable(enum CLK_DCC dcc, bool enable)
{
    if(enable)
    {
        CLK_DCC_GCTRL(dcc) &= ~CLK_DCC_M_GCTRL;
    } else {
        CLK_DCC_GCTRL(dcc) |= 0x05;
    }
}

static inline bool CLK_DCC_CheckFlag(enum CLK_DCC dcc, enum CLK_DCC_Flag flag)
{
    return ((CLK_DCC_STATUS(dcc) & flag)!=0);
}
static inline void CLK_DCC_ClearFlag(enum CLK_DCC dcc, enum CLK_DCC_Flag flag)
{
    CLK_DCC_STATUS(dcc) = flag;
}

// Enable Error/Done Signals
static inline void CLK_DCC_EnableFlag(enum CLK_DCC dcc, enum CLK_DCC_Flag flag)
{
    CLK_DCC_GCTRL(dcc) &= ~CLK_DCC_S_FLAG(flag,CLK_DCC_M_GCTRL);
}

// Disable Error/Done Signal
static inline void CLK_DCC_DisableFlag(enum CLK_DCC dcc, enum CLK_DCC_Flag flag)
{
    CLK_DCC_GCTRL(dcc) |= CLK_DCC_S_FLAG(flag,0x05);
}


#endif /* CLK_H_ */
