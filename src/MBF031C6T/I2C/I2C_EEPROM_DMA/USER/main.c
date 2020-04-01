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
void I2C1MasterDMATest(void);
void I2C1_DMA_TX(unsigned char *tx_buf, unsigned short size);
void I2CInitMasterMode(void);
void DMA_I2C1MasterWrite(unsigned char device_id, unsigned short mem_byte_addr, unsigned short tx_count, unsigned char *tx_data );
void DMA_I2C1MasterRead(unsigned char device_id, unsigned short mem_byte_addr, unsigned short rx_count, unsigned char *rx_data );
void UartSendGroup(u8* buf, u16 len);

unsigned char tx_buffer0[16] = {0xa, 0xb, 0xc, 0xd, 0xe, 0xf, 1, 2, 3, 4, 5, 6, 7, 8, 9, 0};
unsigned char rx_buffer0[16] ;
char printBuf[100];

#define FLASH_DEVICE_ADDR 0xa0
/********************************************************************************************************
**������Ϣ ��int main (void)
**�������� ��
**������� ��
**������� ��
********************************************************************************************************/
int main(void)
{
    uart_initwBaudRate(115200);
    I2C1MasterDMATest();                                                        //I2C�շ�����

    while(1) {

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
**������Ϣ ��void I2CInitMasterMode(void) //unit is Khz
**�������� ����ʼ��I2C
**������� ��I2C_TypeDef *I2Cx��ѡ��I2C1,I2C2
**������� ����
********************************************************************************************************/
void I2C1MasterDMATest(void)
{
    unsigned int i;
    I2C_DeInit(I2C1);
    UartSendGroup((u8*)printBuf, sprintf(printBuf, "i2c1 dma test start\r\n"));

    I2CInitMasterMode();
    DMA_I2C1MasterWrite(FLASH_DEVICE_ADDR, 16 * 0, 16, tx_buffer0 );
    i = 1000000; while(i--); //���������ʱ
    for(i = 0; i < 16 ; i ++) {
        UartSendGroup((u8*)printBuf, sprintf(printBuf, "TX data is: %x \r\n", tx_buffer0[i]));
    }
    DMA_I2C1MasterRead(FLASH_DEVICE_ADDR, 16 * 0, 16, rx_buffer0 );

    for(i = 0; i < 16; i++) {
        UartSendGroup((u8*)printBuf, sprintf(printBuf, "RX data%d is  : %x \r\n", i, rx_buffer0[i]));
    }
    UartSendGroup((u8*)printBuf, sprintf(printBuf, "i2c1 dma test over\r\n"));
}

/********************************************************************************************************
**������Ϣ ��I2CInitMasterMode() //unit is Khz
**�������� ����ʼ��I2C
**������� ����
**������� ����
********************************************************************************************************/
void I2CInitMasterMode(void)
{
    I2C_InitTypeDef I2C_InitStructure;
    GPIO_InitTypeDef  GPIO_InitStructure;

    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE);                         //����GPIOA,GPIOBʱ��
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1, ENABLE);

    GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_8 | GPIO_Pin_9;                     //I2C1��ӳ��IO��
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_20MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;                               //�������߿��м�CLK&DATAΪ��
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    I2C_InitStructure.I2C_Mode = I2C_Mode_MASTER;                               //��ģʽ
    I2C_InitStructure.I2C_OwnAddress = FLASH_DEVICE_ADDR;
    I2C_InitStructure.I2C_Speed = I2C_Speed_STANDARD;
    I2C_InitStructure.I2C_ClockSpeed = 100000;
    I2C_Init(I2C1, &I2C_InitStructure);
    I2C_Cmd(I2C1, ENABLE);

    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_20MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD;                             //��Ҫ�������
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    GPIO_PinAFConfig(GPIOB, GPIO_PinSource8, GPIO_AF_1);
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource9, GPIO_AF_1);

}

/********************************************************************************************************
**������Ϣ ��DMA_I2C1MasterWrite(unsigned char device_id, unsigned short mem_byte_addr, unsigned short tx_count, unsigned char *tx_data )
**�������� ��I2C DMAд
**������� ��device_id,mem_byte_addr,tx_count,*tx_data
**������� ����
********************************************************************************************************/
void DMA_I2C1MasterWrite(unsigned char device_id, unsigned short mem_byte_addr, unsigned short tx_count, unsigned char *tx_data )
{

    I2C1_DMA_TX((u8*)&mem_byte_addr, 1);
    I2C1_DMA_TX(tx_data, tx_count);
    //  while(!I2C_GetFlagStatus(I2C1,I2C_STATUS_FLAG_TFE));
    while(!(I2C1->IC_STATUS & 0x4));
    I2C_GenerateSTOP( I2C1, ENABLE );
}


