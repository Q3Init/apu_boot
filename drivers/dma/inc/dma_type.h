#ifndef DMA_TYPE_H
#define DMA_TYPE_H

#include "platform_types.h"

/**
 * @brief structure for dma Interface
 *
 */
typedef struct dma_device
{
    std_return_t (*init)(void);
    std_return_t (*deinit)(void); // disable dma port
} dma_device_t;

void dma_register(const dma_device_t *p_dev);

#endif
