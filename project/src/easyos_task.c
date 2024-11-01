#include "easyos_task.h"
/* User define */
#include "uartif.h"
#include "iap.h"

OS_LOCAL uint8 Os_TaskHdl_Lv0_Task0(void)
{
    uint8 ret = OS_RET_OK;
    UartIf_MainFunction();
    Iap_MainFunction();
    return ret;
}

OS_LOCAL uint8 Os_TaskHdl_Lv0_Task1(void)
{
    uint8 ret = OS_RET_OK;

    
    return ret;
}

OS_LOCAL uint8 Os_TaskHdl_Lv0_Task2(void)
{
    uint8 ret = OS_RET_OK;
    return ret;
}

OS_LOCAL uint8 Os_TaskHdl_Lv0_Task3(void)
{
    uint8 ret = OS_RET_OK;
    
    return ret;
}
