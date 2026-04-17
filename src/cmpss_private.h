#ifndef CMPSS_PRIVATE_H_
#define CMPSS_PRIVATE_H_

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


#endif /* CMPSS_PRIVATE_H_ */
