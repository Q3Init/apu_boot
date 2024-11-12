#ifndef BOOTCHECK_CFG_H_
#define BOOTCHECK_CFG_H_

#include "Platform_Types.h"
#include "apm32e10x.h"

typedef struct 
{
    /* data */
    uint32 startAddr;
}FlashSlotInfoType;

typedef enum
{
    FLASH_SLOTA_APP_A = 0,
    FLASH_SLOT_CNT
}FLASH_SLOT;

/* Power On */
#define ENTERED_WAY_POWERON             (0x00000000UL)
/* Softrest-SRST */
#define BOOT_ENTER_APP_WAT_SOFT_RESET   (0x53525354UL)

/* APP Jump to Boot */
/* Uart req */
#define APP_JUMP_TO_BOOT_REQ1           (0x00011002UL)

/* Integrity app flag address */
#define INTEGRITY_APP_FLAG_ADDRESS      (0x08010800)
#define INTEGRITY_APP_FLAG_OK           (0xAA)

/* system irq control */
#define __DISENBLE_IRQ() __disable_irq()
#define __ENABLE_IRQ() __enable_irq()

extern const FlashSlotInfoType flashSlotInfoTable[FLASH_SLOT_CNT];

#endif
