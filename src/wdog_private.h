#ifndef WDOG_PRIVATE_H_
#define WDOG_PRIVATE_H_

#define WDOG_SCSR       ( REG16( WDOG_BASE + 0x22UL) )  // System Control & Status Register
#define WDOG_CNTR       ( REG16( WDOG_BASE + 0x23UL) )  // Watchdog Counter Register
#define WDOG_KEY        ( REG16( WDOG_BASE + 0x25UL) )  // Watchdog Reset Key Register
#define WDOG_WCR        ( REG16( WDOG_BASE + 0x2AUL) )  // Watchdog Windowed Control Register

#define WDOG_M_SCSR_WDOVERRIDE  ( 1U )

#define WDOG_V_CR_SYS_RST   (0)  // Value different than WDCR_WDCHK = 5, to cause a system reset.
#define WDOG_V_KEY1         ( 0x55 )
#define WDOG_V_KEY2         ( 0xAA )


#endif /* WDOG_PRIVATE_H_ */
