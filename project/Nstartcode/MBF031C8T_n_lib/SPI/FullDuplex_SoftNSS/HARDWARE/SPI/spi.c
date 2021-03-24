#include "HAL_device.h"
#include "HAL_conf.h"
#include "spi.h"
#include "uart.h"	  

unsigned char tmpdata[256];
unsigned char tmpdata1[256];
unsigned char rxtmpdata[256];
volatile unsigned char spi0_rx_flag;
volatile unsigned char spi0_tx_flag;
extern void DataCompare(unsigned char *p1, unsigned char *p2,unsigned int count);
/********************************************************************************************************
**������Ϣ ��SPIM_CSLow(SPI_TypeDef* SPIx)                      
**�������� :Ϊѡ����SPI ��������ڲ�NSS �ܽ� 
**������� ��SPI_TypeDef* SPIx,��ѡ��SPI1,SPI2
**������� ����
********************************************************************************************************/
void SPIM_CSLow()
{
    GPIO_ResetBits( GPIOB, GPIO_Pin_12 );
}

/********************************************************************************************************
**������Ϣ ��SPIM_CSHigh(SPI_TypeDef* SPIx)                      
**�������� :Ϊѡ����SPI ��������ڲ�NSS �ܽ� 
**������� ��SPI_TypeDef* SPIx,��ѡ��SPI1,SPI2
**������� ����
********************************************************************************************************/
void SPIM_CSHigh()
{
    //Spi cs release
    GPIO_SetBits( GPIOB, GPIO_Pin_12 );
}
/********************************************************************************************************
**������Ϣ ��SPIM_TXEn()                     
**�������� :�ر� SPI ��˫��ģʽ�µ����ݴ��䷽�� 
**������� : ��
**������� ����
********************************************************************************************************/
void SPIM_TXEn()
{
    //Transmit Enable bit TXEN
    SPI_BiDirectionalLineConfig(SPI2, SPI_Direction_Tx);
}

/********************************************************************************************************
**������Ϣ ��SPIM_TXDisable()                     
**�������� :�ر� SPI ��˫��ģʽ�µ����ݴ��䷽�� 
**������� ����
**������� ����
********************************************************************************************************/
void SPIM_TXDisable()
{
    //disable TXEN
    SPI_BiDirectionalLineConfig(SPI2, SPI_Disable_Tx);
}

/********************************************************************************************************
**������Ϣ ��SPIM_RXEn()                     
**�������� :�ر� SPI ��˫��ģʽ�µ����ݴ��䷽�� 
**������� ����
**������� ����
********************************************************************************************************/
void SPIM_RXEn()
{
    //enable RXEN
    SPI_BiDirectionalLineConfig(SPI2, SPI_Direction_Rx);
}

/********************************************************************************************************
**������Ϣ ��SPIM_RXDisable()                     
**�������� :�ر� SPI ��˫��ģʽ�µ����ݴ��䷽�� 
**������� ����
**������� ����
********************************************************************************************************/
void SPIM_RXDisable()
{
    //disable RXEN
    SPI_BiDirectionalLineConfig(SPI2, SPI_Disable_Rx);
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
    while (1)
    {
        if(SPI_GetFlagStatus(SPI2, SPI_FLAG_RXAVL))	
        {
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
    
    
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2, ENABLE);  //SPI2 clk enable
    SPIM_CSHigh();
    
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA|RCC_AHBPeriph_GPIOB, ENABLE);
    
    GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_12;   		//spi2_cs  pb12
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 	// ���⸴�����
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    
    GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_13;   		//spi2_sck  pb13
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; 	// ���⸴�����
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    
    GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_15;   		//spi2_mosi  pb15
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; 	// ���⸴�����
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    
    GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_14;  		//spi2_miso  pb14
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; 		//��������   
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    
    GPIO_PinAFConfig(GPIOB,GPIO_PinSource13,GPIO_AF_0);//��Ҫ��SPI����������IO�ڸ��ó������ģ�Ĭ����AF0��
    GPIO_PinAFConfig(GPIOB,GPIO_PinSource14,GPIO_AF_0);
    GPIO_PinAFConfig(GPIOB,GPIO_PinSource15,GPIO_AF_0);
    
    SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
    SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
    SPI_InitStructure.SPI_DataWidth = SPI_DataWidth_8b;
    SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;    			// mode0 SPI_CPOL_Low, SPI_CPHA_1Edge;
    SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;  			// mode3 SPI_CPOL_High,SPI_CPHA_2Edge
    SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
    SPI_InitStructure.SPI_BaudRatePrescaler = spi_baud_div;
    SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
    SPI_Init(SPI2, &SPI_InitStructure);
    SPIM_TXEn();
    SPIM_RXEn();	
    SPI_Cmd(SPI2, ENABLE); //Enables the specified SPI peripheral SPIʹ�ܡ�����ģʽ 8λ����ģʽ SPI �Ĳ�����
    
}

