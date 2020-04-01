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
#include "string.h"

typedef enum {
    SPIMODE0,
    SPIMODE3
} SPIMODDE;
//#define SPIUSELSB
void uart_initwBaudRate(u32 bound);
void SPIM_Test(void);
void UartSendGroup(u8* buf, u16 len);
void SPIM_Init(unsigned short spi_baud_div);
void SPIM_ReadID(void);
void SPIM_SectorErase(unsigned long address);
void SPIM_PageProgram(unsigned long address, unsigned char *p, unsigned int number);
void SPIM_PageRead(unsigned long address, unsigned char *p, unsigned int number);
void SPIM_checkStatus(void);
void SPIM_WriteEnable(void);
void SPIM_CSHigh(void);
void SPIM_CSLow(void);
void SPIM_TXEn(SPI_TypeDef* SPIx);
void SPIM_RXEn(SPI_TypeDef* SPIx);
unsigned int SPIMReadWriteByte(unsigned char tx_data);

char printBuf[100];
unsigned char tmpdata[256];
unsigned char rxtmpdata[256];

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

void SPI1MasterPinCfgMode1(void)
{

    GPIO_InitTypeDef  GPIO_InitStructure;


    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);

    GPIO_PinAFConfig(GPIOA, GPIO_PinSource4, GPIO_AF_0);
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource5, GPIO_AF_0);
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource6, GPIO_AF_0);
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource7, GPIO_AF_0);

    GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_4;   		//spi1_cs  pa4
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; 	// ���⸴�����
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_5;   		//spi1_sck  pa5
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; 	// ���⸴�����
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_7;   		//spi1_mosi  pa7
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; 	// ���⸴�����
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_6;  		//spi1_miso  pa6
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; 		//��������
    GPIO_Init(GPIOA, &GPIO_InitStructure);

}
void SPI2MasterPinCfgMode1(void)
{
    GPIO_InitTypeDef  GPIO_InitStructure;

    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE);

    GPIO_PinAFConfig(GPIOB, GPIO_PinSource12, GPIO_AF_0);
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource13, GPIO_AF_0);
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource14, GPIO_AF_0);
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource15, GPIO_AF_0);

    GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_12;   		                        //spi2_cs  pb12
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; 	                        // ���⸴�����
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_13;   		                        //spi2_sck  pb13
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; 	                        // ���⸴�����
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_15;   		                        //spi2_mosi  pb15
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; 	                        // ���⸴�����
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_14;  		                        //spi2_miso  pb14
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; 		                        //��������
    GPIO_Init(GPIOB, &GPIO_InitStructure);

}
void SPI2MasterPinCfgMode2(void)
{
    GPIO_InitTypeDef  GPIO_InitStructure;

    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE);

    GPIO_PinAFConfig(GPIOB, GPIO_PinSource12, GPIO_AF_1);
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource13, GPIO_AF_1);
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource14, GPIO_AF_1);
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource15, GPIO_AF_1);


    GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_12;                                 //spi2_cs  pb12
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;                             // ���⸴�����
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_13;                                 //spi2_sck  pb13
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;                               // ���⸴�����
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_15;                                 //spi2_mosi  pb15
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;                             // ���⸴�����
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_14;                                 //spi2_miso  pb14
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;                             //��������
    GPIO_Init(GPIOB, &GPIO_InitStructure);

}
void SPI2MasterPinCfgMode3(void)
{
    GPIO_InitTypeDef  GPIO_InitStructure;

    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE);
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource12, GPIO_AF_3);
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource13, GPIO_AF_3);
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource14, GPIO_AF_3);
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource15, GPIO_AF_3);

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2, ENABLE);                        //SPI2 clk enable
    GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_12;                                 //spi2_cs  pb12
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;                             // ���⸴�����
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_13;                                 //spi2_sck  pb13
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;                             // ���⸴�����
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_15;                                 //spi2_mosi  pb15
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;                               // ���⸴�����
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_14;                                 //spi2_miso  pb14
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;                             //��������
    GPIO_Init(GPIOB, &GPIO_InitStructure);
}
void SPI2MasterPinCfgMode4(void)
{
    GPIO_InitTypeDef  GPIO_InitStructure;

    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE);
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource12, GPIO_AF_4);
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource13, GPIO_AF_4);
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource14, GPIO_AF_4);
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource15, GPIO_AF_4);

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2, ENABLE);                        //SPI2 clk enable
    GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_12;                                 //spi2_cs  pb12
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;                               // ���⸴�����
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_13;                                 //spi2_sck  pb13
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;                             // ���⸴�����
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_15;                                 //spi2_mosi  pb15
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;                             // ���⸴�����
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_14;                                 //spi2_miso  pb14
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;                             //��������
    GPIO_Init(GPIOB, &GPIO_InitStructure);
}
/********************************************************************************************************
**������Ϣ ��SPIM_TXEn()
**�������� :�ر� SPI ��˫��ģʽ�µ����ݴ��䷽��
**������� : ��
**������� ����
********************************************************************************************************/
void SPIM_TXEn(SPI_TypeDef* SPIx)
{
    //Transmit Enable bit TXEN
    SPI_BiDirectionalLineConfig(SPIx, SPI_Direction_Tx);
}
/********************************************************************************************************
**������Ϣ ��SPIM_RXEn()
**�������� :�ر� SPI ��˫��ģʽ�µ����ݴ��䷽��
**������� ����
**������� ����
********************************************************************************************************/
void SPIM_RXEn(SPI_TypeDef* SPIx)
{
    //enable RXEN
    SPI_BiDirectionalLineConfig(SPIx, SPI_Direction_Rx);
}

