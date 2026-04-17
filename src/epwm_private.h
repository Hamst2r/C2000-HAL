#ifndef EPWM_PRIVATE_H_
#define EPWM_PRIVATE_H_

#define EPWM_HRPHASE_COMPENSATION        ( (float) 1.6 )    //  Hi-Res Phase value for compensating delay during phase sync

#define EPWM_TBCTL(epwm)                 ( REG16( EPWM_BASE(epwm) + 0x00 ) )   // Time Base Control 1
#define EPWM_TBCTL2(epwm)                ( REG16( EPWM_BASE(epwm) + 0x01 ) )   // Time Base Control 2
#define EPWM_TBCTL3(epwm)                ( REG16( EPWM_BASE(epwm) + 0x07 ) )   // Time Base Control 3
#define EPWM_TBSTS(epwm)                 ( REG16( EPWM_BASE(epwm) + 0x05 ) )   // Time Base Status
#define EPWM_SYNCINSEL(epwm)             ( REG16( EPWM_BASE(epwm) + 0x03 ) )   // SYNC-IN Source Select
#define EPWM_SYNCOUTEN(epwm)             ( REG16( EPWM_BASE(epwm) + 0x06 ) )   // SYNC-OUT Source Enable
#define EPWM_CMPCTL(epwm,cmp)            ( REG16( EPWM_BASE(epwm) + 0x08 + (cmp >> 1)) )   // Counter Compare Control, multi-map for CMPCTL/CMPCTL2

#define EPWM_DBCTL(epwm)                 ( REG16( EPWM_BASE(epwm) + 0x0C ) )   // Dead-Band Control 1
#define EPWM_DBCTL2(epwm)                ( REG16( EPWM_BASE(epwm) + 0x0D ) )   // Dead-Band Control 2

#define EPWM_HRCNFG(epwm)                ( REG16( EPWM_BASE(epwm) + 0x20 ) )   // HRPWM Config 1
#define EPWM_HRCNFG2(epwm)               ( REG16( EPWM_BASE(epwm) + 0x27 ) )   // HRPWM Config 2
#define EPWM_HRPWR(epwm)                 ( REG16( EPWM_BASE(epwm) + 0x21 ) )   // HRPWM Power
#define EPWM_HRMSTEP(epwm)               ( REG16( EPWM_BASE(epwm) + 0x26 ) )   // HRPWM MEP Step
#define EPWM_HRPCTL(epwm)                ( REG16( EPWM_BASE(epwm) + 0x2D ) )   // Hi-Res Period Control
#define EPWM_TRREM(epwm)                 ( REG16( EPWM_BASE(epwm) + 0x2E ) )   // HRPWM Hi-Res Remainder

#define EPWM_AQCTL(epwm)                 ( REG16( EPWM_BASE(epwm) + 0x10 ) )   // Action Qualifier Control
#define EPWM_AQTSRCSEL(epwm)             ( REG16( EPWM_BASE(epwm) + 0x11 ) )   // Action Qualifier Trigger Source Select
#define EPWM_AQCTLx(epwm,aq,event)       ( REG16( EPWM_BASE(epwm) + 0x40 + (aq) * 2 + (event) >> 4) )   // Action Qualifier x Control, multi-map for AQCTLA/AQCTLA2/AQCTLB/AQCTLB2

#define EPWM_AQSFRC(epwm)                ( REG16( EPWM_BASE(epwm) + 0x47 ) )   // Action Qualifier S/W Force
#define EPWM_AQCSFRC(epwm)               ( REG16( EPWM_BASE(epwm) + 0x49 ) )   // Action Qualifier S/W Force (continuous)

#define EPWM_PCCTL(epwm)                 ( REG16( EPWM_BASE(epwm) + 0x14 ) )   // PWM Chopper Control

#define EPWM_GLDCTL(epwm)                ( REG16( EPWM_BASE(epwm) + 0x34 ) )   // Global Load Control 1
#define EPWM_GLDCTL2(epwm)               ( REG16( EPWM_BASE(epwm) + 0x74 ) )   // Global Load Control 2
#define EPWM_GLDCFG(epwm)                ( REG16( EPWM_BASE(epwm) + 0x35 ) )   // Global Load Config
#define EPWM_EPWMXLINK(epwm)             ( REG32( EPWM_BASE(epwm) + 0x38 ) )   // ePWM Register Link

#define EPWM_TZSEL(epwm)                 ( REG16( EPWM_BASE(epwm) + 0x80 ) )   // Trip Zone Select (connect input event to CBC/OST)
#define EPWM_TZCTL(epwm)                 ( REG16( EPWM_BASE(epwm) + 0x84 ) )   // Trip Zone Control 1 (output action w/o considering direction)
#define EPWM_TZCTL2(epwm)                ( REG16( EPWM_BASE(epwm) + 0x85 ) )   // Trip-Zone Control 2 (output action w/ direction)
#define EPWM_TZCTLDCA(epwm)              ( REG16( EPWM_BASE(epwm) + 0x86 ) )   // Trip Zone Control - Digital Compare A (output action w/ direction)
#define EPWM_TZCTLDCB(epwm)              ( REG16( EPWM_BASE(epwm) + 0x87 ) )   // Trip Zone Control - Digital Compare B (output action w/ direction)
#define EPWM_TZEINT(epwm)                ( REG16( EPWM_BASE(epwm) + 0x8D ) )   // Trip Zone Enable Interrupt

