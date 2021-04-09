/**
******************************************************************************
* @file     main.c
* @author   AE team
* @version  V1.1.1
* @date     15/05/2019
* @brief
******************************************************************************
*/

#include "HAL_device.h"
#include "HAL_conf.h"
#include "stdio.h"

void uart_nvic_init(u32 bound);
void delay_init(void);
void delay_ms(__IO uint32_t nTime);
void TimingDelay_Decrement(void);
void LED_Init(void);
void UartSendGroup(u8* buf, u16 len);
void UartSendByte(u8 dat);
#define UART_REC_LEN            200                                             //定义最大接收字节数 200
#define EN_UART1_RX             1                                               //使能（1）/禁止（0）串口1接收

extern u32 SystemCoreClock;
static __IO uint32_t TimingDelay;
u8 UART_RX_BUF[UART_REC_LEN];     //接收缓冲,最大UART_REC_LEN个字节.
u16 UART_RX_STA = 0;              //接收状态标记
char printBuf[100];




//53
#define LED1_PORT           (GPIOB)
#define LED1_PIN            (GPIO_Pin_7)
#define LED2_PORT           (GPIOB)
#define LED2_PIN            (GPIO_Pin_8)
#define RS_DIR_PORT         (GPIOA)
#define RS_DIR_PIN          (GPIO_Pin_11)
#define RS485_R             GPIO_ResetBits(RS_DIR_PORT,RS_DIR_PIN)
#define RS485_W             GPIO_SetBits(RS_DIR_PORT,RS_DIR_PIN)

//84
// #define LED1_PORT                   (GPIOB)
// #define LED1_PIN                    (GPIO_Pin_3)
// #define LED2_PORT                   (GPIOA)
// #define LED2_PIN                    (GPIO_Pin_15)

// #define RS_DIR_PORT         (GPIOB)
// #define RS_DIR_PIN          (GPIO_Pin_11)
// #define RS485_W             GPIO_ResetBits(RS_DIR_PORT,RS_DIR_PIN)
// #define RS485_R             GPIO_SetBits(RS_DIR_PORT,RS_DIR_PIN)


#define LED4_ON()       GPIO_ResetBits(GPIOA,GPIO_Pin_15)	// PA15
#define LED4_OFF()      GPIO_SetBits(GPIOA,GPIO_Pin_15)	// PA15
#define LED4_TOGGLE()   (GPIO_ReadOutputDataBit(GPIOA,GPIO_Pin_15))?(GPIO_ResetBits(GPIOA,GPIO_Pin_15)):(GPIO_SetBits(GPIOA,GPIO_Pin_15)) // PA15

#define LED3_ON()       GPIO_ResetBits(GPIOB,GPIO_Pin_3)	// PB3
#define LED3_OFF()      GPIO_SetBits(GPIOB,GPIO_Pin_3)	// PB3
#define LED3_TOGGLE()   (GPIO_ReadOutputDataBit(GPIOB,GPIO_Pin_3))?(GPIO_ResetBits(GPIOB,GPIO_Pin_3)):(GPIO_SetBits(GPIOB,GPIO_Pin_3))	// PB3

#define LED2_ON()       GPIO_ResetBits(LED2_PORT,LED2_PIN)	// PB4
#define LED2_OFF()      GPIO_SetBits(LED2_PORT,LED2_PIN)	// PB4
#define LED2_TOGGLE()   (GPIO_ReadOutputDataBit(LED2_PORT,LED2_PIN))?(GPIO_ResetBits(LED2_PORT,LED2_PIN)):(GPIO_SetBits(LED2_PORT,LED2_PIN))	// PB4

#define LED1_ON()       GPIO_ResetBits(LED1_PORT,LED1_PIN)	// PB5
#define LED1_OFF()      GPIO_SetBits(LED1_PORT,LED1_PIN)	// PB5
#define LED1_TOGGLE()   (GPIO_ReadOutputDataBit(LED1_PORT,LED1_PIN))?(GPIO_ResetBits(LED1_PORT,LED1_PIN)):(GPIO_SetBits(LED1_PORT,LED1_PIN))	// PB5

