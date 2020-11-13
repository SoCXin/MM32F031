#include "HAL_device.h"
#include "HAL_conf.h"
#include "stdio.h"


extern u32 SystemCoreClock;
static __IO uint32_t TimingDelay;
void delay_init(void);
void delay_ms(__IO uint32_t nTime);
void TimingDelay_Decrement(void);

void uart_initwBaudRate(u32 bound);
void UartSendGroup(u8 *buf, int len);

char printBuf[100];

/********************************************************************************************************
**函数信息 ：void WWDG_IRQHandler (void)
**功能描述 ：窗口看门狗中断函数
**输入参数 ：无
**输出参数 ：无
**    备注 ：当递减计数器等于0x40时进入中断
计数器参数须小于0x7f大于0x40,若接近于0x40可能会频繁进入此中断
********************************************************************************************************/
void WWDG_IRQHandler(void)
{
    if (WWDG_GetFlagStatus()) {
        WWDG_ClearFlag();
        WWDG_SetCounter(0x66);
    }
}

/********************************************************************************************************
**函数信息 ：Wwdg_reset_test()
**功能描述 ：使用窗口看门狗
**输入参数 ：unsigned char ucTcn 看门狗递减计数器值
unsigned char ucWcnt窗口值
**输出参数 ：无
**    备注 ：系统启动时看门狗为失能状态,看门狗失能后不能手动关闭,复位后看门狗失能
WWDG_SetCounter(0x7e);喂狗函数
WWDG_Enable(0x7e);看门狗使能并赋值计数器
窗口看门狗复位时间为微秒到毫秒级,适用短时间等待复位
WWDG counter clock = (PCLK1/4096)/x
********************************************************************************************************/
void Wwdg_reset_ON(unsigned char ucTcnt, unsigned char ucWcnt)
{
    /*失能窗口看门狗时钟*/
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_WWDG, ENABLE);

    /*WWDG_Prescaler_x,x为1,2,4,8,WWDG counter clock = (PCLK1/4096)/x */
    WWDG_SetPrescaler(WWDG_Prescaler_8);
    WWDG_SetWindowValue(ucWcnt);

    /*为看门狗自减计数器赋值,为7位数据且数值为0x40~0x7f之间*/
    WWDG_Enable(ucTcnt);
}

/********************************************************************************************************
**函数信息 ：void Wwdg_ExterslReloadReset_test(void)
**功能描述 ：立即复位
**输入参数 ：无
**输出参数 ：无
**    备注 ：可以看到此程序调用后程序立即复位,若需要一个即时的复位可考虑此函数,
配合看门狗中断使用避免系统持续复位,若打开中断时已完成设置,则可选
择屏蔽函数内配置项.
********************************************************************************************************/
void Wwdg_ExterslReloadReset_ON(void)
{
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_WWDG, ENABLE);                                                   //配置项
    WWDG_SetPrescaler(WWDG_Prescaler_8);                                                                   //配置项
    /*此处指小于下面值,且符合自身范围,即小于0x80*/
    WWDG_SetWindowValue(0x70);                                                                             //配置项
    /*此处指大于上面值,且符合自身范围,即小于0x7f大于0x40*/
    WWDG_Enable(0x75);
}

/********************************************************************************************************
**函数信息 ：Wwdg_irq_test()
**功能描述 ：窗口看门狗中断配置
**输入参数 ：无
**输出参数 ：无
********************************************************************************************************/
void Wwdg_irq_ON(void)
{
    NVIC_InitTypeDef NVIC_InitStructure;

    NVIC_InitStructure.NVIC_IRQChannel = WWDG_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_WWDG, ENABLE);
    WWDG_SetWindowValue(0x70);
    WWDG_EnableIT();
    /*此处为配置,参数值必须小于窗口值,否则立即产生复位*/
    WWDG_Enable(0x66);
}
/*******************************************************************************
* @name   : PVU_CheckStatus
* @brief  : Check Status
* @param  : None
* @retval : void
*******************************************************************************/
void PVU_CheckStatus(void)
{
    while (IWDG_GetFlagStatus(IWDG_FLAG_PVU));
}

/*******************************************************************************
* @name   : RVU_CheckStatus
* @brief  : Check Status
* @param  : None
* @retval : void
*******************************************************************************/
void RVU_CheckStatus(void)
{
    while (IWDG_GetFlagStatus(IWDG_FLAG_RVU));
}

