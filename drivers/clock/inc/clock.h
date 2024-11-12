#ifndef CLOCK_H__
#define CLOCK_H__

#include "Platform_Types.h"

/******************************************************************************/
/*---------------------------- Macro definition ------------------------------*/
/******************************************************************************/

/******************************************************************************/
/*----------------------------Global Function --------------------------------*/
/******************************************************************************/
/**
 * @brief clock init function for each board
 */
bool clock_init(void);

/**
 * @brief disable clock module
 */
bool clock_deinit(void);

/**
 * @brief write data to clock port
 *
 * @param channel_id  - clock channel ID
 * @param buffer        - pointer to write buffer
 * @param len           - write buffer length
 *
 * @return FALSE - Failed
 *         TRUE  - SUCCESS
 */
bool clock_write(uint8_t channel_id, void *buffer, uint16_t *len);

#endif /* __CRC_H__ */
