#ifndef __DMA_H
#define	__DMA_H	   
#include "HAL_device.h"
//////////////////////////////////////////////////////////////////////////////////	 
//开发板
//DMA 代码	   
//////////////////////////////////////////////////////////////////////////////////
							    					    
void MYDMA_Config(DMA_Channel_TypeDef*DMA_CHx,u32 cpar,u32 cmar,u16 cndtr);//配置DMA1_CHx

void MYDMA_Enable(DMA_Channel_TypeDef*DMA_CHx);//使能DMA1_CHx

   
#endif




