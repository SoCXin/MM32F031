#ifndef __I2C_H_
#define __I2C_H_
#include "HAL_device.h"
#include "HAL_conf.h"
void I2C1MasterDMATest(void);
void I2CInitMasterMode(unsigned int uiI2C_speed,unsigned char TarAddress);
#endif
