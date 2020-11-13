#include "sys.h"
#include "uart_loop.h"
/********************************************************************************************************
**函数信息 UartInit_Loop(void)                      
**功能描述 ：初始化串口
**输入参数 ：无
**输出参数 ：无
********************************************************************************************************/
void UartInit_Loop(void)
{
    
    //GPIO端口设置	 
    RCC->AHBENR|=1<<17;   //使能GPIOA口时钟  
    RCC->APB2ENR|=1<<14;  //使能串口时钟 
    
    
    GPIOA->AFRH&=0xf00f;
    GPIOA->AFRH|=0x110;
    
    
    //UART 初始化设置
    UART1->BRR = (SystemCoreClock / 16) /(115200);
    UART1->FRA=(SystemCoreClock / 16) %(115200);
    UART1->CCR|=0x30; //110000
    UART1->GCR|=0x19;  //11001;
    UART1->GCR |= 0x0001;                    //使能串口1 
    
    GPIOA->CRH&=0XFFFFF00F;//IO状态设置
    GPIOA->CRH|=0X000008B0;//IO状态设置
    
    printf("UART OK!\r\n");
}


/********************************************************************************************************
**函数信息 ：uart_send(UART_TypeDef* UARTx,char c)                    
**功能描述 ：串口发送字节
**输入参数 ：UART_TypeDef* UARTx ，选择UART1、UART2、UART3
**输入参数 ：char c ,串口发送的字节
**输出参数 ：无
********************************************************************************************************/
void uart_send(char c)
{
    UART1->TDR = c;
    while(1)
    {
        if(UART1->ISR&0x1)
        {
            UART1->ICR = 0x1;
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
        if(UART1->ISR&0x2)
        {
            UART1->ICR = 0x2;
            break;
        }
    }
    temp = (uint8_t)UART1->RDR;
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





