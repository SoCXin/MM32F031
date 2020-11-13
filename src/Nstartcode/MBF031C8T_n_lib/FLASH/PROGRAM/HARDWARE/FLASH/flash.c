#include "flash.h"
#include "led.h"
#include "uart.h"
#include "HAL_conf.h"

#define FLASH_PAGE_SIZE    ((uint16_t)0x400)

#define BANK1_WRITE_START_ADDR  ((uint32_t)0x08002800)
#define BANK1_WRITE_END_ADDR    ((uint32_t)0x08003200)
uint32_t EraseCounter = 0x00, Address = 0x00;
uint32_t Data = 0x12345679;
uint32_t NbrOfPage = 0x00;
volatile FLASH_Status FLASHStatus = FLASH_COMPLETE;

u8 FLASH_Program(void)
{
    
    /* Porgram FLASH Bank1 ********************************************************/       
    /* Unlock the Flash Bank1 Program Erase controller */
    FLASH_Unlock();
    
    /* Clear All pending flags */
    FLASH_ClearFlag(FLASH_FLAG_EOP | FLASH_FLAG_PGERR | FLASH_FLAG_WRPRTERR);	
    
    FLASH_ErasePage(BANK1_WRITE_START_ADDR);
    
    FLASH_ClearFlag(FLASH_FLAG_EOP );	
    
    FLASHStatus = FLASH_ProgramWord(0x08001FFC, Data);
    
    FLASH_ClearFlag(FLASH_FLAG_EOP );	
    
    FLASH_Lock();
    
    printf("%x\r\n",(*(__IO uint32_t*) 0x08001FFC));
    
    if((*(__IO uint32_t*) 0x08001FFC) != Data)
    {
        return 1;
    }
    return 0;
}

