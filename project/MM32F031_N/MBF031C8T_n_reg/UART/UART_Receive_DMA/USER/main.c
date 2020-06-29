#include "sys.h"
#include "uart.h"		
#include "delay.h"	
#include "dma.h" 

u8 DST_Buffer[10]= {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};//UART DMA接受到数据存放数组


int main(void)
{	
    u16 i;
    
    delay_init();
    uart_initwBaudRate(115200);	 	//串口初始化为115200		 	
    //显示提示信息	   
    printf("请输入10个数字!\r\n");
    printf("串口助手不能勾选发送新行!\r\n");
    MYDMA_Config(DMA1_Channel3,(u32)&UART1->RDR,(u32)DST_Buffer,10);//DMA1通道4,外设为串口1,存储器为SendBuff,长(TEXT_LENTH+2)*100.
    
    UART1->GCR|=1<<1;           //选择串口1的DMA方式发送  
    UART1->GCR|=1<<3;           //使能串口1的   接收 
    MYDMA_Enable(DMA1_Channel3);//开始一次DMA传输！	  
    while(1)
    {
        if(DMA1->ISR&(1<<9))//等待通道4传输完成
        {
            DMA1->IFCR|=1<<9;//清除通道4传输完成标志
            break; 
        }
    }
    for(i=0;i<10;i++)
    {
        printf("DST_Buffer[%d]==%d\r\n",i,(DST_Buffer[i]-0x30));
    }
    printf("\r\nUART DMA TEST OK!\r\n "); 	    
    
    while(1);
    
}


