#ifndef CLOCK_CFG_H
#define CLOCK_CFG_H

#include "Platform_Types.h"
#include "apm32e10x_rcm.h"

typedef struct
{
    uint32_t Clock;
} CLOCKSignalsCfg;

#define CLOCK_SIGNALS_CNT 2

#endif
