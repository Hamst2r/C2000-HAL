#include "clk.h"
#include "sys.h"
#include "clk_private.h"

#define CLK_TOLERANCE_PERCENT           ( 0.5f )

struct CLK_Status CLK_status;


static void CLK_UpdateFreqTable(void)
{
    uint16_t mult, outDiv, refDiv;
    uint16_t sysDiv;
    enum CLK_CFG config;
    
    // Parse the configuration
    config = (enum CLK_CFG) CLK_SYSPLLMULT;
    mult   = CLKCFG_GetMult(config);
    outDiv = CLKCFG_GetOutDiv(config) + 1;
    refDiv = CLKCFG_GetRefDiv(config) + 1;

    sysDiv = CLK_SYSCLKDIVSEL;

    if(sysDiv == 0x00)
    {
        sysDiv = 1;
    } else {
        sysDiv = 2 * (sysDiv & CLK_M_SYSCLKDIVSEL_DIV) + (sysDiv & CLK_M_SYSCLKDIVSEL_DIV_LSB) != 0;
    }

    CLK_status.downClock = (CLK_status.downClock < 1) ? 1 : CLK_status.downClock;

    CLK_status.pllInMHz = CLK_status.sourceMHz / refDiv;
    CLK_status.pllVCOMHz = CLK_status.pllInMHz * mult;
    CLK_status.pllOutMHz = CLK_status.pllVCOMHz / outDiv;
    CLK_status.sysClkPhysicalMHz = CLK_status.pllOutMHz / sysDiv;
    CLK_status.sysClkLogicalMHz = CLK_status.sysClkPhysicalMHz * CLK_status.downClock;
}
void CLK_EnterExtFreqMHz(float freqMHz)
{
    CLK_status.extMHz = freqMHz;
    // CLK_UpdateFreqTable();
}

static inline void CLK_ResetMCD(void)
{
    CLK_MCDCR |= CLK_M_MCDCR_MCLKCLR;
}

static inline bool CLK_IsClockFailDetected(void)
{
    return ((CLK_MCDCR & CLK_M_MCDCR_MCLKSTS) != 0U);
}

void CLK_WaitX1Counter()
{
    // Poll and clear X1 counter 4 times to ensure clock stabilizes
    for(uint16_t i=0;i<4;i++)
    {
        // Keep clearing the counter until it no longer saturates
        do {
            CLK_X1CNT_CLR = 1U;
        } while(CLK_X1CNT >= 0x01FFU);

        // Wait for X1 counter to saturate
        while(CLK_X1CNT != CLK_X1CNT_MAX);
    }
}

