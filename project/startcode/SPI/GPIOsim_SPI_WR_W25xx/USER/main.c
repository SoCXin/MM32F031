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



/*
  Configure all SPI settings here, the system will automatically complete all
  initialization work according to the configuration.
*/

#define CS_GPIOx              GPIOB
#define CS_Pin_x              GPIO_Pin_12
#define RCC_CS                RCC_AHBPeriph_GPIOB

#define CLK_GPIOx             GPIOB
#define CLK_Pin_x             GPIO_Pin_13
#define RCC_CLK               RCC_AHBPeriph_GPIOB

#define MOSI_GPIOx            GPIOB
#define MOSI_Pin_x            GPIO_Pin_15
#define RCC_MOSI              RCC_AHBPeriph_GPIOB


#define MISO_GPIOx            GPIOB
#define MISO_Pin_x            GPIO_Pin_14
#define RCC_MISO              RCC_AHBPeriph_GPIOB



//----------------- User Config -----------------//
#define CS_L           {CS_GPIOx->BRR = CS_Pin_x;}                              //GPIO_ResetBits(CS_GPIOx,CS_Pin_x)
#define CS_H           {CS_GPIOx->BSRR = CS_Pin_x;}                             //GPIO_SetBits(CS_GPIOx,CS_Pin_x)
#define SCK_L          {CLK_GPIOx->BRR = CLK_Pin_x;}                            //GPIO_ResetBits(CLK_GPIOx,CLK_Pin_x)
#define SCK_H          {CLK_GPIOx->BSRR = CLK_Pin_x;}                           //GPIO_SetBits(CLK_GPIOx,CLK_Pin_x)
#define MOSI_L         {MOSI_GPIOx->BRR = MOSI_Pin_x;}                          //GPIO_ResetBits(MOSI_GPIOx,MOSI_Pin_x)
#define MOSI_H         {MOSI_GPIOx->BSRR = MOSI_Pin_x;}                         //GPIO_SetBits(MOSI_GPIOx,MOSI_Pin_x)
#define MISO_IN        (MISO_GPIOx->IDR&MISO_Pin_x)                             //GPIO_ReadInputData(MISO_GPIOx,MISO_Pin_x)


unsigned char tmpdata[256];
unsigned char rxtmpdata[256];
extern void SPIM_Init(unsigned short spi_baud_div);

extern void SPI_Start(void);
extern void SPI_Stop(void);

extern u8 SPIMReadWrite8Bits(u8 );





#define MSB   1                                                                 // Open this MSB==1 set MSB enable, otherwise LSB
#define BITLENGTH   8                                                           //bits 4~16
#define SPI_MODE 3

#if    (SPI_MODE == 0)                                                          //(SPI_MODE == 1)
#define CPOL 0                                                                  // Open this CPOL=0
#define CPHA 0                                                                  // Open this CPHA=0
#elif  (SPI_MODE == 1)                                                          //(SPI_MODE == 1)
#define CPOL 1                                                                  // Open this CPOL=1
#define CPHA 0                                                                  // Open this CPHA=0
#elif  (SPI_MODE == 2)                                                          //(SPI_MODE == 2)
#define CPOL 0                                                                  // Open this CPOL=0
#define CPHA_1                                                                  // Open this CPHA=1
#else                                                                           //(SPI_MODE == 3)
#define CPOL 1                                                                  // Open this CPOL=1
#define CPHA 1                                                                  // Open this CPHA=1
#endif




#define READ            0x03
#define FAST_READ       0x0B
#define RDID            0x9F
#define WREN            0x06
#define WRDI            0x04
#define SE              0xD8
#define BE              0xC7
#define PP              0x02
#define RDSR            0x05
#define WRSR            0x01
#define DP              0xB9
#define RES             0xAB

extern void SPIM_Test(void);


////////////////////////////////////////////////////////////////////////////////
//������
//LED��������
////////////////////////////////////////////////////////////////////////////////
#define LED4_ON()  GPIO_ResetBits(GPIOA,GPIO_Pin_15)	// PA15
#define LED4_OFF()  GPIO_SetBits(GPIOA,GPIO_Pin_15)	// PA15
#define LED4_TOGGLE()  (GPIO_ReadOutputDataBit(GPIOA,GPIO_Pin_15))?(GPIO_ResetBits(GPIOA,GPIO_Pin_15)):(GPIO_SetBits(GPIOA,GPIO_Pin_15))	// PA15

#define LED3_ON()  GPIO_ResetBits(GPIOB,GPIO_Pin_3)	// PB3
#define LED3_OFF()  GPIO_SetBits(GPIOB,GPIO_Pin_3)	// PB3
#define LED3_TOGGLE()  (GPIO_ReadOutputDataBit(GPIOB,GPIO_Pin_3))?(GPIO_ResetBits(GPIOB,GPIO_Pin_3)):(GPIO_SetBits(GPIOB,GPIO_Pin_3))	// PB3

