/**
********************************************************************************
* @file     main.c
* @author   AE team
* @version  V1.1.1
* @date     15/05/2019
* @brief
********************************************************************************
*/

#include "HAL_device.h"
#include "HAL_conf.h"
#include "stdio.h"

extern u32 SystemCoreClock;
void delay_init(void);
void delay_ms(__IO uint32_t nTime);
void TimingDelay_Decrement(void);
void LED_Init(void);

static __IO uint32_t TimingDelay;

#define LED4_PORT               (GPIOA)
#define LED4_PIN                (GPIO_Pin_6)
#define LED3_PORT               (GPIOA)
#define LED3_PIN                (GPIO_Pin_7)
#define LED2_PORT               (GPIOA)
#define LED2_PIN                (GPIO_Pin_5)
#define LED1_PORT               (GPIOA)
#define LED1_PIN                (GPIO_Pin_15)


#define LED4_ON()            GPIO_ResetBits(LED4_PORT,LED4_PIN)                  // PA15
#define LED4_OFF()           GPIO_SetBits(LED4_PORT,LED4_PIN)                    // PA15
#define LED4_TOGGLE()       (GPIO_ReadOutputDataBit(LED4_PORT,LED4_PIN))? \
    (GPIO_ResetBits(LED4_PORT,LED4_PIN)):         \
    (GPIO_SetBits(LED4_PORT,LED4_PIN))                   // PA15

#define LED3_ON()            GPIO_ResetBits(LED3_PORT,LED3_PIN)                   // PB3
#define LED3_OFF()           GPIO_SetBits(LED3_PORT,LED3_PIN)                     // PB3
#define LED3_TOGGLE()       (GPIO_ReadOutputDataBit(LED3_PORT,LED3_PIN))?  \
    (GPIO_ResetBits(LED3_PORT,LED3_PIN)):          \
    (GPIO_SetBits(LED3_PORT,LED3_PIN))                    // PB3

#define LED2_ON()            GPIO_ResetBits(LED2_PORT,LED2_PIN)                   // PB4
#define LED2_OFF()           GPIO_SetBits(LED2_PORT,LED2_PIN)                     // PB4
#define LED2_TOGGLE()       (GPIO_ReadOutputDataBit(LED2_PORT,LED2_PIN))?  \
    (GPIO_ResetBits(LED2_PORT,LED2_PIN)):          \
    (GPIO_SetBits(LED2_PORT,LED2_PIN))                    // PB4

#define LED1_ON()            GPIO_ResetBits(LED1_PORT,LED1_PIN)                   // PB5
#define LED1_OFF()           GPIO_SetBits(LED1_PORT,LED1_PIN)                     // PB5
#define LED1_TOGGLE()       (GPIO_ReadOutputDataBit(LED1_PORT,LED1_PIN))?  \
    (GPIO_ResetBits(LED1_PORT,LED1_PIN)):          \
    (GPIO_SetBits(LED1_PORT,LED1_PIN))                    // PB5

#define RS_DIR_PORT         (GPIOA)
#define RS_DIR_PIN          (GPIO_Pin_7)
#define  RS485_W            GPIO_ResetBits(RS_DIR_PORT,RS_DIR_PIN);GPIO_SetBits(GPIOA,GPIO_Pin_6);GPIO_ResetBits(GPIOA,GPIO_Pin_5)
#define  RS485_R            GPIO_SetBits(RS_DIR_PORT,RS_DIR_PIN);GPIO_ResetBits(GPIOA,GPIO_Pin_6);GPIO_SetBits(GPIOA,GPIO_Pin_5)


#define KEY1                 GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_13)           //��ȡ����1
#define WK_UP                GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0)            //��ȡ����2 
#define KEY3                 GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_10)           //��ȡ����3
#define KEY4                 GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_11)           //��ȡ����4

#define KEY1_PRES            1                                                  //KEY1 
#define WKUP_PRES            2                                                  //WK_UP  
#define KEY3_PRES            3                                                  //KEY3 
#define KEY4_PRES            4                                                  //KEY4 
char printBuf[100];
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
    RS485_W;
//		delay(10);
    while(len--) UartSendByte(*buf++);
