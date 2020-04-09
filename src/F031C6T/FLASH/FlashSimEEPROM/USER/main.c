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

extern u32 SystemCoreClock;
void delay_init(void);
void delay_ms(__IO uint32_t nTime);
void TimingDelay_Decrement(void);
void LED_Init(void);

u8 Modbus_write_word(void);

static __IO uint32_t TimingDelay;
#define BANK1_WRITE_START_ADDR  ((uint32_t)0x08002800)
uint32_t Data = 0x12345679;

volatile FLASH_Status FLASHStatus = FLASH_COMPLETE;

#define LED4_ON()  GPIO_ResetBits(GPIOA,GPIO_Pin_15)	// PA15
#define LED4_OFF()  GPIO_SetBits(GPIOA,GPIO_Pin_15)		// PA15
#define LED4_TOGGLE()  (GPIO_ReadOutputDataBit(GPIOA,GPIO_Pin_15))?(GPIO_ResetBits(GPIOA,GPIO_Pin_15)):(GPIO_SetBits(GPIOA,GPIO_Pin_15)) // PA15

#define LED3_ON()  GPIO_ResetBits(GPIOB,GPIO_Pin_3)		// PB3
#define LED3_OFF()  GPIO_SetBits(GPIOB,GPIO_Pin_3)		// PB3
#define LED3_TOGGLE()  (GPIO_ReadOutputDataBit(GPIOB,GPIO_Pin_3))?(GPIO_ResetBits(GPIOB,GPIO_Pin_3)):(GPIO_SetBits(GPIOB,GPIO_Pin_3))	// PB3

#define LED2_ON()  GPIO_ResetBits(GPIOB,GPIO_Pin_4)		// PB4
#define LED2_OFF()  GPIO_SetBits(GPIOB,GPIO_Pin_4)		// PB4
#define LED2_TOGGLE()  (GPIO_ReadOutputDataBit(GPIOB,GPIO_Pin_4))?(GPIO_ResetBits(GPIOB,GPIO_Pin_4)):(GPIO_SetBits(GPIOB,GPIO_Pin_4))	// PB4

#define LED1_ON()  GPIO_ResetBits(GPIOB,GPIO_Pin_5)		// PB5
#define LED1_OFF()  GPIO_SetBits(GPIOB,GPIO_Pin_5)		// PB5
#define LED1_TOGGLE()  (GPIO_ReadOutputDataBit(GPIOB,GPIO_Pin_5))?(GPIO_ResetBits(GPIOB,GPIO_Pin_5)):(GPIO_SetBits(GPIOB,GPIO_Pin_5))	// PB5

