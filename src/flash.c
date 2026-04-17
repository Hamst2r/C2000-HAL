#include "flash.h"
#include "support_flash.h"
#include "sys.h"
#include "clk.h"
#include "flash_private.h"
#include <math.h>

#include "FlashAPI/F021_F28003x_C28x.h"

enum FLASH_BankPwr
{
    FLASH_BankPwr_Active    = 0x3U, // Bank stays active
    FLASH_BankPwr_Standby   = 0x1U, // Bank fallback to Standby after timeout
    FLASH_BankPwr_Sleep     = 0x0U  // Bank fallback to Sleep after timeout
};

enum FLASH_PumpPwr
{
    FLASH_PumpPwr_Active,   // Pump stays active
    FLASH_PumpPwr_Sleep     // Pump fallback to Sleep after timeout
};

#pragma CODE_SECTION(FLASH_SetBankFallbackPowerMode, ".TI.ramfunc");
#pragma CODE_SECTION(FLASH_SetPumpFallbackPowerMode, ".TI.ramfunc");
#pragma CODE_SECTION(FLASH_WakeUp, ".TI.ramfunc");
#pragma CODE_SECTION(FLASH_SetWaitstates, ".TI.ramfunc");

#pragma CODE_SECTION(FLASH_EnableCache, ".TI.ramfunc");
#pragma CODE_SECTION(FLASH_DisableCache, ".TI.ramfunc");
#pragma CODE_SECTION(FLASH_EnablePrefetch, ".TI.ramfunc");
#pragma CODE_SECTION(FLASH_DisablePrefetch, ".TI.ramfunc");

#pragma CODE_SECTION(FLASH_EnableECC, ".TI.ramfunc");
#pragma CODE_SECTION(FLASH_DisableECC, ".TI.ramfunc");

#pragma CODE_SECTION(FLASH_Init, ".TI.ramfunc");


static inline void FLASH_SetBankFallbackPowerMode(enum FLASH_Bank bank, enum FLASH_BankPwr mode)
{
    FLASH_FBFALLBACK =  FLASH_FBFALLBACK
                     & ~FLASH_M_FBFALLBACK(bank)
                     |  FLASH_S_FBFALLBACK(bank, mode);
}

static inline void FLASH_SetPumpFallbackPowerMode(enum FLASH_PumpPwr mode)
{
    if(mode)
    {
        FLASH_FPAC1 |= FLASH_M_FPAC1_PMPPWR;
    } else {
        FLASH_FPAC1 &= ~FLASH_M_FPAC1_PMPPWR;
    }
}

static inline void FLASH_WakeUp(void)
{

    // Set the bank fallback power modes to Active
    for(uint16_t bank=0;bank<FLASH_BANK_COUNT;bank++)
    {
        FLASH_SetBankFallbackPowerMode((enum FLASH_Bank)bank, FLASH_BankPwr_Active);
    }

    // Set charge pump fallback power mode to Active
    FLASH_SetPumpFallbackPowerMode(FLASH_PumpPwr_Active);
}

// static inline void FLASH_SetBankFallbackTime(uint16_t cycles);
// static inline void FLASH_SetPumpFallbackTime(uint16_t cycles);
// static inline void FLASH_SetPumpWakeupTime(uint16_t cycles);

static inline void  FLASH_SetWaitstates(uint16_t waitStates)
{
    FLASH_FRDCNTL =  FLASH_FRDCNTL
                  & ~FLASH_M_FRDCNTL_RWAIT
                  |  FLASH_S_FRDCNTL_RWAIT(waitStates);
}

static inline void FLASH_EnableCache(void)
{
    FLASH_FRD_INTF_CTRL |= FLASH_M_FRD_INTF_CTRL_CACHE;
}

static inline void FLASH_DisableCache(void)
{
    FLASH_FRD_INTF_CTRL &= ~FLASH_M_FRD_INTF_CTRL_CACHE;
}

static inline void FLASH_EnablePrefetch(void)
{
    FLASH_FRD_INTF_CTRL |= FLASH_M_FRD_INTF_CTRL_PF;
}

static inline void FLASH_DisablePrefetch(void)
{
    FLASH_FRD_INTF_CTRL &= ~FLASH_M_FRD_INTF_CTRL_PF;
}

static inline void FLASH_EnableECC(void)
{
    FLASH_ECC_ENABLE =  FLASH_ECC_ENABLE
                     & ~FLASH_ECC_M_ENABLE
                     |  0xAU;
}

// static inline void FLASH_DisableECC(void)
// {
//     FLASH_ECC_ENABLE =  FLASH_ECC_ENABLE
//                      & ~FLASH_ECC_M_ENABLE
//                      |  0x5U;
// }

// static inline bool FLASH_IsBankReady(enum FLASH_Bank bank)
// {
//     return ( ( (FLASH_FBPRDY >> bank) & 0x1U ) == 0x1U);
// }

// static inline bool FLASH_IsPumpReady(void)
// {
//     return ( ( FLASH_FBPRDY & FLASH_M_FBPRDY_PUMPRDY ) != 0);
// }

// static void FLASH_EraseAltFW(void)
// {
//     FLASH_ALT
// }

void FLASH_Init(void)
{
    #ifdef FLASH_FEATURE_LPM
    // Wake up flash banks and charge pump.
    FLASH_WakeUp();
    #endif

    // Disable cache and prefetch mechanism before changing wait states
    FLASH_DisableCache();
    FLASH_DisablePrefetch();

    // Set waitstates according to operating frequency
    FLASH_SetWaitstates( ceilf(CLK_status.sysClkPhysicalMHz / FLASH_FCLK_MHZ - 1) );
//    FLASH_SetWaitstates( FLASH_WAIT_STATE_AT_FMAX );

    #ifdef FLASH_FEATURE_TRIM
    FLASH_Trim();
    #endif

    // Enable cache and prefetch mechanism to improve performance of code
    // executed from flash.
    FLASH_EnableCache();
    FLASH_EnablePrefetch();

    // At reset, ECC is enabled. If it is disabled by application software and
    // if application again wants to enable ECC.
    FLASH_EnableECC();

    // Force a CPU pipeline flush to ensure that the write to the last register
    // configured occurs before returning.
    SYS_NOP(8);

    // Initialize the Flash API
    Fapi_initializeAPI(F021_CPU0_BASE_ADDRESS, CLK_status.sysClkPhysicalMHz);
}

void FLASH_Update(void)
{


}