/********************************************************************************************************
**������Ϣ ��SPIM_CSLow()
**�������� :Ϊѡ����SPI ��������ڲ�NSS �ܽ�
**������� ����
**������� ����
********************************************************************************************************/
void SPIM_CSLow()
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
void SPIM_CSHigh()
{
    //Spi cs release
    SPI_CSInternalSelected(SPI2, SPI_CS_BIT0, DISABLE);
}


/**
* @brief  Initializes the SPIx peripheral according to the specified
*   parameters in the SPI_InitStruct.
* @param SPIx: where x can be 0, 1 to select the SPI peripheral.
* @param SPI_InitStruct: pointer to a SPI_InitTypeDef structure that
*   contains the configuration information for the specified
*   SPI peripheral.
* @retval : None
*/
void SPI_Initxbit(SPI_TypeDef *SPIx, SPI_InitTypeDef *SPI_InitStruct)
{
    uint32_t tmpreg = 0;


    /*---------------------------- SPIx GCTL Configuration -------------------*/
    /* Get the SPIx GCTL value */
    tmpreg = SPIx->GCTL;
    /* Clear csn_sel, dmamode, txtlf, rxtlf,data_sel, rxen, txen, mm, int_en, spien bits */
    tmpreg &= ((uint16_t)0xF000);
    /* Configure SPIx: direction, NSS management, first transmitted bit, BaudRate prescaler
    master/salve mode, CPOL and CPHA */
    /* Set dat_sel bits according to SPI_DataSize value */
    /* Set csn and csn_sel bits according to SPI_NSS value */
    /* Set mm bit according to SPI_Mode value */
    tmpreg |= (uint32_t)((uint32_t)SPI_InitStruct->SPI_DataSize | SPI_InitStruct->SPI_NSS |
                         SPI_InitStruct->SPI_Mode);
    /* Write to SPIx GCTL */
    SPIx->GCTL = tmpreg;
    /*---------------------------- SPIx CCTL Configuration -------------------*/
    tmpreg = SPIx->CCTL;
    /* Clear spilen, lsbfe, CPOL, CPHA bits */
    tmpreg &= ((uint16_t)0xFFC0);
    /* Set Spilen bit according to SPI_DataWidth value */
    /* Set LSBFirst bit according to SPI_FirstBit value */
    /* Set CPOL bit according to SPI_CPOL value */
    /* Set CPHA bit according to SPI_CPHA value */
    tmpreg |= (uint16_t)(SPI_InitStruct->SPI_FirstBit | SPI_InitStruct->SPI_CPOL |
                         SPI_InitStruct->SPI_CPHA);

    /* Write to SPIx CCTL */
    SPIx->CCTL = tmpreg | 0x18;

    /*---------------------------- SPIx SPBRG Configuration ------------------*/
    tmpreg = SPIx->SPBRG;
    /* Clear spbrg bits */
    tmpreg &= (uint16_t)((uint16_t)0x0000);
    /* Set BR bits according to SPI_BaudRatePrescaler value */
    tmpreg |= (uint16_t)SPI_InitStruct->SPI_BaudRatePrescaler;
    /* Write to SPIx SPBRG */
    SPIx->SPBRG = tmpreg;

//    if ((SPI_InitStruct->SPI_DataWidth) != SPI_DataWidth_8b) {
//        SPIx->CCTL |= 1 << 2; //lsbfe
    SPIx->CCTL |= 1 << 3; //spilen
//    }
    SPIx->EXTCTL = SPI_InitStruct->SPI_DataWidth;
}