/********************************************************************************************************
**������Ϣ ��SPIM_ReadID(SPI_TypeDef* SPIx)                   
**�������� :��ȡID 
**������� ��SPI_TypeDef* SPIx,��ѡ��SPI1,SPI2
**������� ����
********************************************************************************************************/
void SPIM_ReadID()
{
    unsigned char temp;
    unsigned int i;
    
    SPIM_CSLow();								//Spi cs assign to this pin,select
    SPIMReadWriteByte(RDID);				
    
    for(i=0;i<3;i++)
    {
        temp = SPIMReadWriteByte(0x01);
        printf("temp=0x%x\r\n",temp);		
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
    while(1)
    {
        temp = SPIMReadWriteByte(0x00);	
        if((temp&0x01)==0x0)
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
void SPIM_PageRead(unsigned long address,unsigned char *p,unsigned int number)  //page = 256 bytes
{
    unsigned char addr0,addr1,addr2;
    unsigned int i;
    address = address&0xffffff00; 					//page address
    addr0 = (unsigned char)(address>>16);
    addr1 = (unsigned char)(address>>8);
    addr2 = (unsigned char)address;
    
    SPIM_CSLow(); 								//Spi cs assign to this pin,select
    
    SPIMReadWriteByte(READ);
    SPIMReadWriteByte(addr0);
    SPIMReadWriteByte(addr1);
    SPIMReadWriteByte(addr2);
    
    for(i=0;i<256;i++)
    {
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
void SPIM_PageProgram(unsigned long address,unsigned char *p,unsigned int number)
{
    unsigned int j;
    unsigned char addr0,addr1,addr2;
    address = address&0xffffff00; 					//page address
    addr0 = (unsigned char)(address>>16);
    addr1 = (unsigned char)(address>>8);
    addr2 = (unsigned char)address;
    
    SPIM_WriteEnable();
    SPIM_CSLow();  								//Spi cs assign to this pin,select
    SPIMReadWriteByte(PP);
    SPIMReadWriteByte(addr0);
    SPIMReadWriteByte(addr1);
    SPIMReadWriteByte(addr2);
    for(j=0;j<number;j++)
    {
        SPIMReadWriteByte(*(p++));
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
    unsigned char addr0,addr1,addr2;
    address = address & 0xffff0000;
    addr0 = ((unsigned char)(address>>16))&0xff;
    addr1 = ((unsigned char)(address>>8))&0xff;
    addr2 = ((unsigned char)address)&0xff;
    
    SPIM_WriteEnable();
    
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
    unsigned int i;
    for(i=0;i<256;i++)
    {
        tmpdata[i] = i*2;
    }
    
    printf("SPI2 test\r\n");
    SPIM_Init(0x8);//������ 72M/2=36M
    
    SPIM_ReadID();
    
    SPIM_SectorErase(0);
    
    SPIM_PageProgram(0,tmpdata,256);
    
    for(i=0;i<256;i++)
    {
        rxtmpdata[i] = 0x0;
    }
    SPIM_PageRead(0,rxtmpdata,256); 		//��д��ȥ��һҳ256�ֽڶ�����
    
    for(i=0;i<10;i++)
    {
        printf("rx[%d]=0x%x\r\n",i,rxtmpdata[i]);
    }
    
    printf("SPI2 test over\r\n");
    
}

