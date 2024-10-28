#include "easyos_cfg.h"

OsTimerTbl timerTblLv0Array[OS_LV_TASK_CNT] = 
{
    {
        .no = OS_LV0_TASK0,
        .on = TIMER_ON,
        .type = TIMER_CYCLE,
        .period = TIME_5MS_BASE_1MS,
        .delay = 0,
        .tick = 0
    },
    {
        .no = OS_LV0_TASK1,
        .on = TIMER_ON,
        .type = TIMER_CYCLE,
        .period = TIME_10MS_BASE_1MS,
        .delay = 4,
        .tick = 0
    },
    {
        .no = OS_LV0_TASK2,
        .on = TIMER_ON,
        .type = TIMER_CYCLE,
        .period = TIME_50MS_BASE_1MS,
        .delay = 22,
        .tick = 0
    },
    {
        .no = OS_LV0_TASK3,
        .on = TIMER_ON,
        .type = TIMER_CYCLE,
        .period = TIME_100MS_BASE_1MS,
        .delay = 33,
        .tick = 0
    }
};

OsTimerTbl timerTblLv1Array[OS_LV_TASK_CNT] = 
{
    {
        .no = OS_LV1_TASK0,
        .on = TIMER_ON,
        .type = TIMER_CYCLE,
        .period = TIME_5MS_BASE_1MS,
        .delay = 2,
        .tick = 0
    },
    {
        .no = OS_LV1_TASK1,
        .on = TIMER_ON,
        .type = TIMER_CYCLE,
        .period = TIME_10MS_BASE_1MS,
        .delay = 8,
        .tick = 0

    },
    {
        .no = OS_LV1_TASK2,
        .on = TIMER_ON,
        .type = TIMER_CYCLE,
        .period = TIME_20MS_BASE_1MS,
        .delay = 11,
        .tick = 0

    },
    {
        .no = OS_LV1_TASK3,
        .on = TIMER_ON,
        .type = TIMER_CYCLE,
        .period = TIME_100MS_BASE_1MS,
        .delay = 23,
        .tick = 0

    }
};

OsTimerTbl timerTblLv2Array[OS_LV_TASK_CNT] = 
{
    {
        .no = OS_LV2_TASK0,
        .on = TIMER_ON,
        .type = TIMER_CYCLE,
        .period = TIME_1MS_BASE_1MS,
        .delay = 0,
        .tick = 0
    },
    {
        .no = OS_LV2_TASK1,
        .on = TIMER_OFF,
        .type = TIMER_CYCLE,
        .period = TIME_10MS_BASE_1MS,
        .delay = 0,
        .tick = 0

    },
    {
        .no = OS_LV2_TASK2,
        .on = TIMER_OFF,
        .type = TIMER_CYCLE,
        .period = TIME_20MS_BASE_1MS,
        .delay = 0,
        .tick = 0

    },
    {
        .no = OS_LV2_TASK3,
        .on = TIMER_OFF,
        .type = TIMER_CYCLE,
        .period = TIME_100MS_BASE_1MS,
        .delay = 0,
        .tick = 0

    }
};
