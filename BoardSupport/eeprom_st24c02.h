#ifndef __EEPROM_ST24C02_H_
#define __EEPROM_ST24C02_H_

#include "lpc177x_8x_i2c.h"
#include "DataStruct.h"
#include <stdlib.h>

void EEPROM_24c02_Init(void);
Bool EEPROM_WriteDataOnePage(uint8_t data[],uint8_t size,uint8_t address);
Bool EEPROM_ReadDataOnePage(uint8_t data[],uint8_t size,uint8_t address);
Bool EEPROM_ReadData(uint8_t data[],uint8_t size,uint8_t address);


#endif
