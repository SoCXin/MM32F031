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
void SPIM_Test(SPI_TypeDef* SPIx);
void UartSendGroup(u8* buf, u16 len);
void SPIM_Init(unsigned short spi_baud_div);
void SPIM_ReadID(SPI_TypeDef* SPIx);
void SPIM_SectorErase(SPI_TypeDef* SPIx, unsigned long address);
void SPIM_PageProgram(SPI_TypeDef* SPIx, unsigned long address, unsigned char *p, unsigned int number);
void SPIM_PageRead(SPI_TypeDef* SPIx, unsigned long address, unsigned char *p, unsigned int number);
void SPIM_checkStatus(SPI_TypeDef* SPIx);
void SPIM_WriteEnable(SPI_TypeDef* SPIx);
void SPIM_CSHigh(void);
void SPIM_CSLow(void);
void SPIM_TXEn(void);
void SPIM_RXEn(void);
unsigned int SPIMReadWriteByte(SPI_TypeDef* SPIx, unsigned char tx_data);

char printBuf[100];
unsigned char tmpdata[256];
unsigned char rxtmpdata[256];
volatile unsigned char spi0_rx_flag;
volatile unsigned char spi0_tx_flag;

#define READ        	0x03
#define FAST_READ   	0x0B
#define RDID        	0x9F
#define WREN            0x06
#define WRDI            0x04
#define SE              0xD8
#define BE              0xC7
#define PP              0x02
#define RDSR            0x05
#define WRSR            0x01
#define DP              0xB9
#define RES             0xAB

