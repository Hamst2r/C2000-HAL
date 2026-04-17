#include "cmpss.h"
#include "analog.h"
#include "xbar.h"

#define CMPSS_COMPCTL(cmpss)            ( REG16( CMPSS_BASE(cmpss) + 0x00 ) )   // CMPSS Comparator Control Register
#define CMPSS_COMPHYSCTL(cmpss)         ( REG16( CMPSS_BASE(cmpss) + 0x01 ) )   // CMPSS Comparator Hysteresis Control Register
#define CMPSS_COMPSTS(cmpss)            ( REG16( CMPSS_BASE(cmpss) + 0x02 ) )   // CMPSS Comparator Status Register
#define CMPSS_COMPSTSCLR(cmpss)         ( REG16( CMPSS_BASE(cmpss) + 0x03 ) )   // CMPSS Comparator Status Clear Register
#define CMPSS_COMPDACCTL(cmpss)         ( REG16( CMPSS_BASE(cmpss) + 0x04 ) )   // CMPSS DAC Control Register
#define CMPSS_COMPLOCK(cmpss)           ( REG16( CMPSS_BASE(cmpss) + 0x1A ) )   // CMPSS Lock Register

#define CMPSS_DACHVALS(cmpss)           ( REG16( CMPSS_BASE(cmpss) + 0x06 ) )   // CMPSS High DAC Value Shadow Register
#define CMPSS_DACHVALA(cmpss)           ( REG16( CMPSS_BASE(cmpss) + 0x07 ) )   // CMPSS High DAC Value Active Register
#define CMPSS_DACLVALS(cmpss)           ( REG16( CMPSS_BASE(cmpss) + 0x12 ) )   // CMPSS Low DAC Value Shadow Register
#define CMPSS_DACLVALA(cmpss)           ( REG16( CMPSS_BASE(cmpss) + 0x13 ) )   // CMPSS Low DAC Value Active Register

#define CMPSS_RAMPMAXREFA(cmpss)        ( REG16( CMPSS_BASE(cmpss) + 0x08 ) )   // CMPSS Ramp Max Reference Active Register
#define CMPSS_RAMPMAXREFS(cmpss)        ( REG16( CMPSS_BASE(cmpss) + 0x0A ) )   // CMPSS Ramp Max Reference Shadow Register
#define CMPSS_RAMPDECVALA(cmpss)        ( REG16( CMPSS_BASE(cmpss) + 0x0C ) )   // CMPSS Ramp Decrement Value Active Register
#define CMPSS_RAMPDECVALS(cmpss)        ( REG16( CMPSS_BASE(cmpss) + 0x0E ) )   // CMPSS Ramp Decrement Value ShadowRegister
#define CMPSS_RAMPSTS(cmpss)            ( REG16( CMPSS_BASE(cmpss) + 0x10 ) )   // CMPSS Ramp Status Register
#define CMPSS_RAMPDLYA(cmpss)           ( REG16( CMPSS_BASE(cmpss) + 0x14 ) )   // CMPSS Ramp Delay Active Register
#define CMPSS_RAMPDLYS(cmpss)           ( REG16( CMPSS_BASE(cmpss) + 0x15 ) )   // CMPSS Ramp Delay Shadow Register

// #define CMPSS_CTRIPLFILCTL(cmpss)       ( REG16( CMPSS_BASE(cmpss) + 0x16 ) )   // CTRIPL Filter Control Register
// #define CMPSS_CTRIPLFILCLKCTL(cmpss)    ( REG16( CMPSS_BASE(cmpss) + 0x17 ) )   // CTRIPL Filter Clock Control Register
// #define CMPSS_CTRIPHFILCTL(cmpss)       ( REG16( CMPSS_BASE(cmpss) + 0x18 ) )   // CTRIPH Filter Control Register
// #define CMPSS_CTRIPHFILCLKCTL(cmpss)    ( REG16( CMPSS_BASE(cmpss) + 0x19 ) )   // CTRIPH Filter Clock Control Register
#define CMPSS_CTRIPFILCTL(cmp)          ( REG16( CMPSS_BASE(CMPSS_CMP_GetCMPSS(cmp)) + 0x1A - (CMPSS_CMP_GetHL(cmp)*2U) ) )
#define CMPSS_CTRIPFILCLKCTL(cmp)       ( REG16( CMPSS_BASE(CMPSS_CMP_GetCMPSS(cmp)) + 0x1B - (CMPSS_CMP_GetHL(cmp)*2U) ) )

