#include "dma.h"																	   	  
#include "delay.h"																	   	  
#include "uart.h"																	   	  
#include "sys.h"

unsigned char dma1Flag = 0x0;
unsigned char dma2Flag = 0x0;

u8 R_Buffer[64*4]= {0};//接收地址
u8 T_Buffer[64*4]= {0};//发送地址


void DMA1_Channel1_IRQHandler()
{
    if(DMA1->ISR&DMA_ISR_TCIF1)
    {
        DMA1->IFCR=DMA_IFCR_CTCIF1;
        dma1Flag = 0x1;
    }
}

void DMA1_Channel2_3_IRQHandler()
{
    if(DMA1->ISR&DMA_ISR_TCIF2)
    {
        DMA1->IFCR=DMA_IFCR_CTCIF2;
        dma2Flag = 0x1;
    }
}


void DMAcheckStatus(uint32_t DMA_FLAG)
{
    while(1)
    {
        if(DMA1->ISR&DMA_FLAG)
        {
            DMA1->IFCR=DMA_FLAG;
            break;
        }
    }
}
void DMAdisable(DMA_Channel_TypeDef* DMAy_Channelx)
{
    //disable DMA_EN
    DMAy_Channelx->CCR &= 0xFFFFFFFE;
}

void dma_m8tom8_test()
{
    unsigned int i;
    unsigned int temp;
    for(i=0;i<64;i++)
    {
        *((volatile unsigned char *)((u32)R_Buffer+i)) = i +1;
    }
    RCC->AHBENR|=RCC_AHBENR_DMA1EN;
    delay_ms(5);				//等待DMA时钟稳定
    DMA1_Channel2->CPAR=(u32)T_Buffer; 	 	//DMA1 外设地址 
    DMA1_Channel2->CMAR=(u32)R_Buffer; 	//DMA1,存储器地址
    DMA1_Channel2->CCR|=DMA_CCR1_DIR;
    DMA1_Channel2->CNDTR=64;
    DMA1_Channel2->CCR|=DMA_CCR1_PINC;
    DMA1_Channel2->CCR|=DMA_CCR1_MINC;
    DMA1_Channel2->CCR&=~DMA_CCR1_PSIZE_0; 	 	//外设数据宽度为8位
    DMA1_Channel2->CCR&=~DMA_CCR1_MSIZE_0; 		//存储器数据宽度8位
    DMA1_Channel2->CCR|=DMA_CCR1_PL_0; 		//中等优先级
    DMA1_Channel2->CCR|=DMA_CCR1_MEM2MEM; 		//非存储器到存储器模式		
    
    MY_NVIC_Init(0,1,DMA1_Channel2_3_IRQn,0);//组2，最低优先级 
    
    DMA1_Channel2->CCR |= DMA_CCR1_TCIE;
    dma2Flag = 0x0;
    
    DMA1_Channel2->CCR|=DMA_CCR1_EN;          //开启DMA传输
    while(1)
    {
        if(dma2Flag)
        {
            dma2Flag = 0x0;
            break;
        }
    }
    DMA1_Channel2->CCR&=~(DMA_CCR1_EN);       //关闭DMA传输 
    for(i=0;i<64;i++)
    {
        
        temp = *((volatile unsigned char *)((u32)T_Buffer+i));
        
        printf("temp%d=0x%x\r\n",i,temp);
    }
}

void dma_m8tom16_test()
{
    unsigned int i;
    unsigned int temp;
    for(i=0;i<64;i++)
    {
        *((volatile unsigned char *)((u32)R_Buffer+i)) = i +1;
    }
    RCC->AHBENR|=RCC_AHBENR_DMA1EN ;
    delay_ms(5);				//等待DMA时钟稳定
    DMA1_Channel2->CPAR=(u32)T_Buffer; 	 	//DMA1 外设地址 
    DMA1_Channel2->CMAR=(u32)R_Buffer; 	//DMA1,存储器地址
    DMA1_Channel2->CCR|=DMA_CCR1_DIR;
    DMA1_Channel2->CNDTR=64;
    DMA1_Channel2->CCR|=DMA_CCR1_PINC;
    DMA1_Channel2->CCR|=DMA_CCR1_MINC;
    DMA1_Channel2->CCR|=DMA_CCR1_PSIZE_0; 	 	//外设数据宽度为16位
    DMA1_Channel2->CCR&=~DMA_CCR1_MSIZE_0; 		//存储器数据宽度8位
    DMA1_Channel2->CCR|=DMA_CCR1_PL_0; 		//中等优先级
    DMA1_Channel2->CCR|=DMA_CCR1_MEM2MEM; 		//非存储器到存储器模式		
    
    MY_NVIC_Init(0,1,DMA1_Channel2_3_IRQn,0);//组2，最低优先级 
    
    DMA1_Channel2->CCR |= DMA_CCR1_TCIE;
    dma2Flag = 0x0;
    
    DMA1_Channel2->CCR|=DMA_CCR1_EN;          //开启DMA传输
    while(1)
    {
        if(dma2Flag)
        {
            dma2Flag = 0x0;
            break;
        }
    }
    DMA1_Channel2->CCR&=~(DMA_CCR1_EN);       //关闭DMA传输 
    for(i=0;i<128;i++)
    {
        
        temp = *((volatile unsigned char *)((u32)T_Buffer+i));
        
        printf("temp%d=0x%x\r\n",i,temp);
    }
}

