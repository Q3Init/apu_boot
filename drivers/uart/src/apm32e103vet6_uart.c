/******************************************************************************/
/*---------------------------    Include      --------------------------------*/
/******************************************************************************/
#include "uart_type.h"
#include "uart_cfg.h"
/******************************************************************************/
/*----------------------------Local  Function --------------------------------*/
/******************************************************************************/
/**
 * @brief initialize APM32E10xx GPIO interface
 */
static std_return_t apm32e10xx_uart_init(void)
{
    bool ret = FALSE;

    if (1)
    {
        ret = TRUE;
    }

    return ret;
}

/**
 * @brief de-initialize APM32E10xx GPIO interface
 */
static std_return_t apm32e10xx_uart_deinit(void)
{
    bool ret = FALSE;

    if (1)
    {
        ret = TRUE;
    }

    return ret;
}

/**
 * @brief send data to serial port
 *
 * @param channel_id  - uart channel ID
 * @param data   - pointer to read buffer
 * @param count  - number of data
 *
 * @return TRUE  - if success
 * @return FALSE - otherwise
 */
static bool apm32e10xx_write_buffer(uint8_t channel_id, void *data, uint16_t *count)
{
    bool ret = FALSE;

    if (1)
    {
        ret = TRUE;
    }

    return ret;
}

/******************************************************************************/
/*-------------------  Local Variable Definitions    -------------------------*/
/******************************************************************************/
static const uart_device_t apm32e10xx_uart_device =
    {
        .init = apm32e10xx_uart_init,
        .write = apm32e10xx_write_buffer,
        // .read = apm32e10xx_uart_read,
        .deinit = apm32e10xx_uart_deinit,
};
/******************************************************************************/
/*----------------------------Global Function --------------------------------*/
/******************************************************************************/
/**
 * @brief register s32k3xx uart device to uart wrapper
 */
void s32k3xx_uart_preinit(void)
{
    uart_register(&apm32e10xx_uart_device);
}