#define CMPSS_M_COMPCTL_EN                  ( 0x01U << 15 )
#define CMPSS_M_COMPCTL_SRCINV              ( 0x03U <<  0 )
#define CMPSS_M_COMPCTL_OUTPUT              ( 0x1FU <<  2 )
#define CMPSS_M_COMPDACCTL_BLANKEN          ( 0x01U << 12 )
#define CMPSS_M_COMPDACCTL_BLANKSRC         ( 0x0FU <<  8 )
#define CMPSS_M_COMPDACCTL_SWLOADSEL        ( 0x01U <<  7 )
#define CMPSS_M_COMPDACCTL_RAMPLOADSEL      ( 0x01U <<  6 )
#define CMPSS_M_COMPDACCTL_SELREF           ( 0x01U <<  5 )
#define CMPSS_M_COMPDACCTL_RAMPSOURCE       ( 0x0FU <<  1 )
#define CMPSS_M_COMPDACCTL_DACSOURCE        ( 0x01U <<  0 )
#define CMPSS_M_CTRIPFILCTL_FILINIT         ( 0x01U << 15 )

#define CMPSS_S_COMPCTL_EN(en)              ( (en)  << 15 )
#define CMPSS_S_COMPCTL_SRCINV(x)           ( (x)   <<  0 )
#define CMPSS_S_COMPCTL_OUTPUT(x)           ( (x)   <<  2 )
#define CMPSS_S_COMPDACCTL_BLANKEN(en)      ( (x)   << 12 )
#define CMPSS_S_COMPDACCTL_BLANKSRC(src)    ( (src) <<  8 )
#define CMPSS_S_COMPDACCTL_SWLOADSEL(sel)   ( (sel) <<  7 )
#define CMPSS_S_COMPDACCTL_RAMPLOADSEL(sel) ( (sel) <<  6 )
#define CMPSS_S_COMPDACCTL_SELREF(sel)      ( (sel) <<  5 )
#define CMPSS_S_COMPDACCTL_RAMPSOURCE(src)  ( (src) <<  1 )
#define CMPSS_S_COMPDACCTL_DACSOURCE(src)   ( (src) <<  0 )
#define CMPSS_S_CTRIPFILCTL_THRESH(x)       ( (x)   <<  9 )
#define CMPSS_S_CTRIPFILCTL_SAMPWIN(x)      ( (x)   <<  4 )

#define CMPSS_FILT_THRESHOLD_MAX            (32U)
#define CMPSS_FILT_SAMPWIN_MAX              (32U)


struct CMPSS_Status
{
    uint16_t cmpAvailable;
};

struct CMPSS_Status CMPSS_status = 
{
    .cmpAvailable = CMPSS_CMP_Availability
};

static inline bool CMPSS_GrabCMP(enum CMPSS_CMP cmp)
{
    uint16_t cmpBit = CMPSS_CMP_GetHL(cmp) << CMPSS_CMP_GetCMPSS(cmp);
    if((CMPSS_status.cmpAvailable & cmpBit) != cmpBit)
    {
        return false;
    } else {
        CMPSS_status.cmpAvailable &= cmpBit;
        return true;
    }
}

static inline void CMPSS_ConfigCMP(enum CMPSS_CMP cmp, enum CMPSS_NegSrc negSrc, enum CMPSS_Trig trig)
{
    CMPSS_COMPCTL(CMPSS_CMP_GetCMPSS(cmp)) = CMPSS_COMPCTL(CMPSS_CMP_GetCMPSS(cmp)) 
                                           & ~(CMPSS_M_COMPCTL_SRCINV * CMPSS_Accessor(cmp))
                                           |  (CMPSS_S_COMPCTL_SRCINV((negSrc << 1 | trig)) * CMPSS_Accessor(cmp));
}


