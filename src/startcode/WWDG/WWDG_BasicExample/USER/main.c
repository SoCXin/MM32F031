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

extern u32 SystemCoreClock;
void uart_initwBaudRate(u32 bound);
void delay_init(void);
void delay_ms(__IO uint32_t nTime);
void TimingDelay_Decrement(void);
void UartSendGroup(u8* buf, u16 len);
void Wwdg_reset_ON(unsigned char ucTcnt, unsigned char ucWcnt);

static __IO uint32_t TimingDelay;
char printBuf[100];

/********************************************************************************************************
**������Ϣ ��int main (void)
**�������� ��������ARMLED����
**������� ��
**������� ��
********************************************************************************************************/

int main(void)
{

    delay_init();
    uart_initwBaudRate(115200);
    UartSendGroup((u8*)printBuf, sprintf(printBuf, "uart init!\r\n"));

    /*���ڿ��Ź���,΢�������뼶��λ,��ι������WWDG_SetCounter()����*/
    Wwdg_reset_ON(0x7e, 0x7f);                                                  //���ڿ��Ź�FUN_2,��ʱ����ι��������λ

    while(1) {
        //�޸�λ���������ѭ��,����ϵͳһֱ��ӡ��������
        WWDG_SetCounter(0x7e);
        delay_ms(10);
    }
}

/********************************************************************************************************
**������Ϣ ��Wwdg_reset_test()
**�������� ��ʹ�ô��ڿ��Ź�
**������� ��unsigned char ucTcn ���Ź��ݼ�������ֵ unsigned char ucWcnt����ֵ
**������� ����
**    ��ע ��ϵͳ����ʱ���Ź�Ϊʧ��״̬,���Ź�ʧ�ܺ����ֶ��ر�,��λ���Ź�ʧ��
********************************************************************************************************/
void Wwdg_reset_ON(unsigned char ucTcnt, unsigned char ucWcnt)
{
    /*ʧ�ܴ��ڿ��Ź�ʱ��*/
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_WWDG, ENABLE);

    /*WWDG_Prescaler_x,xΪ1,2,4,8,WWDG counter clock = (PCLK1/4096)/x */
    WWDG_SetPrescaler(WWDG_Prescaler_8);
    WWDG_SetWindowValue(ucWcnt);

    /*Ϊ���Ź��Լ���������ֵ,Ϊ7λ��������ֵΪ0x40~0x7f֮��*/
    WWDG_Enable(ucTcnt);
}

/********************************************************************************************************
**������Ϣ ��Wwdg_irq_test()
**�������� �����ڿ��Ź��ж�����
**������� ����
**������� ����
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
    /*�˴�Ϊ����,����ֵ����С�ڴ���ֵ,��������������λ*/
    WWDG_Enable(0x66);
}

/********************************************************************************************************
**������Ϣ ��void WWDG_IRQHandler (void)
**�������� �����ڿ��Ź��жϺ���
**������� ����
**������� ����
**    ��ע �����ݼ�����������0x40ʱ�����ж�,������������С��0x7f����0x40,���ӽ���0x40���ܻ�Ƶ��������ж�
********************************************************************************************************/
void WWDG_IRQHandler (void)
{
    if(WWDG_GetFlagStatus()) {
        WWDG_ClearFlag();
        WWDG_SetCounter(0x66);
    }
}

/********************************************************************************************************
**������Ϣ ��uart_initwBaudRate(u32 bound)
**�������� ��UART��ʼ��
**������� ��bound
**������� ����
********************************************************************************************************/
void uart_initwBaudRate(u32 bound)
{
    //GPIO�˿�����
    GPIO_InitTypeDef GPIO_InitStructure;
    UART_InitTypeDef UART_InitStructure;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_UART1, ENABLE);                       //ʹ��UART1ʱ��
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);                         //����GPIOAʱ��
    //UART ��ʼ������
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource9, GPIO_AF_1);
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource10, GPIO_AF_1);

    UART_InitStructure.UART_BaudRate = bound;                                   //���ڲ�����
    UART_InitStructure.UART_WordLength = UART_WordLength_8b;                    //�ֳ�Ϊ8λ���ݸ�ʽ
    UART_InitStructure.UART_StopBits = UART_StopBits_1;                         //һ��ֹͣλ
    UART_InitStructure.UART_Parity = UART_Parity_No;                            //����żУ��λ
    UART_InitStructure.UART_HardwareFlowControl = UART_HardwareFlowControl_None;//��Ӳ������������
    UART_InitStructure.UART_Mode = UART_Mode_Rx | UART_Mode_Tx;	                //�շ�ģʽ

    UART_Init(UART1, &UART_InitStructure);                                      //��ʼ������1
    UART_Cmd(UART1, ENABLE);                                                    //ʹ�ܴ���1

    //UART1_TX   GPIOA.9
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;                                   //PA.9
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;                             //�����������
    GPIO_Init(GPIOA, &GPIO_InitStructure);                                      //��ʼ��GPIOA.9

    //UART1_RX	  GPIOA.10��ʼ��
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;                                  //PA10
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;                       //��������
    GPIO_Init(GPIOA, &GPIO_InitStructure);                                      //��ʼ��GPIOA.10

}

/********************************************************************************************************
**������Ϣ ��delay_init(void)
**�������� ��systick��ʱ������ʼ��
**������� ����
**������� ����
********************************************************************************************************/
void delay_init(void)
{
    if (SysTick_Config(SystemCoreClock / 1000)) {
        /* Capture error */
        while (1);
    }
    /* Configure the SysTick handler priority */
    NVIC_SetPriority(SysTick_IRQn, 0x0);                                        //SysTick�ж����ȼ�����
}

/********************************************************************************************************
**������Ϣ ��SysTick_Handler(void)
**�������� ��������жϺ�����Systick���еݼ�
**������� ����
**������� ����
********************************************************************************************************/
void SysTick_Handler(void)
{
    TimingDelay_Decrement();
}

/********************************************************************************************************
**������Ϣ ��TimingDelay_Decrement(void)
**�������� ����1ms���ٶȵݼ�
**������� ��pclk2������ϵͳʱ��Ϊ8MHz������Ӧ����8
**������� ����
********************************************************************************************************/
void TimingDelay_Decrement(void)
{
    if (TimingDelay != 0x00) {
        TimingDelay--;
    }
}

/********************************************************************************************************
**������Ϣ ��delay_ms(__IO uint32_t nTime)
**�������� ������Ӧ�õ�����ʱ��ʹ��systick
**������� ��nTime����ʱ
**������� ����
********************************************************************************************************/
void delay_ms(__IO uint32_t nTime)
{
    TimingDelay = nTime;

    while(TimingDelay != 0);
}

/********************************************************************************************************
**������Ϣ ��void UartSendByte(u8 dat)
**�������� ��UART��������
**������� ��dat
**������� ��
**    ��ע ��
********************************************************************************************************/
void UartSendByte(u8 dat)
{
    UART_SendData(UART1, dat);
    while(!UART_GetFlagStatus(UART1, UART_FLAG_TXEPT));
}

/********************************************************************************************************
**������Ϣ ��void UartSendGroup(u8* buf,u16 len)
**�������� ��UART��������
**������� ��buf,len
**������� ��
**    ��ע ��
********************************************************************************************************/
void UartSendGroup(u8* buf, u16 len)
{
    while(len--)
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

