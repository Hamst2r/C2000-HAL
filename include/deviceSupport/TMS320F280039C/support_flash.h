#ifndef SUPPORT_FLASH_H_
#define SUPPORT_FLASH_H_

#define FLASH_BASE                      (0x0005F800UL)
#define FLASH_ECC_BASE                  (0x0005FB00UL)

#define FLASH_FEATURE_LPM           // Low-Power Mode
#define FLASH_FEATURE_APP_ON_BANK   // Each App slot gets its own bank
// #define FLASH_FEATURE_TRIM          // Pump trim via FW

#define FLASH_FCLK_MHZ                  (20.0f)
#define FLASH_MINIMUM_WAIT_STATE        (0)
#define FLASH_WAIT_STATE_AT_FMAX        (5)
#define FLASH_BANK_COUNT                (3)
#define FLASH_SECTOR_SIZE_KB            (8)
#define FLASH_BANK_SIZE_KB              (128)

enum FLASH_Bank
{
    FLASH_Bank_0,
    FLASH_Bank_1,
    FLASH_Bank_2
};






#endif