/********************************************************************************************************
**������Ϣ ��SPIM_Init(SPI_TypeDef* SPIx, unsigned short spi_baud_div)
**�������� :���޸Ĳ�����ʼ��SPI
**������� ��SPI_TypeDef* SPIx,��ѡ��SPI1,SPI2  ;spi_baud_div
**������� ����
********************************************************************************************************/
void SPIM_Initxbit(SPI_TypeDef* SPIx, unsigned short spi_baud_div, u8 datawidth, SPIMODDE mode)
{
    SPI_InitTypeDef SPI_InitStructure;

    if(datawidth >= 32)
        datawidth = 0;


    if(SPIx == SPI1) {
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);                    //SPI1 clk enable
        SPI1MasterPinCfgMode1();
    }

    if(SPIx == SPI2) {
        RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2, ENABLE);                    //SPI2 clk enable
        SPI2MasterPinCfgMode1();
    }

    SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
    SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;                           //Always 8 bit
    SPI_InitStructure.SPI_DataWidth = datawidth;
    if(SPIMODE3 == mode) {
        SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;                                 // mode0 SPI_CPOL_Low, SPI_CPHA_1Edge;
        SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;                                // mode3 SPI_CPOL_High,SPI_CPHA_2Edge
    } else {                                                                    //SPIMODE0
        SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;                              // mode0 SPI_CPOL_Low, SPI_CPHA_1Edge;
        SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;                            // mode3 SPI_CPOL_High,SPI_CPHA_2Edge
    }
    SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
    SPI_InitStructure.SPI_BaudRatePrescaler = spi_baud_div;

#ifndef SPIUSELSB
    SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
#else
    SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_LSB;
#endif
    SPI_Initxbit(SPIx, &SPI_InitStructure);
    if ((SPI_InitStructure.SPI_DataWidth) != SPI_DataWidth_8b) {
        SPIx->CCTL |= 1 << 2; //lsbfe
        SPIx->CCTL |= 1 << 3; //spilen
    }
    SPI_Cmd(SPIx, ENABLE);                                                      //Enables the specified SPI peripheral SPIʹ�ܡ�����ģʽ 8λ����ģʽ SPI �Ĳ�����
    SPIM_TXEn(SPIx);
    SPIM_RXEn(SPIx);
}



#if 1

u32 Reverse32Bit(u32 n)
{
    n = (n & 0x55555555) << 1 | (n & 0xAAAAAAAA) >> 1;
    n = (n & 0x33333333) << 2 | (n & 0xCCCCCCCC) >> 2;
    n = (n & 0x0F0F0F0F) << 4 | (n & 0xF0F0F0F0) >> 4;
    n = (n & 0x00FF00FF) << 8 | (n & 0xFF00FF00) >> 8;
    n = (n & 0x0000FFFF) << 16 | (n & 0xFFFF0000) >> 16;
    return n;
}

/**
* @brief  Transmits a Data through the SPIx peripheral.
* @param SPIx: where x can be :
*   0, 1 in SPI mode
* @param Data : Data to be transmitted..
* @retval : None
*/
void SPI_Send_1_32bits(SPI_TypeDef *SPIx, uint32_t SendData)
{
    u8 temp = 0;
    u8 *puChars;
//    u32 uldat;

    puChars = (unsigned char*) &SendData;

    /* Write in the TXREG register the data to be sent */
    temp = SPIx->EXTCTL;
    temp = temp & 0x1F;
#ifndef SPIUSELSB
    // use MSB
    if(temp == 0) {
        temp = 32;
    } else {
        SendData = SendData << (32 - temp);
    }

#ifdef USECORTEX_M3
    SendData = __rbit(SendData);
#else
    SendData = Reverse32Bit(SendData);
#endif


#endif
    // use LSB
    if(temp == 0) {
        temp = 32;
    }
    if(temp <= 8) {
        SPIx->TXREG = puChars[0];
    } else if(temp <= 16) {
        SPIx->TXREG = puChars[0];
        SPIx->TXREG = puChars[1];
    } else if(temp <= 24) {
        SPIx->TXREG = puChars[0];
        SPIx->TXREG = puChars[1];
        SPIx->TXREG = puChars[2];
    } else {
        SPIx->TXREG = puChars[0];
        SPIx->TXREG = puChars[1];
        SPIx->TXREG = puChars[2];
        SPIx->TXREG = puChars[3];
    }

}


