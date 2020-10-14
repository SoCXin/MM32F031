#include "HAL_device.h"
#include "uart.h"

volatile unsigned char rx_flag = 0;
volatile unsigned char tx_flag = 0;
extern uint16_t I2C_DMA_DIR;
unsigned char tx_buffer0[16] = {0,1,2,3,4,5,6,7,8,9,0xa,0xb,0xc,0xd,0xe,0xf};
unsigned char rx_buffer0[16] ;
#define FLASH_DEVICE_ADDR 0xa0
/********************************************************************************************************
**函数信息 ：I2CInitMasterMode(void) //unit is Khz                    
**功能描述 ：初始化I2C
**输入参数 ：无
**输出参数 ：无
********************************************************************************************************/
void I2CInitMasterMode(void) 
{	
    RCC->AHBENR|=0x7<<17;
    RCC->APB1ENR|=0x00200000;
    
    GPIOB->CRH|=0x00ff;
    GPIOB->ODR|=0x3;
    
    I2C1->IC_CON = 0x163;
    I2C1->IC_TAR = FLASH_DEVICE_ADDR>>1;
    I2C1->IC_SS_SCL_HCNT = 0x77;
    I2C1->IC_SS_SCL_LCNT = 0x55;
    I2C1->IC_RX_TL=0x0;//Rx FIFO  
    I2C1->IC_TX_TL=0x0;//Tx FIFO 
    I2C1->IC_INTR_MASK=0;//disable all
    I2C1->IC_ENABLE |= 0x01;
    
    GPIOB->AFRH&=~0xff;	
    GPIOB->AFRH|=0x11;
}
void I2CTXEmptyCheck()
{
    while(1)
    {
        if((I2C1->IC_RAW_INTR_STAT & I2C_RAW_INTR_MASK_TX_EMPTY))//发送缓冲区空
            break; 
    }
}

void I2CRXFullCheck()
{
    I2C1->IC_DATA_CMD = I2C_DATA_CMD_CMD;//读数据指令，不可省略
    while(1)
    {
        if((I2C1->IC_RAW_INTR_STAT & I2C_RAW_INTR_MASK_RX_FULL))//接收缓冲区非空
            break; 
    }
}



void I2CTXByte(u8 data)
{
    I2C1->IC_DATA_CMD=data;//发送数据
    I2CTXEmptyCheck();
}


unsigned char I2CRXByte()
{
    unsigned char temp;
    I2CRXFullCheck();
    temp=I2C1->IC_DATA_CMD;
    return temp;
}

void I2CMasterWrite(I2C_TypeDef *I2Cx,unsigned char device_id, unsigned short mem_byte_addr, unsigned short tx_count, unsigned char *tx_data )
{
    unsigned short temp;
    unsigned short i;
    unsigned char *p;
    uint32_t j;
    p = tx_data;
    
    I2C1->IC_ENABLE &= 0xFFFE;
    I2C1->IC_TAR = 0xa0>>1;
    I2C1->IC_ENABLE |= 0x0001;
    temp = ((mem_byte_addr) & 0xff);
    I2CTXByte(temp);  //tx memory addr
    
    for(i=0;i<tx_count;i++)
    {
        temp = *p;
        p++;
        if(i==(tx_count-1))
            I2C1->IC_DATA_CMD =temp| 0x200; // //muaul set stop flag
        else
            I2CTXByte(temp); //tx data
        
    }	
    
    for(j=0;j<0x5000;j++); 
}

void I2CMasterRead(I2C_TypeDef *I2Cx,unsigned char device_id, unsigned short mem_byte_addr, unsigned short rx_count, unsigned char *rx_data )
{
    unsigned char temp=0;
    unsigned short i;
    
    I2Cx->IC_TAR = 0xa0>>1;
    
    I2C1->IC_ENABLE |= 0x0001;
    
    
    temp = ((mem_byte_addr) & 0xff);
    
    I2CTXByte(temp);  //tx memory addr
    
    
    for(i=0;i<rx_count;i++)
    {
        rx_data[i] = I2CRXByte();
        
        printf("RX data%d is  : %x \r\n",i,rx_data[i]);
        
    }
    
}

void I2CMasterTest()
{  
    unsigned char i;
    
    I2CInitMasterMode();
    
    I2CMasterWrite(I2C1,FLASH_DEVICE_ADDR, 16*1, 16, tx_buffer0 );
    
    for(i = 0; i < 16 ;i ++)
    {
        printf("TX data is: %x \r\n", tx_buffer0[i]);
    }
    I2CMasterRead(I2C1,FLASH_DEVICE_ADDR, 16*1, 16, rx_buffer0 );
}
