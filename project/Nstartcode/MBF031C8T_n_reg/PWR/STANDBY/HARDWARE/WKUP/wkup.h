#ifndef __WKUP_H
#define __WKUP_H	 
#include "HAL_device.h"
//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//开发板
//待机唤醒 代码	   
//////////////////////////////////////////////////////////////////////////////////
#define GPIO_A 0
#define GPIO_B 1
#define GPIO_C 2
#define GPIO_D 3
#define GPIO_E 4
#define GPIO_F 5
#define GPIO_G 6 
#define FTIR   1  //下降沿触发
#define RTIR   2  //上升沿触发
				    
	 
u8 Check_WKUP(void);  			//检测WKUP脚的信号
void WKUP_Init(void); 			//PA0 WKUP唤醒初始化
void Ex_NVIC_Config(u8 GPIOx,u8 BITx,u8 TRIM);
void Sys_Enter_Standby(void);	//系统进入待机模式
#endif


