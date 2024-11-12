#ifndef UART_H
#define UART_H

#include "Platform_Types.h"

/******************************************************************************/
/*---------------------------- Macro definition ------------------------------*/
/******************************************************************************/

/******************************************************************************/
/*----------------------------Global Function --------------------------------*/
/******************************************************************************/
/**
 * @brief uart init function for each board
 */
bool uart_init(void);

/**
 * @brief disable uart module
 */
bool uart_deinit(void);

/**
 * @brief flush uart module
 */
void uart_flush(void);

/**
 * @brief write data to uart port
 *
 * @param channel_id  - uart channel ID
 * @param buffer        - pointer to write buffer
 * @param len           - write buffer length
 *
 * @return FALSE - Failed
 *         TRUE  - SUCCESS
 */
bool uart_write(uint8_t channel_id, void *buffer, uint16_t len);

#endif
