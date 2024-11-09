
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
	uint16 gpioIndex;
	GPIO_Config_T gpioConfig;

	for (gpioIndex = 0; gpioIndex < GPIO_SIGNALS_CNT; gpioIndex++)
	{
		/* Enable GPIOA clock */
		if ((gpioSignalsCfgTable + gpioIndex)->APB1_Clock != STD_OFF)
		{
			RCM_EnableAPB2PeriphClock((gpioSignalsCfgTable + gpioIndex)->APB1_Clock);
		}
		if ((gpioSignalsCfgTable + gpioIndex)->APB2_Clock != STD_OFF)
		{
			RCM_EnableAPB2PeriphClock((gpioSignalsCfgTable + gpioIndex)->APB2_Clock);
		}
		/* Configure PC0 (ADC Channel0) as analog input */
		GPIO_ConfigStructInit(&gpioConfig);
		gpioConfig = (gpioSignalsCfgTable + gpioIndex)->mcal_config_gpio;

		GPIO_Config((gpioSignalsCfgTable + gpioIndex)->GPIO, &gpioConfig);

		GPIO_WriteBitValue((gpioSignalsCfgTable + gpioIndex)->GPIO, (gpioSignalsCfgTable + gpioIndex)->mcal_config_gpio.pin, (gpioSignalsCfgTable + gpioIndex)->GPIO_level);
	}
}

/**
 * @brief de-initialize APM32E10xx GPIO interface
 */
static void apm32e10xx_gpio_deinit(void)
{
	;
	;
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

	if (port >= gpio_port_num)
	{
		return;
	}
	GPIO_T *gpio_port = NULL;
	if (port == gpio_port_a)
	{
		gpio_port = GPIOA;
	}
	else if (port == gpio_port_b)
	{
		gpio_port = GPIOB;
	}
	else if (port == gpio_port_c)
	{
		gpio_port = GPIOC;
	}
	else if (port == gpio_port_d)
	{
		gpio_port = GPIOD;
	}
	else if (port == gpio_port_e)
	{
		gpio_port = GPIOE;
	}
	else if (port == gpio_port_f)
	{
		gpio_port = GPIOF;
	}
	else if (port == gpio_port_g)
	{
		gpio_port = GPIOG;
	}
	GPIO_WriteBitValue(gpio_port, pin, val);
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
		.init = apm32e10xx_gpio_init,
		.write = apm32e10xx_gpio_write,
		.read = apm32e10xx_gpio_read,
		.deinit = apm32e10xx_gpio_deinit,
};
/******************************************************************************/
/*----------------------------Global Function --------------------------------*/
/******************************************************************************/
/**
 * @brief register apm32e10xx gpio device to gpio wrapper
 */
void apm32exx_gpio_preinit(void)
{
	gpio_register(&apm32e10xx_gpio_device);
}
