#ifndef CLOCK_TYPE_H
#define CLOCK_TYPE_H

#include "Platform_Types.h"

/**
 * @brief structure for clock Interface
 *
 */
typedef struct clock_device
{
    std_return_t (*init)(void);
    std_return_t (*deinit)(void); // disable clock port
} clock_device_t;

void clock_register(const clock_device_t *p_dev);

#endif
