#ifndef SUPPORT_CMPSS_H_
#define SUPPORT_CMPSS_H_

#define CMPSS_CMP_Availability      (0x00FFUL)
#define CMPSS_START_ADDR            (0x00005C80UL)
#define CMPSS_INCREMENT_ADDR        (0x00000020UL)

#define CMPSS_CMPH      (0b10)
#define CMPSS_CMPL      (0b01)
#define CMPSS_CMPHL     (0b11)

#define CMPSS_S_CMP_Module(mod)     ( (mod) <<  2 )

enum CMPSS
{
    CMPSS_1 = 0,
    CMPSS_2 = 1,
    CMPSS_3 = 2,
    CMPSS_4 = 3
};

// To make XBAR status read faster, CMP bit order is H:L, which is different from CMPSS's bit order (L:H)
enum CMPSS_CMP
{
    CMPSS_CMP1H     = CMPSS_S_CMP_Module(CMPSS_1) | CMPSS_CMPH,
    CMPSS_CMP1L     = CMPSS_S_CMP_Module(CMPSS_1) | CMPSS_CMPL,
    CMPSS_CMP1HL    = CMPSS_S_CMP_Module(CMPSS_1) | CMPSS_CMPHL,
    CMPSS_CMP2H     = CMPSS_S_CMP_Module(CMPSS_2) | CMPSS_CMPH,
    CMPSS_CMP2L     = CMPSS_S_CMP_Module(CMPSS_2) | CMPSS_CMPL,
    CMPSS_CMP2HL    = CMPSS_S_CMP_Module(CMPSS_2) | CMPSS_CMPHL,
    CMPSS_CMP3H     = CMPSS_S_CMP_Module(CMPSS_3) | CMPSS_CMPH,
    CMPSS_CMP3L     = CMPSS_S_CMP_Module(CMPSS_3) | CMPSS_CMPL,
    CMPSS_CMP3HL    = CMPSS_S_CMP_Module(CMPSS_3) | CMPSS_CMPHL,
    CMPSS_CMP4H     = CMPSS_S_CMP_Module(CMPSS_4) | CMPSS_CMPH,
    CMPSS_CMP4L     = CMPSS_S_CMP_Module(CMPSS_4) | CMPSS_CMPL,
    CMPSS_CMP4HL    = CMPSS_S_CMP_Module(CMPSS_4) | CMPSS_CMPHL
};

enum CMPSS_Hysteresis
{
    CMPSS_Hysteresis_Disable,   // Disable Hysteresis for CMPSS Module
    CMPSS_Hysteresis_1x,        // 1x Hysteresis Mode. Min=04, Typ=12, Max=20 (LSB)
    CMPSS_Hysteresis_2x,        // 2x Hysteresis Mode. Min=17, Typ=24, Max=33 (LSB)
    CMPSS_Hysteresis_3x,        // 3x Hysteresis Mode. Min=25, Typ=36, Max=50 (LSB)
    CMPSS_Hysteresis_4x         // 4x Hysteresis Mode. Min=30, Typ=48, Max=67 (LSB)
};

#endif /* SUPPORT_CMPSS_H_ */
