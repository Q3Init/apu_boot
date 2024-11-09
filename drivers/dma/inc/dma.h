#ifndef dma_H
#define dma_H

#include "platform_types.h"

/******************************************************************************/
/*---------------------------- Macro definition ------------------------------*/
/******************************************************************************/

/******************************************************************************/
/*----------------------------Global Function --------------------------------*/
/******************************************************************************/
/**
 * @brief dma init function for each board
 */
bool dma_init(void);

/**
 * @brief disable dma module
 */
bool dma_deinit(void);

/**
 * @brief flush dma module
 */
void dma_flush(void);

/**
 * @brief write data to dma port
 *
 * @param channel_id  - dma channel ID
 * @param buffer        - pointer to write buffer
 * @param len           - write buffer length
 *
 * @return FALSE - Failed
 *         TRUE  - SUCCESS
 */
bool dma_write(uint8_t channel_id, void *buffer, uint16_t *len);

#endif