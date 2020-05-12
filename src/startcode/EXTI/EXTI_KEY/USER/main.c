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

void LED_Init(void);
void KEY_Init(void);
void EXTIX_Init(void);



#define LED4_PORT               (GPIOB)
#define LED4_PIN                (GPIO_Pin_7)
#define LED3_PORT               (GPIOB)
#define LED3_PIN                (GPIO_Pin_6)
#define LED2_PORT               (GPIOB)
#define LED2_PIN                (GPIO_Pin_3)
#define LED1_PORT               (GPIOA)
#define LED1_PIN                (GPIO_Pin_15)

#define LED4_ON()  GPIO_ResetBits(LED4_PORT,LED4_PIN)	// PA15
#define LED4_OFF()  GPIO_SetBits(LED4_PORT,LED4_PIN)	// PA15
#define LED4_TOGGLE()  (GPIO_ReadOutputDataBit(LED4_PORT,LED4_PIN))?(GPIO_ResetBits(LED4_PORT,LED4_PIN)):(GPIO_SetBits(LED4_PORT,LED4_PIN))	// PA15

#define LED3_ON()  GPIO_ResetBits(LED3_PORT,LED3_PIN)	// PB3
#define LED3_OFF()  GPIO_SetBits(LED3_PORT,LED3_PIN)	// PB3
#define LED3_TOGGLE()  (GPIO_ReadOutputDataBit(LED3_PORT,LED3_PIN))?(GPIO_ResetBits(LED3_PORT,LED3_PIN)):(GPIO_SetBits(LED3_PORT,LED3_PIN))	// PB3

#define LED2_ON()  GPIO_ResetBits(LED2_PORT,LED2_PIN)	// PB4
#define LED2_OFF()  GPIO_SetBits(LED2_PORT,LED2_PIN)	// PB4
#define LED2_TOGGLE()  (GPIO_ReadOutputDataBit(LED2_PORT,LED2_PIN))?(GPIO_ResetBits(LED2_PORT,LED2_PIN)):(GPIO_SetBits(LED2_PORT,LED2_PIN))	// PB4

#define LED1_ON()  GPIO_ResetBits(LED1_PORT,LED1_PIN)	// PB5
#define LED1_OFF()  GPIO_SetBits(LED1_PORT,LED1_PIN)	// PB5
#define LED1_TOGGLE()  (GPIO_ReadOutputDataBit(LED1_PORT,LED1_PIN))?(GPIO_ResetBits(LED1_PORT,LED1_PIN)):(GPIO_SetBits(LED1_PORT,LED1_PIN))	// PB5

#define KEY1_PORT               (GPIOB)
#define KEY1_PIN                (GPIO_Pin_5)
#define KEY1  GPIO_ReadInputDataBit(KEY1_PORT,KEY1_PIN)         //��ȡ����1

#define WK_UP   GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0)	  
#define KEY2  GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_2)		
#define KEY3  GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_10)	    //��ȡ����3
#define KEY4  GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_11)	    //��ȡ����4

