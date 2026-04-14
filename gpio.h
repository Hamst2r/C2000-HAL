#ifndef GPIO_H_
#define GPIO_H_

#include "base.h"
#include "support_gpio.h"

#define GPIO_GetGroup(pin)  (pin >> 5)

#define GPIO_DAT(pin)       ( REG32( GPIO_DATA_BASE + GPIO_GetGroup(pin) * 0x8U + 0x0U ) ) // GPIO Data Register
#define GPIO_SET(pin)       ( REG32( GPIO_DATA_BASE + GPIO_GetGroup(pin) * 0x8U + 0x2U ) ) // GPIO Data Set Register
#define GPIO_CLEAR(pin)     ( REG32( GPIO_DATA_BASE + GPIO_GetGroup(pin) * 0x8U + 0x4U ) ) // GPIO Data Clear Register
#define GPIO_TOGGLE(pin)    ( REG32( GPIO_DATA_BASE + GPIO_GetGroup(pin) * 0x8U + 0x6U ) ) // GPIO Data Toggle Register

#define GPIO_PUD(pin)       ( REG32( GPIO_CTRL_BASE + GPIO_GetGroup(pin) * 0x40U + 0x0CU ) ) // GPIO Pull Up Disable Register

#define GPIO_WRITE(pin,dat) ( REG32( GPIO_DATA_BASE + GPIO_GetGroup(pin) * 0x8U + 0x4U - (dat) * 2U) ) // GPIO Data Set/Clear Register depending on dat

#define GPIO_READ(pin)      ( REG32( GPIO_READ_BASE + GPIO_GetGroup(pin) * 0x8U + 0x0U ) ) // GPIO Data Read Register

#define GPIO_GetMask1(pin)  ( 0x01UL << (pin & 0x1F) )
#define GPIO_GetMask2(pin)  ( 0x03UL << ((pin & 0x0F)*2) )
#define GPIO_GetMask4(pin)  ( 0x0FUL << ((pin & 0x07)*4) )

enum GPIO_AGPIO_Mode
{
    GPIO_AGPIO_Mode_Digital = 0,
    GPIO_AGPIO_Mode_Analog  = 1
};

enum GPIO_QualMode
{
    GPIO_QualMode_Sync      = 0,
    GPIO_QualMode_3Sample   = 1,
    GPIO_QualMode_6Sample   = 2,
    GPIO_QualMode_Async     = 3
};

enum GPIO_PinMaster
{
    GPIO_PinMaster_CPU1 = 0,
    GPIO_PinMaster_CLA1 = 1,
    GPIO_PinMaster_CPU2 = 2,
    GPIO_PinMaster_CLA2 = 3,
    GPIO_PinMaster_CM   = 4,
    GPIO_PinMaster_HIC  = 5
};

static inline void GPIO_SetPin(uint16_t pin)
{
    GPIO_SET(pin) = GPIO_GetMask1(pin);
}

static inline void GPIO_ClearPin(uint16_t pin)
{
    GPIO_CLEAR(pin) = GPIO_GetMask1(pin);
}

static inline void GPIO_TogglePin(uint16_t pin)
{
    GPIO_TOGGLE(pin) = GPIO_GetMask1(pin);
}

static inline void GPIO_WritePin(uint16_t pin, bool value)
{
    GPIO_WRITE(pin,value) = GPIO_GetMask1(pin);
}

static inline bool GPIO_ReadPin(uint16_t pin)
{
    return (( GPIO_READ(pin) & GPIO_GetMask1(pin) ) != 0);
}

void GPIO_SetupAGPIO(enum GPIO_AGPIO agpio, enum GPIO_AGPIO_Mode mode);
void GPIO_SetupPinFn(enum GPIO_PinFn pinFn);
void GPIO_SetupPinAsGPIO(uint16_t pin, bool output, bool initialValue);
void GPIO_SetupPinMaster(uint16_t pin, enum GPIO_PinMaster master);
void GPIO_SetupPinInputMode(uint16_t pin, bool invert, enum GPIO_QualMode mode);
// void GPIO_SetupPinQuallicationDivider(
static inline void GPIO_SetupPinPullUp(uint16_t pin, bool pullUp)
{
    if(pullUp)
    {
        GPIO_PUD(pin) &= ~GPIO_GetMask1(pin);
    } else {
        GPIO_PUD(pin) |= GPIO_GetMask1(pin);
    }
}

#endif /* GPIO_H_ */