void uart1_init(u32 bound)
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
////////////////////////////////////////////////////////////////////////////////
/// @brief  Erases a specified FLASH page.
/// @note   This function can be used for all MM32 devices.
/// @param  Page_Address: The page address to be erased.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void exFLASH_EraseEE(u32 pageAddress)
{
    FLASH_Unlock();
    FLASH_ErasePage(pageAddress);
    FLASH_Lock();
}
////////////////////////////////////////////////////////////////////////////////
/// @brief  Programs a buffer at a specified address.
/// @note   This function can be used for all MM32 devices.
/// @param *buf: the pointer of the buffer to be programmed.
/// @param  addr: specifies the address to be programmed.
/// @param  len: the number of bytes in the buffer.
///         This parameter can only be even.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void exFLASH_ProgramEE(u16* buf, u32 addr, u16 len)
{
    FLASH_Unlock();
    for (u16 i = 0; i < len / 2; i++) {
        FLASH_ProgramHalfWord(addr, *buf);
        addr += 2;
        buf++;
    }
    FLASH_Lock();
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  Determine if the data that at the ptr address with the length is len
///         is empty.
/// @note   This function can be used for all MM32 devices.
/// @param *ptr: the pointer of the starting address.
/// @param  len: the number of bytes.
///         This parameter can only be even.
/// @retval True presents the data is empty,
///         false presents the data has been written.
////////////////////////////////////////////////////////////////////////////////
u8 exFLASH_FindEmpty(u16* ptr, u16 len)
{
    for (u16 i = 0; i < (len / 2); i++) {
        if (*(ptr + i) != 0xffff)
            return FALSE;
    }
    return TRUE;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  Locate the writable area on the specified address.
/// @note   This function can be used for all MM32 devices.
/// @param  pageAddress: specifies the beginning of the EEprom.
///         The EEprom can be some continuously pages in the flash.
/// @param  len: the number of bytes to be written.
///         This parameter can only be even.
/// @retval the pointer of the starting address.
////////////////////////////////////////////////////////////////////////////////
void* exFLASH_Locate(u32 pageAddress, u16 len)
{
    u16* ptr = (u16*)pageAddress;
    for (u16 i = 0; i < (0x0800 / len); i++) {
        if (exFLASH_FindEmpty(ptr, len)) {
            if (i == 0)
                return 0;
            break;
        }
        ptr += len / 2;
    }
    return ptr;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  Programs a buffer at a specified address.
/// @note   This function can be used for all MM32 devices.
/// @param *buf: the pointer of the buffer to be programmed.
/// @param  pageAddress: specifies the beginning of the EEprom.
///         The EEprom can be some continuously pages in the flash.
/// @param  len: the number of bytes in the buffer.
///         This parameter can only be even.
/// @retval None.
////////////////////////////////////////////////////////////////////////////////
void exFLASH_WriteEE(u16* buf, u32 pageAddress, u16 len)
{
    u16* ptr = exFLASH_Locate(pageAddress, len);
    if (ptr == 0) {
        exFLASH_EraseEE(pageAddress + 0x000);
        exFLASH_EraseEE(pageAddress + 0x400);
        exFLASH_ProgramEE(buf, pageAddress, len);
    } else {
        if (ptr == (u16*)(pageAddress + ((0x0400 / len) - 1) * len)) {
            exFLASH_EraseEE(pageAddress + 0x400);
            exFLASH_ProgramEE(buf, (u32)ptr, len);
        } else if (ptr == (u16*)(pageAddress + 0x0800)) {
            exFLASH_EraseEE(pageAddress + 0x000);
            exFLASH_ProgramEE(buf, (u32)pageAddress, len);
        } else {
            exFLASH_ProgramEE(buf, (u32)ptr, len);
        }
    }
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  Read the beginning address of the last written data.
/// @note   This function can be used for all MM32 devices.
/// @param  pageAddress: specifies the beginning of the EEprom.
///         The EEprom can be some continuously pages in the flash.
/// @param  len: the number of bytes have been written.
///         This parameter can only be even.
/// @retval the beginning address of the last written data.
///         0 presents that this is the first time to use this as EEprom.
////////////////////////////////////////////////////////////////////////////////
void* exFLASH_ReadEE(u32 pageAddress, u16 len)
{
    u16* ptr = exFLASH_Locate(pageAddress, len);
    return (ptr == 0) ? 0 : (ptr - len / 2);
}
void UartSendGroup(u8* buf, u16 len)
{
    while(len--)
    {
        // UART_SendData(UART1, *buf++);
        UART1->TDR = *buf++ ;
        while(!UART_GetFlagStatus(UART1, UART_FLAG_TXEPT));
        // while ((UART1->SR & UART1_SR_TXE ) != UART1_SR_TXE );
        // while((UART1->CSR & UART_FLAG_TXEPT) == (uint16_t)RESET);
    } 
}
#include "stdio.h"
char printBuf[100];
/********************************************************************************************************
**函数信息 ：main(void)
**功能描述 ：
**输入参数 ：无
**输出参数 ：无
********************************************************************************************************/
int main(void)
{
    u8 t;
    delay_init();
    LED_Init();
    uart1_init(9600);
    t = Modbus_write_word();
    UartSendGroup((u8*)printBuf, sprintf(printBuf, "\r\nready\r\n"));
    if(t == 0) {//success
        while(1) 
				{
            LED1_TOGGLE();
            LED2_TOGGLE();
            delay_ms(500);
					
        }
    } else {//fail
        while(1) 				
				{
            LED3_TOGGLE();
            LED4_TOGGLE();
            delay_ms(100);
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
    if (SysTick_Config(SystemCoreClock / 1000)) {
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
**函数信息 ：u8 Modbus_write_word(void)
**功能描述 ：Program FLASH Bank1
**输入参数 ：
**输出参数 ：
**    备注 ：
********************************************************************************************************/
u8 Modbus_write_word(void)
{
    /* Porgram FLASH Bank1 ********************************************************/
    /* Unlock the Flash Bank1 Program Erase controller */
    FLASH_Unlock();
    /* Clear All pending flags */
    FLASH_ClearFlag(FLASH_FLAG_EOP | FLASH_FLAG_PGERR | FLASH_FLAG_WRPRTERR);
    FLASH_ErasePage(BANK1_WRITE_START_ADDR);
    FLASH_ClearFlag(FLASH_FLAG_EOP );
    FLASHStatus = FLASH_ProgramWord(BANK1_WRITE_START_ADDR, Data);
    FLASH_ClearFlag(FLASH_FLAG_EOP );
    FLASH_Lock();
    if((*(__IO uint32_t*) BANK1_WRITE_START_ADDR) != Data) {
        return 1;
    }
    return 0;
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

/*******************************************************************************
* @name   : GPIO_Clock_Set
* @brief  : RCC clock set
* @param  : Portx , State
* @retval : void
*******************************************************************************/
void GPIO_Clock_Set(GPIO_TypeDef* GPIOx, FunctionalState NewState)
{

    if(GPIOx == GPIOA) {
        RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, NewState);                         //GPIO clock starts
    }
    if(GPIOx == GPIOB) {
        RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, NewState);                         //GPIO clock starts
    }
    if(GPIOx == GPIOC) {
        RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOC, NewState);                         //GPIO clock starts
    }
    if(GPIOx == GPIOD) {
        RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOD, NewState);                         //GPIO clock starts
    }
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

    GPIO_Clock_Set(GPIOA, ENABLE);  //开启GPIOA时钟
    GPIO_Clock_Set(GPIOB, ENABLE);  //开启GPIOB时钟
    GPIO_InitStructure.GPIO_Pin  =  GPIO_Pin_15;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    LED1_ON();
    LED2_ON();
    LED3_ON();
    LED4_ON();
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

