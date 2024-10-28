#ifndef EASY_OS_CFG_h
#define EASY_OS_CFG_h

#include "Platform_Types.h"

#define TIME_1MS_BASE_1MS 1
#define TIME_5MS_BASE_1MS 5
#define TIME_10MS_BASE_1MS 10
#define TIME_20MS_BASE_1MS 20
#define TIME_50MS_BASE_1MS 50
#define TIME_100MS_BASE_1MS 100

#define TIMER_ON TRUE
#define TIMER_OFF FALSE

#define TIMER_CYCLE   1
#define TIMER_ONESHOT 0

#define OS_LV0_TASK0 0
#define OS_LV0_TASK1 1
#define OS_LV0_TASK2 2
#define OS_LV0_TASK3 3

#define OS_LV1_TASK0 0
#define OS_LV1_TASK1 1
#define OS_LV1_TASK2 2
#define OS_LV1_TASK3 3

#define OS_LV2_TASK0 0
#define OS_LV2_TASK1 1
#define OS_LV2_TASK2 2
#define OS_LV2_TASK3 3

#define OS_LV_TASK_CNT 4

typedef struct {
    uint8 no;
    boolean on;
    uint8 type;
    uint16 period;
    uint16 delay;
    uint16 tick;
}OsTimerTbl;

extern OsTimerTbl timerTblLv0Array[];
extern OsTimerTbl timerTblLv1Array[];
extern OsTimerTbl timerTblLv2Array[];

#endif
