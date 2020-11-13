#include "delay.h"
#include "sys.h"
#include "uart.h"
#include "dma.h"

u32 TEXT_TO_SEND[] = {'1','2','3','4','5','6','7','8','9'};
#define TEXT_LENTH 9
u8 SendBuff[(TEXT_LENTH+2)*100];

int main(void)
{
    u16 i;
    u8 t=0; 
    
    delay_init();
    uart_initwBaudRate(115200);	 //串口初始化为115200
    MYDMA_Config(DMA1_Channel2,(u32)&UART1->TDR,(u32)SendBuff,(TEXT_LENTH+2)*10);//DMA1通道2,外设为串口1,存储器为SendBuff,长(TEXT_LENTH+2)*100.
    printf("\r\nDMA SEND DATA:\r\n"); 	  
    for(i=0;i<(TEXT_LENTH+2)*100;i++)//填充ASCII字符集数据
    {
        if(t>=TEXT_LENTH)//加入换行符
        { 
            SendBuff[i++]=0x0d; 
            SendBuff[i]=0x0a; 
            t=0;
        }else SendBuff[i]=TEXT_TO_SEND[t++];//复制TEXT_TO_SEND语句    
    }		 
    
    UART1->GCR |= 0x0002;
    DMA1_Channel2->CCR |= 0x00000001;  
    //等待DMA传输完成，此时我们来做另外一些事，点灯
    //实际应用中，传输数据期间，可以执行另外的任务
    while(1)
    {
        if(DMA1->ISR&0x00000020)//等待通道2传输完成
        {
            DMA1->IFCR = 0x00000020;
            break; 
        }
    }
    
    printf("\r\nUART DMA TEST OK!\r\n "); 	    
    
    while(1);
    
}



