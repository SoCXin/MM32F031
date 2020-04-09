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

void UartInit_Loop(void);
void UartSendGroup(u8* buf, u16 len);
void Uart1RxTest(UART_TypeDef* UARTx);
unsigned char inbyte(UART_TypeDef* UARTx);

char printBuf[100];

/********************************************************************************************************
**������Ϣ ��int main (void)
**�������� ��������ARMLED����
**������� ��
**������� ��
********************************************************************************************************/

int main(void)
{
    UartInit_Loop();         //UART1�ķ��ͣ�����ͨ�����������ӡUART OK
    while(1) {
        Uart1RxTest(UART1);  //UART1�Ľ��գ��ڴ�������������ַ�������ͨ����ӡ��֤���յ������Ƿ���ȷ
    }
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

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_UART1, ENABLE);                       //ʹ��UART1ʱ��
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);                         //����GPIOAʱ��
    //UART ��ʼ������
//    GPIO_PinAFConfig(GPIOA, GPIO_PinSource2, GPIO_AF_1);
//    GPIO_PinAFConfig(GPIOA, GPIO_PinSource3, GPIO_AF_1);

    UART_InitStructure.UART_BaudRate = 9600;                                  //���ڲ�����
    UART_InitStructure.UART_WordLength = UART_WordLength_8b;                    //�ֳ�Ϊ8λ���ݸ�ʽ
    UART_InitStructure.UART_StopBits = UART_StopBits_1;                         //һ��ֹͣλ
    UART_InitStructure.UART_Parity = UART_Parity_No;                            //����żУ��λ
    UART_InitStructure.UART_HardwareFlowControl = UART_HardwareFlowControl_None;//��Ӳ������������
    UART_InitStructure.UART_Mode = UART_Mode_Rx | UART_Mode_Tx;	                //�շ�ģʽ

    UART_Init(UART1, &UART_InitStructure);                                      //��ʼ������1
    UART_Cmd(UART1, ENABLE);                                                    //ʹ�ܴ���1

    //UART1_TX   GPIOA.9
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;                                   //PA.9
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;                             //�����������
    GPIO_Init(GPIOA, &GPIO_InitStructure);                                      //��ʼ��GPIOA.9

    //UART1_RX	  GPIOA.10��ʼ��
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;                                  //PA10
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;                       //��������
    GPIO_Init(GPIOA, &GPIO_InitStructure);                                      //��ʼ��GPIOA.10

    UartSendGroup((u8*)printBuf, sprintf(printBuf, "UART OK!\r\n"));
}

/********************************************************************************************************
**������Ϣ ��void Uart1RxTest(UART_TypeDef* UARTx)
**�������� �����ڽ��պ�������
**������� ��UART_TypeDef* UARTx ��ѡ��UART1��UART2��UART3
**������� ����
********************************************************************************************************/

void Uart1RxTest(UART_TypeDef* UARTx)
{
    unsigned char temp;
    temp = inbyte(UARTx);
    if(temp != 0) 
		{
        UartSendGroup((u8*)printBuf, sprintf(printBuf, "�����������Ϊ:%c\r\n", temp));
    }
}

/********************************************************************************************************
**������Ϣ ��unsigned char inbyte(UART_TypeDef* UARTx)
**�������� �����ڽ��պ���
**������� ��UART_TypeDef* UARTx ��ѡ��UART1��UART2��UART3
**������� ��unsigned char ���ڽ��շ��ص��ֽ�
********************************************************************************************************/
unsigned char inbyte(UART_TypeDef* UARTx)
{
    unsigned char temp;

    while(1) {
        if(UART_GetITStatus(UARTx, UART_IT_RXIEN)) {
            UART_ClearITPendingBit(UARTx, UART_IT_RXIEN);                       //��������ж�λ
            break;
        }
    }
    temp = (uint8_t)UART_ReceiveData(UARTx);
    if(temp == 0xd) 
		{                                                           	//���������յ�����
        return 0;
    }
    return temp;
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