/*******************************************************************************
* @name   : Write_Iwdg_ON
* @brief  : Write_Iwdg_ON
* @param  : None
* @retval : void
*******************************************************************************/
void Write_Iwdg_ON(u16 IWDG_Prescaler, u16 Reload)
{

    RCC_LSICmd(ENABLE);                                                         //enable LSI
    while (RCC_GetFlagStatus(RCC_FLAG_LSIRDY) == RESET);

    PVU_CheckStatus();                                                          //get IWDG status
    IWDG_WriteAccessCmd(0x5555);
    IWDG_SetPrescaler(IWDG_Prescaler);

    RVU_CheckStatus();                                                          //get IWDG status
    IWDG_WriteAccessCmd(0x5555);
    IWDG_SetReload(Reload & 0xfff);

    IWDG_ReloadCounter();                                                       //load and enable IWDG
    IWDG_Enable();
}

/*******************************************************************************
* @name   : Write_Iwdg_RL
* @brief  : Write_Iwdg_RL
* @param  : None
* @retval : void
*******************************************************************************/
void Write_Iwdg_RL(void)
{
    IWDG_ReloadCounter();
}



int WWDG_Reset_Test(void)
{
    UartSendGroup((u8 *)printBuf, sprintf(printBuf, "WWDG Reset Test!\r\n"));
    UartSendGroup((u8 *)printBuf, sprintf(printBuf, "1.RCC->CSR =0x%x\r\n", RCC->CSR));
    delay_ms(100);
    RCC->CSR |= 1 << 24;
    /*
    RCC_CSR
    24 RMVF rw 0 RMVF：清除复位标志 (Remove reset flag)
    由软件置‘1’来清除复位标志。
    0：无作用
    1：清除复位标志
    */
    delay_ms(100);
    UartSendGroup((u8 *)printBuf, sprintf(printBuf, "2.RCC->CSR =0x%x\r\n", RCC->CSR));
    delay_ms(2000);
    /*窗口看门狗打开,微秒至毫秒级复位,与喂狗函数WWDG_SetCounter()并用*/
    Wwdg_reset_ON(0x7e, 0x7f);                     //窗口看门狗FUN_2,短时内无喂狗动作复位
    while (1) {                                    //无限循环
        //窗口看门狗喂狗，则不复位
        //WWDG_SetCounter(0x7e);
        delay_ms(1);
    }
}
int IWDG_Reset_Test(void)
{
    UartSendGroup((u8 *)printBuf, sprintf(printBuf, "IWDG Reset Test!\r\n"));
    UartSendGroup((u8 *)printBuf, sprintf(printBuf, "1.RCC->CSR =0x%x\r\n", RCC->CSR));
    delay_ms(100);
    RCC->CSR |= 1 << 24;
    /*
    RCC_CSR
    24 RMVF rw 0 RMVF：清除复位标志 (Remove reset flag)
    由软件置‘1’来清除复位标志。
    0：无作用
    1：清除复位标志
    */
    delay_ms(100);
    UartSendGroup((u8 *)printBuf, sprintf(printBuf, "2.RCC->CSR =0x%x\r\n", RCC->CSR));
    delay_ms(2000);
    Write_Iwdg_ON(IWDG_Prescaler_256, 0xff);
    /* 独立看门狗打开 */
    while (1) {
        //独立看门狗喂狗，则不复位
        //Write_Iwdg_RL();
        delay_ms(1);
    }
}
int Soft_Reset_Test(void)
{
    UartSendGroup((u8 *)printBuf, sprintf(printBuf, "SOFT Reset Test!\r\n"));
    UartSendGroup((u8 *)printBuf, sprintf(printBuf, "1.RCC->CSR =0x%x\r\n", RCC->CSR));
    delay_ms(100);
    RCC->CSR |= 1 << 24;
    /*
    RCC_CSR
    24 RMVF rw 0 RMVF：清除复位标志 (Remove reset flag)
    由软件置‘1’来清除复位标志。
    0：无作用
    1：清除复位标志
    */
    delay_ms(100);
    UartSendGroup((u8 *)printBuf, sprintf(printBuf, "2.RCC->CSR =0x%x\r\n", RCC->CSR));
    delay_ms(2000);
    NVIC_SystemReset();
    while (1) {
        //运行不到此处，软件已经复位
        delay_ms(1);
    }
}
int PinAndPOR_Reset_Test(void)
{
    UartSendGroup((u8 *)printBuf, sprintf(printBuf, "Pin and POR Reset Test!\r\n"));
    UartSendGroup((u8 *)printBuf, sprintf(printBuf, "1.RCC->CSR =0x%x\r\n", RCC->CSR));
    delay_ms(100);
    RCC->CSR |= 1 << 24;
    /*
    RCC_CSR
    24 RMVF rw 0 RMVF：清除复位标志 (Remove reset flag)
    由软件置‘1’来清除复位标志。
    0：无作用
    1：清除复位标志
    */
    delay_ms(100);
    UartSendGroup((u8 *)printBuf, sprintf(printBuf, "2.RCC->CSR =0x%x\r\n", RCC->CSR));
    delay_ms(2000);

    while (1) {                                    //无限循环
        //等待按键或重新上电复位
        delay_ms(1);
    }
}
/********************************************************************************************************
**函数信息 ：int main (void)
**功能描述 ：开机后，ARMLED闪动
**输入参数 ：
**输出参数 ：
********************************************************************************************************/
int main(void)
{
    static u8 i = 0;
    //i=0;
    //i=1;
    i = 2;
    //i=3;
    delay_init();
    uart_initwBaudRate(115200);  //串口初始化为115200
    UartSendGroup((u8 *)printBuf, sprintf(printBuf, "uart ok!\r\n"));
    if (i == 1) {
        WWDG_Reset_Test();
    } else if (i == 2) {
        IWDG_Reset_Test();
    } else if (i == 3) {
        Soft_Reset_Test();
    } else {
        PinAndPOR_Reset_Test();
    }
    while (1) {
        delay_ms(1);
    }
}


