#include "flash.h"
#include "uart.h"
#include "flash.h"

#define FLASH_PAGE_SIZE    ((uint16_t)0x400)

#define BANK1_WRITE_START_ADDR  ((uint32_t)0x08002800)
#define BANK1_WRITE_END_ADDR    ((uint32_t)0x08003200)
uint32_t EraseCounter = 0x00, Address = 0x00;
uint32_t Data = 0x12345678;
uint32_t NbrOfPage = 0x00;

u8 FLASH_Program(void)
{
    
    /* Porgram FLASH Bank1 ********************************************************/       
    /* Unlock the Flash Bank1 Program Erase controller */
    FLASH->KEYR = 0x45670123;
    FLASH->KEYR = 0xCDEF89AB;
    
    /* Clear All pending flags */
    FLASH->SR = 0x00000001|0x00000004|0x00000010;
    
    FLASH->CR |= 0x1;
    
    *(__IO uint16_t*)0x08001ffc = (uint16_t)Data;
    *(__IO uint16_t*)(0x08001ffc + 2) = Data >> 16;
    
    FLASH->CR |= 0x00000080;
    
    printf("%x\r\n",(*(__IO uint32_t*) 0x08001ffc));
    
    if((*(__IO uint32_t*) 0x08001ffc) != Data)
    {
        return 1;
    }
    return 0;
}