static inline void CMPSS_Enable(enum CMPSS cmpss, bool enable)
{
    CMPSS_COMPCTL(cmpss) = CMPSS_COMPCTL(cmpss) & ~CMPSS_M_COMPCTL_EN | CMPSS_S_COMPCTL_EN(enable);
}

void CMPSS_SetupHysteresis(enum CMPSS_CMP cmp, enum CMPSS_Hysteresis hyst)
{
    CMPSS_COMPHYSCTL(CMPSS_CMP_GetCMPSS(cmp)) = hyst;
}


// Setup CMPSS DAC. Note: Setting is applied to whole CMPSS module (both CMPH and CMPL)
void CMPSS_SetupDAC(enum CMPSS_CMP cmp, enum CMPSS_DacMode mode, enum CMPSS_DacLoad loadMode)
{
    CMPSS_COMPDACCTL(CMPSS_CMP_GetCMPSS(cmp)) = CMPSS_COMPDACCTL(CMPSS_CMP_GetCMPSS(cmp))
                                              & ~(CMPSS_M_COMPDACCTL_SELREF | CMPSS_M_COMPDACCTL_DACSOURCE | CMPSS_M_COMPDACCTL_SWLOADSEL)
                                              | (CMPSS_S_COMPDACCTL_DACSOURCE(mode) | CMPSS_S_COMPDACCTL_SWLOADSEL(loadMode));
}


void CMPSS_SetupBlanking(enum CMPSS_CMP cmp, enum EPWM epwm)
{
    CMPSS_COMPDACCTL(CMPSS_CMP_GetCMPSS(cmp)) = CMPSS_COMPDACCTL(CMPSS_CMP_GetCMPSS(cmp))
                                              & ~CMPSS_M_COMPDACCTL_BLANKSRC
                                              | (CMPSS_S_COMPDACCTL_BLANKSRC(epwm) | CMPSS_M_COMPDACCTL_BLANKEN);
}

void CMPSS_DisableBlanking(enum CMPSS_CMP cmp)
{
    CMPSS_COMPDACCTL(CMPSS_CMP_GetCMPSS(cmp)) &= ~CMPSS_M_COMPDACCTL_BLANKEN;
}

void CMPSS_SetupOutput(enum CMPSS_CMP cmp, enum CMPSS_Output trip, enum CMPSS_Output tripOut)
{
    uint16_t value = CMPSS_S_COMPCTL_OUTPUT(trip | tripOut << 2) & CMPSS_M_COMPCTL_OUTPUT;

    CMPSS_COMPCTL(CMPSS_CMP_GetCMPSS(cmp)) = CMPSS_COMPCTL(CMPSS_CMP_GetCMPSS(cmp))
                                           & ~(CMPSS_M_COMPCTL_OUTPUT * CMPSS_Accessor(cmp))
                                           | (value * CMPSS_Accessor(cmp));
}

void CMPSS_SetupFilter(enum CMPSS_CMP cmp, uint16_t window, uint16_t threshold, uint16_t clkPrescale)
{
    uint16_t thresholdLim = (threshold > CMPSS_FILT_THRESHOLD_MAX) ? CMPSS_FILT_THRESHOLD_MAX : threshold;
    uint16_t windowLim    = (window    > CMPSS_FILT_SAMPWIN_MAX  ) ? CMPSS_FILT_SAMPWIN_MAX   : window;

    uint16_t value = CMPSS_M_CTRIPFILCTL_FILINIT
                 | CMPSS_S_CTRIPFILCTL_THRESH(thresholdLim - 1)
                 | CMPSS_S_CTRIPFILCTL_SAMPWIN(windowLim-1);

    if(CMPSS_CMP_GetHL(cmp)==CMPSS_CMPHL)
    {
        enum CMPSS_CMP hiCmp = CMPSS_CMP_GetHiCMP(cmp);
        enum CMPSS_CMP loCmp = CMPSS_CMP_GetLoCMP(cmp);

        CMPSS_CTRIPFILCTL(hiCmp) = value;
        CMPSS_CTRIPFILCTL(loCmp) = value;

        CMPSS_CTRIPFILCLKCTL(hiCmp) = clkPrescale;
        CMPSS_CTRIPFILCLKCTL(loCmp) = clkPrescale;
        
    }

    CMPSS_CTRIPFILCTL(cmp) = value;
    CMPSS_CTRIPFILCLKCTL(cmp) = clkPrescale;
}