/**
* @brief  Returns the most recent received data by the SPIx peripheral.
* @param SPIx: where x can be :
*   0, 1 in SPI mode
* @retval : The value of the received data.
*/
uint32_t SPI_Recv_1_32bits(SPI_TypeDef *SPIx)
{
    u8 temp = 0;
    u32 RecvData = 0;
    u8 *puChars;

    puChars = (unsigned char*) &RecvData;

    temp = SPIx->EXTCTL;
    if(temp == 0) {
        temp = 32;
    }
    // use LSB
    if(temp <= 8) {
        puChars[0] = (u8)SPIx->RXREG;
    } else if(temp <= 16) {
        puChars[0] = (u8)SPIx->RXREG;
        puChars[1] = (u8)SPIx->RXREG;
    } else if(temp <= 24) {
        puChars[0] = (u8)SPIx->RXREG;
        puChars[1] = (u8)SPIx->RXREG;
        puChars[2] = (u8)SPIx->RXREG;
    } else {
        puChars[0] = (u8)SPIx->RXREG;
        puChars[1] = (u8)SPIx->RXREG;
        puChars[2] = (u8)SPIx->RXREG;
        puChars[3] = (u8)SPIx->RXREG;
    }
#ifndef SPIUSELSB
    // use MSB
#ifdef USECORTEX_M3
    SendData = __rbit(SendData);
#else
    RecvData = Reverse32Bit(RecvData);
#endif
    RecvData = RecvData >> (32 - temp);
#endif
    return RecvData;
}

#endif
#if 0
//MSB have issue


//#define DSPI_SendData(IP,SendData)  (IP->TXREG = (SendData))

/**
* @brief  Transmits a Data through the SPIx peripheral.
* @param SPIx: where x can be :
*   0, 1 in SPI mode
* @param Data : Data to be transmitted..
* @retval : None
*/
void SPI_Send_1_32bits(SPI_TypeDef *SPIx, uint32_t SendData)
{
    u8 temp = 0;
    u8 *puChars;

    puChars = (unsigned char*) &SendData;

    /* Write in the TXREG register the data to be sent */
    temp = SPIx->EXTCTL;
    temp = temp & 0x1F;
#ifndef SPIUSELSB
    // use MSB
    if(temp == 0) {
        temp = 32;
    } else {
        SendData = SendData << (32 - temp);
    }
    if(temp <= 8) {
        SPIx->TXREG = puChars[3];
    } else if(temp <= 16) {
        SPIx->TXREG = puChars[3];
        SPIx->TXREG = puChars[2];
    } else if(temp <= 24) {
        SPIx->TXREG = puChars[3];
        SPIx->TXREG = puChars[2];
        SPIx->TXREG = puChars[1];
    } else {
        SPIx->TXREG = puChars[3];
        SPIx->TXREG = puChars[2];
        SPIx->TXREG = puChars[1];
        SPIx->TXREG = puChars[0];
    }
#else
    // use LSB
    if(temp == 0) {
        temp = 32;
    }
    if(temp <= 8) {
        SPIx->TXREG = puChars[0];
    } else if(temp <= 16) {
        SPIx->TXREG = puChars[0];
        SPIx->TXREG = puChars[1];
    } else if(temp <= 24) {
        SPIx->TXREG = puChars[0];
        SPIx->TXREG = puChars[1];
        SPIx->TXREG = puChars[2];
    } else {
        SPIx->TXREG = puChars[0];
        SPIx->TXREG = puChars[1];
        SPIx->TXREG = puChars[2];
        SPIx->TXREG = puChars[3];
    }
#endif
}


