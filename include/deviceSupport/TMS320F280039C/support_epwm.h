#ifndef SUPPORT_EPWM_H_
#define SUPPORT_EPWM_H_

#define EPWM_START_ADDR         (0x00004000UL)
#define EPWM_INCREMENT_ADDR     (0x00000100UL)

#define EPWM_COUNT              8
#define EPWM_HR_Availability    (0b00001111)

#define EPWM_HR_EPWMCLK_MIN_MHZ (60)

// EPWM modules
enum EPWM
{
    EPWM_1 = 0,
    EPWM_2 = 1,
    EPWM_3 = 2,
    EPWM_4 = 3,
    EPWM_5 = 4,
    EPWM_6 = 5,
    EPWM_7 = 6,
    EPWM_8 = 7
};


enum EPWM_SyncInSrc
{
    EPWM_SyncInSrc_Disable      = 0x00, // Disable Sync Input
    EPWM_SyncInSrc_ePWM1        = 0x01, // ePWM1 Sync-Out Pulse
    EPWM_SyncInSrc_ePWM2        = 0x02, // ePWM2 Sync-Out Pulse
    EPWM_SyncInSrc_ePWM3        = 0x03, // ePWM3 Sync-Out Pulse
    EPWM_SyncInSrc_ePWM4        = 0x04, // ePWM4 Sync-Out Pulse
    EPWM_SyncInSrc_ePWM5        = 0x05, // ePWM5 Sync-Out Pulse
    EPWM_SyncInSrc_ePWM6        = 0x06, // ePWM6 Sync-Out Pulse
    EPWM_SyncInSrc_ePWM7        = 0x07, // ePWM7 Sync-Out Pulse
    EPWM_SyncInSrc_ePWM8        = 0x08, // ePWM8 Sync-Out Pulse
    EPWM_SyncInSrc_eCAP1        = 0x11, // eCAP1 Sync-Out Pulse
    EPWM_SyncInSrc_eCAP2        = 0x12, // eCAP2 Sync-Out Pulse
    EPWM_SyncInSrc_eCAP3        = 0x13, // eCAP3 Sync-Out Pulse
    EPWM_SyncInSrc_InputXBAR5   = 0x18, // Input XBAR 5
    EPWM_SyncInSrc_InputXBAR6   = 0x19, // Input XBAR 6
    EPWM_SyncInSrc_FSIRX_Trig1  = 0x1F, // FSI RX Trigger 1
};

// EPWM Digital Compare Input Source
enum EPWM_DcSrc
{
    EPWM_DcSrc_01_Input1    =  0,   // Input XBAR 1
    EPWM_DcSrc_02_Input2    =  1,   // Input XBAR 2
    EPWM_DcSrc_03_Input3    =  2,   // Input XBAR 3
    EPWM_DcSrc_06_Input6    =  5,   // Input XBAR 6
    EPWM_DcSrc_04_XBAR4_OST =  3,   // ePWM XBAR4  (TRIP4), dedicated for OST
    EPWM_DcSrc_05_XBAR5     =  4,   // ePWM XBAR5  (TRIP5)
    EPWM_DcSrc_07_XBAR7     =  6,   // ePWM XBAR7  (TRIP7)
    EPWM_DcSrc_08_XBAR8     =  7,   // ePWM XBAR8  (TRIP8)
    EPWM_DcSrc_09_XBAR9     =  8,   // ePWM XBAR9  (TRIP9)
    EPWM_DcSrc_10_XBAR10    =  9,   // ePWM XBAR10 (TRIP10)
    EPWM_DcSrc_11_XBAR11    = 10,   // ePWM XBAR11 (TRIP11)
    EPWM_DcSrc_12_XBAR12    = 11,   // ePWM XBAR12 (TRIP12)
    EPWM_DcSrc_14_ECCDBLERR = 13,   // ECC Double Bit Error
    EPWM_DcSrc_15_PIEVECTERR= 14,   // PIE Vector Error
    EPWM_DcSrc_Combination  = 15,   // Combination (ORed) input, need to configure
};

#endif /* SUPPORT_EPWM_H_ */
