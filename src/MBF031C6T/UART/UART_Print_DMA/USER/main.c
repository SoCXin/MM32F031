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
void UartSendGroup(u8* buf, u16 len);
void MYDMA_Config(DMA_Channel_TypeDef* DMA_CHx, u32 cpar, u32 cmar, u16 cndtr);
void MYDMA_Enable(DMA_Channel_TypeDef*DMA_CHx);

u32 TEXT_TO_SEND[] = {'1', '2', '3', '4', '5', '6', '7', '8', '9'};
#define TEXT_LENTH 9
u8 SendBuff[(TEXT_LENTH + 2) * 100];
char printBuf[100];
u16 DMA1_MEM_LEN;

/********************************************************************************************************
**������Ϣ ��main(void)
**�������� ��
**������� ����
**������� ����
********************************************************************************************************/
int main(void)
{
    u16 i;
    u8 t = 0;

    uart_initwBaudRate(115200);
    UartSendGroup((u8*)printBuf, sprintf(printBuf, "\r\nDMA SEND DATA:\r\n"));
    MYDMA_Config(DMA1_Channel2, (u32)&UART1->TDR, (u32)SendBuff, (TEXT_LENTH + 2) * 10); //DMA1ͨ��2,����Ϊ����1,�洢��ΪSendBuff,��(TEXT_LENTH+2)*10.
    for(i = 0; i < (TEXT_LENTH + 2) * 100; i++) {                               //���ASCII�ַ�������
        if(t >= TEXT_LENTH) {                                                   //���뻻�з�
            SendBuff[i++] = 0x0d;
            SendBuff[i] = 0x0a;
            t = 0;
        } else SendBuff[i] = TEXT_TO_SEND[t++];                                  //����TEXT_TO_SEND���
    }

    UART_DMACmd(UART1, UART_DMAReq_EN, ENABLE);                                 //ʹ��uart1 DMA
    MYDMA_Enable(DMA1_Channel2);                                                //��ʼһ��DMA���䣡
    //�ȴ�DMA������ɣ���ʱ������������һЩ�£����
    //ʵ��Ӧ���У����������ڼ䣬����ִ�����������
    while(1) {
        if(DMA_GetFlagStatus(DMA1_FLAG_TC2) != RESET) {                         //�ȴ�ͨ��2�������
            DMA_ClearFlag(DMA1_FLAG_TC2);                                       //���ͨ��2������ɱ�־
            break;
        }
    }

    UartSendGroup((u8*)printBuf, sprintf(printBuf, "\r\nUART DMA TEST OK!\r\n"));

    while(1);

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
**������Ϣ ��MYDMA_Config(DMA_Channel_TypeDef* DMA_CHx,u32 cpar,u32 cmar,u16 cndtr)
**�������� �����޸ĸ�����������޸����õ�DMA����
**������� ����
**������� ����
********************************************************************************************************/
void MYDMA_Config(DMA_Channel_TypeDef* DMA_CHx, u32 cpar, u32 cmar, u16 cndtr)
{
    DMA_InitTypeDef DMA_InitStructure;

    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);	                        //ʹ��DMA����

    DMA_DeInit(DMA_CHx);                                                        //��DMA��ͨ��1�Ĵ�������Ϊȱʡֵ
    DMA1_MEM_LEN = cndtr;
    DMA_InitStructure.DMA_PeripheralBaseAddr = cpar;                            //DMA����ADC����ַ
    DMA_InitStructure.DMA_MemoryBaseAddr = cmar;                                //DMA�ڴ����ַ
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;                          //���ݴ��䷽�򣬴��ڴ��ȡ���͵�����
    DMA_InitStructure.DMA_BufferSize = cndtr;                                   //DMAͨ����DMA����Ĵ�С
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;            //�����ַ�Ĵ�������
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;                     //�ڴ��ַ�Ĵ�������
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;     //���ݿ��Ϊ8λ
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;             //���ݿ��Ϊ8λ
    DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;                               //��������������ģʽ
    DMA_InitStructure.DMA_Priority = DMA_Priority_Medium;                       //DMAͨ�� xӵ�������ȼ�
    DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;                                //DMAͨ��xû������Ϊ�ڴ浽�ڴ洫��
    DMA_Init(DMA_CHx, &DMA_InitStructure);                                      //����DMA_InitStruct��ָ���Ĳ�����ʼ��DMA��ͨ��UART1_Tx_DMA_Channel����ʶ�ļĴ���

}
/********************************************************************************************************
**������Ϣ ��MYDMA_Enable(DMA_Channel_TypeDef*DMA_CHx)
**�������� ����ʼһ��DMA����
**������� ����
**������� ����
********************************************************************************************************/
void MYDMA_Enable(DMA_Channel_TypeDef*DMA_CHx)
{
    //	DMA_Cmd(DMA_CHx, DISABLE );  //�ر�UART1 TX DMA1 ��ָʾ��ͨ��
    DMA_Cmd(DMA_CHx, ENABLE);  //ʹ��UART1 TX DMA1 ��ָʾ��ͨ��
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

