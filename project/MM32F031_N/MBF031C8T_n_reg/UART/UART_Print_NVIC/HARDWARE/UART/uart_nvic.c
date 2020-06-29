#include "uart_nvic.h"
#include "sys.h"



#if EN_UART1_RX   //如果使能了接收
//串口1中断服务程序
//注意,读取UARTx->SR能避免莫名其妙的错误   	
u8 UART_RX_BUF[UART_REC_LEN];     //接收缓冲,最大UART_REC_LEN个字节.
//接收状态
//bit15，	接收完成标志
//bit14，	接收到0x0d
//bit13~0，	接收到的有效字节数目
u16 UART_RX_STA=0;       //接收状态标记	  

void uart_nvic_init(u32 bound)
{  	 
    
    u32 tempBaud;	
    
    //-------------------------------------------------
    RCC->AHBENR|=0x7<<17;   //使能GPIOA口时钟  
    RCC->APB2ENR|=1<<14;  //使能串口时钟 
    
    RCC->APB2RSTR|=1<<14;   //复位串口1
    RCC->APB2RSTR&=~(1<<14);//停止复位	 
    
    GPIOA->AFRH&=0x00f;
    GPIOA->AFRH|=0x110;
    
    //-------------------------------------------------	
    //波特率设置 
    /* Determine the uart_baud*/
    tempBaud = (SystemCoreClock / 16) /(bound);
    UART1->FRA=(SystemCoreClock / 16) %(bound);
    /* Write to UART BRR */
    UART1->BRR = tempBaud;
    UART1->CCR|=0X30;  //1位停止,无校验位.
    //-------------------------------------------------	
#if EN_UART1_RX		  //如果使能了接收
    //使能接收中断 
    UART1->GCR = 0X19;			//收发使能	UART1->CCR|=1<<5;    //接收缓冲区非空中断使能	
    UART1->IER = 0X2;			//接收中断使能
    /*
    23.5.5 UART 中断使能寄存器(UART_IER)
    偏移地址：0x10
    复位值：0x0000
    位1
    RXIEN:接收缓冲中断使能位
    1=中断使能
    0=中断禁止	
    */
    UART1->ICR = 0X2;			//清接收中断
    /*
    23.5.6 UART 中断清除寄存器(UART_ICR)
    偏移地址：0x14
    复位值：0x0000
    位1
    RXICLR: 接收中断清除位
    1=中断清除
    0=中断没有清除
    */
    MY_NVIC_Init(3,3,UART1_IRQn,2);//组2，最低优先级 
#endif
    /*
    位14
    UART1EN：UART1时钟使能
    由软件置’1’或清’0’
    0：UART1时钟关闭；
    1：UART1时钟开启。	
    */
    GPIOA->CRH&=0XFFFFF00F;//IO状态设置
    GPIOA->CRH|=0X000008B0;//IO状态设置
    
}
#endif	
#if 1
void UART1_IRQHandler(void)                	//串口1中断服务程序
{
    u8 Res;
    if((UART1->ISR & 0x2) != (uint16_t)RESET)	//接收到数据
    {	 
        Res=(uint16_t)(UART1->RDR & (uint16_t)0x00FF);
        UART1->ICR |= 2;//清接收中断 //		UART_ClearITPendingBit(UART1,UART_IT_RXIEN);
        if((Res==0x0d)&&((UART_RX_STA&0X3FFF)>0))
        {
            UART_RX_STA|=0x4000;
            UART_RX_BUF[UART_RX_STA&0X3FFF]=Res ;
            UART_RX_STA++;
        }
        else if((UART_RX_STA&0x4000)&&((UART_RX_STA&0X3FFF)>0))//接收到了0x0d
        {
            if(Res==0x0a)
            {
                UART_RX_STA|=0x8000;				
            }
            UART_RX_BUF[UART_RX_STA&0X3FFF]=Res ;
            UART_RX_STA++;
        }
        else{
            UART_RX_BUF[UART_RX_STA&0X3FFF]=Res ;
            UART_RX_STA++;
            UART_RX_STA=UART_RX_STA&0X3FFF;
            if((UART_RX_STA)>(UART_REC_LEN-1))
                UART_RX_STA=0;//接收数据错误,重新开始接收	
        }
    } 
} 
#endif	

