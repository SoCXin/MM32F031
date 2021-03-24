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
void I2CMasterTest(I2C_TypeDef *I2Cx);
void I2CInitMasterMode(void);
void I2CMasterWrite(I2C_TypeDef *I2Cx, unsigned char device_id, unsigned short mem_byte_addr, unsigned short tx_count, unsigned char *tx_data );
void I2CMasterRead(I2C_TypeDef *I2Cx, unsigned char device_id, unsigned short mem_byte_addr, unsigned short rx_count, unsigned char *rx_data );
void UartSendGroup(u8* buf, u16 len);
void I2CTXByte(I2C_TypeDef *I2Cx, unsigned short cmd, unsigned char temp);
void I2CTXEmptyCheck(I2C_TypeDef *I2Cx);
unsigned char I2CRXByte(I2C_TypeDef *I2Cx);
void I2CRXFullCheck(I2C_TypeDef *I2Cx);

#define FLASH_DEVICE_ADDR 0xa8
char printBuf[100];
unsigned char tx_buffer0[16] = {0x55, 1, 2, 3, 4, 5, 6, 7, 8, 9, 0xa, 0xb, 0xc, 0xd, 0xe, 0xf};
unsigned char rx_buffer0[16] ;

/********************************************************************************************************
**������Ϣ ��main(void)
**�������� ��
**������� ��
**������� ��
**    ��ע ��
********************************************************************************************************/
int main(void)
{
    uart_initwBaudRate(115200);
    I2CMasterTest(I2C1);                                                        //I2C�շ�����
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
**������Ϣ ��I2CMasterTest(I2C_TypeDef *I2Cx)
**�������� ��I2C���շ������ݲ��Գ���
**������� ��I2C_TypeDef *I2Cx��ѡ��I2C1,I2C2
**������� ����
********************************************************************************************************/
unsigned char rxBuffer[16] = {0};

void I2CMasterTest(I2C_TypeDef *I2Cx)
{
    unsigned char i;

    I2CInitMasterMode();


    I2CMasterWrite(I2C1, FLASH_DEVICE_ADDR, 16 * 1, 16, tx_buffer0 );

    for(i = 0; i < 16 ; i ++) {
        UartSendGroup((u8*)printBuf, sprintf(printBuf, "TX data is: %x \r\n", tx_buffer0[i]));
    }

    I2CMasterRead(I2C1, FLASH_DEVICE_ADDR, 16 * 1, 16, rx_buffer0 );

    for(i = 0; i < 16; i++) {
        UartSendGroup((u8*)printBuf, sprintf(printBuf, "RX data%d is  : %x \r\n", i, rx_buffer0[i]));
    }

}

/********************************************************************************************************
**������Ϣ ��I2CInitMasterMode(void) //unit is Khz
**�������� ����ʼ��I2C
**������� ��I2C_TypeDef *I2Cx��ѡ��I2C1,I2C2
**������� ����
********************************************************************************************************/
void I2CInitMasterMode(void)
{
    I2C_InitTypeDef I2C_InitStructure;
    GPIO_InitTypeDef  GPIO_InitStructure;
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE);                         //����GPIOBʱ��

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1, ENABLE);

    GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_8 | GPIO_Pin_9;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_20MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    I2C_InitStructure.I2C_Mode = I2C_Mode_MASTER;
    I2C_InitStructure.I2C_OwnAddress = FLASH_DEVICE_ADDR;
    I2C_InitStructure.I2C_Speed = I2C_Speed_STANDARD;
    I2C_InitStructure.I2C_ClockSpeed = 100000;
    I2C_Init(I2C1, &I2C_InitStructure);
    I2C_Cmd(I2C1, ENABLE);

    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_20MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    GPIO_PinAFConfig(GPIOB, GPIO_PinSource8, GPIO_AF_1);
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource9, GPIO_AF_1);

}

/********************************************************************************************************
**������Ϣ ��I2CMasterWrite(I2C_TypeDef *I2Cx,unsigned char device_id, unsigned short mem_byte_addr, unsigned short tx_count, unsigned char *tx_data )
**�������� ��I2C��������
**������� ��I2C_TypeDef *I2Cx��ѡ��I2C1,I2C2��cmd��temp
**������� ����
********************************************************************************************************/
void I2CMasterWrite(I2C_TypeDef *I2Cx, unsigned char device_id, unsigned short mem_byte_addr, unsigned short tx_count, unsigned char *tx_data )
{
    unsigned short temp;
    unsigned short i;
    unsigned char *p;
    uint32_t j;
    p = tx_data;

    I2C_Cmd(I2Cx, DISABLE);
    I2C_Send7bitAddress(I2Cx, FLASH_DEVICE_ADDR, I2C_Direction_Transmitter);
    I2C_Cmd(I2Cx, ENABLE);
    temp = ((mem_byte_addr) & 0xff);
    I2CTXByte(I2Cx, CMD_WRITE, temp);                                           //tx memory addr

    for(i = 0; i < tx_count; i++) {
        temp = *p;
        p++;
        if(i == (tx_count - 1))
            I2Cx->IC_DATA_CMD = temp | 0x200;                                   //muaul set stop flag
        else
            I2CTXByte(I2Cx, CMD_WRITE, temp);                                   //tx data
    }

    for(j = 0; j < 0x5000; j++);

}

/********************************************************************************************************
**������Ϣ ��I2CMasterRead(I2C_TypeDef *I2Cx,unsigned char device_id, unsigned short mem_byte_addr, unsigned short rx_count, unsigned char *rx_data )
**�������� ��I2C��������
**������� ��I2C_TypeDef *I2Cx��ѡ��I2C1,I2C2��device_id��mem_byte_addr��rx_count��rx_data
**������� ����
********************************************************************************************************/
void I2CMasterRead(I2C_TypeDef *I2Cx, unsigned char device_id, unsigned short mem_byte_addr, unsigned short rx_count, unsigned char *rx_data )
{
    unsigned char temp = 0;
    unsigned short i;

    I2C_Cmd(I2Cx, DISABLE);
    I2C_Send7bitAddress(I2Cx, FLASH_DEVICE_ADDR, I2C_Direction_Transmitter);

    I2C_Cmd(I2Cx, ENABLE);

    temp = ((mem_byte_addr) & 0xff);

    I2CTXByte(I2Cx, CMD_WRITE, temp); //tx memory addr


    for(i = 0; i < rx_count; i++) {
        rx_data[i] = I2CRXByte(I2Cx);


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

