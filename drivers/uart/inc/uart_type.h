#ifndef UART_TYPE_H
#define UART_TYPE_H

#include "platform_types.h"

/**
 * @brief structure for uart Interface
 *
 */
typedef struct uart_device
{
    std_return_t (*init)(void);
    std_return_t (*deinit)(void);               // disable uart port
    bool (*read)(uint8_t, void *, uint16_t *);  // read from FIFO
    bool (*write)(uint8_t, void *, uint16_t *); // write to FIFO
    void (*flush)(void);                        // flush uart
} uart_device_t;

void uart_register(const uart_device_t *p_dev);

#endif