#define EPWM_ETSEL(epwm)                 ( REG16( EPWM_BASE(epwm) + 0xA4 ) )   // Event Trigger Select
#define EPWM_ETPS(epwm)                  ( REG16( EPWM_BASE(epwm) + 0xA6 ) )   // Event Trigger Pre-Scale
#define EPWM_ETFLG(epwm)                 ( REG16( EPWM_BASE(epwm) + 0xA8 ) )   // Event Trigger Flag
#define EPWM_ETCLR(epwm)                 ( REG16( EPWM_BASE(epwm) + 0xAA ) )   // Event Trigger Clear
#define EPWM_ETFRC(epwm)                 ( REG16( EPWM_BASE(epwm) + 0xAC ) )   // Event Trigger Force
#define EPWM_ETINTPS(epwm)               ( REG16( EPWM_BASE(epwm) + 0xAE ) )   // Event Trigger Interrupt Pre-Scale
#define EPWM_ETSOCPS(epwm)               ( REG16( EPWM_BASE(epwm) + 0xB0 ) )   // Event Trigger SOC Pre-Scale
#define EPWM_ETCNTINITCTL(epwm)          ( REG16( EPWM_BASE(epwm) + 0xB2 ) )   // Event Trigger Counter Initialization Control
#define EPWM_ETCNTINIT(epwm)             ( REG16( EPWM_BASE(epwm) + 0xB4 ) )   // Event Trigger Counter Initialization

#define EPWM_DcOrSel(epwm,cmp)           ( REG16( EPWM_BASE(epwm) + 0xD2 + cmp) )   // Digital Compare Trip Select (controls ORed/Combination input), multi-map for DCxxTRIPSEL
#define EPWM_DCTRIPSEL(epwm)             ( REG16( EPWM_BASE(epwm) + 0xC0 ) )   // Digital Compare Trip Select (TRIP to DCxH/L)
#define EPWM_TZDCSEL(epwm)               ( REG16( EPWM_BASE(epwm) + 0x82 ) )   // Trip Zone Digital Comparator Select (DCxH/L to DCxEVT1/2)
#define EPWM_DCxCTL(epwm,dcEvt)          ( REG16( EPWM_BASE(epwm) + 0xC3 + EPWM_GetDcModule(dcEvt)) )   // Digital Compare Control, multi-map for DCACTL/DCBCTL
#define EPWM_DCFCTL(epwm)                ( REG16( EPWM_BASE(epwm) + 0xC7 ) )   // Digital Compare Filter Control
#define EPWM_DCFOFFSET(epwm)             ( REG16( EPWM_BASE(epwm) + 0xC9 ) )   // Digital Compare Filter Offset
#define EPWM_DCFOFFSETCNT(epwm)          ( REG16( EPWM_BASE(epwm) + 0xCA ) )   // Digital Compare Filter Offset Counter
#define EPWM_DCFWINDOW(epwm)             ( REG16( EPWM_BASE(epwm) + 0xCB ) )   // Digital Compare Filter Window
#define EPWM_DCFWINDOWCNT(epwm)          ( REG16( EPWM_BASE(epwm) + 0xCC ) )   // Digital Compare Filter Window Counter
#define EPWM_BLANKPULSEMIXSEL(epwm)      ( REG16( EPWM_BASE(epwm) + 0xCD ) )   // Blanking Window Trigger Pulse Select
#define EPWM_DCCAPCTL(epwm)              ( REG16( EPWM_BASE(epwm) + 0xC8 ) )   // Digital Compare TB Counter Capture Control
#define EPWM_DCCAP(epwm)                 ( REG16( EPWM_BASE(epwm) + 0xCF ) )   // Digital Compare TB Counter Capture

#define EPWM_EPWMLOCK(epwm)              ( ATOMIC32( EPWM_BASE(epwm) + 0xFA ) )   // EPWM Lock
#define EPWM_VCAPCTL(epwm)               ( REG16( EPWM_BASE(epwm) + 0x18 ) )   // Valley Capture Control
#define EPWM_VCNTCFG(epwm)               ( REG16( EPWM_BASE(epwm) + 0x19 ) )   // Valley Counter Config
#define EPWM_SWVDELVAL(epwm)             ( REG16( EPWM_BASE(epwm) + 0x77 ) )   // Software Valley Mode Delay
#define EPWM_HWVDELVAL(epwm)             ( REG16( EPWM_BASE(epwm) + 0xFD ) )   // Hardware Valley Mode Delay
#define EPWM_VCNTVAL(epwm)               ( REG16( EPWM_BASE(epwm) + 0xFE ) )   // Hardware Valley Counter

