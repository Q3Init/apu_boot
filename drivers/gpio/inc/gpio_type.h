#ifndef GPIO_TYPE_H
#define GPIO_TYPE_H

#include "Platform_Types.h"

typedef struct gpio_device
{
    /** interface specific initialization */
    void (*init)(void);
    
    /** interface specificde-initialization */
    void (*deinit)(void);

    /** function pointer to write to a channel */
    void (*write)(uint32_t, uint16_t, uint8_t);

    /** function pointer to read from a channel */
    uint8_t (*read)(uint32_t, uint16_t);

}gpio_device_t;

void gpio_register(const gpio_device_t *p_dev);
#endif