#define LED2_ON()  GPIO_ResetBits(GPIOB,GPIO_Pin_4)	// PB4
#define LED2_OFF()  GPIO_SetBits(GPIOB,GPIO_Pin_4)	// PB4
#define LED2_TOGGLE()  (GPIO_ReadOutputDataBit(GPIOB,GPIO_Pin_4))?(GPIO_ResetBits(GPIOB,GPIO_Pin_4)):(GPIO_SetBits(GPIOB,GPIO_Pin_4))	// PB4

#define LED1_ON()  GPIO_ResetBits(GPIOB,GPIO_Pin_5)	// PB5
#define LED1_OFF()  GPIO_SetBits(GPIOB,GPIO_Pin_5)	// PB5
#define LED1_TOGGLE()  (GPIO_ReadOutputDataBit(GPIOB,GPIO_Pin_5))?(GPIO_ResetBits(GPIOB,GPIO_Pin_5)):(GPIO_SetBits(GPIOB,GPIO_Pin_5))	// PB5

void LED_Init(void);//��ʼ��

/********************************************************************************************************
**������Ϣ ��void SPIM_SimPinConfig(void)
**�������� : ��ʼ��GPIO
**������� ����
**������� ����
********************************************************************************************************/
void SPIM_SimPinConfig(void)
{
    GPIO_InitTypeDef  GPIO_InitStructure;
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE);

    GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_12;   		//spi2_cs  pb12
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 	// ���⸴�����
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_13;   		//spi2_sck  pb13
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 	// ���⸴�����
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_15;   		//spi2_mosi  pb15
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 	// ���⸴�����
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_14;  		//spi2_miso  pb14
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//GPIO_Mode_IPD;//GPIO_Mode_IPU; 		//��������
    GPIO_Init(GPIOB, &GPIO_InitStructure);


}


void SPI_GPIO_Config(void)
{
    SPIM_SimPinConfig();
    SPI_Stop();
}


void SPI_Start(void)
{
    MOSI_L;
#if (CPOL == 0)
    SCK_L;
    CS_L;
#else
    SCK_H;
    CS_L;
#endif
}

void SPI_Stop(void)
{
#if (CPOL == 0)
    SCK_L;
    CS_H;
#else
    SCK_H;
    CS_H;
#endif
    MOSI_L;
}

//bits 1~8
/********************************************************************************************************
**������Ϣ �uu8 SPIMReadWrite8Bits(u8 senddata)
**�������� : ��ѡ��SPI��Macro ���壬��ѡ��ģʽ
**������� �����͵�����
**������� �����յ�����
********************************************************************************************************/
u8 SPIMReadWrite8Bits(u8 senddata)
{
    unsigned char i = 0;
    unsigned char recvdata = 0x00;

    for(i = 0; i < BITLENGTH; i++) {
#if (((CPOL == 0)&&(CPHA == 0))||((CPOL == 1)&&(CPHA == 1)))
        SCK_L;
#else
        SCK_H;
#endif
#if ( MSB ==1)
        if(senddata & ((0x01) << (BITLENGTH - 1))) { // Tx data use MSB mode
            MOSI_H;
        } else {
            MOSI_L;
        }
        senddata <<= 1;

#else
        if(senddata & 0x1) {    // Tx data use LSB mode
            MOSI_H;
        } else {
            MOSI_L;
        }
        senddata >>= 1;
#endif

#if (((CPOL == 0)&&(CPHA == 0))||((CPOL == 1)&&(CPHA == 1)))
        SCK_H;
#else
        SCK_L;
#endif

#if ( MSB ==1)               // Rx data use MSB mode
        recvdata <<= 1 ;
        if((MISO_IN) != 0) {
            recvdata++;
        }
#else
        if((MISO_IN) != 0) {
            recvdata = recvdata + (1 << i);// Rx data use LSB mode
        }
#endif
    }

    return recvdata;
}                          // There aren't CS_Hign
//bits BITLENGTH = 9~16
/********************************************************************************************************
**������Ϣ �uu8 SPIMReadWrite8Bits(u8 senddata)
**�������� : ��ѡ��SPI��Macro ���壬��ѡ��ģʽ
**������� �����͵�����
**������� �����յ�����
********************************************************************************************************/
u16 SPIMReadWrite16Bits(u16 senddata)
{
    unsigned char i = 0;
    u16 recvdata = 0x00;

    for(i = 0; i < BITLENGTH; i++) {
#if (((CPOL == 0)&&(CPHA == 0))||((CPOL == 1)&&(CPHA == 1)))
        SCK_L;
#else
        SCK_H;
#endif
#if ( MSB ==1)
        if(senddata & ((0x01) << (BITLENGTH - 1))) { // Tx data use MSB mode
            MOSI_H;
        } else {
            MOSI_L;
        }
        senddata <<= 1;

#else
        if(senddata & 0x1) {    // Tx data use LSB mode
            MOSI_H;
        } else {
            MOSI_L;
        }
        senddata >>= 1;
#endif

#if (((CPOL == 0)&&(CPHA == 0))||((CPOL == 1)&&(CPHA == 1)))
        SCK_H;
#else
        SCK_L;
#endif

#if ( MSB ==1)               // Rx data use MSB mode
        recvdata <<= 1 ;
        if((MISO_IN) != 0) {
            recvdata++;
        }
#else
        if((MISO_IN) != 0) {
            recvdata = recvdata + (1 << i);// Rx data use LSB mode
        }
#endif
    }

    return recvdata;
}                          // There aren't CS_Hign