void dma_m8tom32_test()
{
    unsigned int i;
    unsigned int temp;
    for(i=0;i<64;i++)
    {
        *((volatile unsigned char *)((u32)R_Buffer+i)) = i +1;
    }
    RCC->AHBENR|=RCC_AHBENR_DMA1EN ;
    delay_ms(5);				//等待DMA时钟稳定
    DMA1_Channel2->CPAR=(u32)T_Buffer; 	 	//DMA1 外设地址 
    DMA1_Channel2->CMAR=(u32)R_Buffer; 	//DMA1,存储器地址
    DMA1_Channel2->CCR|=DMA_CCR1_DIR;
    DMA1_Channel2->CNDTR=64;
    DMA1_Channel2->CCR|=DMA_CCR1_PINC;
    DMA1_Channel2->CCR|=DMA_CCR1_MINC;
    DMA1_Channel2->CCR|=DMA_CCR1_PSIZE_1; 	 	//外设数据宽度为32位
    DMA1_Channel2->CCR&=~DMA_CCR1_MSIZE_0; 		//存储器数据宽度8位
    DMA1_Channel2->CCR|=DMA_CCR1_PL_0; 		//中等优先级
    DMA1_Channel2->CCR|=DMA_CCR1_MEM2MEM; 		//非存储器到存储器模式		
    
    MY_NVIC_Init(0,1,DMA1_Channel2_3_IRQn,0);//组2，最低优先级 
    
    DMA1_Channel2->CCR |= DMA_CCR1_TCIE;
    dma2Flag = 0x0;
    
    DMA1_Channel2->CCR|=DMA_CCR1_EN;          //开启DMA传输
    while(1)
    {
        if(dma2Flag)
        {
            dma2Flag = 0x0;
            break;
        }
    }
    DMA1_Channel2->CCR&=~(DMA_CCR1_EN);       //关闭DMA传输 
    for(i=0;i<256;i++)
    {
        
        temp = *((volatile unsigned char *)((u32)T_Buffer+i));
        
        printf("temp%d=0x%x\r\n",i,temp);
    }
}


void DMA_m16tom8_test()
{
    unsigned int i;
    unsigned int temp;
    for(i=0;i<64;i++)
    {
        *((volatile unsigned short *)((u32)R_Buffer+(i*2))) = 0xb1b0+i;
        
    }
    RCC->AHBENR|=RCC_AHBENR_DMA1EN ;
    delay_ms(5);				//等待DMA时钟稳定
    DMA1_Channel2->CPAR=(u32)T_Buffer; 	 	//DMA1 外设地址 
    DMA1_Channel2->CMAR=(u32)R_Buffer; 	//DMA1,存储器地址
    DMA1_Channel2->CCR|=DMA_CCR1_DIR;
    DMA1_Channel2->CNDTR=64;
    DMA1_Channel2->CCR|=DMA_CCR1_PINC;
    DMA1_Channel2->CCR|=DMA_CCR1_MINC;
    DMA1_Channel2->CCR&=~DMA_CCR1_PSIZE_0; 	 	//外设数据宽度为8位
    DMA1_Channel2->CCR|=DMA_CCR1_MSIZE_0; 		//存储器数据宽16位
    DMA1_Channel2->CCR|=DMA_CCR1_PL_0; 		//中等优先级
    DMA1_Channel2->CCR|=DMA_CCR1_MEM2MEM; 		//非存储器到存储器模式		
    
    MY_NVIC_Init(0,1,DMA1_Channel2_3_IRQn,0);//组2，最低优先级 
    
    DMA1_Channel2->CCR |= DMA_CCR1_TCIE;
    dma2Flag = 0x0;
    
    DMA1_Channel2->CCR|=DMA_CCR1_EN;          //开启DMA传输
    while(1)
    {
        if(dma2Flag)
        {
            dma2Flag = 0x0;
            break;
        }
    }
    DMA1_Channel2->CCR&=~(DMA_CCR1_EN);       //关闭DMA传输 
    for(i=0;i<32;i++)
    {
        
        temp = *((volatile unsigned char *)((u32)T_Buffer+i));
        
        printf("temp%d=0x%x\r\n",i,temp);
    }
}

