#ifndef TIMER_PRIVATE_H_
#define TIMER_PRIVATE_H_

#define TIMER_PRD(timer)              ( REG32( TIMER_BASE(timer) + 0x02UL ) ) // Timer Period
#define TIMER_TCR(timer)              ( REG32( TIMER_BASE(timer) + 0x04UL ) ) // Timer Control
#define TIMER_TPR(timer)              ( REG16( TIMER_BASE(timer) + 0x06UL ) ) // Timer Prescale Low
#define TIMER_TPRH(timer)             ( REG16( TIMER_BASE(timer) + 0x07UL ) ) // Timer Prescale High

#define TIMER_M_TCR_TSS         ( 1U << 4 )
#define TIMER_S_TPR_TDDR(d)     (((d) << 0) & 0xffu) // CPU timer divide down.
#define TIMER_S_TPR_PSC(d)      (((d) << 8) & 0xff00u) // CPU timer prescale counter.
#define TIMER_S_TPRH_TDDRH(d)   (((d) << 0) & 0xffu) // CPU timer divide down.
#define TIMER_S_TPRH_PSCH(d)    (((d) << 8) & 0xff00u) // CPU timer prescale counter.


#endif /* TIMER_PRIVATE_H_ */
