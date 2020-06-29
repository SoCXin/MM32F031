#include "dma.h"
//////////////////////////////////////////////////////////////////////////////////	 
//开发板
//DMA 代码	   
//////////////////////////////////////////////////////////////////////////////////

u16 DMA1_MEM_LEN;//保存DMA每次数据传送的长度 	    
//DMA1的各通道配置
//这里的传输形式是固定的,这点要根据不同的情况来修改
//从存储器->外设模式/8位数据宽度/存储器增量模式
//DMA_CHx:DMA通道CHx
//cpar:外设地址
//cmar:存储器地址
//cndtr:数据传输量 
/********************************************************************************************************
**函数信息 ：MYDMA_Config(DMA_Channel_TypeDef* DMA_CHx,u32 cpar,u32 cmar,u16 cndtr)                      
**功能描述 ：可修改根据输入参数修改配置的DMA配置
**输入参数 ：无
**输出参数 ：无
********************************************************************************************************/
void MYDMA_Config(DMA_Channel_TypeDef* DMA_CHx,u32 cpar,u32 cmar,u16 cndtr)
{
    RCC->AHBENR|=RCC_AHBENR_DMA1EN;			//开启DMA1时钟
    DMA_CHx->CPAR=cpar; 	 	//DMA1 外设地址 
    DMA_CHx->CMAR=(u32)cmar; 	//DMA1,存储器地址
    DMA1_MEM_LEN=cndtr;      	//保存DMA传输数据量
    DMA_CHx->CNDTR=cndtr;    	//DMA1,传输数据量
    DMA_CHx->CCR=0X00000000;	//复位
    DMA_CHx->CCR|=DMA_CCR1_DIR;  		//从存储器读
    DMA_CHx->CCR&=~DMA_CCR1_CIRC;  		//普通模式
    DMA_CHx->CCR&=~DMA_CCR1_PINC; 		//外设地址非增量模式
    DMA_CHx->CCR|=DMA_CCR1_MINC; 	 	//存储器增量模式
    DMA_CHx->CCR&=~DMA_CCR1_PSIZE_0; 	 	//外设数据宽度为8位
    DMA_CHx->CCR&=~DMA_CCR1_MSIZE_0; 		//存储器数据宽度8位
    DMA_CHx->CCR|=DMA_CCR1_PL_0; 		//中等优先级
    DMA_CHx->CCR&=~DMA_CCR1_MEM2MEM; 		//非存储器到存储器模式  	
} 



