/**
* @brief  Returns the most recent received data by the SPIx peripheral.
* @param SPIx: where x can be :
*   0, 1 in SPI mode
* @retval : The value of the received data.
*/
uint32_t SPI_Recv_1_32bits(SPI_TypeDef *SPIx)
{
    u8 temp = 0;
    u32 RecvData = 0;
    u8 *puChars;

    puChars = (unsigned char*) &RecvData;

    temp = SPIx->EXTCTL;
    if(temp == 0) {
        temp = 32;
    }
#ifndef SPIUSELSB
    // use MSB
    if(temp <= 8) {
        puChars[3] = (u8)SPIx->RXREG;
    } else if(temp <= 16) {
        puChars[3] = (u8)SPIx->RXREG;
        puChars[2] = (u8)SPIx->RXREG;
    } else if(temp <= 24) {
        puChars[3] = (u8)SPIx->RXREG;
        puChars[2] = (u8)SPIx->RXREG;
        puChars[1] = (u8)SPIx->RXREG;
    } else {
        puChars[3] = (u8)SPIx->RXREG;
        puChars[2] = (u8)SPIx->RXREG;
        puChars[1] = (u8)SPIx->RXREG;
        puChars[0] = (u8)SPIx->RXREG;
    }
    RecvData = RecvData >> (32 - temp);
#else
    // use LSB
    if(temp <= 8) {
        puChars[0] = (u8)SPIx->RXREG;
    } else if(temp <= 16) {
        puChars[0] = (u8)SPIx->RXREG;
        puChars[1] = (u8)SPIx->RXREG;
    } else if(temp <= 24) {
        puChars[0] = (u8)SPIx->RXREG;
        puChars[1] = (u8)SPIx->RXREG;
        puChars[2] = (u8)SPIx->RXREG;
    } else {
        puChars[0] = (u8)SPIx->RXREG;
        puChars[1] = (u8)SPIx->RXREG;
        puChars[2] = (u8)SPIx->RXREG;
        puChars[3] = (u8)SPIx->RXREG;
    }
#endif
    return RecvData;
}
#endif
#if 0
/**
* @brief  Transmits a Data through the SPIx peripheral.
* @param SPIx: where x can be :
*   0, 1 in SPI mode
* @param Data : Data to be transmitted..
* @retval : None
*/
void SPI_Send_1_32bits(SPI_TypeDef *SPIx, uint32_t SendData)
{
    u8 temp = 0;
    /* Check the parameters */
    assert_param(IS_SPI_ALL_PERIPH(SPIx));

    /* Write in the TXREG register the data to be sent */
    temp = SPIx->EXTCTL;
    if(temp == 0) {
        temp = 32;
        //SPIx->GCTL |= SPI_DataSize_32b;
    }
    SPIx->TXREG = SendData;
    if (temp > 0x8   )
        SPIx->TXREG = SendData >> 8;
    if (temp > 0x10  )
        SPIx->TXREG = SendData >> 16;
    if (temp > 0x18  )
        SPIx->TXREG = SendData >> 24;
}




/**
* @brief  Returns the most recent received data by the SPIx peripheral.
* @param SPIx: where x can be :
*   0, 1 in SPI mode
* @retval : The value of the received data.
*/
uint32_t SPI_Recv_1_32bits(SPI_TypeDef *SPIx)
{
    u8 temp = 0;
    u32 RecvData = 0;
    /* Check the parameters */
    assert_param(IS_SPI_ALL_PERIPH(SPIx));
    temp = SPIx->EXTCTL;
    if(temp == 0) {
        temp = 32;
        //SPIx->GCTL |= SPI_DataSize_32b;
    }
    RecvData = (u32)SPIx->RXREG;
    if (temp > 0x8   )
        RecvData |= (u32)(SPIx->RXREG) << 8;
    if (temp > 0x10  )
        RecvData |= (u32)(SPIx->RXREG) << 16;
    if (temp > 0x18 )
        RecvData |= (u32)(SPIx->RXREG) << 24;

    return RecvData;
}
#endif

/********************************************************************************************************
**������Ϣ ��SPIMReadWriteByte(SPI_TypeDef* SPIx,unsigned char tx_data)
**�������� : ͨ������ SPIx �շ����� ,����ȫ˫��ģʽ(ͬʱ�շ�)
**������� ��SPI_TypeDef* SPIx,��ѡ��SPI1,SPI2  ;  tx_data
**������� ����
********************************************************************************************************/
unsigned int SPIMWriteRead_1_32bits(SPI_TypeDef* SPIx, unsigned int tx_data)
{
    unsigned int rx_data;
    SPI_Send_1_32bits(SPIx, tx_data);
    while (1) {
        if(SPI_GetFlagStatus(SPIx, SPI_FLAG_TXEPT)) {
            break;
        }
    }

    while (1) {
        if(SPI_GetFlagStatus(SPIx, SPI_FLAG_RXAVL)) {
            rx_data = SPI_Recv_1_32bits(SPIx);
            break;
        }
    }
    return rx_data;
}
#define LEN 32
u32 txtestdata[][LEN] = {
    {0, 1, 2, 3, 4, 5, 6, 7, 8, 9},
    {0, 1, 2, 3, 4, 5, 6, 7, 8, 9}
};

