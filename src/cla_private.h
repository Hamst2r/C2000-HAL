#ifndef CLA_PRIVATE_H_
#define CLA_PRIVATE_H_

#define CLA_MVECT(task)         ( REG16( CLA_BASE + (task) ) )   // Task Interrupt Vector
#define CLA_MCTL                ( REG16( CLA_BASE + 0x10U ) )    // Control Register
#define CLA__MVECTBGRNDACTIVE   ( REG16( CLA_BASE + 0x1BU ) )    // Active register for MVECTBGRND.
#define CLA_SOFTINTEN           ( REG16( CLA_BASE + 0x1CU ) )    // CLA Software Interrupt Enable Register
#define CLA__MSTSBGRND          ( REG16( CLA_BASE + 0x1DU ) )    // Status register for the back ground task.
#define CLA__MCTLBGRND          ( REG16( CLA_BASE + 0x1EU ) )    // Control register for the back ground task.
#define CLA__MVECTBGRND         ( REG16( CLA_BASE + 0x1FU ) )    // Vector for the back ground task.
#define CLA_MIFR                ( REG16( CLA_BASE + 0x20U ) )    // Interrupt Flag Register
#define CLA_MIOVF               ( REG16( CLA_BASE + 0x21U ) )    // Interrupt Overflow Flag Register
#define CLA_MICLR               ( REG16( CLA_BASE + 0x23U ) )    // Interrupt Flag Clear Register
#define CLA_MICLROVF            ( REG16( CLA_BASE + 0x24U ) )    // Interrupt Overflow Flag Clear Register
#define CLA_MIER                ( REG16( CLA_BASE + 0x25U ) )    // Interrupt Enable Register
#define CLA_MIRUN               ( REG16( CLA_BASE + 0x26U ) )    // Interrupt Run Status Register
#define CLA__MPC                ( REG16( CLA_BASE + 0x28U ) )    // CLA Program Counter
#define CLA__MAR0               ( REG16( CLA_BASE + 0x2AU ) )    // CLA Auxiliary Register 0
#define CLA__MAR1               ( REG16( CLA_BASE + 0x2BU ) )    // CLA Auxiliary Register 1
#define CLA__MSTF               ( REG16( CLA_BASE + 0x2EU ) )    // CLA Floating-Point Status Register
#define CLA__MR0                ( REG16( CLA_BASE + 0x30U ) )    // CLA Floating-Point Result Register 0
#define CLA__MR1                ( REG16( CLA_BASE + 0x34U ) )    // CLA Floating-Point Result Register 1
#define CLA__MR2                ( REG16( CLA_BASE + 0x38U ) )    // CLA Floating-Point Result Register 2
#define CLA__MR3                ( REG16( CLA_BASE + 0x3CU ) )    // CLA Floating-Point Result Register 3
#define CLA__MPSACTL            ( REG16( CLA_BASE + 0x42U ) )    // CLA PSA Control Register
#define CLA__MPSA1              ( REG16( CLA_BASE + 0x44U ) )    // CLA PSA1 Register
#define CLA__MPSA2              ( REG16( CLA_BASE + 0x46U ) )    // CLA PSA2 Register

#define CLA_SRCLOCK             ( REG32( CLA_SRC_BASE + 0x00U ) )
#define CLA_SRCSEL(task)        ( REG32( CLA_SRC_BASE + 0x06U + ((task) >> 2) * 2 ) )

#define CLA_M_SRCSEL(task)              ( (0xFFU) << (((task) & 0x03U) * 8) )
#define CLA_S_SRCSEL(task,trig)         ( (trig) << (((task) & 0x03U) * 8) )


#endif /* CLA_PRIVATE_H_ */
