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
#include "HAL_conf.h"
#include "HAL_device.h"
#include "stdio.h"


#define DISSWDIO
#define LEN 32
u32 txtestdata[][LEN] = {
    {0, 1, 2, 3, 4, 5, 6, 7, 8, 9},
    {0, 1, 2, 3, 4, 5, 6, 7, 8, 9}
};

u32 rxtestdata[LEN];

static u32 cnt = 0;
#define LENGTH (4*LEN)
static unsigned char rxdata[LENGTH];
static unsigned char txdata[LENGTH] = {0x55, 0xaa, 0xbb, 0xdd, 0x33, 0x77, 0x22, 0x11, 0x55, 0xaa, 0xbb, 0xdd, 0x33, 0x77, 0x22, 0x11};
extern uint32_t SystemCoreClock;
char printBuf[100];
void Uart_ConfigInit(u32 bound);
void UartSendGroup(u8* buf, u16 len);
void UartSendAscii(char *str);

void RCC_ConfigInit(void);
void GPIO_ConfigInit(void);



#define GPIO_Pin_0_2  GPIO_Pin_2

void SPIM_CSLow(SPI_TypeDef* SPIx);
void SPIM_CSHigh(SPI_TypeDef* SPIx);
unsigned int SPI_FLASH_WriteRead(SPI_TypeDef* SPIx, unsigned char tx_data);
void SPIM_Init(SPI_TypeDef* SPIx, unsigned short spi_baud_div);
void SPIM_RXEn(SPI_TypeDef* SPIx);
void SPIM_TXEn(SPI_TypeDef* SPIx);


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

/********************************************************************************************************
**������Ϣ ��SPIM_Init(SPI_TypeDef* SPIx, unsigned short spi_baud_div)
**�������� :���޸Ĳ�����ʼ��SPI
**������� ��SPI_TypeDef* SPIx,��ѡ��SPI1,SPI2  ;spi_baud_div
**������� ����
********************************************************************************************************/
void SPIM_Initxbit(SPI_TypeDef* SPIx, unsigned short spi_baud_div, u8 datawidth)
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
    SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
    SPI_InitStructure.SPI_DataWidth = datawidth;
#define SPIMODE3
#if  defined(SPIMODE0)
    SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;    			// mode0 SPI_CPOL_Low, SPI_CPHA_1Edge;
    SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;  			// mode3 SPI_CPOL_High,SPI_CPHA_2Edge
#elif  defined(SPIMODE3)
    SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;    			// mode0 SPI_CPOL_Low, SPI_CPHA_1Edge;
    SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;  			// mode3 SPI_CPOL_High,SPI_CPHA_2Edge
#else

#endif
    SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
    SPI_InitStructure.SPI_BaudRatePrescaler = spi_baud_div;
    SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
    SPI_Init(SPIx, &SPI_InitStructure);

    SPI_Cmd(SPIx, ENABLE); 									//Enables the specified SPI peripheral SPIʹ�ܡ�����ģʽ 8λ����ģʽ SPI �Ĳ�����
    SPIM_TXEn(SPIx);
    SPIM_RXEn(SPIx);
}


/*******************************************************************************
* @name   : InitSystick
* @brief  : Init Systick
* @param  : None
* @retval : void
*******************************************************************************/
void InitSystick()
{
    SysTick_Config(SystemCoreClock / 1000);
    NVIC_SetPriority(SysTick_IRQn, 0x00);
}

/*******************************************************************************
* @name   : SysTick_Handler
* @brief  : Systick interrupt
* @param  : None
* @retval : void
*******************************************************************************/
void SysTick_Handler()
{

    cnt++;
}

void delay(u32 uldelay)
{
    cnt = 0;
    while(1) {
        if(cnt >= uldelay)
            break;
    }
}

/********************************************************************************************************
**������Ϣ ��SPIMReadWriteByte(SPI_TypeDef* SPIx,unsigned char tx_data)
**�������� : ͨ������ SPIx �շ����� ,����ȫ˫��ģʽ(ͬʱ�շ�)
**������� ��SPI_TypeDef* SPIx,��ѡ��SPI1,SPI2  ;  tx_data
**������� ����
********************************************************************************************************/
unsigned int SPIMReadWriteByte(SPI_TypeDef* SPIx, unsigned char tx_data)
{
    SPI_SendData(SPIx, tx_data);
    while (1) {
        if(SPI_GetFlagStatus(SPIx, SPI_FLAG_RXAVL)) {	//���ռĴ����ǿձ�־λ
            return SPI_ReceiveData(SPIx);
        }
    }
}

