/******************************************************************************/
/*---------------------------    Include      --------------------------------*/
/******************************************************************************/
#include "device.h"
#include "uart_type.h"

/******************************************************************************/
/*-------------------  Local Variable Definitions    -------------------------*/
/******************************************************************************/
static const uart_device_t *gp_uart_device = NULL;
static bool g_uart_initialized = FALSE;

/******************************************************************************/
/*----------------------------Local Function ---------------------------------*/
/******************************************************************************/
DEVICE_INITFUNC(apm32exx_uart_preinit);

/**
 * @brief preinitialize uart module,register uart devices
 */
static void uart_preinit(void)
{
    apm32exx_uart_preinit();
}

/******************************************************************************/
/*----------------------------Global Function --------------------------------*/
/******************************************************************************/
/**
 * @brief uart init function for each board
 */
bool uart_init(void)
{
    bool ret = 0U;

    if (FALSE == g_uart_initialized)
    {
        uart_preinit();
        if ((NULL != gp_uart_device) && (NULL != gp_uart_device->init))
        {
            gp_uart_device->init();
            g_uart_initialized = TRUE;
            ret = TRUE;
        }
    }

    return ret;
}

/**
 * @brief disable uart module
 */
bool uart_deinit(void)
{
    bool ret = FALSE;

    if ((TRUE == g_uart_initialized) &&
        (NULL != gp_uart_device) &&
        (NULL != gp_uart_device->deinit))
    {
        ret = gp_uart_device->deinit();
    }

    g_uart_initialized = FALSE;

    return ret;
}

/**
 * @brief flush uart
 */
void uart_flush(void)
{
    if ((TRUE == g_uart_initialized) &&
        (NULL != gp_uart_device) &&
        (NULL != gp_uart_device->flush))
    {
        gp_uart_device->flush();
    }
}

/**
 * @brief read data from uart port
 *
 * @param channel_id  - uart channel ID
 * @param buffer        - pointer to read buffer
 * @param len           - read buffer length
 *
 * @return FALSE - Failed
 *         TRUE  - SUCCESS
 */
bool uart_read(uint8_t channel_id, void *buffer, uint16_t *len)
{
    bool ret = FALSE;

    if ((TRUE == g_uart_initialized) &&
        (NULL != gp_uart_device) &&
        (NULL != gp_uart_device->read))
    {
        ret = gp_uart_device->read(channel_id, buffer, len);
    }

    return ret;
}

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
bool uart_write(uint8_t channel_id, void *buffer, uint16_t len)
{
    bool ret = FALSE;

    if ((TRUE == g_uart_initialized) &&
        (NULL != gp_uart_device) &&
        (NULL != gp_uart_device->write))
    {
        ret = gp_uart_device->write(channel_id, buffer, len);
    }

    return ret;
}

/**
 * @brief function to register uart device
 *
 * @param p_dev device pointer
 *
 * @return NULL
 */
void uart_register(const uart_device_t *p_dev)
{
    if (p_dev)
    {
        gp_uart_device = p_dev;
    }
}
