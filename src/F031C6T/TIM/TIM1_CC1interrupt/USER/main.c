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

#define LED4_ON()  GPIO_ResetBits(GPIOA,GPIO_Pin_15)	// PA15
#define LED4_OFF()  GPIO_SetBits(GPIOA,GPIO_Pin_15)	// PA15
#define LED4_TOGGLE()  (GPIO_ReadOutputDataBit(GPIOA,GPIO_Pin_15))?(GPIO_ResetBits(GPIOA,GPIO_Pin_15)):(GPIO_SetBits(GPIOA,GPIO_Pin_15))	// PA15

#define LED3_ON()  GPIO_ResetBits(GPIOB,GPIO_Pin_3)	// PB3
#define LED3_OFF()  GPIO_SetBits(GPIOB,GPIO_Pin_3)	// PB3
#define LED3_TOGGLE()  (GPIO_ReadOutputDataBit(GPIOB,GPIO_Pin_3))?(GPIO_ResetBits(GPIOB,GPIO_Pin_3)):(GPIO_SetBits(GPIOB,GPIO_Pin_3))	// PB3

#define LED2_ON()  GPIO_ResetBits(GPIOB,GPIO_Pin_4)	// PB4
#define LED2_OFF()  GPIO_SetBits(GPIOB,GPIO_Pin_4)	// PB4
#define LED2_TOGGLE()  (GPIO_ReadOutputDataBit(GPIOB,GPIO_Pin_4))?(GPIO_ResetBits(GPIOB,GPIO_Pin_4)):(GPIO_SetBits(GPIOB,GPIO_Pin_4))	// PB4

#define LED1_ON()  GPIO_ResetBits(GPIOB,GPIO_Pin_5)	// PB5
#define LED1_OFF()  GPIO_SetBits(GPIOB,GPIO_Pin_5)	// PB5
#define LED1_TOGGLE()  (GPIO_ReadOutputDataBit(GPIOB,GPIO_Pin_5))?(GPIO_ResetBits(GPIOB,GPIO_Pin_5)):(GPIO_SetBits(GPIOB,GPIO_Pin_5))	// PB5

void LED_Init(void);

void Tim1_init(u16 Prescaler, u16 Period);

extern u32 SystemCoreClock;

unsigned int uiCnt = 0;
unsigned int uiCCCnt = 0;
u8 timeflag = 0;
u8 timeflagcc = 0;

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
void UartSendByte(u8 dat)
{
    UART_SendData(UART1, dat);
    while(!UART_GetFlagStatus(UART1, UART_FLAG_TXEPT));
}

void UartSendGroup(u8* buf, u16 len)
{
    while(len--)
        UartSendByte(*buf++);
}

/********************************************************************************************************
**������Ϣ ��main(void)
**�������� ��
**������� ����
**������� ����
********************************************************************************************************/
char printBuf[100];

int main(void)
{
    unsigned int uiCnt = 0;
    LED_Init();
    uart_initwBaudRate(115200);
    Tim1_init(SystemCoreClock/50000-1, 9999);  
    while(1) {
        /* �ȴ���ʱ����� */
        if(timeflag == 1) {
            uiCnt++;
            timeflag = 0;
            /*��ӡ��ǰuiCntֵ */
            // UartSendGroup((u8*)printBuf, sprintf(printBuf, "UP=%d\r\n", uiCnt));
            uiCnt++;
            LED1_TOGGLE();
            LED2_TOGGLE();
        }
        if(timeflagcc == 1) 
        {
            uiCCCnt++;
            timeflagcc = 0;
            /*��ӡ��ǰuiCntֵ */
            // UartSendGroup((u8*)printBuf, sprintf(printBuf, "CC=%d\r\n", uiCnt));
            LED3_TOGGLE();
            LED4_TOGGLE();
        }
    }
}
/********************************************************************************************************
**������Ϣ ��TIM1_BRK_UP_TRG_COM_IRQHandler(void)
**�������� ���ȴ���ʱ������������ж�
**������� ����
**������� ����
********************************************************************************************************/
void TIM1_BRK_UP_TRG_COM_IRQHandler(void)
{
    if(TIM_GetITStatus(TIM1, TIM_IT_Update) != RESET) {
        TIM_ClearITPendingBit(TIM1, TIM_IT_Update);
        timeflag = 1;
        UartSendGroup((u8*)printBuf, sprintf(printBuf, "UP\r\n"));
    }
}
/********************************************************************************************************
**������Ϣ ��
**�������� ���ȴ���ʱ������������ж�
**������� ����
**������� ����
********************************************************************************************************/
void TIM1_CC_IRQHandler(void)
{
    if(TIM_GetITStatus(TIM1, TIM_IT_CC1) != RESET) {
        TIM_ClearITPendingBit(TIM1, TIM_IT_CC1);
        timeflagcc = 1;
        UartSendGroup((u8*)printBuf, sprintf(printBuf, "CC\r\n"));
    }
}

