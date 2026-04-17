#include "xbar.h"
#include "xbar_private.h"

//
// Set Input XBAR source, with specified GPIO number
//
void XBAR_Input_SetSource(enum XBAR_Input inputXbar, uint16_t gpio)
{
    XBAR_INPUT_SELECT(inputXbar) = gpio;
}

enum EPWM_DcSrc XBAR_Trip_SetupSource(enum XBAR_Trip trip, enum XBAR_TripSrc source)
{
    XBAR_TRIP_CONFIG(trip,source) = XBAR_TRIP_CONFIG(trip,source) 
                                  & ~XBAR_S_Select(XBAR_Src_GetMuxNum(source),XBAR_M_Select)
                                  |  XBAR_S_Select(XBAR_Src_GetMuxNum(source),XBAR_Src_GetSelect(source));
    
    XBAR_TRIP_ENABLE(trip) |= (1 << XBAR_Src_GetMuxNum(source));

    return XBAR_TRIP_TO_DC_SRC(trip);

}

// enum EPWM_DcSrc XBAR_Trip_SetupCMPSS(enum XBAR_Trip trip, enum CMPSS_CMP cmp)
// {
//     enum CMPSS cmpss = CMPSS_CMP_GetCMPSS(cmp);
//     enum EPWM_DcSrc src;
//     switch(CMPSS_CMP_GetHL(cmp))
//     {
//         case CMPSS_CMPH:
//             src = XBAR_Trip_SetupSource(trip, (enum XBAR_TripSrc)(XBAR_S_SRC_MUX_NUM(cmpss*2)   | 0));
//             break;
            
//         case CMPSS_CMPL:
//             src = XBAR_Trip_SetupSource(trip, (enum XBAR_TripSrc)(XBAR_S_SRC_MUX_NUM(cmpss*2+1) | 0));
//             break;

//         case CMPSS_CMPHL:
//             XBAR_Trip_SetupSource(trip, (enum XBAR_TripSrc)(XBAR_S_SRC_MUX_NUM(cmpss*2) | 0));
//             src = XBAR_Trip_SetupSource(trip, (enum XBAR_TripSrc)(XBAR_S_SRC_MUX_NUM(cmpss*2+1) | 0));
//             break;

//     }
//     return src;
// }

void XBAR_Trip_SetupInvert(enum XBAR_Trip trip, bool invert)
{
    if(invert)
    {
        XBAR_TRIP_INVERT |= (1U << trip);
    } 
    else
    {
        XBAR_TRIP_INVERT &= ~(1U << trip);
    }
}

// void XBAR_Trip_EnableMux(enum XBAR_Trip trip, enum XBAR_TripSrc source)
// {
    
// }

// uint32_t XBAR_Trip_GetEnable(enum XBAR_Trip trip)
// {

// }

// uint32_t XBAR_Trip_Disable(enum XBAR_Trip trip)
// {

// }

// void XBAR_Trip_Enable(enum XBAR_Trip trip, uint32_t mask)
// {
    
// }


// Output XBAR

void XBAR_Output_SetupSource(enum XBAR_Output output, enum XBAR_OutSrc source)
{
    XBAR_OUTPUT_CONFIG(output,source) = XBAR_OUTPUT_CONFIG(output,source)
                                      & ~XBAR_S_Select(XBAR_Src_GetMuxNum(source),XBAR_M_Select)
                                      |  XBAR_S_Select(XBAR_Src_GetMuxNum(source),XBAR_Src_GetSelect(source));

    XBAR_OUTPUT_ENABLE(output) |= (1 << XBAR_Src_GetMuxNum(source));
}

void XBAR_Output_SetupLatch(enum XBAR_Output output, bool latch)
{

}


void XBAR_Output_SetupInvert(enum XBAR_Output output, bool invert)
{

}


// CLB XBAR, only compiled if device supports CLB
#if __has_include("support_clb.h")

void XBAR_CLB_SetupSource(enum XBAR_CLB xbar, enum XBAR_CLBSrc source)
{
    XBAR_CLB_CONFIG(xbar,source) = XBAR_CLB_CONFIG(xbar,source)
                                 & ~XBAR_S_Select(XBAR_Src_GetMuxNum(source),XBAR_M_Select)
                                 |  XBAR_S_Select(XBAR_Src_GetMuxNum(source),XBAR_Src_GetSelect(source));

    XBAR_CLB_ENABLE(xbar) |= (1 << XBAR_Src_GetMuxNum(source));
}


void XBAR_CLB_SetupInvert(enum XBAR_CLB xbar, bool invert)
{
    if(invert)
    {
        XBAR_CLB_INVERT |= (1U << xbar);
    } 
    else
    {
        XBAR_CLB_INVERT &= ~(1U << xbar);
    }
}

#endif