enum CMPSS_CMP CMPSS_SetupSingle(enum AnalogPin pin, enum CMPSS_CMP cmp, enum CMPSS_Trig trig, uint16_t threshold, bool isOst)
{
    enum CMPSS cmpss = ANALOG_GetCMPSS(pin);

    if (cmpss != CMPSS_CMP_GetCMPSS(cmp))
    {
        // Specified CMP is not available to specified analog pin. Stop the code
        // TODO: Report error here
        HAL_FATAL(HAL_ErrorCode_InvalidArgument, (uint32_t)pin, (uint32_t)cmp);
    }

    if(!CMPSS_GrabCMP(cmp))
    {
        // CMP has been occupied. Stop the code
        // TODO: Report error here
        HAL_FATAL(HAL_ErrorCode_ResourceConflict, (uint32_t)cmp, 0U);
    }
    
    ANALOG_SetCmpPMux(pin, cmp);
    CMPSS_ConfigCMP(cmp, CMPSS_NegSrc_DAC, trig);
    CMPSS_SetupDAC(cmp,CMPSS_DacMode_SW,CMPSS_DacLoad_SysClk);
    CMPSS_SetThreshold(cmp,threshold);

    if(isOst)
    {
        // Connect CMP output to ePWM XBAR Trip 4 (OST)
        XBAR_Trip_SetupCMPSS(XBAR_Trip_4_OST,cmp);
    }

    CMPSS_Enable(cmpss, true);
    return cmp;
}

enum CMPSS_CMP CMPSS_SetupWindow(enum AnalogPin pin, uint16_t upperThreshold, uint16_t lowerThreshold, bool isOst)
{
    enum CMPSS cmpss = ANALOG_GetCMPSS(pin);
    enum CMPSS_CMP cmp = (enum CMPSS_CMP)(CMPSS_S_CMP_Module(cmpss) | CMPSS_CMPHL);
    enum CMPSS_CMP hiCmp = CMPSS_CMP_GetHiCMP(cmp);
    enum CMPSS_CMP loCmp = CMPSS_CMP_GetLoCMP(cmp);

    if (cmpss != CMPSS_CMP_GetCMPSS(cmp))
    {
        // Specified CMP is not available to specified analog pin. Stop the code
        // TODO: Report error here
        HAL_FATAL(HAL_ErrorCode_InvalidArgument, (uint32_t)pin, (uint32_t)cmp);
    }

    if(!CMPSS_GrabCMP(cmp))
    {
        // CMP has been occupied. Stop the code
        // TODO: Report error here
        HAL_FATAL(HAL_ErrorCode_ResourceConflict, (uint32_t)cmp, 0U);
    }

    ANALOG_SetCmpPMux(pin, hiCmp);
    ANALOG_SetCmpPMux(pin, loCmp);

    CMPSS_ConfigCMP(hiCmp, CMPSS_NegSrc_DAC, CMPSS_Trig_LargerThan);
    CMPSS_ConfigCMP(loCmp, CMPSS_NegSrc_DAC, CMPSS_Trig_SmallerThan);
    CMPSS_SetupDAC(cmp,CMPSS_DacMode_SW,CMPSS_DacLoad_SysClk);

    CMPSS_SetThreshold(hiCmp,upperThreshold);
    CMPSS_SetThreshold(loCmp,lowerThreshold);

    if(isOst)
    {
        // Connect CMP output to ePWM XBAR Trip 4 (OST)
        XBAR_Trip_SetupCMPSS(XBAR_Trip_4_OST,cmp);
    }
    
    CMPSS_Enable(cmpss, true);
    return cmp;
}

enum CMPSS_CMP CMPSS_SetupTwoPin(enum AnalogPin pinP, enum AnalogPin pinN, enum CMPSS_CMP cmp, enum CMPSS_Trig trig, bool isOst);
