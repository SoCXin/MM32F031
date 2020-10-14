#include "HAL_device.h"
#include "spi.h"
#include "uart.h"	  

unsigned char tmpdata[256];
unsigned char tmpdata1[256];
unsigned char rxtmpdata[256];
volatile unsigned char spi0_rx_flag;
volatile unsigned char spi0_tx_flag;
extern void DataCompare(unsigned char *p1, unsigned char *p2,unsigned int count);
/********************************************************************************************************
**������Ϣ ��SPIM_CSLow()                      
**�������� :Ϊѡ����SPI ��������ڲ�NSS �ܽ� 
**������� ����
**������� ����
********************************************************************************************************/
void SPIM_CSLow()
{
    //Spi cs assign to this pin,select	
    SPI2->NSSR &= 0xfffe;
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
    SPI2->NSSR |= ~0xfffe;
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
    SPI2->GCTL |= 0x0008;
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
    SPI2->GCTL |= 0xfff7;
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
    SPI2->GCTL |= 0x0010;
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
    SPI2->GCTL |= 0xffef;
}


/********************************************************************************************************
**������Ϣ ��SPIMReadWriteByte(unsigned char tx_data)                    
**�������� : ͨ������ SPIx �շ����� ,����ȫ˫��ģʽ(ͬʱ�շ�)
**������� ����
**������� ����
********************************************************************************************************/
unsigned int SPIMReadWriteByte(unsigned char tx_data)
{	
    SPI2->TXREG = tx_data;
    while (1)
    {
        //if(SPI_GetFlagStatus(SPI2, SPI_FLAG_RXAVL))	
        if((SPI2->CSTAT&0x0002)==0x0002)
        {
            return SPI2->RXREG;
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
    RCC->APB1ENR|=1<<14;
    SPIM_CSHigh();
    
    RCC->AHBENR|=0x7<<17;
    
    GPIOB->CRH&=0x0000FFFF;//����PB12,PB13,PB14,PB15Ϊȱʡֵ
    GPIOB->CRH|=0xB0BB0000;//PB15,PB13���츴�����
    GPIOB->CRH|=0x08000000;//PB14��������
    
    GPIOB->AFRH&=~0xffff0000;
    
    GPIOB->ODR|=GPIO_ODR_ODR14;
    SPI2->GCTL|=SPI_GCTL_MM;//����ģʽ
    SPI2->CCTL|=SPI_CCTL_SPILEN;//8λ���ݿ��
    
    SPI2->CCTL&=~SPI_CCTL_CPOL;//SPI_CPOL_Low
    SPI2->CCTL|=SPI_CCTL_CPHA;//SPI_CPHA_1Edge
    
    SPI2->GCTL&=~SPI_GCTL_NSS_SEL;//�������NSS
    SPI2->CCTL |=0x10;//��������ģʽ
    SPI2->SPBRG =0x2;//2��Ƶ��������Ӧ��Ϊ72M/2=36M
    SPI2->CCTL&=~SPI_CCTL_LSBFE;//MSB
    SPI2->GCTL |=SPI_GCTL_SPIEN;//ʹ��SPI
    
    SPIM_TXEn();
    SPIM_RXEn();	
    SPI2->GCTL |= 0x01;
    
}

/********************************************************************************************************
**������Ϣ ��SPIM_ReadID(SPI_TypeDef* SPIx)                   
**�������� :��ȡID 
**������� ��SPI_TypeDef* SPIx,��ѡ��SPI2,SPI2
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
    SPIM_Init(0x4);//������ 72M/2=36M
    
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

