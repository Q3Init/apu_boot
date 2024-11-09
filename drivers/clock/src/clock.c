/******************************************************************************/
/*---------------------------    Include      --------------------------------*/
/******************************************************************************/
#include "device.h"
#include "clock_type.h"

/******************************************************************************/
/*-------------------  Local Variable Definitions    -------------------------*/
/******************************************************************************/
static const clock_device_t *gp_clock_device = NULL;
static bool g_clock_initialized = FALSE;

/******************************************************************************/
/*----------------------------Local Function ---------------------------------*/
/******************************************************************************/
DEVICE_INITFUNC(apm32exx_clock_preinit);

/**
 * @brief preinitialize clock module,register clock devices
 */
static void clock_preinit(void)
{
    apm32exx_clock_preinit();
}

/******************************************************************************/
/*----------------------------Global Function --------------------------------*/
/******************************************************************************/
/**
 * @brief clock init function for each board
 */
bool clock_init(void)
{
    bool ret = 0U;

    if (FALSE == g_clock_initialized)
    {
        clock_preinit();
        if ((NULL != gp_clock_device) && (NULL != gp_clock_device->init))
        {
            gp_clock_device->init();
            g_clock_initialized = TRUE;
            ret = TRUE;
        }
    }

    return ret;
}

/**
 * @brief disable clock module
 */
bool clock_deinit(void)
{
    bool ret = FALSE;

    if ((TRUE == g_clock_initialized) &&
        (NULL != gp_clock_device) &&
        (NULL != gp_clock_device->deinit))
    {
        ret = gp_clock_device->deinit();
    }

    g_clock_initialized = FALSE;

    return ret;
}

/**
 * @brief function to register clock device
 *
 * @param p_dev device pointer
 *
 * @return NULL
 */
void clock_register(const clock_device_t *p_dev)
{
    if (p_dev)
    {
        gp_clock_device = p_dev;
    }
}