static void CLK_SelectClkSrc(enum CLK_Src source)
{
    switch(source)
    {
        case CLK_Src_INTOSC2:
            // Turn on INTOSC2
            // Then wait 300 NOP for INTOSC2 to start
            CLK_CLKSRCCTL1 &= ~CLK_M_CLKSRCCTL1_INTOSC2OFF;
            SYS_NOP(100);
            SYS_NOP(200);
            // Select INTOSC2 as clock cource (by clearing to field to 00)
            CLK_CLKSRCCTL1 = CLK_CLKSRCCTL1 & ~CLK_M_CLKSRCCTL1_OSCCLKSRCSEL;
            CLK_status.sourceMHz = 10.0;
            break;

        case CLK_Src_XTAL:
            // Precondition X1/X2 pins to speed up crystal start-up, and wait a few cycles for load capacitors to charge
            CLK_XTALCR2 |= CLK_M_XTALCR2_FEN | CLK_M_XTALCR2_XOF | CLK_M_XTALCR2_XIF;
            SYS_NOP(5);

            // Turn on external clock circuit in crystal mode, disable precondition
            CLK_XTALCR  &= ~CLK_M_XTALCR_SE & ~CLK_M_XTALCR_OSCOFF;
            CLK_XTALCR2 &= ~CLK_M_XTALCR2_FEN;

            // If a missing clock failure was detected, try waiting for the X1 counter
            // to saturate again. Consider modifying this code to add a 10ms timeout.
            do {
                // Clear the MCD failure
                CLK_ResetMCD();

                // Wait 1ms for crystal to start up, which is 10,000 INTOSC cycles
                for(uint16_t i=0;i<40;i++)
                {
                    SYS_NOP(248);
                }

                // Wait for the X1 counter to saturate
                CLK_WaitX1Counter();

                // Select external clock circuit as clock source
                CLK_CLKSRCCTL1 = CLK_CLKSRCCTL1 & ~CLK_M_CLKSRCCTL1_OSCCLKSRCSEL
                               | CLKSRC_GetOSCSRC(CLK_Src_XTAL);
            } while(CLK_IsClockFailDetected());
            CLK_status.sourceMHz = CLK_status.extMHz;
            break;

        case CLK_Src_EXT:
            // Turn on external clock circuit in single-ended mode (external oscillator)
            CLK_XTALCR = CLK_M_XTALCR_SE | (CLK_M_XTALCR_OSCOFF & 0x00U);

            // Wait for the X1 counter to saturate
            CLK_WaitX1Counter();

            // Select external clock circuit as clock source
            CLK_CLKSRCCTL1 = CLK_CLKSRCCTL1 & ~CLK_M_CLKSRCCTL1_OSCCLKSRCSEL
                           | CLKSRC_GetOSCSRC(CLK_Src_EXT);

            // // Something is wrong with the oscillator module. Replace the ESTOP0 with
            // // an appropriate error-handling routine.
            while(CLK_IsClockFailDetected())
            {
                HAL_FATAL(HAL_ErrorCode_HardwareFault, (uint32_t)CLK_Src_EXT, 0U);
            }
            CLK_status.sourceMHz = CLK_status.extMHz;
            break;

        case CLK_Src_INTOSC1:
            // Select Internal oscillator 1 as clock source
            CLK_CLKSRCCTL1 = CLK_CLKSRCCTL1 & ~CLK_M_CLKSRCCTL1_OSCCLKSRCSEL
                           | CLKSRC_GetOSCSRC(CLK_Src_INTOSC1);
            CLK_status.sourceMHz = 10.0;
            break;

        default:
            // Do nothing. Not a valid source
            break;
    }

    CLK_status.source = source;
    // Wait 300 NOP for configuration to take effect
    SYS_NOP(100);
    SYS_NOP(200);
}

// Check if clock source configuration matches the input parameter
static inline bool CLK_CheckClockSource(enum CLK_Src source)
{
    enum CLK_Src currentSource = (enum CLK_Src) 
                               ( (CLK_CLKSRCCTL1 & CLK_M_CLKSRCCTL1_OSCCLKSRCSEL)
                               | ((CLK_XTALCR & CLK_M_XTALCR_SE) << 1) );

    return (source == currentSource);
}

static inline bool CLK_CheckPllConfig(enum CLK_CFG config)
{
    return (CLKCFG_GetPLL(config) == CLK_SYSPLLMULT);
}

static inline void CLK_SetSysClkDiv(uint16_t div)
{
    if(div == 1U)
    {
        CLK_SYSCLKDIVSEL = 0U;
    }
    else
    {
        CLK_SYSCLKDIVSEL = (div / 2U) | ((div % 2U) * CLK_M_SYSCLKDIVSEL_DIV_LSB);
    }
}

// Configure Clock Source 0 to clock source
void CLK_DCC_SetClockSource0(enum CLK_DCC dcc, enum CLK_DCC_ClkSrc0 source)
{
    CLK_DCC_CLKSRC0(dcc) = source;
}

// Configure Clock Source1 to PLL
void CLK_DCC_SetClockSource1(enum CLK_DCC dcc, enum CLK_DCC_ClkSrc1 source)
{
    CLK_DCC_CLKSRC1(dcc) = source;
}

// Configure COUNTER-0, COUNTER-1 & Valid Window
void CLK_DCC_SetCounterSeeds(enum CLK_DCC dcc, uint32_t seed0, uint32_t validSeed0, uint32_t seed1)
{
    CLK_DCC_CNTSEED0(dcc)   = seed0;
    CLK_DCC_VALIDSEED0(dcc) = validSeed0;
    CLK_DCC_CNTSEED1(dcc)   = seed1;
}