u32 rxtestdata[LEN];

/********************************************************************************************************
**������Ϣ ��SPIM_Test()
**�������� :���Գ���ʹ�ô��ڴ�ӡ256ҳ����
**������� ����
**������� ����
********************************************************************************************************/
void SPIM_1_32_loopbackTest(void)
{

    unsigned short i, j, k;
    for(k = 1; k <= 32; k++) {
        SPIM_Initxbit(SPI2, 0x2, k, SPIMODE3);

        for(j = 0; j < k; j++) {

            SPIM_CSLow();
            for(i = 0; i < LEN; i++) {
                rxtestdata[i] = 0;
                txtestdata[1][i] = (0x2 << i) - 2;
                rxtestdata[i] = SPIMWriteRead_1_32bits(SPI2, txtestdata[1][i]);
            }
            SPIM_CSHigh();
            for(i = 0; i < LEN; i++) {
                UartSendGroup((u8*)printBuf, sprintf(printBuf, "rx[%d]=0x%x\r\n", i, rxtestdata[i]));
            }
        }
    }



    UartSendGroup((u8*)printBuf, sprintf(printBuf, "SPI2 test over\r\n"));

}



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
    SPIM_1_32_loopbackTest();
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
**������Ϣ ��SPIM_Test()
**�������� :���Գ���ʹ�ô��ڴ�ӡ256ҳ����
**������� ����
**������� ����
********************************************************************************************************/
void SPIM_Test(void)
{
    unsigned int i;
    for(i = 0; i < 256; i++) {
        tmpdata[i] = i;
    }

    UartSendGroup((u8*)printBuf, sprintf(printBuf, "SPI2 test\r\n"));
    SPIM_Init(0x8);

    SPIM_ReadID();

    SPIM_SectorErase(0);

    SPIM_PageProgram(0, tmpdata, 256);

    for(i = 0; i < 256; i++) {
        rxtmpdata[i] = 0x0;
    }
    SPIM_PageRead(0, rxtmpdata, 256);                                           //��д��ȥ��һҳ256�ֽڶ�����

    for(i = 0; i < 10; i++) {
        UartSendGroup((u8*)printBuf, sprintf(printBuf, "rx[%d]=0x%x\r\n", i, rxtmpdata[i]));
    }

    UartSendGroup((u8*)printBuf, sprintf(printBuf, "SPI2 test over\r\n"));

}

/********************************************************************************************************
**������Ϣ ��SPIM_PageRead(unsigned long address,unsigned char *p,unsigned int number)
**�������� :��ȡ256ҳ����
**������� �� address  ;*p ;number
**������� ����
********************************************************************************************************/
void SPIM_PageRead(unsigned long address, unsigned char *p, unsigned int number) //page = 256 bytes
{
    unsigned char addr0, addr1, addr2;
    unsigned int i;
    address = address & 0xffffff00;                                             //page address
    addr0 = (unsigned char)(address >> 16);
    addr1 = (unsigned char)(address >> 8);
    addr2 = (unsigned char)address;

    SPIM_CSLow();                                                               //Spi cs assign to this pin,select

    SPIMReadWriteByte(READ);
    SPIMReadWriteByte(addr0);
    SPIMReadWriteByte(addr1);
    SPIMReadWriteByte(addr2);

    for(i = 0; i < 256; i++) {
        rxtmpdata[i] = SPIMReadWriteByte(0x00);
    }


    SPIM_CSHigh();                                                              //Spi cs release
}

/********************************************************************************************************
**������Ϣ ��SPIM_PageProgram(unsigned long address,unsigned char *p,unsigned int number)
**�������� :����256ҳ����
**������� �� address;*p;number
**������� ����
********************************************************************************************************/
void SPIM_PageProgram(unsigned long address, unsigned char *p, unsigned int number)
{
    unsigned int j;
    unsigned char addr0, addr1, addr2;
    address = address & 0xffffff00;                                             //page address
    addr0 = (unsigned char)(address >> 16);
    addr1 = (unsigned char)(address >> 8);
    addr2 = (unsigned char)address;

    SPIM_WriteEnable();
    SPIM_CSLow();                                                               //Spi cs assign to this pin,select
    SPIMReadWriteByte(PP);
    SPIMReadWriteByte(addr0);
    SPIMReadWriteByte(addr1);
    SPIMReadWriteByte(addr2);
    for(j = 0; j < number; j++) {
        SPIMReadWriteByte(*(p++));
    }

    SPIM_CSHigh();                                                              //Spi cs release

    SPIM_checkStatus();
}

