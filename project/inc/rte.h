#ifndef RTE_H_
#define RTE_H_

#include "Platform_Types.h"

#define __DISENBLE_IRQ() __disable_irq()
#define __ENABLE_IRQ() __enable_irq()

extern void rte_init(void);
extern boolean Get_rteBswRdyFlg(void);

#endif
