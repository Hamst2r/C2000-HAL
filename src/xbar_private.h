#ifndef XBAR_PRIVATE_H_
#define XBAR_PRIVATE_H_

#define XBAR_TRIP_TO_DC_SRC(trip)   ( (enum EPWM_DcSrc) ((trip) - 3) )

#define XBAR_Input_SELECTLOCK(xbar) ( REG16( XBAR_INPUT_BASE + XBAR_GetOffset(xbar) + 0x1E ) )    //  Input Select Lock Register

#define XBAR_TRIP_CONFIG(trip,src)  ( REG32( XBAR_TRIP_BASE + 0x00 + (trip) * 4 + ((src) >> 6) * 2 ) )
#define XBAR_TRIP_INVERT            ( REG16( XBAR_TRIP_BASE + 0x38 ) )

#define XBAR_OUTPUT_CONFIG(output,src)  ( REG32( XBAR_OUTPUT_BASE + XBAR_GetOffset(output) + 0x00 + XBAR_GetChannel(output) * 4 + ((src) >> 6) * 2 ) )
#define XBAR_OUTPUT_ENABLE(output)      ( REG32( XBAR_OUTPUT_BASE + XBAR_GetOffset(output) + 0x20 + XBAR_GetChannel(output) * 2) )
#define XBAR_OUTPUT_INVERT(output)      ( REG16( XBAR_OUTPUT_BASE + XBAR_GetOffset(output) + 0x38 ) )

#define XBAR_Src_GetMuxNum(src)     ( ((src) >> 2) & 0x1FU)
#define XBAR_Src_GetSelect(src)     ( (src) & 0x03U)

#define XBAR_M_Select           ( 0x03U )
#define XBAR_S_Select(mux,val)  ( (val) << (((mux) & 0x0F) * 2) )

#if __has_include("support_clb.h")
#define XBAR_CLB_CONFIG(xbar,src)   ( REG32( XBAR_CLB_BASE + 0x00 + (xbar) * 4 + ((src) >> 6) * 2 ) )
#define XBAR_CLB_ENABLE(xbar)       ( REG32( XBAR_CLB_BASE + 0x20 + (xbar) * 2) )
#define XBAR_CLB_INVERT             ( REG16( XBAR_CLB_BASE + 0x38 ) )
#endif


#endif /* XBAR_PRIVATE_H_ */