// Enable Single Shot mode
void CLK_DCC_SetSingleShotMode(enum CLK_DCC dcc, enum CLK_DCC_SingleShot mode)
{
    CLK_DCC_GCTRL(dcc) = CLK_DCC_GCTRL(dcc) & ~CLK_DCC_S_SINGLESHOT(CLK_DCC_M_GCTRL)
                       | mode;
}

// Disable Single Shot mode
void CLK_DCC_DisableSingleShotMode(enum CLK_DCC dcc)
{
    CLK_DCC_GCTRL(dcc) &= ~CLK_DCC_S_SINGLESHOT(CLK_DCC_M_GCTRL);
}

static bool CLK_VerifyPLLRAWCLK(enum CLK_CFG config)
{
    uint16_t mult, outDiv, refDiv;
    enum CLK_Src clkSrc;
    enum CLK_DCC_ClkSrc0 dccSrc0;
    enum CLK_DCC_ClkSrc1 dccSrc1;

    float pllRatio;
    float total_error, window;
    
    uint32_t dccCounterSeed0, dccCounterSeed1, dccValidSeed0;

    // Parse the configuration
    mult   = CLKCFG_GetMult(config);
    outDiv = CLKCFG_GetOutDiv(config) + 1;
    refDiv = CLKCFG_GetRefDiv(config) + 1;
    clkSrc = CLKCFG_GetSrc(config);

    switch(clkSrc)
    {
        case CLK_Src_INTOSC2:
            // DCC Clock Source 0 is INTOSC2
            dccSrc0 = CLK_DCC_ClkSrc0_INTOSC2;
            break;
        case CLK_Src_XTAL:
        case CLK_Src_EXT:
            // DCC Clock Source 0 is external clock circuit (XTAL or EXT)
            dccSrc0 = CLK_DCC_ClkSrc0_X1;
            break;
        case CLK_Src_INTOSC1:
            // DCC Clock Source 0 is INTOSC1
            dccSrc0 = CLK_DCC_ClkSrc0_INTOSC1;
            break;
        default:
            // Default DCC Clock Source 0 is INTOSC1
            dccSrc0 = CLK_DCC_ClkSrc0_INTOSC1;
            break;
    }

    // DCC Clock Source 1 is PLLRAWCLK (PLL output)
    dccSrc1 = CLK_DCC_ClkSrc1_PLLRAWCLK;

    pllRatio = (float)mult / ( ((float)outDiv) *  ((float)refDiv) );


    // For the math below, Fsysclk/Fclk0 = 1 because PLL is still bypassed
    if(pllRatio >= 1.0f)
    {
        total_error = 2+2+8;    // Async Err = 2 + 2*(Fsysclk/Fclk0), Digitization Err = 8
    }
    else
    {
        total_error = 2.0f / pllRatio + 2 + 8;  // Async Err = 2*(Fclk0/Fclk1) + 2*(Fsysclk/Fclk0), Digitization Err = 8
    }
    // Calculations as per TRM
    window = total_error / (CLK_TOLERANCE_PERCENT / 100.0f);
    dccCounterSeed0 = window - total_error;
    dccValidSeed0   = 2.0 * total_error;

    // Multiplying Counter-0 window with PLL Integer Multiplier
    dccCounterSeed1 = (window * pllRatio);

    // Enable DCC 0 clock gate
    SYS_EnablePeripheral(SYS_ClkGate_DCC0);

    // Clear Error & Done Flag
    CLK_DCC_ClearFlag(CLK_DCC_0,CLK_DCC_Flag_Error);
    CLK_DCC_ClearFlag(CLK_DCC_0,CLK_DCC_Flag_Done);

    // Disable DCC
    CLK_DCC_Enable(CLK_DCC_0, false);

    // Disable Error/Done Signals
    CLK_DCC_DisableFlag(CLK_DCC_0, CLK_DCC_Flag_Error);
    CLK_DCC_DisableFlag(CLK_DCC_0, CLK_DCC_Flag_Done);

    // Configure DCC Clock Sources
    CLK_DCC_SetClockSource0(CLK_DCC_0, dccSrc0);
    CLK_DCC_SetClockSource1(CLK_DCC_0, dccSrc1);

    // Configure COUNTER-0, COUNTER-1 & Valid Window
    CLK_DCC_SetCounterSeeds(CLK_DCC_0, dccCounterSeed0, dccValidSeed0, dccCounterSeed1);

    // Start DCC in sinsle-shot mode
    CLK_DCC_SetSingleShotMode(CLK_DCC_0, CLK_DCC_SingleShot_Counter0);
    CLK_DCC_Enable(CLK_DCC_0,true);


    // Wait until Error or Done Flag is generated
    while(!CLK_DCC_CheckFlag(CLK_DCC_0,CLK_DCC_Flag_Ended));

    // Returns true if DCC completes without error
    return(!CLK_DCC_CheckFlag(CLK_DCC_0,CLK_DCC_Flag_Error));
}


