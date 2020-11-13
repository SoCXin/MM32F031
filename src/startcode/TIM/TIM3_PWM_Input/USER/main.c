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

void uart_initwBaudRate(u32 bound);
void UartSendGroup(u8* buf, u16 len);
void delay_init(void);
void delay_ms(__IO uint32_t nTime);
void TimingDelay_Decrement(void);
void TIM2_PWM_Init(u16 arr, u16 psc);
void Tim3_PWMinput_init(u16 arr, u16 psc);

static __IO uint32_t TimingDelay;
extern u32 SystemCoreClock;
char printBuf[100];
u16 period = 0;
u16 duty  = 0;
u8 CollectFlag = 0;

/********************************************************************************************************
**������Ϣ ��int main (void)
**�������� ��ʹ�öŰ������Ӱ����ϵ�PA6(Ain4)��PB10(K3)                                                                                                                                                                                                                                                                                                                                          )
**������� ��
**������� ��
********************************************************************************************************/
int main(void)
{

    delay_init();
    uart_initwBaudRate(115200);
    TIM2_PWM_Init(1000 - 1, SystemCoreClock / 1000000 - 1);                     //1KHZ����
    TIM_SetCompare3(TIM2, 600);                                                 //����ռ�ձ�  pa2
    Tim3_PWMinput_init(0xffff, SystemCoreClock / 1000000 - 1);                  //pwm�����ʼ����1M��Ƶ�ʲ�׽
    UartSendGroup((u8*)printBuf, sprintf(printBuf, "������MiniBoard�ϵ�PA6(Ain4)��PB10(K3)\r\n"));
    while(1) {
        delay_ms(500);
        if(CollectFlag == 1) {
            UartSendGroup((u8*)printBuf, sprintf(printBuf, "period  = %dus\r\n", period));  //��ӡ����,��Լ��1MS
            CollectFlag = 0;
        }
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
**������Ϣ ��void TIM2_PWM_Init(u16 arr,u16 psc)
**�������� ��TIM2 PWM��ʼ��
**������� ��u16 arr,u16 psc
**������� ��
**���ú��� ��
********************************************************************************************************/
void TIM2_PWM_Init(u16 arr, u16 psc)                                            //��T2��CH3   APB1
{
    GPIO_InitTypeDef GPIO_InitStructure;
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
    TIM_OCInitTypeDef  TIM_OCInitStructure;

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE);                         //����GPIOBʱ��

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);                      //ʹ��GPIO����ʱ��ʹ��

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;                                  //TIM2_CH3
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;                             //�����������
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource10, GPIO_AF_2);                       //PB10���ù��ܵ�AF2
    TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
    TIM_TimeBaseStructure.TIM_Period = arr;                                     //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ	 80K
    TIM_TimeBaseStructure.TIM_Prescaler = psc;                                  //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ  ����Ƶ
    TIM_TimeBaseStructure.TIM_ClockDivision = 0;                                //����ʱ�ӷָ�:TDTS = Tck_tim
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;                 //TIM���ϼ���ģʽ
    TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);                             //����TIM_TimeBaseInitStruct��ָ���Ĳ�����ʼ��TIMx��ʱ�������λ

    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2;                           //ѡ��ʱ��ģʽ:TIM�����ȵ���ģʽ2
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;               //�Ƚ����ʹ��
    TIM_OCInitStructure.TIM_Pulse = 0;                                          //���ô�װ�벶��ȽϼĴ���������ֵ
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;                   //�������:TIM����Ƚϼ��Ը�
    TIM_OC3Init(TIM2, &TIM_OCInitStructure);                                    //����TIM_OCInitStruct��ָ���Ĳ�����ʼ������TIMx


    TIM_OC3PreloadConfig(TIM2, TIM_OCPreload_Enable);                           //CH3Ԥװ��ʹ��

    TIM_ARRPreloadConfig(TIM2, ENABLE);                                         //ʹ��TIMx��ARR�ϵ�Ԥװ�ؼĴ���

    TIM_Cmd(TIM2, ENABLE);                                                      //ʹ��TIM2


}

/********************************************************************************************************
**������Ϣ ��void Tim3_PWMinput_init(u16 arr,u16 psc)
**�������� ��TIM3 PWM��ʼ��
**������� ��u16 arr,u16 psc
**������� ��
**���ú��� ��
********************************************************************************************************/
void Tim3_PWMinput_init(u16 arr, u16 psc)                                       //��T3��CH1 APB1  PA6    AF1
{

    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
    NVIC_InitTypeDef NVIC_InitStructure;
    TIM_ICInitTypeDef  TIM3_ICInitStructure;
    GPIO_InitTypeDef GPIO_InitStructure;

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);                        //Open TIM3 clock
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);                         //����GPIOAʱ��

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource6, GPIO_AF_1);                        //PA6���ù��ܵ�AF1
    TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
    TIM_TimeBaseStructure.TIM_Period = arr;                                     //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ
    TIM_TimeBaseStructure.TIM_Prescaler = psc;                                  //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ
    TIM_TimeBaseStructure.TIM_ClockDivision = 0;                                //����ʱ�ӷָ�:TDTS = Tck_tim
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;                 //TIM���ϼ���ģʽ
    TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);                             //����TIM_TimeBaseInitStruct��ָ���Ĳ�����ʼ��TIMx��ʱ�������λ


    /*�����ж����ȼ�*/
    NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    TIM3_ICInitStructure.TIM_Channel = TIM_Channel_1;
    TIM3_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;
    TIM3_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;
    TIM3_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;
    TIM3_ICInitStructure.TIM_ICFilter = 0x0;

    TIM_PWMIConfig(TIM3, &TIM3_ICInitStructure);                                //PWM��������
    TIM_SelectInputTrigger(TIM3, TIM_TS_TI1FP1);                                //ѡ����Ч�����
    TIM_SelectSlaveMode(TIM3, TIM_SlaveMode_Reset);                             //����Ϊ���Ӹ�λģʽ

    TIM_ITConfig(TIM3, TIM_IT_CC1 | TIM_IT_Update, ENABLE);                     //�ж�����
    TIM_ClearITPendingBit(TIM3, TIM_IT_CC1 | TIM_IT_Update);                    //����жϱ�־λ
    TIM_Cmd(TIM3, ENABLE);
}

/********************************************************************************************************
**������Ϣ ��void TIM3_IRQHandler(void)
**�������� ��TIM3�жϺ���
**������� ��
**������� ��
**���ú��� ��
********************************************************************************************************/
void TIM3_IRQHandler(void)
{

    if (TIM_GetITStatus(TIM3, TIM_IT_CC1) != RESET) {                           //����1���������¼�
        period	=   TIM_GetCapture1(TIM3);                                      //�ɼ�����
        duty = TIM_GetCapture2(TIM3);                                           //�ɼ�ռ�ձ�
        CollectFlag = 1;
    }
    TIM_ClearITPendingBit(TIM3, TIM_IT_CC1 | TIM_IT_Update);                    //����жϱ�־λ
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

