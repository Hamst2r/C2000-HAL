#ifndef CLA_H_
#define CLA_H_

#include "support_cla.h"

#define CLA_MIFRC              ( REG16( CLA_BASE + 0x22U ) )  // Interrupt Force Register

enum CLA_Task
{
    CLA_Task_1 = 0,   // CLA Task 1 Flag
    CLA_Task_2 = 1,   // CLA Task 2 Flag
    CLA_Task_3 = 2,   // CLA Task 3 Flag
    CLA_Task_4 = 3,   // CLA Task 4 Flag
    CLA_Task_5 = 4,   // CLA Task 5 Flag
    CLA_Task_6 = 5,   // CLA Task 6 Flag
    CLA_Task_7 = 6,   // CLA Task 7 Flag
    CLA_Task_8 = 7    // CLA Task 8 Flag
};


void CLA_Init(void);
void CLA_SetupTask(enum CLA_Task task, enum CLA_Trig trigger, void* taskAddr);


//
// Force a CLA Task to start
//
static inline void CLA_StartTask(enum CLA_Task task)
{
    // Force the task
    CLA_MIFRC = (1U << task);
}





#endif /* CLA_H_ */