#define KEY1_PRES	1		//KEY1 
#define KEY2_PRES	2		  
#define KEY3_PRES	3		//KEY3 
#define KEY4_PRES	4		//KEY4 

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
**������Ϣ ��main(void)
**�������� ��
**������� ����
**������� ����
********************************************************************************************************/
int main(void)
{
    //GPIO�ڲ��ԣ���˸LED��

    LED_Init();                                                                 //��ʼ����LED���ӵ�Ӳ���ӿ�
    KEY_Init();                                                                 //��ʼ���밴�����ӵ�Ӳ���ӿ�
    EXTIX_Init();

    while(1) 
    {
        if(WK_UP == 0) 
        {
            LED1_ON();
        }
        else
        {
            LED1_OFF();
        }
        
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
    GPIO_Clock_Set(LED1_PORT, ENABLE);  
    GPIO_Clock_Set(LED2_PORT, ENABLE);  
    GPIO_Clock_Set(LED4_PORT, ENABLE);
    GPIO_Clock_Set(LED3_PORT, ENABLE);  
    GPIO_InitStructure.GPIO_Pin  =  LED1_PIN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(LED1_PORT, &GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin  =  LED2_PIN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(LED2_PORT, &GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin  =  LED3_PIN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(LED3_PORT, &GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin  =  LED4_PIN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(LED4_PORT, &GPIO_InitStructure);
    LED2_ON();
    LED3_ON();
}

/********************************************************************************************************
**������Ϣ ��void KEY_Init(void)
**�������� ��������ʼ������
**������� ��
**������� ��
**    ��ע ��
********************************************************************************************************/
void KEY_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    // RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA |RCC_AHBPeriph_GPIOB | RCC_AHBPeriph_GPIOC, ENABLE); //ʹ��GPIOA,GPIOB,GPIOCʱ��

//    GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_13;                                 //PC13��K1
//    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;                               //���ó���������
//    GPIO_Init(GPIOC, &GPIO_InitStructure);                                      //��ʼ��GPIOC13
   
    GPIO_Clock_Set(KEY1_PORT, ENABLE);  
    GPIO_InitStructure.GPIO_Pin  = KEY1_PIN;                                 
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;            
    GPIO_Init(KEY1_PORT, &GPIO_InitStructure);                               

//    GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_0;                                  //PA0,K2��WK_UP��
//    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;                               //���ó���������
//    GPIO_Init(GPIOA, &GPIO_InitStructure);                                      //��ʼ��GPIOA0

//    GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_0;                                 
//    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;                               
//    GPIO_Init(GPIOA, &GPIO_InitStructure);                                     

    // GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_10 | GPIO_Pin_11;        
    // GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;                               //���ó���������
    // GPIO_Init(GPIOB, &GPIO_InitStructure);                                      //��ʼ��GPIOB.10,11
}

/********************************************************************************************************
**������Ϣ ��void EXTIX_Init(void)
**�������� ���ⲿ�жϳ�ʼ������
**������� ��
**������� ��
**���ú��� ��
********************************************************************************************************/
void EXTIX_Init(void)
{
    EXTI_InitTypeDef EXTI_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);                      //�ⲿ�жϣ���Ҫʹ��AFIOʱ��

    KEY_Init();                                                                 //��ʼ��������Ӧioģʽ

    //GPIOC.13 �ж����Լ��жϳ�ʼ������
//    SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOC, EXTI_PinSource13);

//    EXTI_InitStructure.EXTI_Line = EXTI_Line13;
//    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
//    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;                     //�½��ش���
//    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
//    EXTI_Init(&EXTI_InitStructure);                                             //����EXTI_InitStruct��ָ���Ĳ�����ʼ������EXTI�Ĵ���

//    //GPIOA.0	  �ж����Լ��жϳ�ʼ������
    SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOA, EXTI_PinSource0);
    EXTI_InitStructure.EXTI_Line = EXTI_Line0;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;                      //�����ش���
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);                                             //����EXTI_InitStruct��ָ���Ĳ�����ʼ������EXTI�Ĵ���
		
    //GPIOB.1	  �ж����Լ��жϳ�ʼ������
    SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOB, EXTI_PinSource1);

    EXTI_InitStructure.EXTI_Line = EXTI_Line1;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;                  
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);                                             //����EXTI_InitStruct��ָ���Ĳ�����ʼ������EXTI�Ĵ���
    NVIC_InitStructure.NVIC_IRQChannel = EXTI0_1_IRQn;                          //ʹ�ܰ������ڵ��ⲿ�ж�ͨ��
    NVIC_InitStructure.NVIC_IRQChannelPriority = 0x01;                          //�����ȼ�1
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;                             //ʹ���ⲿ�ж�ͨ��
    NVIC_Init(&NVIC_InitStructure);
		
    //GPIOB.2	  �ж����Լ��жϳ�ʼ������
    SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOB, EXTI_PinSource2);
    EXTI_InitStructure.EXTI_Line = EXTI_Line2;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;               
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);                                             //����EXTI_InitStruct��ָ���Ĳ�����ʼ������EXTI�Ĵ���
    NVIC_InitStructure.NVIC_IRQChannel = EXTI2_3_IRQn;                          //ʹ�ܰ������ڵ��ⲿ�ж�ͨ��
    NVIC_InitStructure.NVIC_IRQChannelPriority = 0x01;                          //�����ȼ�1
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;                             //ʹ���ⲿ�ж�ͨ��
    NVIC_Init(&NVIC_InitStructure);
    
    //GPIOB.5	  �ж����Լ��жϳ�ʼ������
    SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOB, EXTI_PinSource5);
    EXTI_InitStructure.EXTI_Line = EXTI_Line5;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;               
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);                                             //����EXTI_InitStruct��ָ���Ĳ�����ʼ������EXTI�Ĵ���
    NVIC_InitStructure.NVIC_IRQChannel = EXTI4_15_IRQn;                          //ʹ�ܰ������ڵ��ⲿ�ж�ͨ��
    NVIC_InitStructure.NVIC_IRQChannelPriority = 0x01;                          //�����ȼ�1
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;                             //ʹ���ⲿ�ж�ͨ��
    NVIC_Init(&NVIC_InitStructure);

    //GPIOB.10	  �ж����Լ��жϳ�ʼ������
    SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOB, EXTI_PinSource10);
    EXTI_InitStructure.EXTI_Line = EXTI_Line10;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;                   
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);                                             //����EXTI_InitStruct��ָ���Ĳ�����ʼ������EXTI�Ĵ���
    //GPIOB.11	  �ж����Լ��жϳ�ʼ������
    SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOB, EXTI_PinSource11);
    EXTI_InitStructure.EXTI_Line = EXTI_Line11;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;                   
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);                                             //����EXTI_InitStruct��ָ���Ĳ�����ʼ������EXTI�Ĵ���
    NVIC_InitStructure.NVIC_IRQChannel = EXTI4_15_IRQn;                         //ʹ�ܰ������ڵ��ⲿ�ж�ͨ��
    NVIC_InitStructure.NVIC_IRQChannelPriority = 0x01;                          //�����ȼ�2
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;                             //ʹ���ⲿ�ж�ͨ��
    NVIC_Init(&NVIC_InitStructure);  	                                        //����NVIC_InitStruct��ָ���Ĳ�����ʼ������NVIC�Ĵ���


}