/********************************************************************************************************
**������Ϣ ��SPIM_Init(unsigned short spi_baud_div)
**�������� :���޸Ĳ�����ʼ��SPI
**������� ��spi_baud_div
**������� ����
********************************************************************************************************/
void SPIM_Init(unsigned short spi_baud_div)
{
    SPI_GPIO_Config();
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
    SPI_Start();
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
    SPI_Stop();
}


/********************************************************************************************************
**������Ϣ ��SPIMReadWriteByte(unsigned char tx_data)
**�������� : ͨ������ SPIx �շ����� ,����ȫ˫��ģʽ(ͬʱ�շ�)
**������� ����
**������� ����
********************************************************************************************************/
u8 SPIMReadWriteByte(u8 tx_data)
{
    return ((u8)SPIMReadWrite8Bits((u8)tx_data));
}



/********************************************************************************************************
**������Ϣ ��SPIM_ReadID(SPI_TypeDef* SPIx)
**�������� :��ȡID
**������� ��SPI_TypeDef* SPIx,��ѡ��SPI1,SPI2
**������� ����
********************************************************************************************************/
void SPIM_ReadID()
{
    __IO unsigned char temp[5];
    unsigned int i;

    SPIM_CSLow();								//Spi cs assign to this pin,select
    SPIMReadWriteByte(RDID);

    for(i = 0; i < 3; i++) {
        temp[i] = SPIMReadWriteByte(0x01);
    }
    SPIM_CSHigh();   							//Spi cs release
}

/********************************************************************************************************
**������Ϣ ��SPIM_WriteEnable()
**�������� :д����ʹ��
**������� ����
**������� ����
********************************************************************************************************/
void SPIM_WriteEnable()
{
    SPIM_CSLow(); 								//Spi cs assign to this pin,select
    SPIMReadWriteByte(WREN);
    SPIM_CSHigh(); 								//Spi cs release
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
    SPIM_CSLow(); 								//Spi cs assign to this pin,select
    SPIMReadWriteByte(RDSR);
    while(1) {
        temp = SPIMReadWriteByte(0x00);
        if((temp & 0x01) == 0x0)
            break;
    }
    SPIM_CSHigh();  							//Spi cs release
}

/********************************************************************************************************
**������Ϣ ��SPIM_WriteEnable()
**�������� :д����ʧ��
**������� ����
**������� ����
********************************************************************************************************/
void SPIM_WriteDisable()
{
    SPIM_CSLow();
    SPIMReadWriteByte(WRDI);
    SPIM_CSHigh();
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
    address = address & 0xffffff00; 					//page address
    addr0 = (unsigned char)(address >> 16);
    addr1 = (unsigned char)(address >> 8);
    addr2 = (unsigned char)address;

    SPIM_CSLow(); 								//Spi cs assign to this pin,select

    SPIMReadWriteByte(READ);
    SPIMReadWriteByte(addr0);
    SPIMReadWriteByte(addr1);
    SPIMReadWriteByte(addr2);

    for(i = 0; i < number; i++) {
        rxtmpdata[i] = SPIMReadWriteByte(0x00);
    }


    SPIM_CSHigh();  							//Spi cs release
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
    address = address & 0xffffff00; 					//page address
    addr0 = (unsigned char)(address >> 16);
    addr1 = (unsigned char)(address >> 8);
    addr2 = (unsigned char)address;

    SPIM_WriteEnable();
    SPIM_CSLow();  								//Spi cs assign to this pin,select
    SPIMReadWriteByte(PP);
    SPIMReadWriteByte(addr0);
    SPIMReadWriteByte(addr1);
    SPIMReadWriteByte(addr2);
    for(j = 0; j < number; j++) {
        SPIMReadWriteByte(*p);
        p++;
    }

    SPIM_CSHigh();  							//Spi cs release

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
    SPIM_checkStatus();
    SPIM_CSLow();  								//Spi cs assign to this pin,select

    SPIMReadWriteByte(SE);
    SPIMReadWriteByte(addr0);
    SPIMReadWriteByte(addr1);
    SPIMReadWriteByte(addr2);
    SPIM_CSHigh();  							//Spi cs release

    SPIM_checkStatus();
}

