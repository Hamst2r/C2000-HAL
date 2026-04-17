#include "analog.h"
#include "cmpss.h"

#define ANALOG_CONFIGLOCK       ( REG32( ANALOG_BASE + 0x05EUL ) ) // Lock all ANALOG registers
#define ANALOG_TSNSCTL          ( REG16( ANALOG_BASE + 0x060UL ) ) // Temperature Sensor Control
#define ANALOG_ANAREFCTL        ( REG16( ANALOG_BASE + 0x068UL ) ) // Analog Reference Control
#define ANALOG_VMONCTL          ( REG16( ANALOG_BASE + 0x070UL ) ) // Voltage Monitor Control

// #define ANALOG_CMPHPMXSEL       ( REG32( ANALOG_BASE + 0x082UL ) ) // CMPSS High Comparator P-Mux Select
// #define ANALOG_CMPLPMXSEL       ( REG32( ANALOG_BASE + 0x084UL ) ) // CMPSS Low Comparator P-Mux Select
// #define ANALOG_CMPHNMXSEL       ( REG16( ANALOG_BASE + 0x086UL ) ) // CMPSS High Comparator N-Mux Select
// #define ANALOG_CMPLNMXSEL       ( REG16( ANALOG_BASE + 0x087UL ) ) // CMPSS Low Comparator N-Mux Select
#define ANALOG_CMPPMUXSEL(cmp)  ( REG32( ANALOG_BASE + 0x086UL - (CMPSS_CMP_GetHL(cmp) * 2U ) )) // CMPSS Comparator P-Mux Select, multi-map for CMPHPMXSEL/CMPLPMXSEL
#define ANALOG_CMPNMUXSEL(cmp)  ( REG16( ANALOG_BASE + 0x088UL - (CMPSS_CMP_GetHL(cmp) ) ))      // CMPSS Comparator N-Mux Select, multi-map for CMPHNMXSEL/CMPLNMXSEL

#define ANALOG_ADCDACLOOPBACK   ( REG32( ANALOG_BASE + 0x088UL ) ) // Enable loopback from DAC to ADC
#define ANALOG_LOCK             ( REG32( ANALOG_BASE + 0x08EUL ) ) // Lock Register

#define ANALOG_S_CMPPMUXSEL(cmpss,sel)  ( (sel) << ((cmpss) * 3))


#define ANALOG_M_CMPPMUXSEL(cmpss)      ( 0x07 << ((cmpss) * 3))


struct ANALOG_Status ANALOG_status;


void ANALOG_SetupReference(enum ANALOG_Ref ref)
{
    ANALOG_ANAREFCTL = (ANALOG_ANAREFCTL & 0xFEFE) | ref;
    ANALOG_status.refMode = ref;
}

void ANALOG_SetCmpPMux(enum AnalogPin pin, enum CMPSS_CMP cmp)
{
    enum CMPSS cmpss = CMPSS_CMP_GetCMPSS(cmp);
    ANALOG_CMPPMUXSEL(cmp) = ANALOG_CMPPMUXSEL(cmp) & ~ANALOG_M_CMPPMUXSEL(cmpss)
                           | ANALOG_S_CMPPMUXSEL(cmpss,ANALOG_GetPMux(pin));
}