void Tim1_Int_Enable(void)
{
    NVIC_InitTypeDef NVIC_StructInit;
    //config Tim1 interrupt
    NVIC_StructInit.NVIC_IRQChannel = TIM1_BRK_UP_TRG_COM_IRQn;
    NVIC_StructInit.NVIC_IRQChannelPriority = 1;
    NVIC_StructInit.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_StructInit);

    TIM_ClearFlag(TIM1, TIM_FLAG_Update);
    TIM_ITConfig(TIM1, TIM_IT_Update, ENABLE);
}
void Tim1_CC1_Enable(void)
{
    NVIC_InitTypeDef NVIC_StructInit;
    //config Tim1 interrupt
    NVIC_StructInit.NVIC_IRQChannel = TIM1_CC_IRQn;
    NVIC_StructInit.NVIC_IRQChannelPriority = 1;
    NVIC_StructInit.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_StructInit);

    TIM_ClearFlag(TIM1, TIM_FLAG_CC1);
    TIM_ITConfig(TIM1, TIM_IT_CC1, ENABLE);
}
/********************************************************************************************************
**������Ϣ ��void Tim1_init(u16 Period,u16 Prescaler)
**�������� �����ö�ʱ��1���ϼ���ģʽ
**������� ��Period 16λ����������ֵ,Prescaler ʱ��Ԥ��Ƶֵ
**������� ����
********************************************************************************************************/
void Tim1_init(u16 Prescaler, u16 Period)
{
    TIM_TimeBaseInitTypeDef TIM_StructInit;
    TIM_OCInitTypeDef  TIM_OCInitStructure;
    /*ʹ��TIM1ʱ��,Ĭ��ʱ��ԴΪPCLK2(PCLK2δ��Ƶʱ����Ƶ,������PCLK2��Ƶ���),��ѡ����ʱ��Դ*/
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);
    TIM_TimeBaseStructInit(&TIM_StructInit);
    TIM_StructInit.TIM_Period = Period;                                         //ARR�Ĵ���ֵ
    TIM_StructInit.TIM_Prescaler = Prescaler;                                   //Ԥ��Ƶֵ
    /*�����˲�������Ƶ��,��Ӱ�춨ʱ��ʱ��*/
    TIM_StructInit.TIM_ClockDivision = TIM_CKD_DIV1;                            //������Ƶֵ
    TIM_StructInit.TIM_CounterMode = TIM_CounterMode_Up;                        //����ģʽ
    TIM_StructInit.TIM_RepetitionCounter = 0;

    TIM_OCStructInit(&TIM_OCInitStructure);
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_Timing;//TIM_OCMode_PWM1; //ѡ��ʱ��ģʽ:TIM�����ȵ���ģʽ2
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //�Ƚ����ʹ��
    TIM_OCInitStructure.TIM_Pulse = (Period + 1) / 5; //���ô�װ�벶��ȽϼĴ���������ֵ
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //�������:TIM����Ƚϼ��Ը�
    TIM_OC1Init(TIM1, &TIM_OCInitStructure);  //����TIM_OCInitStruct��ָ���Ĳ�����ʼ������TIMx

    TIM_TimeBaseInit(TIM1, &TIM_StructInit);
    TIM_ARRPreloadConfig(TIM1, ENABLE);
    Tim1_Int_Enable();
    Tim1_CC1_Enable();
    TIM_Cmd(TIM1, ENABLE);
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
**������Ϣ ��LED_Init(void)
**�������� ��LED��ʼ��
**������� ����
**������� ����
********************************************************************************************************/
void LED_Init(void)
{

    GPIO_InitTypeDef  GPIO_InitStructure;

    GPIO_Clock_Set(GPIOA, ENABLE);  //����GPIOAʱ��
    GPIO_Clock_Set(GPIOB, ENABLE);  //����GPIOBʱ��
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