/********************************************************************************************************
**函数信息 ：main(void)
**功能描述 ：
**输入参数 ：无
**输出参数 ：无
********************************************************************************************************/
int main(void)
{
    u8 t;
    u8 len;
    u16 times = 0;
    delay_init();
    LED_Init();
    uart_nvic_init(9600);
    LED2_OFF();
    LED1_ON();
    while(1)
    {
        if(UART_RX_STA & 0x8000)
        {
            LED1_OFF();
            LED2_ON();
            len = UART_RX_STA & 0x3fff;                                         //得到此次接收到的数据长度
            UartSendGroup((u8*)printBuf, sprintf(printBuf, "\r\n"));
            for(t = 0; t < len; t++)
            {
                while((UART1->CSR & UART_IT_TXIEN) == 0);                       //等待发送结束
                UART1->TDR = UART_RX_BUF[t];
            }
            UartSendGroup((u8*)printBuf, sprintf(printBuf, "\r\n"));        //插入换行
            UartSendByte(UART_RX_STA>>8);
            UartSendByte(UART_RX_STA & 0xff);
            UartSendByte(len);
            UART_RX_STA = 0;
        }
        else
        {
            times++;
            if(times % 3000 == 0)
            {
                LED2_OFF();
                LED1_ON();
                UartSendGroup((u8*)printBuf, sprintf(printBuf, "\r\n串口实验\r\n"));
            }
            // if(times % 200 == 0) UartSendGroup((u8*)printBuf, sprintf(printBuf, "请输入数据,以回车键结束\r\n"));
            // if(times % 30 == 0)
            // {
            //     LED1_TOGGLE();
            // }
            delay_ms(10);
        }
    }
}

/********************************************************************************************************
**函数信息 ：delay_init(void)
**功能描述 ：systick延时函数初始化
**输入参数 ：无
**输出参数 ：无
********************************************************************************************************/
void delay_init(void)
{
    if (SysTick_Config(SystemCoreClock / 1000))
    {
        /* Capture error */
        while (1);
    }
    /* Configure the SysTick handler priority */
    NVIC_SetPriority(SysTick_IRQn, 0x0);                                        //SysTick中断优先级设置
}

/********************************************************************************************************
**函数信息 ：SysTick_Handler(void)
**功能描述 ：进入该中断函数后，Systick进行递减
**输入参数 ：无
**输出参数 ：无
********************************************************************************************************/
void SysTick_Handler(void)
{
    TimingDelay_Decrement();
}

/********************************************************************************************************
**函数信息 ：TimingDelay_Decrement(void)
**功能描述 ：以1ms的速度递减
**输入参数 ：pclk2，例如系统时钟为8MHz，参数应传入8
**输出参数 ：无
********************************************************************************************************/
void TimingDelay_Decrement(void)
{
    if (TimingDelay != 0x00) {
        TimingDelay--;
    }
}

/********************************************************************************************************
**函数信息 ：delay_ms(__IO uint32_t nTime)
**功能描述 ：程序应用调用延时，使用systick
**输入参数 ：nTime：延时
**输出参数 ：无
********************************************************************************************************/
void delay_ms(__IO uint32_t nTime)
{
    TimingDelay = nTime;

    while(TimingDelay != 0);
}

