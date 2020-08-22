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

#define UART_REC_LEN            200                                             //�����������ֽ��� 200
#define EN_UART1_RX             1                                               //ʹ�ܣ�1��/��ֹ��0������1����

extern u32 SystemCoreClock;
static __IO uint32_t TimingDelay;
u8 UART_RX_BUF[UART_REC_LEN];     //���ջ���,���UART_REC_LEN���ֽ�.
u16 UART_RX_STA = 0;              //����״̬���
char printBuf[100];

#define LED4_ON()  GPIO_ResetBits(GPIOA,GPIO_Pin_15)	// PA15
#define LED4_OFF()  GPIO_SetBits(GPIOA,GPIO_Pin_15)	// PA15
#define LED4_TOGGLE()  (GPIO_ReadOutputDataBit(GPIOA,GPIO_Pin_15))?(GPIO_ResetBits(GPIOA,GPIO_Pin_15)):(GPIO_SetBits(GPIOA,GPIO_Pin_15)) // PA15

#define LED3_ON()  GPIO_ResetBits(GPIOB,GPIO_Pin_3)	// PB3
#define LED3_OFF()  GPIO_SetBits(GPIOB,GPIO_Pin_3)	// PB3
#define LED3_TOGGLE()  (GPIO_ReadOutputDataBit(GPIOB,GPIO_Pin_3))?(GPIO_ResetBits(GPIOB,GPIO_Pin_3)):(GPIO_SetBits(GPIOB,GPIO_Pin_3))	// PB3

#define LED2_ON()  GPIO_ResetBits(GPIOB,GPIO_Pin_4)	// PB4
#define LED2_OFF()  GPIO_SetBits(GPIOB,GPIO_Pin_4)	// PB4
#define LED2_TOGGLE()  (GPIO_ReadOutputDataBit(GPIOB,GPIO_Pin_4))?(GPIO_ResetBits(GPIOB,GPIO_Pin_4)):(GPIO_SetBits(GPIOB,GPIO_Pin_4))	// PB4

#define LED1_ON()  GPIO_ResetBits(GPIOB,GPIO_Pin_5)	// PB5
#define LED1_OFF()  GPIO_SetBits(GPIOB,GPIO_Pin_5)	// PB5
#define LED1_TOGGLE()  (GPIO_ReadOutputDataBit(GPIOB,GPIO_Pin_5))?(GPIO_ResetBits(GPIOB,GPIO_Pin_5)):(GPIO_SetBits(GPIOB,GPIO_Pin_5))	// PB5

/********************************************************************************************************
**������Ϣ ��main(void)
**�������� ��
**������� ����
**������� ����
********************************************************************************************************/
int main(void)
{
    u8 t;
    u8 len;
    u16 times = 0;
    delay_init();
    LED_Init();
    uart_nvic_init(9600);
    while(1) 
		{
        if(UART_RX_STA & 0x8000) 
				{
            len = UART_RX_STA & 0x3fff;                                         //�õ��˴ν��յ������ݳ���
            UartSendGroup((u8*)printBuf, sprintf(printBuf, "\r\n�����͵���ϢΪ:\r\n"));
            for(t = 0; t < len; t++) 
						{
                while((UART1->CSR & UART_IT_TXIEN) == 0);                       //�ȴ����ͽ���
                UART1->TDR = UART_RX_BUF[t];
            }
            UartSendGroup((u8*)printBuf, sprintf(printBuf, "\r\n\r\n"));        //���뻻��
            UART_RX_STA = 0;
        } 
				else 
				{
            times++;
            if(times % 5000 == 0) 
						{
                UartSendGroup((u8*)printBuf, sprintf(printBuf, "\r\nMini Board ����ʵ��\r\n"));
            }
            if(times % 200 == 0) UartSendGroup((u8*)printBuf, sprintf(printBuf, "����������,�Իس�������\r\n"));
            if(times % 30 == 0)LED1_TOGGLE();                                   //��˸LED,��ʾϵͳ��������.
            delay_ms(10);
        }
    }
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
**������Ϣ ��LED_Init(void)
**�������� ��LED��ʼ��
**������� ����
**������� ����
********************************************************************************************************/
void LED_Init(void)
{

    GPIO_InitTypeDef  GPIO_InitStructure;

    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA | RCC_AHBPeriph_GPIOB, ENABLE);   //����GPIOA,GPIOBʱ��

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

/********************************************************************************************************
**������Ϣ ��void uart_nvic_init(u32 bound)
**�������� ��UART�˿ڡ��жϳ�ʼ��
**������� ��bound
**������� ��
**    ��ע ��
********************************************************************************************************/
void uart_nvic_init(u32 bound)
{
    //GPIO�˿�����
    GPIO_InitTypeDef GPIO_InitStructure;
    UART_InitTypeDef UART_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_UART1, ENABLE);                       //ʹ��UART1
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);                         //����GPIOAʱ��

    //UART1 NVIC ����
    NVIC_InitStructure.NVIC_IRQChannel = UART1_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPriority = 3;                             //�����ȼ�3
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;                             //IRQͨ��ʹ��
    NVIC_Init(&NVIC_InitStructure);                                             //����ָ���Ĳ�����ʼ��VIC�Ĵ���

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
    UART_ITConfig(UART1, UART_IT_RXIEN, ENABLE);                                //�������ڽ����ж�
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
**������Ϣ ��void UART1_IRQHandler(void)
**�������� ������1�жϷ������
**������� ��
**������� ��
**    ��ע ��
********************************************************************************************************/
void UART1_IRQHandler(void)
{
    u8 Res;
    if(UART_GetITStatus(UART1, UART_IT_RXIEN)  != RESET) {                      //�����ж�(���յ������ݱ�����0x0d 0x0a��β)
        UART_ClearITPendingBit(UART1, UART_IT_RXIEN);
        Res = UART_ReceiveData(UART1);	                                       //��ȡ���յ�������
        if((Res == 0x0d) && ((UART_RX_STA & 0X3FFF) > 0)) {
            UART_RX_STA |= 0x4000;
            UART_RX_BUF[UART_RX_STA & 0X3FFF] = Res ;
            UART_RX_STA++;
        } else if((UART_RX_STA & 0x4000) && ((UART_RX_STA & 0X3FFF) > 0)) {     //���յ���0x0d
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
                UART_RX_STA = 0;                                                //�������ݴ���,���¿�ʼ����
        }
    }
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