void DMA_m16tom16_test()
{
    unsigned int i;
    unsigned int temp;
    for(i=0;i<64;i++)
    {
        *((volatile unsigned short *)((u32)R_Buffer+(i*2))) = 0xa1a0+i;
        
    }
    RCC->AHBENR|=RCC_AHBENR_DMA1EN ;
    delay_ms(5);				//等待DMA时钟稳定
    DMA1_Channel2->CPAR=(u32)T_Buffer; 	 	//DMA1 外设地址 
    DMA1_Channel2->CMAR=(u32)R_Buffer; 	//DMA1,存储器地址
    DMA1_Channel2->CCR|=DMA_CCR1_DIR;
    DMA1_Channel2->CNDTR=64;
    DMA1_Channel2->CCR|=DMA_CCR1_PINC;
    DMA1_Channel2->CCR|=DMA_CCR1_MINC;
    DMA1_Channel2->CCR|=DMA_CCR1_PSIZE_0; 	 	//外设数据宽度为16位
    DMA1_Channel2->CCR|=DMA_CCR1_MSIZE_0; 		//存储器数据宽16位
    DMA1_Channel2->CCR|=DMA_CCR1_PL_0; 		//中等优先级
    DMA1_Channel2->CCR|=DMA_CCR1_MEM2MEM; 		//非存储器到存储器模式		
    
    MY_NVIC_Init(0,1,DMA1_Channel2_3_IRQn,0);//组2，最低优先级 
    
    DMA1_Channel2->CCR |= DMA_CCR1_TCIE;
    dma2Flag = 0x0;
    
    DMA1_Channel2->CCR|=DMA_CCR1_EN;          //开启DMA传输
    while(1)
    {
        if(dma2Flag)
        {
            dma2Flag = 0x0;
            break;
        }
    }
    DMA1_Channel2->CCR&=~(DMA_CCR1_EN);       //关闭DMA传输 
    for(i=0;i<64;i++)
    {
        
        temp = *((volatile unsigned short *)((u32)T_Buffer+(i*2)));
        
        printf("temp%d=0x%x\r\n",i,temp);
    }
}



void DMA_m16tom32_test()
{
    unsigned int i;
    unsigned int temp;
    for(i=0;i<64;i++)
    {
        *((volatile unsigned short *)((u32)R_Buffer+(i*2))) = 0xc1c0+i;
        
    }
    RCC->AHBENR|=RCC_AHBENR_DMA1EN ;
    delay_ms(5);				//等待DMA时钟稳定
    DMA1_Channel2->CPAR=(u32)T_Buffer; 	 	//DMA1 外设地址 
    DMA1_Channel2->CMAR=(u32)R_Buffer; 	//DMA1,存储器地址
    DMA1_Channel2->CCR|=DMA_CCR1_DIR;
    DMA1_Channel2->CNDTR=64;
    DMA1_Channel2->CCR|=DMA_CCR1_PINC;
    DMA1_Channel2->CCR|=DMA_CCR1_MINC;
    DMA1_Channel2->CCR|=DMA_CCR1_PSIZE_1; 	 	//外设数据宽度为16位
    DMA1_Channel2->CCR|=DMA_CCR1_MSIZE_0; 		//存储器数据宽16位
    DMA1_Channel2->CCR|=DMA_CCR1_PL_0; 		//中等优先级
    DMA1_Channel2->CCR|=DMA_CCR1_MEM2MEM; 		//非存储器到存储器模式		
    
    MY_NVIC_Init(0,1,DMA1_Channel2_3_IRQn,0);//组2，最低优先级 
    
    DMA1_Channel2->CCR |= DMA_CCR1_TCIE;
    dma2Flag = 0x0;
    
    DMA1_Channel2->CCR|=DMA_CCR1_EN;          //开启DMA传输
    while(1)
    {
        if(dma2Flag)
        {
            dma2Flag = 0x0;
            break;
        }
    }
    DMA1_Channel2->CCR&=~(DMA_CCR1_EN);       //关闭DMA传输 
    for(i=0;i<256;i++)
    {
        
        temp = *((volatile unsigned short *)((u32)T_Buffer+(i*2)));
        
        printf("temp%d=0x%x\r\n",i,temp);
    }
}