#define EPWM_S_TBCTL_FREESOFT(mode)         ( (mode) << 14 )
#define EPWM_S_TBCTL_PHSDIR(dir)            ( (dir)  << 13 )
#define EPWM_S_TBCTL_CLKDIV(div)            ( (div)  <<  7 )
#define EPWM_S_TBCTL_PRDLD(load)            ( (load) <<  3 )
#define EPWM_S_TBCTL_PHSEN(en)              ( (en)   <<  2 )
#define EPWM_S_TBCTL_CTRMODE(mode)          ( (mode) <<  0 )
#define EPWM_S_TBCTL2_PRDLDSYNC(load)       ( (load) << 14 )
#define EPWM_S_CMPCTL(cmp,load)             ( (load) << ((cmp&1U) * 2))
#define EPWM_S_AQCTL(aq,load)               ( (load) << ((aq) * 2))
#define EPWM_S_DBCTL_OUTSWAP(swap)          ( (swap) << 12 )
#define EPWM_S_DBCTL_INMODE(mode)           ( (mode) <<  4 )
#define EPWM_S_DCxCTL_SYNCSEL(dcEvt,sync)   ( sync  << (EPWM_GetDcEvtNum(dcEvt)*8 + 1) )
#define EPWM_S_DCxCTL_LATCH(dcEvt,latch)    ( latch << (EPWM_GetDcEvtNum(dcEvt)*8 + 4) )
#define EPWM_S_TZCLR_CBC_PULSE(clear)       ( (clear) << 14 )
#define EPWM_S_ETSOCPS_PRD2(soc, prd)       ( (prd) <<  ((soc)*8) )
#define EPWM_S_ETSEL_SOCSEL(soc,sel)        ( (sel) <<  ((soc)*4 + 8U)  )

#define EPWM_M_TBCTL_FREESOFT            ( 0x03U << 14 )
#define EPWM_M_TBCTL_PHSDIR              ( 0x01U << 13 )
#define EPWM_M_TBCTL_CLKDIV              ( 0x3FU <<  7 )
#define EPWM_M_TBCTL_PRDLD               ( 0x01U <<  3 )
#define EPWM_M_TBCTL_PHSEN               ( 0x01U <<  2 )
#define EPWM_M_TBCTL_CTRMODE             ( 0x03U <<  0 )
#define EPWM_M_TBCTL2_PRDLDSYNC          ( 0x03U << 14 )
#define EPWM_M_AQCTLx(event)             ( 0x03U << (event) )
#define EPWM_M_CMPCTL(cmp)               ( 0x0C13U << ((cmp&1U) * 2))
#define EPWM_M_AQCTL(aq)                 ( 0x0313U << ((aq) * 2))
#define EPWM_M_DBCTL_HALFCYCLE           ( 0x01U << 15 )
#define EPWM_M_DBCTL_OUTSWAP             ( 0x03U << 12 )
#define EPWM_M_DBCTL_LOADMODE(path)      ( (path) ? 0x0B00 : 0x04C0 )
#define EPWM_M_DBCTL_INMODE              ( 0x03U <<  4 )
#define EPWM_M_DBCTL_OPMODE              ( 0x0FU <<  0 )    // Combination of POLSEL and OUTMODE
#define EPWM_M_TZDCSEL(evt)              ( 0x07U << ((evt)*3) )
#define EPWM_M_DCTRIPSEL(evt)            ( 0x0FU << ((evt)*4) )
#define EPWM_M_DCxCTL_SRCSEL(dcEvt)      ( 0x01U << (EPWM_GetDcEvtNum(dcEvt)*8) )
#define EPWM_M_DCxCTL_LATSYNC(dcEvt)     ( 0x72U << (EPWM_GetDcEvtNum(dcEvt)*8) )
#define EPWM_M_TZCTL(tzEvt)              ( 0x03U <<  (tzEvt)  )
#define EPWM_M_ETPS_SOCPSSEL             ( 0x01U <<  5  )
#define EPWM_M_ETSOCPS_PRD2(soc)         ( 0x0FU <<  ((soc)*8) )
#define EPWM_M_ETSEL_SOCEN(soc)          ( 0x01U <<  ((soc)*4 + 11U) )
#define EPWM_M_ETSEL_SOCSEL(soc)         ( 0x07U <<  ((soc)*4 + 8U)  )
#define EPWM_M_ETSEL_SOCSELCMP(soc)      ( 0x01U <<  ((soc)*1 + 4U)  )

#define EPWM_IsHiRes(epwm)               ( ( ((epwm) >> 6) & 1U ) == 1 )
#define EPWM_GetDcModule(dcEvt)          ( (dcEvt) >> 1 )
#define EPWM_GetDcEvtNum(dcEvt)          ( (dcEvt) & 1U )


#endif /* EPWM_PRIVATE_H_ */
