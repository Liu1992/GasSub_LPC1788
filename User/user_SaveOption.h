
#ifndef __USER_SAVE_OPTION_H_
#define __USER_SAVE_OPTION_H_

#include "ucos_ii.h"
#include "eeprom_st24c02.h"

Bool EEPROM_WriteStruct(EEPROMDataStruct savestruct);
EEPROMDataStruct EEPROM_ReadStruct(void);
Bool EEPROM_WriteRati(Correct_Input savestruct);
Correct_Input EEPROM_ReadRati();

#endif
