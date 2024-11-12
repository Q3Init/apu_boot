#ifndef FLASH_CFG_H
#define FLASH_CFG_H

#include "Platform_Types.h"
#include "apm32e10x_fmc.h"
#include "apm32e10x.h"

#define CONFIG_APM32E1XX_PFLASH_SECTOR_SIZE  0x800
#define CONFIG_APM32E1XX_PFLASH_PAGE_SIZE    0x800
#define APM32_FLASH_BASE_ADDR FMC_BASE
#define APM32_FLASH_SIZE 0x80000

#endif
