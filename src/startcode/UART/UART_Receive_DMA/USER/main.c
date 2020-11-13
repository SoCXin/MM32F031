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

u8 DST_Buffer[10] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};                             //UART DMA���ܵ����ݴ������

void DMA_Configuration(void);
void UartSendGroup(u8* buf, u16 len);
void uart_initwBaudRate(u32 bound);

char printBuf[100];

#define RS_DIR_PORT         (GPIOB)
#define RS_DIR_PIN          (GPIO_Pin_11)
#define RS485_W             GPIO_ResetBits(RS_DIR_PORT,RS_DIR_PIN)
#define RS485_R             GPIO_SetBits(RS_DIR_PORT,RS_DIR_PIN)
/********************************************************************************************************
**������Ϣ ��int main (void)
**�������� ��������ʹ�ô������ַ���10�����֣�ע�⴮�����ֲ���ѡ��������
**������� ��
**������� ��
********************************************************************************************************/

int main(void)
{
    //ע�⣺���ڵ������ֲ��ܹ�ѡ��������
    u8 i;
    uart_initwBaudRate(115200);
    DMA_Configuration();                                                        //UART DMA����
    UartSendGroup((u8*)printBuf, sprintf(printBuf, "������10������!\r\n"));
    UartSendGroup((u8*)printBuf, sprintf(printBuf, "�������ֲ��ܹ�ѡ��������!\r\n"));
    while(1) {
        for(i = 0; i < 10; i++) {
            DST_Buffer[i] = 0;                                                  //������������
        }
        while(!DMA_GetFlagStatus(DMA1_FLAG_TC3));                               //�ȴ�DMA�������
        DMA_ClearFlag(DMA1_FLAG_TC3);                                           //���DMA������ɱ�־λ
        for(i = 0; i < 10; i++) {
            UartSendGroup((u8*)printBuf, sprintf(printBuf, "DST_Buffer[%d]==%d\r\n", i, (DST_Buffer[i] - 0x30)));
        }
        UartSendGroup((u8*)printBuf, sprintf(printBuf, "\r\n"));
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
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE);                       
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

    GPIO_InitStructure.GPIO_Pin = RS_DIR_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(RS_DIR_PORT, &GPIO_InitStructure);
}


/********************************************************************************************************
**������Ϣ ��DMA_Configuration
**�������� ��UART����DMA����
**������� ����
**������� ����
********************************************************************************************************/
void DMA_Configuration(void)
{
    DMA_InitTypeDef  DMA_InitStructure;
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
    /* UARTy_DMA1_Channel Config */
    DMA_DeInit(DMA1_Channel3);
    DMA_InitStructure.DMA_PeripheralBaseAddr = (u32) & (UART1->RDR);
    DMA_InitStructure.DMA_MemoryBaseAddr = (u32)DST_Buffer;
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
    DMA_InitStructure.DMA_BufferSize = 10;
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
    DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;                             //ѭ��ģʽ����λ�����Բ�����������
    DMA_InitStructure.DMA_Priority = DMA_Priority_Low;
    DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
    DMA_Init(DMA1_Channel3, &DMA_InitStructure);

    UART_DMACmd(UART1, UART_DMAReq_EN, ENABLE);                                 //����UART1 DMA
    /* UARTy_DMA1_Channel enable */
    DMA_Cmd(DMA1_Channel3, ENABLE);
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
    RS485_W;
    while(len--)  UartSendByte(*buf++);
    RS485_R;
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

