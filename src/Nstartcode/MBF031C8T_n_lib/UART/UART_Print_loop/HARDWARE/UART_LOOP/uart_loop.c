#include "HAL_conf.h"
#include  "stdio.h"
#include "dtype.h"

/********************************************************************************************************
**函数信息 ：UartInit_Loop(void)                      
**功能描述 ：初始化串口
**输入参数 ：无
**输出参数 ：无
********************************************************************************************************/
void UartInit_Loop(void)
{
    
    //GPIO端口设置
    GPIO_InitTypeDef GPIO_InitStructure;
    UART_InitTypeDef UART_InitStructure;
    
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_UART1, ENABLE);	//使能UART1
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);  //开启GPIOA时钟
    
    //UART1 NVIC 配置
    
    //UART 初始化设置
    GPIO_PinAFConfig(GPIOA,GPIO_PinSource9,GPIO_AF_1);
    GPIO_PinAFConfig(GPIOA,GPIO_PinSource10,GPIO_AF_1);
    
    
    UART_InitStructure.UART_BaudRate = 115200;//串口波特率
    UART_InitStructure.UART_WordLength = UART_WordLength_8b;//字长为8位数据格式
    UART_InitStructure.UART_StopBits = UART_StopBits_1;//一个停止位
    UART_InitStructure.UART_Parity = UART_Parity_No;//无奇偶校验位
    UART_InitStructure.UART_HardwareFlowControl = UART_HardwareFlowControl_None;//无硬件数据流控制
    UART_InitStructure.UART_Mode = UART_Mode_Rx | UART_Mode_Tx;	//收发模式
    
    UART_Init(UART1, &UART_InitStructure); //初始化串口1
    UART_Cmd(UART1, ENABLE);                    //使能串口1 
    
    //UART1_TX   GPIOA.9
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9; //PA.9
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//复用推挽输出
    GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化GPIOA.9
    
    //UART1_RX	  GPIOA.10初始化
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;//PA10
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//浮空输入
    GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化GPIOA.10  
    
    printf("UART OK!\r\n");
}


/********************************************************************************************************
**函数信息 ：uart_send(UART_TypeDef* UARTx,char c)                    
**功能描述 ：串口发送字节
**输入参数 ：UART_TypeDef* UARTx ，选择UART1、UART2、UART3
**输入参数 ：char c ,串口发送的字节
**输出参数 ：无
********************************************************************************************************/
void uart_send(UART_TypeDef* UARTx,char c)
{
    UART_SendData(UARTx,(uint16_t)c);  
    while(1)
    {
        if(UART_GetITStatus(UARTx, UART_IT_TXIEN))
        {
            UART_ClearITPendingBit(UARTx, UART_IT_TXIEN);
            break;
        }
    }
}

/********************************************************************************************************
**函数信息 ：unsigned char inbyte(UART_TypeDef* UARTx)                
**功能描述 ：串口接收函数
**输入参数 ：UART_TypeDef* UARTx ，选择UART1、UART2、UART3
**输出参数 ：unsigned char 串口接收返回的字节
********************************************************************************************************/
unsigned char inbyte(UART_TypeDef* UARTx)
{
    unsigned char temp;
    
    while(1)
    {
        if(UART_GetITStatus(UARTx, UART_IT_RXIEN))
        {
            UART_ClearITPendingBit(UARTx, UART_IT_RXIEN);  //清除接受中断位
            break;
        }
    }
    temp = (uint8_t)UART_ReceiveData(UARTx);
    if(temp==0xd)//清除错误接收的数据
    {
        return 0;		
    }
    return temp;
}


/********************************************************************************************************
**函数信息 ：void Uart1RxTest(UART_TypeDef* UARTx)               
**功能描述 ：串口接收函数测试
**输入参数 ：UART_TypeDef* UARTx ，选择UART1、UART2、UART3
**输出参数 ：无
********************************************************************************************************/

void Uart1RxTest(UART_TypeDef* UARTx)
{
    unsigned char temp;
    temp = inbyte(UARTx);
    if(temp!=0)
    {
        printf("您输入的数据为:%c\r\n",temp);	
    }			
}





