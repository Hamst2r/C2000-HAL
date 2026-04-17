#ifndef GPIO_PRIVATE_H_
#define GPIO_PRIVATE_H_

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


#endif /* GPIO_PRIVATE_H_ */