/********************************************************************************************************
**������Ϣ SPI_DMA_RX_Init
**�������� : DMA�������ݳ�ʼ��
**������� ����
**������� ����
**������ע ����Ҫ��MISO&MOSI�̽�
********************************************************************************************************/
void SPI_DMA_RX_Init()
{
    DMA_InitTypeDef  DMA_InitStructure;
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);	//ʹ��DMA����
    DMA_DeInit(DMA1_Channel4);
    DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t) & (SPI2->RXREG); //SPI�������ݼĴ���
    DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)rxdata; //SPI���յ������ݴ�Ż���ַ
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;//���ݴ����赽memory
    DMA_InitStructure.DMA_BufferSize = 20;
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
    DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
    DMA_InitStructure.DMA_Priority = DMA_Priority_VeryHigh;
    DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
    DMA_Init(DMA1_Channel4, &DMA_InitStructure);//SPI2 RXΪDMAͨ��4
}

/********************************************************************************************************
**������Ϣ ��SPI_DMA_TX_Init
**�������� : SPI����DMA��ʼ��
**������� ����
**������� ����
********************************************************************************************************/
void SPI_DMA_TX_Init()
{
    DMA_InitTypeDef  DMA_InitStructure;
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);	//ʹ��DMA����
    DMA_DeInit(DMA1_Channel5);
    DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t) & (SPI2->TXREG); //SPI�������ݼĴ���
    DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)txdata;
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;//���ݴ�memory������
    DMA_InitStructure.DMA_BufferSize = 8;//�������ݸ���
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;//�����ַ���ı�
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;//memory��ַ��������
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
    DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;//��ͨģʽ
    DMA_InitStructure.DMA_Priority = DMA_Priority_VeryHigh;
    DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
    DMA_Init(DMA1_Channel5, &DMA_InitStructure);//SPI RXΪDMAͨ��5
}
////////////////////////////////////////////////////////////////////////////////
/// @brief  Set the DMA Channeln's Peripheral address.
/// @param  channel : where n can be 1 to 7 for DMA1 to select the DMA Channel.
/// @param  length : Transmit lengths.
/// @retval : None
////////////////////////////////////////////////////////////////////////////////
void DMA_Set_TransmitLen(DMA_Channel_TypeDef* channel, u16 length)
{
    channel->CNDTR = length;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief  Set the DMA Channeln's Peripheral address.
/// @param  channel : where n can be 1 to 7 for DMA1 to select the DMA Channel.
/// @param  address : DMA memery address.
/// @retval : None
////////////////////////////////////////////////////////////////////////////////
void DMA_Set_MemoryAddress(DMA_Channel_TypeDef* channel, u32 address)
{
    channel->CMAR = address;
}
////////////////////////////////////////////////////////////////////////////////
/// @brief   DMA transmits packet
/// @param   ch: Pointer to a DMA channel.
/// @param   addr: The memory Buffer address.
/// @param   len: The length of data to be transferred.
/// @retval : None.
////////////////////////////////////////////////////////////////////////////////
void DRV_DMA_TransmitPacket(DMA_Channel_TypeDef* ch, u32 addr, u32 len)
{
    DMA_ITConfig(ch, DMA_IT_TC, ENABLE);
    DMA_Cmd(ch, DISABLE);
    DMA_Set_MemoryAddress(ch, addr);
    DMA_Set_TransmitLen(ch, len);
    DMA_Cmd(ch, ENABLE);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief   DMA transmit and receive packet
/// @param   ptx_buf: Pointer to SPI DMA send buffer(include send and recv bytes).
/// @param   prx_buf: Pointer to SPI DMA recv buffer(include send and recv bytes).
/// @param   len: The length of data , length equal send + recv len).
/// @retval : None.
////////////////////////////////////////////////////////////////////////////////********************************************************************************************************/
void SPI2MasterReadWritebuf(u8 *ptx_buf, u8 *prx_buf, u32 len)
{
    DRV_DMA_TransmitPacket(DMA1_Channel5, (u32)ptx_buf, len);
    DRV_DMA_TransmitPacket(DMA1_Channel4, (u32)prx_buf, len);
    SPIM_CSLow(SPI2);
    SPI_DMACmd(SPI2, SPI_DMAReq_EN, ENABLE);
    while(!DMA_GetFlagStatus(DMA1_FLAG_TC5));
    DMA_ClearFlag(DMA1_FLAG_TC5);
    while(!DMA_GetFlagStatus(DMA1_FLAG_TC4));
    DMA_ClearFlag(DMA1_FLAG_TC4);
    SPI_DMACmd(SPI2, SPI_DMAReq_EN, DISABLE);
    SPIM_CSHigh(SPI2);
}



/********************************************************************************************************
**������Ϣ ��SPI_master_DMA_TXRX
**�������� : ͨ��DMA����ģʽ������Է�����
**������� ����
**������� ����
**������ע ����Ҫ��MISO&MOSI�̽�
********************************************************************************************************/
void SPI_master_DMA_TXRXloop_Test(void)
{
    unsigned short i, j, k, len;


    for(k = 4; k <= 32; k++) {
        SPIM_Initxbit(SPI2, 0x2, k);
        SPI_DMA_RX_Init();
        SPI_DMA_TX_Init();


        if(k <= 8) {
            len = LEN;
        } else if(k <= 16) {
            len = 2 * LEN;
        } else if(k <= 24) {
            len = 3 * LEN;
        } else {
            len = 4 * LEN;
        }
        for(i = 0; i < LEN; i++) {
            rxtestdata[i] = 0;
            txtestdata[1][i] = (0x2 << i) - 1;
            if(k <= 8)
                txdata[i] = (u8)txtestdata[1][i];
            else if(k <= 16) {
                txdata[i * 2] = (u8)txtestdata[1][i];
                txdata[i * 2 + 1] = (u8)(txtestdata[1][i] >> 8);
            } else if(k <= 24) {
                txdata[i * 3] = (u8)txtestdata[1][i];
                txdata[i * 3 + 1] = (u8)(txtestdata[1][i] >> 8);
                txdata[i * 3 + 2] = (u8)(txtestdata[1][i] >> 16);
            } else {
                txdata[i * 4] = (u8)txtestdata[1][i];
                txdata[i * 4 + 1] = (u8)(txtestdata[1][i] >> 8);
                txdata[i * 4 + 2] = (u8)(txtestdata[1][i] >> 16);
                txdata[i * 4 + 3] = (u8)(txtestdata[1][i] >> 24);
            }
        }
        for(j = 0; j < 3; j++) {


            SPIM_CSLow(SPI2);

            SPI2MasterReadWritebuf(txdata, rxdata, len);

            SPIM_CSHigh(SPI2);

        }
    }

//    UartSendGroup((u8*)printBuf, sprintf(printBuf,"SPI2 test\r\n"));
//    //while(1)
//    {
//        for(i=1;i<=LENGTH;i++)
//        {
//            for(j=0;j<LENGTH;j++)
//            {
//                rxdata[j] = 0x0;
//            }
//            //memset(rxdata,0,sizeof(rxdata));
//            SPI2MasterReadWritebuf(txdata , rxdata,i);

//        }
//    }
    for(i = 0; i < len; i++) {
        UartSendGroup((u8*)printBuf, sprintf(printBuf, "rx[%d]=0x%x\r\n", i, rxdata[i]));
    }
    j = 0;
    for(i = 0; i < len; i++) {
        if(rxdata[i] != txdata[i]) {
            j = 1;
            break;
        }

    }
    if(j == 1) {
        UartSendGroup((u8*)printBuf, sprintf(printBuf, "SPI2 TxRx Loopback test fail\r\n"));
    } else {
        UartSendGroup((u8*)printBuf, sprintf(printBuf, "SPI2 TxRx Loopback test Sucessful\r\n"));
    }
    UartSendGroup((u8*)printBuf, sprintf(printBuf, "SPI2 test over\r\n"));

}
void setPA13asSWDIO(void)
{
    GPIO_InitTypeDef  GPIO_InitStructure;
#ifdef DISSWDIO
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource13, GPIO_AF_0);                       //SPI_MISO

    GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_13;                                 //SPI_MISO

    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;                               //Pull-up input
    GPIO_Init(GPIOA, &GPIO_InitStructure);
#endif
}

/*******************************************************************************
* @name   : main
* @brief  : Read/Write SPI FLASH by SPI interface
* @param  : None
* @retval : void
*******************************************************************************/
int main(void)
{
//    u32 i ;

    InitSystick();
    delay(1000);

    Uart_ConfigInit(9600);
    UartSendGroup((u8*)printBuf, sprintf(printBuf, "\r\nsprintf ok\r\n"));
    UartSendGroup((u8*)printBuf, sprintf(printBuf, "\r\nStart SPI test\r\n"));

    SPI_master_DMA_TXRXloop_Test();

    setPA13asSWDIO();
    while(1) {


    }
}


/*******************************************************************************
* @name   : SPIM_CSLow
* @brief  : SPIM CS Low
* @param  : SPI_TypeDef* SPIx
* @retval : void
*******************************************************************************/
void SPIM_CSLow(SPI_TypeDef* SPIx)
{

    SPI_CSInternalSelected(SPIx, SPI_CS_BIT0, ENABLE);
//	GPIO_ResetBits(GPIOA,GPIO_Pin_0_2);
}

/*******************************************************************************
* @name   : SPIM_TXEn
* @brief  : SPIM TX Enable
* @param  : SPI_TypeDef* SPIx
* @retval : void
*******************************************************************************/
void SPIM_TXEn(SPI_TypeDef* SPIx)
{

    SPI_BiDirectionalLineConfig(SPIx, SPI_Direction_Tx);
}

/*******************************************************************************
* @name   : SPIM_RXEn
* @brief  : SPIM RX Enable
* @param  : SPI_TypeDef* SPIx
* @retval : void
*******************************************************************************/
void SPIM_RXEn(SPI_TypeDef* SPIx)
{
    SPI_BiDirectionalLineConfig(SPIx, SPI_Direction_Rx);
}

/*******************************************************************************
* @name   : SPIM_CSHigh
* @brief  : SPIM CS High
* @param  : SPI_TypeDef* SPIx
* @retval : void
*******************************************************************************/
void SPIM_CSHigh(SPI_TypeDef* SPIx)
{
    SPI_CSInternalSelected(SPIx, SPI_CS_BIT0, DISABLE);
//	GPIO_SetBits(GPIOA,GPIO_Pin_0_2);
}


/*******************************************************************************
* @name   : SPIM_Init
* @brief  : SPIM Init
* @param  : SPI_TypeDef* SPIx,unsigned short spi_baud_div
* @retval : void
*******************************************************************************/
void SPIM_Init(SPI_TypeDef* SPIx, unsigned short spi_baud_div)
{
    SPI_InitTypeDef SPI_InitStructure;
    GPIO_InitTypeDef  GPIO_InitStructure;

    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA | RCC_AHBPeriph_GPIOB, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2, ENABLE);

    GPIO_PinAFConfig(GPIOA, GPIO_PinSource0, GPIO_AF_3);                      //SPI_NSS  PA0
#ifdef DISSWDIO
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource13, GPIO_AF_4);                       //SPI_MISO
#else
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource12, GPIO_AF_4);                       //SPI_MISO
#endif
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource13, GPIO_AF_4);                       //SPI_MOSI
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource14, GPIO_AF_3);                       //SPI_SCK

    GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_0;                                //SPI_NSS PA0
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;                             //Push to avoid multiplexing output
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_13 | GPIO_Pin_14;                   //SPI_MOSI | SPI_SCK
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

