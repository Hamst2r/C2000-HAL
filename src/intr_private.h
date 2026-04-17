#ifndef INTR_PRIVATE_H_
#define INTR_PRIVATE_H_

#define INTR_GetGroup(src)         ( (enum INTR_Grp)((src) >>  8 ) )
#define INTR_GetChannel(src)       ( (enum INTR_Ch)((src) & 0xFF) )
#define INTR_VECTOR(vectorID)      ATOMIC32(INTR_VECTOR_BASE + (vectorID) * 2UL)

#define INTR_PIE_CTRL       ( REG16( INTR_PIE_BASE + 0x00UL ) )
#define INTR_PIE_IER(grp)   ( REG16( INTR_PIE_BASE + 0x00UL + 0x02 * (grp) ) )
#define INTR_PIE_IFR(grp)   ( REG16( INTR_PIE_BASE + 0x01UL + 0x02 * (grp) ) )

#define INTR_M_PIE_CTRL_ENPIE   ( 1U << 0 )


#endif /* INTR_PRIVATE_H_ */