bool CLK_SetupClkConfig(enum CLK_CFG config)
{
    bool changePll, changeSrc;

    enum CLK_Src clkSrc = CLKCFG_GetSrc(config);

    bool status = false;

    // In case an MCD failure is detected, do not proceed with initialization
    if(CLK_IsClockFailDetected())
    {
        // Clock failure detected. Return false. Need to clear MCD error.
        status = false;
    } else {

        // Check if clock source/PLL are already configured to target values
        changeSrc = !CLK_CheckClockSource(clkSrc);
        changePll = !CLK_CheckPllConfig(config);


        // If either PLL config, or clock source is changed, bypass and power down PLL
        if(changePll || changeSrc)
        {
            // Set SYSCLK divider to safe value to ensure SYSCLK frequency remains valid before/after PLL bypass
            CLK_SetSysClkDiv(CLK_SAFE_SYSCLKDIV);

            // Bypass PLL
            // Delay of at least 60 NOP instructions required after PLL bypass
            CLK_SYSPLLCTL1 &= ~CLK_M_SYSPLLCTL1_PLLCLKEN;
            SYS_NOP(60);

            // PLL Bypassed. SYSCLK will never exceed maximum frequency. Set SYSCLK divider to 1 to speed up execution
            CLK_SetSysClkDiv(1);

            // Turn off PLL
            // Delay of at least 60 NOP instructions required after PLL power down
            CLK_SYSPLLCTL1 &= ~CLK_M_SYSPLLCTL1_PLLEN;
            SYS_NOP(60);
        }

        if (changeSrc)
        {
            // Select clock source
            CLK_SelectClkSrc(clkSrc);
        } else {
            CLK_status.source = clkSrc;
        }

        // If PLL is bypassed (either because it is not initialized, or powered down in the lines above)
        // Configure PLL according to the table
        if(!(CLK_SYSPLLCTL1 & CLK_M_SYSPLLCTL1_PLLCLKEN))
        {
            // Write PLL configuration to PLL config register
            // It should automatically power up the PLL, but will still write the enable bit just in case
            // Then wait 60 NOP to ensure the register is available to write again
            CLK_SYSPLLMULT = CLKCFG_GetPLL(config);
            CLK_SYSPLLCTL1 |= CLK_M_SYSPLLCTL1_PLLEN;
            SYS_NOP(60);

            // Wait for PLL to lock
            while(!(CLK_SYSPLLSTS & CLK_M_SYSPLLSTS_LOCKS));

            // Check PLL output frequency (PLLRAWCLK) using DCC
            status = CLK_VerifyPLLRAWCLK(config);

        } else {
            // Nothing changes
            status = true;
        }

        if(status)
        {
            // Connect PLL output to system clock
            CLK_SYSPLLCTL1 |= CLK_M_SYSPLLCTL1_PLLCLKEN;
        } else {
            HAL_FATAL(HAL_ErrorCode_HardwareFault, (uint32_t)config, 1U);
        }
    }
    CLK_UpdateFreqTable();
    return status;
}
void CLK_SetupDownclock(uint16_t ratio)
{
    CLK_SetSysClkDiv(ratio);

    CLK_status.downClock = ratio;
    CLK_UpdateFreqTable();
}

void CLK_SetCLKSRCCTL2(uint16_t shift, uint16_t value)
{
    CLK_CLKSRCCTL2 = CLK_CLKSRCCTL2 & ~(CLK_M_CLKSRCCTL2 << shift) | (value << shift);
}

bool CLK_SetupClkOut(enum CLK_OutSrc source, enum CLK_OutDiv div)
{
    CLK_CLKSRCCTL3 = source;
    CLK_XCLKOUTDIVSEL = div;
}