/********************************************************************************************************
**������Ϣ ��I2CSetDeviceAddr(I2C_TypeDef *I2Cx,unsigned char deviceaddr)
**�������� ��ѡ��ӻ���ַ
**������� ��I2C_TypeDef *I2Cx��ѡ��I2C1,I2C2;deviceaddr:�ӻ���ַ
**������� ����
********************************************************************************************************/
void I2CSetDeviceAddr(unsigned char deviceaddr)
{
    GPIO_InitTypeDef  GPIO_InitStructure;

    GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_8 | GPIO_Pin_9;                     //I2C1��ӳ��IO��
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_20MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    I2C_Cmd(I2C1, DISABLE);
    I2C_Send7bitAddress(I2C1, deviceaddr, I2C_Direction_Transmitter);
    I2C_Cmd(I2C1, ENABLE);

    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_20MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
}

/********************************************************************************************************
**������Ϣ ��I2CTXEmptyCheck(I2C_TypeDef *I2Cx)
**�������� ����鷢���жϱ�־λ
**������� ��I2C_TypeDef *I2Cx��ѡ��I2C1,I2C2
**������� ����
********************************************************************************************************/
void I2CTXEmptyCheck(I2C_TypeDef *I2Cx)
{
    while(1) {
        if(I2C_GetFlagStatus(I2Cx, I2C_FLAG_TX_EMPTY)) {
            break;
        }
    }
}

/********************************************************************************************************
**������Ϣ ��I2CTXEmptyCheck(I2C_TypeDef *I2Cx)
**�������� ���������жϱ�־λ
**������� ��I2C_TypeDef *I2Cx��ѡ��I2C1,I2C2
**������� ����
********************************************************************************************************/
void I2CRXFullCheck(I2C_TypeDef *I2Cx)
{

    while(1) {
        if(I2C_GetFlagStatus(I2Cx, I2C_FLAG_RX_FULL)) {
            break;
        }
    }
}

/********************************************************************************************************
**������Ϣ ��I2CTXByte(I2C_TypeDef *I2Cx,unsigned short cmd,unsigned char temp)
**�������� ��I2C��������
**������� ��I2C_TypeDef *I2Cx��ѡ��I2C1,I2C2��cmd��temp
**������� ����
********************************************************************************************************/
void I2CTXByte(I2C_TypeDef *I2Cx, unsigned short cmd, unsigned char temp)
{
    I2C_SendData(I2Cx, temp);
    I2CTXEmptyCheck(I2Cx);

}

/********************************************************************************************************
**������Ϣ ��I2CRXByte(I2C_TypeDef *I2Cx)
**�������� ��I2C��������
**������� ��I2C_TypeDef *I2Cx��ѡ��I2C1,I2C2
**������� ��temp
********************************************************************************************************/
unsigned char I2CRXByte(I2C_TypeDef *I2Cx)
{
    unsigned short temp;

    I2CRXFullCheck(I2Cx);

    temp = I2C_ReceiveData(I2Cx);
    return (unsigned char)temp;
}

/********************************************************************************************************
**������Ϣ ��I2C1_DMA_TX(unsigned char *tx_buf,unsigned short size)
**�������� ��I2C DMA����
**������� ��*tx_buf,size
**������� ��temp
********************************************************************************************************/
void I2C1_DMA_TX(unsigned char *tx_buf, unsigned short size)
{
    DMA_InitTypeDef DMA_InitStruct;
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
    DMA_DeInit(DMA1_Channel2);

    /* Initialize the DMA_PeripheralBaseAddr member */
    DMA_InitStruct.DMA_PeripheralBaseAddr = I2C1_BASE + 0x10;                   //I2C���շ����ݵ�ַ
    /* Initialize the DMA_MemoryBaseAddr member */
    DMA_InitStruct.DMA_MemoryBaseAddr = (unsigned int)tx_buf;
    /* Initialize the DMA_DIR member */
    DMA_InitStruct.DMA_DIR = DMA_DIR_PeripheralDST;
    /* Initialize the DMA_BufferSize member */
    DMA_InitStruct.DMA_BufferSize = size;
    /* Initialize the DMA_PeripheralInc member */
    DMA_InitStruct.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
    /* Initialize the DMA_MemoryInc member */
    DMA_InitStruct.DMA_MemoryInc = DMA_MemoryInc_Enable ;
    /* Initialize the DMA_PeripheralDataSize member */
    DMA_InitStruct.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Word;
    /* Initialize the DMA_MemoryDataSize member */
    DMA_InitStruct.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
    /* Initialize the DMA_Mode member */
    DMA_InitStruct.DMA_Mode = DMA_Mode_Normal;
    /* Initialize the DMA_Priority member */
    DMA_InitStruct.DMA_Priority = DMA_Priority_Low;
    /* Initialize the DMA_M2M member */
    DMA_InitStruct.DMA_M2M = DMA_M2M_Disable;
    DMA_Init(DMA1_Channel2, &DMA_InitStruct);//TX
    DMA_Cmd(DMA1_Channel2, ENABLE);

    I2C1->IC_DMA_CR |= TDMAE_SET;                                               //ʹ��DMA����
    while(!DMA_GetFlagStatus(DMA1_FLAG_TC2));                                   //�ȴ�DMA�������
    DMA_Cmd(DMA1_Channel2, DISABLE);
    I2C1->IC_DMA_CR &= ~TDMAE_SET;                                              //�ر�DMA����
}