/********************************************************************************************************
**������Ϣ ��SPIM_BlockErase()
**�������� :��������
**������� ����
**������� ����
********************************************************************************************************/
void SPIM_BlockErase()
{
    SPIM_WriteEnable();

    SPIM_CSLow();  								//Spi cs assign to this pin,select

    SPIMReadWriteByte(BE);

    SPIM_CSHigh();  							//Spi cs release

    SPIM_checkStatus();
}
/********************************************************************************************************
**������Ϣ ��SPIM_Close()
**�������� : CS�ź����ߣ�ֹͣͨ��
**������� ����
**������� ����
********************************************************************************************************/
void SPIM_Close()
{
    SPIM_CSHigh();  							//Spi cs release
    SPIMReadWriteByte(0x01);
}

/********************************************************************************************************
**������Ϣ ��SPIM_Test()
**�������� :���Գ���ʹ�ô��ڴ�ӡ256ҳ����
**������� ����
**������� ����
********************************************************************************************************/
void SPIM_Test(void)
{
    int result = 0;
    unsigned int i;


    SPIM_Init(0x8);//������ 72M/2=36M

    SPIM_ReadID();
    for(i = 0; i < 5; i++) {
        SPIM_ReadID();
    }
    SPIM_SectorErase(0x400);
    for(i = 0; i < 256; i++) {
        rxtmpdata[i] = 0x0;
    }
    SPIM_PageRead(0x400, rxtmpdata, 256); 		//��д��ȥ��һҳ256�ֽڶ�����
    for(i = 0; i < 256; i++) {
        tmpdata[i] = i;
    }
    SPIM_PageProgram(0x400, tmpdata, 256);

    for(i = 0; i < 256; i++) {
        rxtmpdata[i] = 0x0;
    }
    SPIM_PageRead(0x400, rxtmpdata, 256); 		//��д��ȥ��һҳ256�ֽڶ�����

    for(i = 0; i < 256; i++) {
        if(rxtmpdata[i] != tmpdata[i]) {
            result = 1;
            break;
        }
    }
    SPIM_SectorErase(0x400);
    for(i = 0; i < 256; i++) {
        rxtmpdata[i] = 0x0;
    }
    SPIM_PageRead(0x400, rxtmpdata, 256); 		//��д��ȥ��һҳ256�ֽڶ�����
    for(i = 0; i < 256; i++) {
        tmpdata[i] = 2 * i;
    }
    SPIM_PageProgram(0x400, tmpdata, 256);

    for(i = 0; i < 256; i++) {
        rxtmpdata[i] = 0x0;
    }
    SPIM_PageRead(0x400, rxtmpdata, 256); 		//��д��ȥ��һҳ256�ֽڶ�����

    for(i = 0; i < 256; i++) {
        if(rxtmpdata[i] != tmpdata[i]) {
            result = 1;
            break;
        }
    }
    if ( result == 1) {
        LED1_ON();
        LED2_ON();
        LED3_ON();
        LED4_ON();
    } else {
        LED1_OFF();
        LED2_OFF();
        LED3_OFF();
        LED4_OFF();
    }
}


/*******************************************************************************
* @name   : GPIO_Clock_Set
* @brief  : RCC clock set
* @param  : Portx , State
* @retval : void
*******************************************************************************/
void GPIO_Clock_Set(GPIO_TypeDef* GPIOx, FunctionalState NewState)
{

    if(GPIOx == GPIOA) {
        RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, NewState);                         //GPIO clock starts
    }
    if(GPIOx == GPIOB) {
        RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, NewState);                         //GPIO clock starts
    }
    if(GPIOx == GPIOC) {
        RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOC, NewState);                         //GPIO clock starts
    }
    if(GPIOx == GPIOD) {
        RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOD, NewState);                         //GPIO clock starts
    }
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

    GPIO_Clock_Set(GPIOA, ENABLE);  //����GPIOAʱ��
    GPIO_Clock_Set(GPIOB, ENABLE);  //����GPIOBʱ��
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
    LED1_TOGGLE();
    LED2_TOGGLE();
    LED3_TOGGLE();
    LED4_TOGGLE();

    LED1_TOGGLE();
    LED2_TOGGLE();
    LED3_TOGGLE();
    LED4_TOGGLE();
}

int main(void)
{
    LED_Init();
    SPIM_Test();
    while(1) {                                                                  //����ѭ��

    }
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

