
/******************************************************************************/
/*---------------------------    Include      --------------------------------*/
/******************************************************************************/
#include "gpio_type.h"
#include "gpio_cfg.h"
/******************************************************************************/
/*----------------------------Local  Function --------------------------------*/
/******************************************************************************/
/**
 * @brief initialize APM32E10xx GPIO interface
 */
static void apm32e10xx_gpio_init(void)
{
    ;;
}

/**
 * @brief de-initialize APM32E10xx GPIO interface
 */
static void apm32e10xx_gpio_deinit(void)
{
    ;;
}

/** 
 * @brief write/update a gpio pin 
 * 
 * @param port : port number
 * @param pin :  pin number
 * @param val : set new value
 */
static void apm32e10xx_gpio_write(uint32_t port, uint16_t pin, uint8_t val)
{
    
}

/** 
 * @brief read from a gpio pin
 * 
 * @param port : port number
 * @param pin :  pin number
 */
static uint8_t apm32e10xx_gpio_read(uint32_t port, uint16_t pin)
{
    uint8_t ret = 0xffu;


    return ret;
}

/******************************************************************************/
/*-------------------  Local Variable Definitions    -------------------------*/
/******************************************************************************/
static const gpio_device_t apm32e10xx_gpio_device = 
{
    .init   = apm32e10xx_gpio_init,
    .write  = apm32e10xx_gpio_write,
    .read   = apm32e10xx_gpio_read,
    .deinit = apm32e10xx_gpio_deinit,
};
/******************************************************************************/
/*----------------------------Global Function --------------------------------*/
/******************************************************************************/
/** 
 * @brief register s32k3xx gpio device to gpio wrapper
 */
void s32k3xx_gpio_preinit(void)
{
    gpio_register(&apm32e10xx_gpio_device);
}