/********************************************************************************************************
**������Ϣ ��SPIM_SectorErase()
**�������� :��������
**������� ��unsigned long address,ɾ��ָ��sector�ĵ�ַ each sector = 64Kbytes
**������� ����
********************************************************************************************************/
void SPIM_SectorErase(unsigned long address)
{
    unsigned char addr0, addr1, addr2;
    address = address & 0xffff0000;
    addr0 = ((unsigned char)(address >> 16)) & 0xff;
    addr1 = ((unsigned char)(address >> 8)) & 0xff;
    addr2 = ((unsigned char)address) & 0xff;

    SPIM_WriteEnable();

    SPIM_CSLow();                                                               //Spi cs assign to this pin,select

    SPIMReadWriteByte(SE);
    SPIMReadWriteByte(addr0);
    SPIMReadWriteByte(addr1);
    SPIMReadWriteByte(addr2);
    SPIM_CSHigh();                                                              //Spi cs release

    SPIM_checkStatus();
}

/********************************************************************************************************
**������Ϣ ��SPIMReadWriteByte(unsigned char tx_data)
**�������� : ͨ������ SPIx �շ����� ,����ȫ˫��ģʽ(ͬʱ�շ�)
**������� ����
**������� ����
********************************************************************************************************/
unsigned int SPIMReadWriteByte(unsigned char tx_data)
{
    SPI_SendData(SPI2, tx_data);
    while (1) {
        if(SPI_GetFlagStatus(SPI2, SPI_FLAG_TXEPT)) {
            break;
        }
    }
    while (1) {
        if(SPI_GetFlagStatus(SPI2, SPI_FLAG_RXAVL)) {
            return SPI_ReceiveData(SPI2);
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


    RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2, ENABLE);                        //SPI2 clk enable

    SPIM_CSHigh();

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

    SPI_Cmd(SPI2, ENABLE);                                                      //Enables the specified SPI peripheral SPIʹ�ܡ�����ģʽ 8λ����ģʽ SPI �Ĳ�����
    SPIM_TXEn(SPI2);
    SPIM_RXEn(SPI2);
}

/********************************************************************************************************
**������Ϣ ��SPIM_ReadID(SPI_TypeDef* SPIx)
**�������� :��ȡID
**������� ��SPI_TypeDef* SPIx,��ѡ��SPI1,SPI2
**������� ����
********************************************************************************************************/
void SPIM_ReadID()
{
    unsigned char temp[5];
    unsigned int i;

    SPIM_CSLow();                                                               //Spi cs assign to this pin,select
    SPIMReadWriteByte(RDID);

    for(i = 0; i < 3; i++) {
        temp[i] = SPIMReadWriteByte(0x01);
    }
    for(i = 0; i < 3; i++) {
        UartSendGroup((u8*)printBuf, sprintf(printBuf, "temp=0x%x\r\n", temp[i]));
    }
    SPIM_CSHigh();                                                              //Spi cs release
}

/********************************************************************************************************
**������Ϣ ��SPIM_WriteEnable()
**�������� :д����ʹ��
**������� ����
**������� ����
********************************************************************************************************/
void SPIM_WriteEnable()
{
    SPIM_CSLow();                                                               //Spi cs assign to this pin,select
    SPIMReadWriteByte(WREN);
    SPIM_CSHigh();                                                              //Spi cs release
}

/********************************************************************************************************
**������Ϣ ��SSPIM_checkStatus()
**�������� :������ݷ����Ƿ���ȷ
**������� ����
**������� ����
********************************************************************************************************/
void SPIM_checkStatus()
{
    unsigned char temp;
    SPIM_CSLow();                                                               //Spi cs assign to this pin,select
    SPIMReadWriteByte(RDSR);
    while(1) {
        temp = SPIMReadWriteByte(0x00);
        if((temp & 0x01) == 0x0)
            break;
    }
    SPIM_CSHigh();                                                              //Spi cs release
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