//		delay(10);
    RS485_R;
}
/********************************************************************************************************
**������Ϣ ��UartInit_Loop(void)
**�������� ����ʼ������
**������� ����
**������� ����
********************************************************************************************************/
void UartInit_Loop(void)
{

    //GPIO�˿�����
    GPIO_InitTypeDef GPIO_InitStructure;
    UART_InitTypeDef UART_InitStructure;
    //485����ʹ��
 

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_UART1, ENABLE);                       //ʹ��UART1ʱ��
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);                         //����GPIOAʱ��
		RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE);                         //����GPIOAʱ��
    //UART ��ʼ������
//    GPIO_PinAFConfig(GPIOA, GPIO_PinSource2, GPIO_AF_1);
//    GPIO_PinAFConfig(GPIOA, GPIO_PinSource3, GPIO_AF_1);
		GPIO_PinAFConfig(GPIOA, GPIO_PinSource9, GPIO_AF_3);
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource10, GPIO_AF_3);	
	
    UART_InitStructure.UART_BaudRate = 9600;                                  //���ڲ�����
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
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING ;                             //�����������
    GPIO_Init(GPIOA, &GPIO_InitStructure);                                      //��ʼ��GPIOA.9

    //UART1_RX	  GPIOA.10��ʼ��
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;                                  //PA10
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;                       	//��������
    GPIO_Init(GPIOA, &GPIO_InitStructure);                                      //��ʼ��GPIOA.10
		

		
    GPIO_InitStructure.GPIO_Pin  =  RS_DIR_PIN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(RS_DIR_PORT, &GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin  =  GPIO_Pin_6|GPIO_Pin_5;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(RS_DIR_PORT, &GPIO_InitStructure);
//    UartSendGroup((u8*)printBuf, sprintf(printBuf, "UART OK!\r\n"));
}

/*******************************************************************************
**������Ϣ ��main(void)
**�������� ��
**������� ����
**������� ����
*******************************************************************************/
int main(void)
{
    delay_init();
    LED_Init();
		UartInit_Loop();
    while(1) 
		{
//        LED1_TOGGLE();
//        LED2_TOGGLE();
//        LED3_TOGGLE();
//        LED4_TOGGLE();
				UartSendGroup((u8*)printBuf, sprintf(printBuf, "UART OK!\r\n"));
        delay_ms(300);
    }
}

/*******************************************************************************
**������Ϣ ��void delay_init(void)
**�������� ����ʼ���ӳٺ���
**������� ��
**������� ��
*******************************************************************************/
void delay_init(void)
{
    if (SysTick_Config(SystemCoreClock / 1000)) {
        /* Capture error */
        while (1);
    }
    /* Configure the SysTick handler priority */
    NVIC_SetPriority(SysTick_IRQn, 0x0);//SysTick�ж����ȼ�����
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
/*******************************************************************************
**������Ϣ ��LED_Init(void)
**�������� ��LED��ʼ��
**������� ����
**������� ����
*******************************************************************************/
void LED_Init(void)
{
    GPIO_InitTypeDef  GPIO_InitStructure;
    //LED����ʹ��
    GPIO_Clock_Set(LED4_PORT, ENABLE);  
    GPIO_InitStructure.GPIO_Pin  =  LED4_PIN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(LED4_PORT, &GPIO_InitStructure);
    GPIO_Clock_Set(LED3_PORT, ENABLE);  
    GPIO_InitStructure.GPIO_Pin  =  LED3_PIN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(LED3_PORT, &GPIO_InitStructure);
    GPIO_Clock_Set(LED2_PORT, ENABLE);  
    GPIO_InitStructure.GPIO_Pin  =  LED2_PIN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(LED2_PORT, &GPIO_InitStructure);
    GPIO_Clock_Set(LED1_PORT, ENABLE);  
    GPIO_InitStructure.GPIO_Pin  =  LED1_PIN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(LED1_PORT, &GPIO_InitStructure);
    LED1_ON();
    LED2_ON();
    LED3_ON();
    LED4_ON();
}

/**
* @}
*/


/*-------------------------(C) COPYRIGHT 2020 Qitas ---------------------*/

