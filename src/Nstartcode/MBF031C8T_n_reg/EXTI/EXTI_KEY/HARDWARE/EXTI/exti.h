#ifndef __EXTI_H
#define __EXIT_H	 
#include "sys.h"
//////////////////////////////////////////////////////////////////////////////////	 
//开发板
//外部中断 驱动代码			   
////////////////////////////////////////////////////////////////////////////////// 	  


void EXTIX_Init(void);//IO初始化

#define GPIO_A 0
#define GPIO_B 1
#define GPIO_C 2
#define GPIO_D 3
#define GPIO_E 4
#define GPIO_F 5
#define GPIO_G 6 
#define FTIR   1  //下降沿触发
#define RTIR   2  //上升沿触发

#endif