#ifdef DISSWDIO
    GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_13;                                 //SPI_MISO
#else
    GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_12;                                 //SPI_MISO
#endif
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;                               //Pull-up input
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
    SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
    SPI_InitStructure.SPI_DataWidth = SPI_DataWidth_8b;
    SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;
    SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;
    SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
    SPI_InitStructure.SPI_BaudRatePrescaler = spi_baud_div;
    SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
    SPI_Init(SPIx, &SPI_InitStructure);

    SPI_Cmd(SPIx, ENABLE);
    SPIM_TXEn(SPIx);
    SPIM_RXEn(SPIx);
}




/*******************************************************************************
* @name   : Uart_ConfigInit
* @brief  : Uart Config Init
* @param  : u32 bound
* @retval : void
*******************************************************************************/
void Uart_ConfigInit(u32 bound)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    UART_InitTypeDef UART_InitStructure;

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART2, ENABLE);
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA | RCC_AHBPeriph_GPIOB, ENABLE);

    GPIO_PinAFConfig(GPIOA, GPIO_PinSource6, GPIO_AF_3); //RX
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource7, GPIO_AF_4); //TX

    UART_InitStructure.UART_BaudRate = bound;
    UART_InitStructure.UART_WordLength = UART_WordLength_8b;
    UART_InitStructure.UART_StopBits = UART_StopBits_1;
    UART_InitStructure.UART_Parity = UART_Parity_No;
    UART_InitStructure.UART_HardwareFlowControl = UART_HardwareFlowControl_None;
    UART_InitStructure.UART_Mode = UART_Mode_Rx | UART_Mode_Tx;

    UART_Init(UART2, &UART_InitStructure);
    UART_Cmd(UART2, ENABLE);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
}

/*******************************************************************************
* @name   : UartSendByte
* @brief  : Uart Send Byte
* @param  : u8 dat
* @retval : void
*******************************************************************************/
void UartSendByte(u8 dat)
{
    UART_SendData( UART2, dat);
    while(!UART_GetFlagStatus(UART2, UART_FLAG_TXEPT));
}

/*******************************************************************************
* @name   : UartSendGroup
* @brief  : Uart Send Group
* @param  : u8* buf,u16 len
* @retval : void
*******************************************************************************/
void UartSendGroup(u8* buf, u16 len)
{
    while(len--)
        UartSendByte(*buf++);
}

/*******************************************************************************
* @name   : UartSendAscii
* @brief  : Uart Send Ascii
* @param  : char *str
* @retval : void
*******************************************************************************/
void UartSendAscii(char *str)
{
    while(*str)
        UartSendByte(*str++);
}
/*-------------------------(C) COPYRIGHT 2019 MindMotion ----------------------*/

