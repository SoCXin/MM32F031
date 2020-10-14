#include "delay.h"
#include "sys.h"
#include "uart.h"
#include "led.h"
#include "dma.h"

u8 DST_Buffer[10]= {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};//UART DMA接受到数据存放数
u8 TestStatus=0;//DMA传输完成标志位
/* Private function prototypes -----------------------------------------------*/
void DMA_Configuration(void);

int main(void)
{
    //注意：串口调试助手不能勾选发送新行
    u8 i;
    
    delay_init();
    LED_Init();		  	 	//初始化与LED连接的硬件接口
    uart_initwBaudRate(115200);	 	//串口初始化为115200 
    DMA_Configuration();//UART DMA配置
    printf("请输入10个数字!\r\n");
    printf("串口助手不能勾选发送新行!\r\n");
    while(1)
    {
        if(TestStatus == 1)
        {
            TestStatus=0;
            for(i=0;i<10;i++)
                printf("DST_Buffer[%d]==%d\r\n",i,(DST_Buffer[i]-0x30));
            printf("\r\n");
        }
    }
}


/********************************************************************************************************
**函数信息 ：DMA1_Channel2_3_IRQHandler(void)                        
**功能描述 ：DMA中断函数
**输入参数 ：无
**输出参数 ：无
********************************************************************************************************/
void DMA1_Channel2_3_IRQHandler(void)
{
    if(DMA_GetITStatus(DMA1_IT_TC3))             
    {
        DMA_ClearITPendingBit(DMA1_IT_GL3);
        /* Check the received buffer */
        TestStatus = 1;
    }
}

/********************************************************************************************************
**函数信息 ：DMA_Configuration(void)                          
**功能描述 ：UART接收DMA配置
**输入参数 ：无
**输出参数 ：无
********************************************************************************************************/
void DMA_Configuration(void)
{
    DMA_InitTypeDef  DMA_InitStructure;
    NVIC_InitTypeDef  NVIC_InitStructure;
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
    /* UARTy_DMA1_Channel Config */
    DMA_DeInit(DMA1_Channel3);
    DMA_InitStructure.DMA_PeripheralBaseAddr =(u32) &(UART1->RDR);
    DMA_InitStructure.DMA_MemoryBaseAddr = (u32)DST_Buffer;
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
    DMA_InitStructure.DMA_BufferSize = 10;
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
    DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
    DMA_InitStructure.DMA_Priority = DMA_Priority_Low;
    DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
    DMA_Init(DMA1_Channel3, &DMA_InitStructure);
    
    SCB->AIRCR = 0x05FA0000 | 0x600;
    
    
    /* Enable the UARTy_DMA1_IRQn Interrupt */
    NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel2_3_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
    /* Enable UARTy_DMA1_Channel Transfer complete interrupt */
    DMA_ITConfig(DMA1_Channel3, DMA_IT_TC, ENABLE);             //配DMA1的通道3 允许传输完成中断
    
    UART_DMACmd(UART1,UART_DMAReq_EN,ENABLE);                //DMA方式选择位 UART->GCR 
    /* UARTy_DMA1_Channel enable */
    DMA_Cmd(DMA1_Channel3, ENABLE);
}
