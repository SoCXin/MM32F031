#include "delay.h"
#include "sys.h"
#include "uart.h"
#include "led.h"

u8 DST_Buffer[10]= {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};//UART DMA接受到数据存放数组
/* Private function prototypes -----------------------------------------------*/
void DMA_Configuration(void);

/********************************************************************************************************
**函数信息 ：int main (void)                          
**功能描述 ：开机后，使用串口助手发送10个数字，注意串口助手不能选择发送新行
**输入参数 ：
**输出参数 ：
********************************************************************************************************/

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
        for(i=0;i<10;i++)
        {
            DST_Buffer[i]=0;//接受数组清零
        }
        while(!DMA_GetFlagStatus(DMA1_FLAG_TC3));//等待DMA传输完成
        DMA_ClearFlag(DMA1_FLAG_TC3);//清除DMA传输完成标志位
        for(i=0;i<10;i++)
        {
            printf("DST_Buffer[%d]==%d\r\n",i,(DST_Buffer[i]-0x30));
        }
        printf("\r\n");
    }
    
    
}


/********************************************************************************************************
**函数信息 ：DMA_Configuration                         
**功能描述 ：UART接收DMA配置
**输入参数 ：无
**输出参数 ：无
********************************************************************************************************/
void DMA_Configuration(void)
{
    DMA_InitTypeDef  DMA_InitStructure;
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
    DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;//循环模式，上位机可以不断输入数据
    DMA_InitStructure.DMA_Priority = DMA_Priority_Low;
    DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
    DMA_Init(DMA1_Channel3, &DMA_InitStructure);
    
    UART_DMACmd(UART1,UART_DMAReq_EN,ENABLE); //开启UART1 DMA   
    /* UARTy_DMA1_Channel enable */
    DMA_Cmd(DMA1_Channel3, ENABLE);
}