/********************************************************************************************************
**������Ϣ ��int main (void)
**�������� �������󣬴������ִ�ӡ256ҳ����
**������� ��
**������� ��
**    ��ע ��ע�⣺�ı�SPI1��SP2�������϶�Ӧ��·ñҲӦ�ı�
********************************************************************************************************/
int main(void)
{
    uart_initwBaudRate(115200);
    SPIM_Test(SPI2);
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
**������Ϣ ��SPIM_Test(void)
**�������� :���Գ���ʹ�ô��ڴ�ӡ256ҳ����
**������� ����
**������� ����
********************************************************************************************************/
void SPIM_Test(SPI_TypeDef* SPIx)
{
    unsigned int i;
    for(i = 0; i < 256; i++) {
        tmpdata[i] = i + 2;
    }

    UartSendGroup((u8*)printBuf, sprintf(printBuf, "SPI2 test\r\n"));
    SPIM_Init(0x8);

    SPIM_ReadID(SPIx);

    SPIM_SectorErase(SPIx, 0);

    SPIM_PageProgram(SPIx, 0, tmpdata, 256);

    for(i = 0; i < 8; i++) {
        rxtmpdata[i] = 0x0;
    }
    SPIM_PageRead(SPIx, 0, rxtmpdata, 256);                                     //��д��ȥ��һҳ256�ֽڶ�����

    for(i = 0; i < 8; i++) {
        UartSendGroup((u8*)printBuf, sprintf(printBuf, "rx[%d]=0x%x\r\n", i, rxtmpdata[i]));
    }

    UartSendGroup((u8*)printBuf, sprintf(printBuf, "SPI2 test over\r\n"));

}

/********************************************************************************************************
**������Ϣ ��SPIM_PageRead(unsigned long address,unsigned char *p,unsigned int number)
**�������� :��ȡ256ҳ����
**������� ��address  ;*p ;number
**������� ����
********************************************************************************************************/
void SPIM_PageRead(SPI_TypeDef* SPIx, unsigned long address, unsigned char *p, unsigned int number) //page = 256 bytes
{
    unsigned char addr0, addr1, addr2;
    unsigned int i;
    address = address & 0xffffff00;                                             //page address
    addr0 = (unsigned char)(address >> 16);
    addr1 = (unsigned char)(address >> 8);
    addr2 = (unsigned char)address;

    SPIM_CSLow();                                                               //Spi cs assign to this pin,select

    SPIMReadWriteByte(SPIx, READ);
    SPIMReadWriteByte(SPIx, addr0);
    SPIMReadWriteByte(SPIx, addr1);
    SPIMReadWriteByte(SPIx, addr2);

    for(i = 0; i < 256; i++) {
        rxtmpdata[i] = SPIMReadWriteByte(SPIx, 0x00);
    }

    SPIM_CSHigh();                                                              //Spi cs release
}

/********************************************************************************************************
**������Ϣ ��SPIM_PageProgram(unsigned long address,unsigned char *p,unsigned int number)
**�������� :����256ҳ����
**�������   address;*p;number
**������� ����
********************************************************************************************************/
void SPIM_PageProgram(SPI_TypeDef* SPIx, unsigned long address, unsigned char *p, unsigned int number)
{
    unsigned int j;
    unsigned char addr0, addr1, addr2;
    address = address & 0xffffff00;                                             //page address
    addr0 = (unsigned char)(address >> 16);
    addr1 = (unsigned char)(address >> 8);
    addr2 = (unsigned char)address;

    SPIM_WriteEnable(SPIx);
    SPIM_CSLow();                                                               //Spi cs assign to this pin,select
    SPIMReadWriteByte(SPIx, PP);
    SPIMReadWriteByte(SPIx, addr0);
    SPIMReadWriteByte(SPIx, addr1);
    SPIMReadWriteByte(SPIx, addr2);
    for(j = 0; j < number; j++) {
        SPIMReadWriteByte(SPIx, *(p++));
    }

    SPIM_CSHigh();                                                              //Spi cs release

    SPIM_checkStatus(SPIx);
}

/********************************************************************************************************
**������Ϣ ��SPIM_SectorErase()
**�������� :��������
**������� ��unsigned long address,ɾ��ָ��sector�ĵ�ַ each sector = 64Kbytes
**������� ����
********************************************************************************************************/
void SPIM_SectorErase(SPI_TypeDef* SPIx, unsigned long address)
{
    unsigned char addr0, addr1, addr2;
    address = address & 0xffff0000;
    addr0 = ((unsigned char)(address >> 16)) & 0xff;
    addr1 = ((unsigned char)(address >> 8)) & 0xff;
    addr2 = ((unsigned char)address) & 0xff;

    SPIM_WriteEnable(SPIx);

    SPIM_CSLow();                                                               //Spi cs assign to this pin,select

    SPIMReadWriteByte(SPIx, SE);
    SPIMReadWriteByte(SPIx, addr0);
    SPIMReadWriteByte(SPIx, addr1);
    SPIMReadWriteByte(SPIx, addr2);
    SPIM_CSHigh();                                                              //Spi cs release

    SPIM_checkStatus(SPIx);
}

/********************************************************************************************************
**������Ϣ ��SPIMReadWriteByte(unsigned char tx_data)
**�������� : ͨ������ SPIx �շ����� ,����ȫ˫��ģʽ(ͬʱ�շ�)
**������� ��tx_data
**������� ����
********************************************************************************************************/
unsigned int SPIMReadWriteByte(SPI_TypeDef* SPIx, unsigned char tx_data)
{
    SPI_SendData(SPIx, tx_data);
    while(1) {
        if(spi0_tx_flag == 1) {
            spi0_tx_flag = 0;
            break;
        }
    }
    while (1) {
        if(spi0_rx_flag == 1) {
            spi0_rx_flag = 0;
            return SPI_ReceiveData(SPIx);
        }
    }


}
/********************************************************************************************************
**������Ϣ ��SPIM_Init(unsigned short spi_baud_div)
**�������� :���޸Ĳ�����ʼ��SPI
**������� ��spi_baud_div
**������� ����
********************************************************************************************************/
void SPIM_Init(unsigned short spi_baud_div)
{
    SPI_InitTypeDef SPI_InitStructure;
    GPIO_InitTypeDef  GPIO_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2, ENABLE);                        //SPI2 clk enable
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA | RCC_AHBPeriph_GPIOB, ENABLE);

    GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_12;                                 //spi2_cs  pb12
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_13;                                 //spi2_sck  pb13
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_15;                                 //spi2_mosi  pb15
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_14;                                 //spi2_miso  pb14
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;                               //��������
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    GPIO_PinAFConfig(GPIOB, GPIO_PinSource12, GPIO_AF_0);                       //��Ҫ��SPI����������IO�ڸ��ó������ģ�Ĭ����AF0��
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource13, GPIO_AF_0);
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource14, GPIO_AF_0);
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource15, GPIO_AF_0);

    SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
    SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
    SPI_InitStructure.SPI_DataWidth = SPI_DataWidth_8b;
    SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;
    SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;
    SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
    SPI_InitStructure.SPI_BaudRatePrescaler = spi_baud_div;
    SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
    SPI_Init(SPI2, &SPI_InitStructure);


    NVIC_InitStructure.NVIC_IRQChannel = SPI2_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPriority = 2;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);


    spi0_rx_flag = 0;
    spi0_tx_flag = 0;
    SPI_ITConfig(SPI2, SPI_IT_RX | SPI_IT_TXEPT, ENABLE);
    SPIM_TXEn();
    SPIM_RXEn();
    SPI_Cmd(SPI2, ENABLE);                                                      //Enables the specified SPI peripheral SPIʹ�ܡ�����ģʽ 8λ����ģʽ SPI �Ĳ�����

}

