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
**函数信息 ：SPIM_CSLow()                      
**功能描述 :为选定的SPI 软件重置内部NSS 管脚 
**输入参数 ：无
**输出参数 ：无
********************************************************************************************************/
void SPIM_CSLow()
{
    //Spi cs assign to this pin,select	
    SPI2->NSSR &= 0xfffe;
}

/********************************************************************************************************
**函数信息 ：SPIM_CSHigh()                      
**功能描述 :为选定的SPI 软件配置内部NSS 管脚 
**输入参数 ：无
**输出参数 ：无
********************************************************************************************************/
void SPIM_CSHigh()
{
    //Spi cs release
    SPI2->NSSR |= ~0xfffe;
}

/********************************************************************************************************
**函数信息 ：SPIM_TXEn()                     
**功能描述 :关闭 SPI 在双向模式下的数据传输方向 
**输入参数 : 无
**输出参数 ：无
********************************************************************************************************/
void SPIM_TXEn()
{
    //Transmit Enable bit TXEN
    SPI2->GCTL |= 0x0008;
}

/********************************************************************************************************
**函数信息 ：SPIM_TXDisable()                     
**功能描述 :关闭 SPI 在双向模式下的数据传输方向 
**输入参数 ：无
**输出参数 ：无
********************************************************************************************************/
void SPIM_TXDisable()
{
    //disable TXEN
    SPI2->GCTL |= 0xfff7;
}

/********************************************************************************************************
**函数信息 ：SPIM_RXEn()                     
**功能描述 :关闭 SPI 在双向模式下的数据传输方向 
**输入参数 ：无
**输出参数 ：无
********************************************************************************************************/
void SPIM_RXEn()
{
    //enable RXEN
    SPI2->GCTL |= 0x0010;
}

/********************************************************************************************************
**函数信息 ：SPIM_RXDisable()                     
**功能描述 :关闭 SPI 在双向模式下的数据传输方向 
**输入参数 ：无
**输出参数 ：无
********************************************************************************************************/
void SPIM_RXDisable()
{
    //disable RXEN
    SPI2->GCTL |= 0xffef;
}


/********************************************************************************************************
**函数信息 ：SPIMReadWriteByte(unsigned char tx_data)                    
**功能描述 : 通过外设 SPIx 收发数据 ,用于全双工模式(同时收发)
**输入参数 ：无
**输出参数 ：无
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
**函数信息 ：SPIM_Init(unsigned short spi_baud_div)            
**功能描述 :可修改参数初始化SPI
**输入参数 ：spi_baud_div
**输出参数 ：无
********************************************************************************************************/
void SPIM_Init(unsigned short spi_baud_div)
{
    RCC->APB1ENR|=1<<14;
    SPIM_CSHigh();
    
    RCC->AHBENR|=0x7<<17;
    
    GPIOB->CRH&=0x0000FFFF;//设置PB12,PB13,PB14,PB15为缺省值
    GPIOB->CRH|=0xB0BB0000;//PB15,PB13推挽复用输出
    GPIOB->CRH|=0x08000000;//PB14上拉输入
    
    GPIOB->AFRH&=~0xffff0000;
    
    GPIOB->ODR|=GPIO_ODR_ODR14;
    SPI2->GCTL|=SPI_GCTL_MM;//主机模式
    SPI2->CCTL|=SPI_CCTL_SPILEN;//8位数据宽度
    
    SPI2->CCTL&=~SPI_CCTL_CPOL;//SPI_CPOL_Low
    SPI2->CCTL|=SPI_CCTL_CPHA;//SPI_CPHA_1Edge
    
    SPI2->GCTL&=~SPI_GCTL_NSS_SEL;//软件控制NSS
    SPI2->CCTL |=0x10;//开启高速模式
    SPI2->SPBRG =0x2;//2分频，波特率应该为72M/2=36M
    SPI2->CCTL&=~SPI_CCTL_LSBFE;//MSB
    SPI2->GCTL |=SPI_GCTL_SPIEN;//使能SPI
    
    SPIM_TXEn();
    SPIM_RXEn();	
    SPI2->GCTL |= 0x01;
    
}

/********************************************************************************************************
**函数信息 ：SPIM_ReadID(SPI_TypeDef* SPIx)                   
**功能描述 :读取ID 
**输入参数 ：SPI_TypeDef* SPIx,可选择SPI2,SPI2
**输出参数 ：无
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
**函数信息 ：SPIM_WriteEnable()                  
**功能描述 :写数据使能
**输入参数 ：无
**输出参数 ：无
********************************************************************************************************/
void SPIM_WriteEnable()
{
    SPIM_CSLow(); 								//Spi cs assign to this pin,select
    SPIMReadWriteByte(WREN);	
    SPIM_CSHigh(); 								//Spi cs release
}

/********************************************************************************************************
**函数信息 ：SSPIM_checkStatus()                 
**功能描述 :检查数据发送是否正确
**输入参数 ：无
**输出参数 ：无
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
**函数信息 ：SPIM_WriteEnable()                  
**功能描述 :写数据失能
**输入参数 ：无
**输出参数 ：无
********************************************************************************************************/
void SPIM_WriteDisable()
{	
    SPIM_CSLow();
    SPIMReadWriteByte(WRDI);
    SPIM_CSHigh();
}

/********************************************************************************************************
**函数信息 ：SPIM_PageRead(unsigned long address,unsigned char *p,unsigned int number)               
**功能描述 :读取256页数据
**输入参数 ： address  ;*p ;number
**输出参数 ：无
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
**函数信息 ：SPIM_PageProgram(unsigned long address,unsigned char *p,unsigned int number)              
**功能描述 :发送256页数据
**输入参数 ： address;*p;number
**输出参数 ：无
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
**函数信息 ：SPIM_SectorErase()             
**功能描述 :擦除数据
**输入参数 ：unsigned long address,删除指定sector的地址 each sector = 64Kbytes
**输出参数 ：无
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
**函数信息 ：SPIM_BlockErase()             
**功能描述 :擦除数据
**输入参数 ：无
**输出参数 ：无
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
**函数信息 ：SPIM_Close()             
**功能描述 : CS信号拉高，停止通信
**输入参数 ：无
**输出参数 ：无
********************************************************************************************************/
void SPIM_Close()
{
    SPIM_CSHigh();  							//Spi cs release
    SPIMReadWriteByte(0x01);
}

/********************************************************************************************************
**函数信息 ：SPIM_Test()          
**功能描述 :测试程序，使用串口打印256页数据
**输入参数 ：无
**输出参数 ：无
********************************************************************************************************/
void SPIM_Test(void)
{
    unsigned int i;
    for(i=0;i<256;i++)
    {
        tmpdata[i] = i*2;
    }
    
    printf("SPI2 test\r\n");
    SPIM_Init(0x4);//波特率 72M/2=36M
    
    SPIM_ReadID();
    
    SPIM_SectorErase(0);
    
    SPIM_PageProgram(0,tmpdata,256);
    
    for(i=0;i<256;i++)
    {
        rxtmpdata[i] = 0x0;
    }
    SPIM_PageRead(0,rxtmpdata,256); 		//把写进去的一页256字节读出来
    
    for(i=0;i<10;i++)
    {
        printf("rx[%d]=0x%x\r\n",i,rxtmpdata[i]);
    }
    
    printf("SPI2 test over\r\n");
    
    
    
}

