#ifndef __WKUP_H
#define __WKUP_H	 
#include "dtype.h"
//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//开发板
//待机唤醒 代码	   
//////////////////////////////////////////////////////////////////////////////////
					    

	 
unsigned char Check_WKUP(void);  			//检测WKUP脚的信号
void WKUP_Init(void); 			//PA0 WKUP唤醒初始化
void Sys_Enter_Standby(void);	//系统进入待机模式
#endif


