#ifndef ANALOG_PRIVATE_H_
#define ANALOG_PRIVATE_H_

#define ANALOG_CONFIGLOCK       ( REG32( ANALOG_BASE + 0x05EUL ) ) // Lock all ANALOG registers
#define ANALOG_TSNSCTL          ( REG16( ANALOG_BASE + 0x060UL ) ) // Temperature Sensor Control
#define ANALOG_ANAREFCTL        ( REG16( ANALOG_BASE + 0x068UL ) ) // Analog Reference Control
#define ANALOG_VMONCTL          ( REG16( ANALOG_BASE + 0x070UL ) ) // Voltage Monitor Control

#define ANALOG_CMPPMUXSEL(cmp)  ( REG32( ANALOG_BASE + 0x086UL - (CMPSS_CMP_GetHL(cmp) * 2U ) )) // CMPSS Comparator P-Mux Select, multi-map for CMPHPMXSEL/CMPLPMXSEL
#define ANALOG_CMPNMUXSEL(cmp)  ( REG16( ANALOG_BASE + 0x088UL - (CMPSS_CMP_GetHL(cmp) ) ))      // CMPSS Comparator N-Mux Select, multi-map for CMPHNMXSEL/CMPLNMXSEL

#define ANALOG_ADCDACLOOPBACK   ( REG32( ANALOG_BASE + 0x088UL ) ) // Enable loopback from DAC to ADC
#define ANALOG_LOCK             ( REG32( ANALOG_BASE + 0x08EUL ) ) // Lock Register

#define ANALOG_S_CMPPMUXSEL(cmpss,sel)  ( (sel) << ((cmpss) * 3))
#define ANALOG_M_CMPPMUXSEL(cmpss)      ( 0x07 << ((cmpss) * 3))


#endif /* ANALOG_PRIVATE_H_ */
