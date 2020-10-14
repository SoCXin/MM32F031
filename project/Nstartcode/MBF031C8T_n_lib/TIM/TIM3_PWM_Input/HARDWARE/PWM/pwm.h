#ifndef __PWM_H_
#define __PWM_H_
#include "HAL_device.h"
#include "HAL_conf.h"


void TIM2_PWM_Init(u16 arr,u16 psc);
void TIM3_PWMINPUT_INIT(u16 arr,u16 psc);

#endif
