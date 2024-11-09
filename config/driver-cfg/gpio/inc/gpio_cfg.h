#ifndef GPIO_CFG_H
#define GPIO_CFG_H

#include "Platform_Types.h"
#include "apm32e10x_gpio.h"
#include "apm32e10x_rcm.h"

#define GPIO_SIGNALS_CNT 80

typedef struct
{
    GPIO_T *GPIO;
    uint32_t APB1_Clock;
    uint32_t APB2_Clock;
    GPIO_Config_T mcal_config_gpio;
    uint32_t GPIO_level;
} GPIOSignalsCfg;

extern const GPIOSignalsCfg gpioSignalsCfgTable[];

#endif
