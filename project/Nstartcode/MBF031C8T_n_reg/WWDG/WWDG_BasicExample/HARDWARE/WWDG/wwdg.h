#ifndef __WWDG_H_
#define __WWDG_H_
#include "HAL_device.h"
void Wwdg_reset_ON(unsigned char ucTcnt,unsigned char ucWcnt);
void Wwdg_ExterslReloadReset_ON(void);
void Wwdg_irq_ON(void);
#endif