/*******************************************************************************************************
**������Ϣ ��I2C1_DMA_RX
**�������� ��
**������� ��
**������� ����
******************************************************************************************************/
void I2C1_DMA_RX(unsigned char *rx_buf, unsigned short size)
{

    unsigned int ui_Tdata = 0x100;
    DMA_InitTypeDef DMA_InitStruct;
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);

    I2C1->IC_DMA_CR |= RDMAE_SET | TDMAE_SET; //ʹ��DMA�շ�

    DMA_DeInit(DMA1_Channel3);

    /* Initialize the DMA_PeripheralBaseAddr member */
    DMA_InitStruct.DMA_PeripheralBaseAddr = I2C1_BASE + 0x10;
    /* Initialize the DMA_MemoryBaseAddr member */
    DMA_InitStruct.DMA_MemoryBaseAddr = (unsigned int)rx_buf;
    /* Initialize the DMA_DIR member */
    DMA_InitStruct.DMA_DIR = DMA_DIR_PeripheralSRC;
    /* Initialize the DMA_BufferSize member */
    DMA_InitStruct.DMA_BufferSize = size;
    /* Initialize the DMA_PeripheralInc member */
    DMA_InitStruct.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
    /* Initialize the DMA_MemoryInc member */
    DMA_InitStruct.DMA_MemoryInc = DMA_MemoryInc_Enable ;
    /* Initialize the DMA_PeripheralDataSize member */
    DMA_InitStruct.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
    /* Initialize the DMA_MemoryDataSize member */
    DMA_InitStruct.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
    /* Initialize the DMA_Mode member */
    DMA_InitStruct.DMA_Mode = DMA_Mode_Normal;
    /* Initialize the DMA_Priority member */
    DMA_InitStruct.DMA_Priority = DMA_Priority_Low;
    /* Initialize the DMA_M2M member */
    DMA_InitStruct.DMA_M2M = DMA_M2M_Disable;
    DMA_Init(DMA1_Channel3, &DMA_InitStruct);//RX
    DMA_Cmd(DMA1_Channel3, ENABLE);

    DMA_DeInit(DMA1_Channel2);

    /* Initialize the DMA_PeripheralBaseAddr member *///����DMA���͸���I2C������
    DMA_InitStruct.DMA_PeripheralBaseAddr = I2C1_BASE + 0x10;
    /* Initialize the DMA_MemoryBaseAddr member */
    DMA_InitStruct.DMA_MemoryBaseAddr = (unsigned int)&ui_Tdata;
    /* Initialize the DMA_DIR member */
    DMA_InitStruct.DMA_DIR = DMA_DIR_PeripheralDST;
    /* Initialize the DMA_BufferSize member */
    DMA_InitStruct.DMA_BufferSize = size;
    /* Initialize the DMA_PeripheralInc member */
    DMA_InitStruct.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
    /* Initialize the DMA_MemoryInc member */
    DMA_InitStruct.DMA_MemoryInc = DMA_MemoryInc_Disable  ;
    /* Initialize the DMA_PeripheralDataSize member */
    DMA_InitStruct.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Word;
    /* Initialize the DMA_MemoryDataSize member */
    DMA_InitStruct.DMA_MemoryDataSize = DMA_MemoryDataSize_Word;
    /* Initialize the DMA_Mode member */
    DMA_InitStruct.DMA_Mode = DMA_Mode_Normal;
    /* Initialize the DMA_Priority member */
    DMA_InitStruct.DMA_Priority = DMA_Priority_Low;
    /* Initialize the DMA_M2M member */
    DMA_InitStruct.DMA_M2M = DMA_M2M_Disable;
    DMA_Init(DMA1_Channel2, &DMA_InitStruct);//TX
    DMA_Cmd(DMA1_Channel2, ENABLE);

    while(!DMA_GetFlagStatus(DMA1_FLAG_TC3)) { //�ȴ�DMA�������,Ҳ�����ж�ʵ��
        //I2C1->IC_DATA_CMD = 0x100;
    }
    DMA_Cmd(DMA1_Channel3, DISABLE);
    DMA_Cmd(DMA1_Channel2, DISABLE);
    I2C1->IC_DMA_CR &= ~(RDMAE_SET | TDMAE_SET); //�ر�I2C_DMA����
}

/*******************************************************************************************************
**������Ϣ ��DMA_I2C1MasterRead(unsigned char device_id, unsigned short mem_byte_addr, unsigned short rx_count, unsigned char *rx_data )
**�������� ��I2C DMA ��
**������� ��
**������� ����
******************************************************************************************************/
void DMA_I2C1MasterRead(unsigned char device_id, unsigned short mem_byte_addr, unsigned short rx_count, unsigned char *rx_data )
{

    I2C1_DMA_TX((u8*)&mem_byte_addr, 1);
    I2C1_DMA_RX(rx_data, rx_count);
    while(!(I2C1->IC_STATUS & 0x4));
    I2C_GenerateSTOP( I2C1, ENABLE );
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
    UART_SendData( UART1, dat);
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

