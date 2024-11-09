#ifndef RTE_H_
#define RTE_H_

#include "Platform_Types.h"

#define CONFIG_GPIO
#define CONFIG_FLASH
#define CONFIG_CLOCK
#define CONFIG_UART
#define CONFIG_DMA

#define __DISENBLE_IRQ() __disable_irq()
#define __ENABLE_IRQ() __enable_irq()

extern void rte_init(void);
extern boolean Get_rteBswRdyFlg(void);
extern void Rte_McuPerformReset(void);

#endif
