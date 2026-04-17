#ifndef CLK_PRIVATE_H_
#define CLK_PRIVATE_H_

#define CLK_CLKCFGLOCK1         ( REG32( CLK_BASE + 0x02 ) )  // Lock bit for CLKCFG registers
#define CLK_CLKSRCCTL1          ( REG16( CLK_BASE + 0x08 ) )  // Clock Source Control register-1
#define CLK_CLKSRCCTL2          ( REG32( CLK_BASE + 0x0A ) )  // Clock Source Control register-2
#define CLK_CLKSRCCTL3          ( REG16( CLK_BASE + 0x0C ) )  // Clock Source Control register-3
#define CLK_SYSPLLCTL1          ( REG16( CLK_BASE + 0x0E ) )  // SYSPLL Control register-1
#define CLK_SYSPLLMULT          ( ATOMIC32( CLK_BASE + 0x14 ) )  // SYSPLL Multiplier register
#define CLK_SYSPLLSTS           ( REG16( CLK_BASE + 0x16 ) )  // SYSPLL Status register
#define CLK_SYSCLKDIVSEL        ( REG16( CLK_BASE + 0x22 ) )  // System Clock Divider Select register
#define CLK_XCLKOUTDIVSEL       ( REG16( CLK_BASE + 0x28 ) )  // XCLKOUT Divider Select register
#define CLK_LOSPCP              ( REG16( CLK_BASE + 0x2C ) )  // Low Speed Clock Source Prescalar
#define CLK_MCDCR               ( REG16( CLK_BASE + 0x2E ) )  // Missing Clock Detect Control Register
#define CLK_X1CNT               ( REG16( CLK_BASE + 0x30 ) )  // 11-bit Counter on X1 Clock, lower 16 bits of X1CNT
#define CLK_X1CNT_CLR           ( REG16( CLK_BASE + 0x31 ) )  // X1 Clock Counter Clear,     upper 16 bits of X1CNT
#define CLK_XTALCR              ( REG16( CLK_BASE + 0x32 ) )  // XTAL Control Register
#define CLK_XTALCR2             ( REG16( CLK_BASE + 0x3A ) )  // XTAL Control Register for pad init
#define CLK_CLKFAILCFG          ( REG16( CLK_BASE + 0x3C ) )  // Clock Fail cause Configuration

// CLK_DCC_GCTRL and CLK_DCC_STATUS are defined in header
#define CLK_DCC_CNTSEED0(dcc)   ( REG32( CLK_DCC_BASE(dcc) + 0x08 ) )  // Clock Source 0 counter seed value
#define CLK_DCC_VALIDSEED0(dcc) ( REG16( CLK_DCC_BASE(dcc) + 0x0C ) )  // Clock Source 0 timeout (valid) counter seed value
#define CLK_DCC_CNTSEED1(dcc)   ( REG32( CLK_DCC_BASE(dcc) + 0x10 ) )  // Clock Source 1 counter seed value
#define CLK_DCC_CNT0(dcc)       ( REG32( CLK_DCC_BASE(dcc) + 0x18 ) )  // Clock Source 0 counter
#define CLK_DCC_VALID0(dcc)     ( REG16( CLK_DCC_BASE(dcc) + 0x1C ) )  // Clock Source 0 timeout (valid) counter
#define CLK_DCC_CNT1(dcc)       ( REG32( CLK_DCC_BASE(dcc) + 0x20 ) )  // Clock Source 1 counter
#define CLK_DCC_CLKSRC1(dcc)    ( REG16( CLK_DCC_BASE(dcc) + 0x24 ) )  // Counter 1 clock source select
#define CLK_DCC_CLKSRC0(dcc)    ( REG16( CLK_DCC_BASE(dcc) + 0x28 ) )  // Counter 0 clock source select

#define CLK_M_CLKSRCCTL1_OSCCLKSRCSEL   ( 0x03U <<  0 )
#define CLK_M_CLKSRCCTL1_INTOSC2OFF     ( 0x01U <<  3 )
#define CLK_M_CLKSRCCTL2                ( 0x03U <<  0 )
#define CLK_M_XTALCR_SE                 ( 0x01U <<  1 )
#define CLK_M_XTALCR_OSCOFF             ( 0x01U <<  0 )
#define CLK_M_XTALCR2_FEN               ( 0x01U <<  2 )
#define CLK_M_XTALCR2_XOF               ( 0x01U <<  1 )
#define CLK_M_XTALCR2_XIF               ( 0x01U <<  0 )
#define CLK_M_SYSPLLCTL1_PLLCLKEN       ( 0x01U <<  1 )
#define CLK_M_SYSPLLCTL1_PLLEN          ( 0x01U <<  0 )
#define CLK_M_SYSPLLSTS_LOCKS           ( 0x01U <<  0 )
#define CLK_M_SYSCLKDIVSEL_DIV          ( 0x3FU <<  0 )
#define CLK_M_SYSCLKDIVSEL_DIV_LSB      ( 0x01U <<  8 )
#define CLK_M_MCDCR_MCLKCLR             ( 0x01U <<  1 )
#define CLK_M_MCDCR_MCLKSTS             ( 0x01U <<  0 )

#define CLK_X1CNT_MAX                   ( 0x07FFU )

#define CLKSRC_GetOSCSRC(src)   ( (src) & 0x03 )

#define CLKCFG_GetSrc(cfg)      ( (enum CLK_Src)((cfg) >> 29) )
#define CLKCFG_GetPLL(cfg)      ( (cfg) & 0x1F1F00FF )
#define CLKCFG_GetRefDiv(cfg)   ( ((cfg) >> 24) & 0x1F )
#define CLKCFG_GetOutDiv(cfg)   ( ((cfg) >> 16) & 0x1F )
#define CLKCFG_GetMult(cfg)     ( (cfg)  & 0xFF )


#endif /* CLK_PRIVATE_H_ */
