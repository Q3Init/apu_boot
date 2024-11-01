#include "rte.h"
#ifdef EASY_OS
#include "Os_EvtHdl.h"
#include "Os_TimerHdl.h"
#include "Os_TaskHdl_Lv0.h"
#include "Os_TaskHdl_Lv1.h"
#include "Os_TaskHdl_Lv2.h"
#endif
#ifdef CONFIG_GPIO
#include "gpio.h"
#endif
#include "uartif.h"
#include "intertp.h"
#include "iap.h"
/* user file*/
#include "apm32e10x.h"

static volatile boolean rteBswRdyFlg = FALSE;
void rte_init(void)
{
    __DISENBLE_IRQ(); /* chip disable irq */

    /* OS Init */
#ifdef EASY_OS
    SysTick_Config(SystemCoreClock / 1000);
    OsEvt_Init();
    OsTimer_Init();
#endif
#if CONFIG_GPIO
    gpio_init();
#endif
    UartIf_Init();
    InterTp_Init();
    Iap_Init();   

    rteBswRdyFlg = TRUE; /* Init complete flag */
    __ENABLE_IRQ(); /* chip enable irq */
}


boolean Get_rteBswRdyFlg(void)
{
	return(rteBswRdyFlg);
}
