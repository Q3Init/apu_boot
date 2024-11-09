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
void s32k3xx_clock_preinit(void)
{
    clock_register(&apm32e10xx_clock_device);
}
