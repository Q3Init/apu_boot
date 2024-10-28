#ifndef OS_TIMER_HANDLE_H_
#define OS_TIMER_HANDLE_H_

#include "Os_Common.h"
#include "easyos_cfg.h"

OS_EXTERN void OsTimer_Init(void);
OS_EXTERN void OsTimer_CheckTimeout(uint8 evtLv);
OS_EXTERN void OsTimer_StartTimer(uint8 evtLv, uint8 no);
OS_EXTERN void OsTimer_StopTimer(uint8 evtLv, uint8 no);
#endif
