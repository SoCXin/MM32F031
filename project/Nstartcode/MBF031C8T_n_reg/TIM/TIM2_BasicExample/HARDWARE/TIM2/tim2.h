#ifndef __TIM2_H_
#define __TIM2_H_
#include "sys.h"
void Tim2_UPCount_test(u16 Prescaler,u16 Period);
void Tim2_UPStatusOVCheck_test(void);
void TIM2_IRQHandler(void);

extern unsigned int ucTim2Flag;
#endif
