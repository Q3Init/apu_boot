#ifndef FLASH_TYPE_H
#define FLASH_TYPE_H

#include "platform_types.h"

typedef struct flash_device
{
    void (*init)(void);
    void (*deinit)(void);
    std_return_t (*read)(uint32_t, uint32_t, uint32_t *);
    std_return_t (*write)(uint32_t, uint32_t, uint32_t *);
    uint32_t (*get_sector_size)(uint8_t);
    uint32_t (*get_page_size)(uint8_t);
    std_return_t (*erase)(uint32_t, uint32_t);
    uint8_t flash_driver_index;
} flash_device_t;

#endif

