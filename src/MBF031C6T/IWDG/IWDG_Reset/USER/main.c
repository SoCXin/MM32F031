/**
******************************************************************************
* @file     main.c
* @author   AE team
* @version  V1.1.1
* @date     15/05/2019
* @brief
******************************************************************************
* @copy
*
* THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
* WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
* TIME. AS A RESULT, MindMotion SHALL NOT BE HELD LIABLE FOR ANY
* DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
* FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
* CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*
* <h2><center>&copy; COPYRIGHT 2019 MindMotion</center></h2>
*/

#include "HAL_device.h"
#include "HAL_conf.h"
#include "stdio.h"

void uart_initwBaudRate(u32 bound);
void Write_Iwdg_ON(unsigned short int IWDG_Prescaler, unsigned short int Reload);
void RVU_CheckStatus(void);
void PVU_CheckStatus(void);
void Write_Iwdg_RL(void);
void UartSendGroup(u8* buf, u16 len);

char printBuf[100];

static __IO uint32_t TimingDelay;


void delay_init(void)
{
    if (SysTick_Config(SystemCoreClock / 1000)) 
		{
        while (1);
    }
    NVIC_SetPriority(SysTick_IRQn, 0x0);        
}

void TimingDelay_Decrement(void)
{
    if (TimingDelay != 0x00) {
        TimingDelay--;
    }
}
void SysTick_Handler(void)
{
    TimingDelay_Decrement();
}

void delay_ms(__IO uint32_t nTime)
{
    TimingDelay = nTime;
    while(TimingDelay != 0);
}

/********************************************************************************************************
**������Ϣ ��main(void)
**�������� ��
**������� ��
**������� ��
**    ��ע ��
********************************************************************************************************/
int main(void)
{
	  delay_init();
    uart_initwBaudRate(115200);
    UartSendGroup((u8*)printBuf, sprintf(printBuf, "uart init!\r\n"));
    /*����ΪLSI32��Ƶ,��������ʼֵΪ0x7ff,��λʱ��ԼΪ1.6s*/
    Write_Iwdg_ON(IWDG_Prescaler_256, 0xf);
    while(1) 
		{
				//����ѭ��
        //�޸�λ���������ѭ��,����ϵͳһֱ��ӡ��������
        Write_Iwdg_RL();
				delay_ms(10);
				
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
**������Ϣ ��Write_Iwdg_PR(void)
**�������� �������������Ź�
**������� ��IWDG_Prescaler ��ѡIWDG_Prescaler_X, XΪ4,8,16,32,64,128,256,��Ӧ��Ƶֵ��Xȡֵ��ͬ
**������� ����
**    ��ע ��Reload<=0xfff,Ϊ����������ֵ  ��λʱ��������LSI 40KHzΪ�ο�  Tiwdg=(X/LSI)*Reload
********************************************************************************************************/
void Write_Iwdg_ON(unsigned short int IWDG_Prescaler, unsigned short int Reload)
{
    /*�����ڲ�����ʱ��,�ȴ�ʱ�Ӿ���*/
    RCC_LSICmd(ENABLE);
    while(RCC_GetFlagStatus(RCC_FLAG_LSIRDY) == RESET);

    /*����ʱ��Ԥ��Ƶ*/
    PVU_CheckStatus();
    IWDG_WriteAccessCmd(0x5555);
    IWDG_SetPrescaler(IWDG_Prescaler);

    /*�������ؼĴ���ֵ*/
    RVU_CheckStatus();
    IWDG_WriteAccessCmd(0x5555);
    IWDG_SetReload(Reload & 0xfff);

    /*װ�ز�ʹ�ܼ�����*/
    IWDG_ReloadCounter();
    IWDG_Enable();
}

/********************************************************************************************************
**������Ϣ ��RVU_CheckStatus(void)
**�������� �����������Ź����ر�־
**������� ����
**������� ����
********************************************************************************************************/
void RVU_CheckStatus(void)
{
    while(1) {
        /*������ر�־״̬*/
        if(IWDG_GetFlagStatus(IWDG_FLAG_RVU) == RESET) {
            break;
        }
    }
}

/********************************************************************************************************
**������Ϣ ��void Write_Iwdg_RL(void)
**�������� ��ι������
**������� ��
**������� ����
********************************************************************************************************/
void Write_Iwdg_RL(void)
{
    IWDG_ReloadCounter();
}

/********************************************************************************************************
**������Ϣ ��PVU_CheckStatus(void)
**�������� �����������Ź�Ԥ��Ƶλ״̬
**������� ����
**������� ����
********************************************************************************************************/
void PVU_CheckStatus(void)
{
    while(1) {
        /*���Ԥ��Ƶλ״̬,ΪRESET�ſɸı�Ԥ��Ƶֵ*/
        if(IWDG_GetFlagStatus(IWDG_FLAG_PVU) == RESET) {
            break;
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