/********************************************************************************************************
**函数信息 ：LED_Init(void)
**功能描述 ：LED初始化
**输入参数 ：无
**输出参数 ：无
********************************************************************************************************/
void LED_Init(void)
{

    GPIO_InitTypeDef  GPIO_InitStructure;

    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA | RCC_AHBPeriph_GPIOB, ENABLE);   //开启GPIOA,GPIOB时钟

    GPIO_InitStructure.GPIO_Pin  = LED2_PIN ;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(LED2_PORT, &GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin  = LED1_PIN  ;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(LED1_PORT, &GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin  = RS_DIR_PIN  ;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(RS_DIR_PORT, &GPIO_InitStructure);
    LED1_ON();
    LED2_ON();
    RS485_W;
    // LED3_ON();
    // LED4_ON();
}

/********************************************************************************************************
**函数信息 ：void uart_nvic_init(u32 bound)
**功能描述 ：UART端口、中断初始化
**输入参数 ：bound
**输出参数 ：
**    备注 ：
********************************************************************************************************/
void uart_nvic_init(u32 bound)
{
    //GPIO端口设置
    GPIO_InitTypeDef GPIO_InitStructure;
    UART_InitTypeDef UART_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_UART1, ENABLE);                       //使能UART1
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);                         //开启GPIOA时钟

    //UART1 NVIC 配置
    NVIC_InitStructure.NVIC_IRQChannel = UART1_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPriority = 3;                             //子优先级3
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;                             //IRQ通道使能
    NVIC_Init(&NVIC_InitStructure);                                             //根据指定的参数初始化VIC寄存器

    //UART 初始化设置
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource9, GPIO_AF_1);
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource10, GPIO_AF_1);

    UART_InitStructure.UART_BaudRate = bound;                                   //串口波特率
    UART_InitStructure.UART_WordLength = UART_WordLength_8b;                    //字长为8位数据格式
    UART_InitStructure.UART_StopBits = UART_StopBits_1;                         //一个停止位
    UART_InitStructure.UART_Parity = UART_Parity_No;                            //无奇偶校验位
    UART_InitStructure.UART_HardwareFlowControl = UART_HardwareFlowControl_None;//无硬件数据流控制
    UART_InitStructure.UART_Mode = UART_Mode_Rx | UART_Mode_Tx;	                //收发模式

    UART_Init(UART1, &UART_InitStructure);                                      //初始化串口1
    UART_ITConfig(UART1, UART_IT_RXIEN, ENABLE);                                //开启串口接受中断
    UART_Cmd(UART1, ENABLE);                                                    //使能串口1

    //UART1_TX   GPIOA.9
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;                                   //PA.9
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;                             //复用推挽输出
    GPIO_Init(GPIOA, &GPIO_InitStructure);                                      //初始化GPIOA.9

    //UART1_RX	  GPIOA.10初始化
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;                                  //PA10
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;                       //浮空输入
    GPIO_Init(GPIOA, &GPIO_InitStructure);                                      //初始化GPIOA.10

}

/********************************************************************************************************
**函数信息 ：void UART1_IRQHandler(void)
**功能描述 ：串口1中断服务程序
**输入参数 ：
**输出参数 ：
**    备注 ：
********************************************************************************************************/
void UART1_IRQHandler(void)
{
    u8 Res;
    if(UART_GetITStatus(UART1, UART_IT_RXIEN)  != RESET) {                      //接收中断(接收到的数据必须是0x0d 0x0a结尾)
        UART_ClearITPendingBit(UART1, UART_IT_RXIEN);
        Res = UART_ReceiveData(UART1);	                                       //读取接收到的数据
        if((Res == 0x0d) && ((UART_RX_STA & 0X3FFF) > 0)) {
            UART_RX_STA |= 0x4000;
            UART_RX_BUF[UART_RX_STA & 0X3FFF] = Res ;
            UART_RX_STA++;
        } else if((UART_RX_STA & 0x4000) && ((UART_RX_STA & 0X3FFF) > 0)) {     //接收到了0x0d
            if(Res == 0x0a) {
                UART_RX_STA |= 0x8000;
            }
            UART_RX_BUF[UART_RX_STA & 0X3FFF] = Res ;
            UART_RX_STA++;
        } else {
            UART_RX_BUF[UART_RX_STA & 0X3FFF] = Res ;
            UART_RX_STA++;
            UART_RX_STA = UART_RX_STA & 0X3FFF;
            if((UART_RX_STA) > (UART_REC_LEN - 1))
                UART_RX_STA = 0;                                                //接收数据错误,重新开始接收
        }
    }
}

/********************************************************************************************************
**函数信息 ：void UartSendByte(u8 dat)
**功能描述 ：UART发送数据
**输入参数 ：dat
**输出参数 ：
**    备注 ：
********************************************************************************************************/
void UartSendByte(u8 dat)
{
    #ifdef RS485_W
    RS485_W;
    #endif
    UART_SendData(UART1, dat);
    while(!UART_GetFlagStatus(UART1, UART_FLAG_TXEPT));
		#ifdef RS485_R
    RS485_R;
    #endif
}

/********************************************************************************************************
**函数信息 ：void UartSendGroup(u8* buf,u16 len)
**功能描述 ：UART发送数据
**输入参数 ：buf,len
**输出参数 ：
**    备注 ：
********************************************************************************************************/
void UartSendGroup(u8* buf, u16 len)
{
    #ifdef RS485_W
    RS485_W;
    #endif
    while(len--) UartSendByte(*buf++);
    #ifdef RS485_R
    delay_ms(2);
    RS485_R;
    #endif
}

/**
* @}
*/

/**
* @}
*/

/**
* @}
*/

/*-------------------------(C) COPYRIGHT 2019 MindMotion ----------------------*/

