#include "gpio.h"
#include "support_analog.h"

#define GPIO_CTRL(pin)     ( REG32( GPIO_CTRL_BASE + GPIO_GetGroup(pin) * 0x40U + 0x00U ) ) // GPIO Qualification Sampling Period Control
#define GPIO_QSEL(pin)     ( REG32( GPIO_CTRL_BASE + GPIO_GetGroup(pin) * 0x40U + 0x02U + (((pin) >> 4) & 1U) * 2U ) ) // GPIO Qualifier Select 1 Register
#define GPIO_MUX(pin)      ( REG32( GPIO_CTRL_BASE + GPIO_GetGroup(pin) * 0x40U + 0x06U + (((pin) >> 4) & 1U) * 2U ) ) // GPIO Mux 1 Register
#define GPIO_DIR(pin)      ( REG32( GPIO_CTRL_BASE + GPIO_GetGroup(pin) * 0x40U + 0x0AU ) ) // GPIO Direction Register
#define GPIO_INV(pin)      ( REG32( GPIO_CTRL_BASE + GPIO_GetGroup(pin) * 0x40U + 0x10U ) ) // GPIO Input Polarity Invert Registers
#define GPIO_ODR(pin)      ( REG32( GPIO_CTRL_BASE + GPIO_GetGroup(pin) * 0x40U + 0x12U ) ) // GPIO Open Drain Output Register
#define GPIO_AMSEL(pin)    ( REG32( GPIO_CTRL_BASE + GPIO_GetGroup(pin) * 0x40U + 0x14U ) ) // GPIO Analog Mode Select register
#define GPIO_GMUX(pin)     ( REG32( GPIO_CTRL_BASE + GPIO_GetGroup(pin) * 0x40U + 0x20U + (((pin) >> 4) & 1U) * 2U ) ) // GPIO Peripheral Group Mux
#define GPIO_CSEL(pin)     ( REG32( GPIO_CTRL_BASE + GPIO_GetGroup(pin) * 0x40U + 0x28U + (((pin) >> 3) & 3U) * 2U ) ) // GPIO Core Select Register (GPIO0 to 7)
#define GPIO_LOCK(pin)     ( REG32( GPIO_CTRL_BASE + GPIO_GetGroup(pin) * 0x40U + 0x3CU ) ) // GPIO Lock Configuration Register
#define GPIO_CR(pin)       ( REG32( GPIO_CTRL_BASE + GPIO_GetGroup(pin) * 0x40U + 0x3EU ) ) // GPIO Lock Commit Register


#define GPIO_S_2BIT(pin,val)    ( (val) << ((pin & 0x0F)*2) )
#define GPIO_S_4BIT(pin,val)    ( (val) << ((pin & 0x07)*4) )

#define GPIO_PinFn_GetPin(cfg)  ( (cfg) >> 4 )
#define GPIO_PinFn_GetGMux(cfg) ( ((cfg) >> 2) & 0x3U )
#define GPIO_PinFn_GetMux(cfg)  ( (cfg) & 0x3U )


void GPIO_SetupAGPIO(enum GPIO_AGPIO agpio, enum GPIO_AGPIO_Mode mode)
{
    if(mode == GPIO_AGPIO_Mode_Analog)
    {
        GPIO_AMSEL(agpio)       |= GPIO_GetMask1(agpio);
        ANALOG_AGPIOCTRL(agpio) |= GPIO_GetMask1(agpio);
    } else {
        GPIO_AMSEL(agpio)       &= ~GPIO_GetMask1(agpio);
        ANALOG_AGPIOCTRL(agpio) &= ~GPIO_GetMask1(agpio);
    }
}

void GPIO_SetupPinFn(enum GPIO_PinFn pinFn)
{
    uint16_t pin = GPIO_PinFn_GetPin(pinFn);
    GPIO_MUX(pin)  &= ~(3U << (pin & 0x0F));
    GPIO_GMUX(pin)  =  GPIO_GMUX(pin)
                    & ~GPIO_GetMask2(pin)
                    |  GPIO_S_2BIT(pin,GPIO_PinFn_GetGMux(pinFn));
    GPIO_MUX(pin)  |=  GPIO_S_2BIT(pin,GPIO_PinFn_GetMux(pinFn));
}
void GPIO_SetupPinAsGPIO(uint16_t pin, bool output, bool initialValue)
{
    if(output)
    {
        GPIO_WritePin(pin, initialValue);
        GPIO_DIR(pin) |= GPIO_GetMask1(pin);
        GPIO_MUX(pin) &= ~GPIO_GetMask2(pin);
    } else {
        GPIO_DIR(pin) &= ~GPIO_GetMask1(pin);
        GPIO_MUX(pin) &= ~GPIO_GetMask2(pin);
    }
}

void GPIO_SetupPinMaster(uint16_t pin, enum GPIO_PinMaster master)
{
    GPIO_CSEL(pin) = GPIO_CSEL(pin)
                   & ~GPIO_GetMask4(pin)
                   | GPIO_S_4BIT(pin,master);
}

void GPIO_SetupPinInputMode(uint16_t pin, bool invert, enum GPIO_QualMode mode)
{
    if(invert)
    {
        GPIO_INV(pin) |= GPIO_GetMask1(pin);
    } else {
        GPIO_INV(pin) &= ~GPIO_GetMask1(pin);
    }

    GPIO_QSEL(pin) = GPIO_QSEL(pin)
                   & ~GPIO_GetMask2(pin)
                   | GPIO_S_2BIT(pin,mode);
}
// void GPIO_SetupPinQuallicationDivider(
void GPIO_SetupPinOpenDrain(uint16_t pin, bool openDrain)
{
    if(openDrain)
    {
        GPIO_ODR(pin) |= GPIO_GetMask1(pin);
    } else {
        GPIO_ODR(pin) &= ~GPIO_GetMask1(pin);
    }
}