/********************************************************************************************************
**������Ϣ ��void EXTI0_1_IRQHandler(void)
**�������� ���ⲿ�жϺ���
**������� ��
**������� ��
**���ú��� ��
********************************************************************************************************/
u16 EXTI_DELAY = 0;

void EXTI0_1_IRQHandler(void)
{
    if(EXTI_GetFlagStatus(EXTI_Line0) != RESET) 
    {
        for(EXTI_DELAY = 0; EXTI_DELAY < 100; EXTI_DELAY++);
        if(KEY1 == 0) 
        {
            LED1_TOGGLE();
        }
        EXTI_ClearFlag(EXTI_Line0);   
    } 
    if(EXTI_GetFlagStatus(EXTI_Line1) != RESET) 
    {
        for(EXTI_DELAY = 0; EXTI_DELAY < 100; EXTI_DELAY++);
        if(KEY1 == 1) 
        {
            LED1_TOGGLE();
        }
        EXTI_ClearFlag(EXTI_Line1);   
    }                                      
}

/********************************************************************************************************
**������Ϣ ��void EXTI4_15_IRQHandler(void)
**�������� ���ⲿ�жϺ���
**������� ��
**������� ��
**���ú��� ��
********************************************************************************************************/
void EXTI2_3_IRQHandler(void)
{
    if(EXTI_GetFlagStatus(EXTI_Line2) != RESET) 
    {
        for(EXTI_DELAY = 0; EXTI_DELAY < 100; EXTI_DELAY++);
        if(KEY2 == 1) 
        {
            LED2_TOGGLE();
        }
        EXTI_ClearFlag(EXTI_Line2);      
    }                                         
}

/********************************************************************************************************
**������Ϣ ��void EXTI4_15_IRQHandler(void)
**�������� ���ⲿ�жϺ���
**������� ��
**������� ��
**���ú��� ��
********************************************************************************************************/
void EXTI4_15_IRQHandler(void)
{
    if(EXTI_GetFlagStatus(EXTI_Line5) != RESET) 
    {
        for(EXTI_DELAY = 0; EXTI_DELAY < 100; EXTI_DELAY++);
        if(KEY1 == 1) 
        {
            LED3_TOGGLE();
            LED2_TOGGLE();
        }
        EXTI_ClearFlag(EXTI_Line5);   
    } 
    if(EXTI_GetFlagStatus(EXTI_Line10) != RESET) 
    {
        for(EXTI_DELAY = 0; EXTI_DELAY < 100; EXTI_DELAY++);
        if(KEY3 == 1) 
        {
            LED3_TOGGLE();
        }
        EXTI_ClearFlag(EXTI_Line10);   
    } 
    if(EXTI_GetFlagStatus(EXTI_Line11) != RESET) 
    {
        for(EXTI_DELAY = 0; EXTI_DELAY < 100; EXTI_DELAY++);
        if(KEY4 == 1) 
        {
            LED4_TOGGLE();
        }
        EXTI_ClearFlag(EXTI_Line11);                    //���EXTI10��11��13��·����λ
    }
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

