/******************************************************************************/
/*---------------------------    Include      --------------------------------*/
/******************************************************************************/
#include "clock_type.h"
#include "clock_cfg.h"

/******************************************************************************/
/*----------------------------Local  Function --------------------------------*/
/******************************************************************************/
/**
 * @brief initialize APM32E10xx GPIO interface
 */
static std_return_t apm32e10xx_clock_init(void)
{
	bool ret = FALSE;
	RCM_EnableAHBPeriphClock(RCM_AHB_PERIPH_DMA1);
	RCM_EnableAPB2PeriphClock(RCM_APB2_PERIPH_GPIOA);
	RCM_EnableAPB2PeriphClock(RCM_APB2_PERIPH_GPIOB);
	RCM_EnableAPB2PeriphClock(RCM_APB2_PERIPH_GPIOC);
	RCM_EnableAPB2PeriphClock(RCM_APB2_PERIPH_GPIOD);
	RCM_EnableAPB2PeriphClock(RCM_APB2_PERIPH_GPIOC);
	RCM_EnableAPB2PeriphClock(RCM_APB2_PERIPH_GPIOE);
	RCM_EnableAPB2PeriphClock(RCM_APB2_PERIPH_USART1);
	RCM_EnableAPB1PeriphClock(RCM_APB1_PERIPH_USART2);

	return ret;
}

/**
 * @brief de-initialize APM32E10xx GPIO interface
 */
static std_return_t apm32e10xx_clock_deinit(void)
{
	bool ret = TRUE;

	return ret;
}

/******************************************************************************/
/*-------------------  Local Variable Definitions    -------------------------*/
/******************************************************************************/
static const clock_device_t apm32e10xx_clock_device =
	{
		.init = apm32e10xx_clock_init,
		.deinit = apm32e10xx_clock_deinit,
};
/******************************************************************************/
/*----------------------------Global Function --------------------------------*/
/******************************************************************************/
/**
 * @brief register s32k3xx clock device to clock wrapper
 */
void apm32exx_clock_preinit(void)
{
	clock_register(&apm32e10xx_clock_device);
}
