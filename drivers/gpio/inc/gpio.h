#ifndef GPIO_H
#define GPIO_H

#include "Platform_Types.h"

/**
 * @brief initialize gpio interfaces
 */
void gpio_init(void);

/**
 * @brief de-initialize gpio interfaces
 */
void gpio_deinit(void);

/**
 * @brief write/update a gpio pin
 *
 * @param port : port number
 * @param pin :  pin number
 * @param val : set new value
 */
void gpio_write(uint32_t port, uint16_t pin, uint8_t val);

/**
 * @brief read from a gpio pin
 *
 * @param port : port number
 * @param pin :  pin number
 */
uint8_t gpio_read(uint32_t port, uint16_t pin);

#endif
