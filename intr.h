#ifndef INTR_H_
#define INTR_H_

#include "base.h"
#include "support_intr.h"

#define INTR_PIE_ACK        ( REG16( INTR_PIE_BASE + 0x01UL ) )

enum INTR_Grp
{
    INTR_Grp_1  =  1,
    INTR_Grp_2  =  2,
    INTR_Grp_3  =  3,
    INTR_Grp_4  =  4,
    INTR_Grp_5  =  5,
    INTR_Grp_6  =  6,
    INTR_Grp_7  =  7,
    INTR_Grp_8  =  8,
    INTR_Grp_9  =  9,
    INTR_Grp_10 = 10,
    INTR_Grp_11 = 11,
    INTR_Grp_12 = 12,
};

enum INTR_Ch
{
    INTR_Ch_1  =  0,
    INTR_Ch_2  =  1,
    INTR_Ch_3  =  2,
    INTR_Ch_4  =  3,
    INTR_Ch_5  =  4,
    INTR_Ch_6  =  5,
    INTR_Ch_7  =  6,
    INTR_Ch_8  =  7,
    INTR_Ch_9  =  8,
    INTR_Ch_10 =  9,
    INTR_Ch_11 = 10,
    INTR_Ch_12 = 11,
    INTR_Ch_13 = 12,
    INTR_Ch_14 = 13,
    INTR_Ch_15 = 14,
    INTR_Ch_16 = 15
};




void INTR_Init(void);
void INTR_RegisterISR(enum INTR_Src src, void* isr);
void INTR_Enable(enum INTR_Src src);
void INTR_Disable(enum INTR_Src src);

static inline void INTR_AckGroup(enum INTR_Grp group)
{
    INTR_PIE_ACK = (1U << (group - 1U));
}

static inline uint16_t INTR_EnableGlobal(void)
{
    return(__enable_interrupts());
}

static inline uint16_t INTR_DisableGlobal(void)
{
    return (__disable_interrupts());
}

static inline void INTR_RestoreGlobal(uint16_t context)
{
    __restore_interrupts(context);
}


#endif /* INTR_H_ */
