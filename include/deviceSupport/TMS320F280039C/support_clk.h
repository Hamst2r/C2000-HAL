#ifndef SUPPORT_CLK_H_
#define SUPPORT_CLK_H_

#define CLK_BASE                (0x0005D200UL)

#define CLK_DCC_START_ADDR      (0x0005E700UL)
#define CLK_DCC_INCREMENT_ADDR  (0x00000040UL)
#define CLK_DCC_COUNT           (2)

#define CLK_SOURCE_MIN_MHZ      ( 10)
#define CLK_SOURCE_MAX_MHZ      ( 20)
#define CLK_INTCLK_MIN_MHZ      (  2)
#define CLK_INTCLK_MAX_MHZ      ( 20)
#define CLK_PLLVCO_MIN_MHZ      (220)
#define CLK_PLLVCO_MAX_MHZ      (600)
#define CLK_PLLOUT_MIN_MHZ      (  2)
#define CLK_PLLOUT_MAX_MHZ      (120)
#define CLK_SYSCLK_MIN_MHZ      (  2)
#define CLK_SYSCLK_MAX_MHZ      (120)

#define CLK_SAFE_SYSCLKDIV      (2)

#define CLK_DCC_CLKSRC_KEY      ( 0b1010 << 12 )

enum CLK_DCC
{
    CLK_DCC_0 = 0,
    CLK_DCC_1 = 1
};

enum CLK_DCC_ClkSrc0
{
    CLK_DCC_ClkSrc0_X1          = CLK_DCC_CLKSRC_KEY | 0b0000,   // X1 pin (XTAL or EXT clock)
    CLK_DCC_ClkSrc0_INTOSC1     = CLK_DCC_CLKSRC_KEY | 0b0001,   // Internal oscillator 1 (10MHz)
    CLK_DCC_ClkSrc0_INTOSC2     = CLK_DCC_CLKSRC_KEY | 0b0010,   // Internal oscillator 2 (10MHz)
    CLK_DCC_ClkSrc0_CPU1SYSCLK  = CLK_DCC_CLKSRC_KEY | 0b0101,   // CPU1 system clock
    CLK_DCC_ClkSrc0_INPUTXBAR16 = CLK_DCC_CLKSRC_KEY | 0b1100    // Input XBAR 16
};

enum CLK_DCC_ClkSrc1
{
    CLK_DCC_ClkSrc1_PLLRAWCLK   = CLK_DCC_CLKSRC_KEY | 0b00000,  // PLL output
    CLK_DCC_ClkSrc1_INTOSC1     = CLK_DCC_CLKSRC_KEY | 0b00010,  // Internal oscillator 1 (10MHz)
    CLK_DCC_ClkSrc1_INTOSC2     = CLK_DCC_CLKSRC_KEY | 0b00011,  // Internal oscillator 2 (10MHz)
    CLK_DCC_ClkSrc1_CPU1SYSCLK  = CLK_DCC_CLKSRC_KEY | 0b00110,  // CPU1 system clock
    CLK_DCC_ClkSrc1_INPUTXBAR15 = CLK_DCC_CLKSRC_KEY | 0b01001,  // Input XBAR 16
    CLK_DCC_ClkSrc1_AUXCLKIN    = CLK_DCC_CLKSRC_KEY | 0b01010,  // Auxilluary clock in
    CLK_DCC_ClkSrc1_EPWMCLK     = CLK_DCC_CLKSRC_KEY | 0b01011,  // ePWM clock
    CLK_DCC_ClkSrc1_LSPCLK      = CLK_DCC_CLKSRC_KEY | 0b01100,  // Low-Speed clock
    CLK_DCC_ClkSrc1_ADCCLK      = CLK_DCC_CLKSRC_KEY | 0b01101,  // ADC clock
    CLK_DCC_ClkSrc1_WDCLK       = CLK_DCC_CLKSRC_KEY | 0b01110,  // Watch-Dog clock
    CLK_DCC_ClkSrc1_CAN0BITCLK  = CLK_DCC_CLKSRC_KEY | 0b01111   // CAN0 Bit clock
};
enum CLK_DCC_SingleShot
{
    //! Use to stop counting when counter0 and valid0 both reach zero
    CLK_DCC_SingleShot_Counter0 = 0xA00U

    //! Use to stop counting when counter1 reaches zero, not available to F280039C
    // CLK_DCC_SingleShot_Counter1 = 0xB00U
} ;


#endif /* SUPPORT_CLK_H_ */