/********************************************************************************************************
**������Ϣ ��SPIM_ReadID(void)
**�������� :��ȡID
**������� ����
**������� ����
********************************************************************************************************/
void SPIM_ReadID(SPI_TypeDef* SPIx)
{
    unsigned char temp;
    unsigned int i;

    SPIM_CSLow();                                                               //Spi cs assign to this pin,select
    SPIMReadWriteByte(SPIx, RDID);

    for(i = 0; i < 3; i++) {
        temp = SPIMReadWriteByte(SPIx, 0x01);
        UartSendGroup((u8*)printBuf, sprintf(printBuf, "temp=0x%x\r\n", temp));
    }
    SPIM_CSHigh();                                                              //Spi cs release
}

/********************************************************************************************************
**������Ϣ ��SPIM_WriteEnable(void)
**�������� : д����ʹ��
**������� ����
**������� ����
********************************************************************************************************/
void SPIM_WriteEnable(SPI_TypeDef* SPIx)
{
    SPIM_CSLow();                                                               //Spi cs assign to this pin,select
    SPIMReadWriteByte(SPIx, WREN);
    SPIM_CSHigh();                                                              //Spi cs release
}

/********************************************************************************************************
**������Ϣ ��SSPIM_checkStatus(void)
**�������� :������ݷ����Ƿ���ȷ
**������� ����
**������� ����
********************************************************************************************************/
void SPIM_checkStatus(SPI_TypeDef* SPIx)
{
    unsigned char temp;
    SPIM_CSLow();                                                               //Spi cs assign to this pin,select
    SPIMReadWriteByte(SPIx, RDSR);
    while(1) {
        temp = SPIMReadWriteByte(SPIx, 0x00);
        if((temp & 0x01) == 0x0)
            break;
    }
    SPIM_CSHigh();                                                              //Spi cs release
}

/********************************************************************************************************
**������Ϣ ��SPI2_IRQHandler (void)
**�������� : SPI2�жϺ�������ȡSPI2���ͽ����жϱ�־λ
**������� ����
**������� ����
********************************************************************************************************/
void SPI2_IRQHandler (void)
{
    while(1) {
        if(SPI_GetITStatus(SPI2, SPI_IT_TXEPT)) {
            SPI_ClearITPendingBit(SPI2, SPI_IT_TXEPT);
            spi0_tx_flag = 1;
            break;
        }

        if(SPI_GetITStatus(SPI2, SPI_IT_RX)) {
            SPI_ClearITPendingBit(SPI2, SPI_IT_RX);                             //clear rx interrupt
            spi0_rx_flag = 1;
            break;
        }
    }
}


/********************************************************************************************************
**������Ϣ ��SPIM_CSLow()
**�������� :Ϊѡ����SPI ��������ڲ�NSS �ܽ�
**������� ����
**������� ����
********************************************************************************************************/
void SPIM_CSLow(void)
{
    //Spi cs assign to this pin,select
    SPI_CSInternalSelected(SPI2, SPI_CS_BIT0, ENABLE);
}

/********************************************************************************************************
**������Ϣ ��SPIM_CSHigh()
**�������� :Ϊѡ����SPI ��������ڲ�NSS �ܽ�
**������� ����
**������� ����
********************************************************************************************************/
void SPIM_CSHigh(void)
{
    //Spi cs release
    SPI_CSInternalSelected(SPI2, SPI_CS_BIT0, DISABLE);
}

/********************************************************************************************************
**������Ϣ ��SPIM_TXEn()
**�������� :�ر� SPI ��˫��ģʽ�µ����ݴ��䷽��
**������� : ��
**������� ����
********************************************************************************************************/
void SPIM_TXEn(void)
{
    //Transmit Enable bit TXEN
    SPI_BiDirectionalLineConfig(SPI2, SPI_Direction_Tx);
}


/********************************************************************************************************
**������Ϣ ��SPIM_RXEn()
**�������� :�ر� SPI ��˫��ģʽ�µ����ݴ��䷽��
**������� ����
**������� ����
********************************************************************************************************/
void SPIM_RXEn(void)
{
    //enable RXEN
    SPI_BiDirectionalLineConfig(SPI2, SPI_Direction_Rx);
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