/*******************************************************************************
**函数信息 ：void delay_init(void)
**功能描述 ：初始化延迟函数
**输入参数 ：
**输出参数 ：
*******************************************************************************/
void delay_init(void)
{
    if (SysTick_Config(SystemCoreClock / 1000)) {
        /* Capture error */
        while (1);
    }
    /* Configure the SysTick handler priority */
    NVIC_SetPriority(SysTick_IRQn, 0x0);//SysTick中断优先级设置
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

    while (TimingDelay != 0);
}

/********************************************************************************************************
**函数信息 ：uart_initwBaudRate(u32 bound)
**功能描述 ：UART初始化
**输入参数 ：bound
**输出参数 ：无
********************************************************************************************************/
void uart_initwBaudRate(u32 bound)
{
    //GPIO端口设置
    GPIO_InitTypeDef GPIO_InitStructure;
    UART_InitTypeDef UART_InitStructure;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_UART1, ENABLE);                       //使能UART1时钟
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);                         //开启GPIOA时钟
    //UART 初始化设置
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource9, GPIO_AF_1);
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource10, GPIO_AF_1);

    UART_InitStructure.UART_BaudRate = bound;                                   //串口波特率
    UART_InitStructure.UART_WordLength = UART_WordLength_8b;                    //字长为8位数据格式
    UART_InitStructure.UART_StopBits = UART_StopBits_1;                         //一个停止位
    UART_InitStructure.UART_Parity = UART_Parity_No;                            //无奇偶校验位
    UART_InitStructure.UART_HardwareFlowControl = UART_HardwareFlowControl_None;//无硬件数据流控制
    UART_InitStructure.UART_Mode = UART_Mode_Rx | UART_Mode_Tx;                 //收发模式

    UART_Init(UART1, &UART_InitStructure);                                      //初始化串口1
    UART_Cmd(UART1, ENABLE);                                                    //使能串口1

    //UART1_TX   GPIOA.9
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;                                   //PA.9
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;                             //复用推挽输出
    GPIO_Init(GPIOA, &GPIO_InitStructure);                                      //初始化GPIOA.9

    //UART1_RX    GPIOA.10初始化
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;                                  //PA10
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;                       //浮空输入
    GPIO_Init(GPIOA, &GPIO_InitStructure);                                      //初始化GPIOA.10

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
    UART_SendData(UART1, dat);
    while (!UART_GetFlagStatus(UART1, UART_FLAG_TXEPT));
}

/********************************************************************************************************
**函数信息 ：void UartSendGroup(u8* buf,u16 len)
**功能描述 ：UART发送数据
**输入参数 ：buf,len
**输出参数 ：
**    备注 ：
********************************************************************************************************/
void UartSendGroup(u8 *buf, int len)
{
    while (len--)
        UartSendByte(*buf++);
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

