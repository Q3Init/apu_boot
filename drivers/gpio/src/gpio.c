
/******************************************************************************/
/*---------------------------    Include      --------------------------------*/
/******************************************************************************/
#include "device.h"
#include "gpio_type.h"

/******************************************************************************/
/*-------------------  Local Variable Definitions    -------------------------*/
/******************************************************************************/
static const gpio_device_t *gp_gpio_device = NULL;
static bool g_gpio_initialized = FALSE;

/******************************************************************************/
/*----------------------------Local Function ---------------------------------*/
/******************************************************************************/
DEVICE_INITFUNC(apm32exx_gpio_preinit);

/**
 * @brief preinitialize gpio module,register gpio devices
 */
static void gpio_preinit(void) 
{
    apm32exx_gpio_preinit();
}

/******************************************************************************/
/*----------------------------Global Function --------------------------------*/
/******************************************************************************/
/**
 * @brief initialize GPIO interface
 */
void gpio_init(void) 
{
    if (!g_gpio_initialized)
    {
        gpio_preinit();
        if ((NULL != gp_gpio_device) && \
             (NULL != gp_gpio_device->init))
        {
            gp_gpio_device->init();
            g_gpio_initialized = TRUE;
        }
    }
}

/**
 * @brief de-initialize GPIO interface
 */
void gpio_deinit(void)
{
    if((TRUE == g_gpio_initialized) && \
       (NULL != gp_gpio_device) && \
       (NULL != gp_gpio_device->deinit))
    {
        gp_gpio_device->deinit();
        g_gpio_initialized = FALSE;
    }
}

/** 
 * @brief write/update a gpio pin 
 * 
 * @param port : port number
 * @param pin :  pin number
 * @param val : set new value
 */
void gpio_write(uint32_t port, uint16_t pin, uint8_t val)
{
    if((TRUE == g_gpio_initialized) && \
       (NULL != gp_gpio_device) && \
       (NULL != gp_gpio_device->write))
    {
        gp_gpio_device->write(port,pin,val);
    }
}

/** 
 * @brief read from a gpio pin
 * 
 * @param port : port number
 * @param pin :  pin number
 */
uint8_t gpio_read(uint32_t port, uint16_t pin)
{
    uint8_t ret = 0xFFu;

    if((TRUE == g_gpio_initialized) && \
       (NULL != gp_gpio_device) && \
       (NULL != gp_gpio_device->read))
    {
        ret = gp_gpio_device->read(port,pin);
    }

    return ret;
}

/**
 * @brief function to register gpio device
 * 
 * @param p_dev device pointer
 *  
 * @return NULL
 */
void gpio_register(const gpio_device_t *p_dev)
{
    if (p_dev)
    {
        gp_gpio_device = p_dev;
    }
}
