/******************************************************************************/
/*---------------------------    Include      --------------------------------*/
/******************************************************************************/
#include "device.h"
#include "dma_type.h"

/******************************************************************************/
/*-------------------  Local Variable Definitions    -------------------------*/
/******************************************************************************/
static const dma_device_t *gp_dma_device = NULL;
static bool g_dma_initialized = FALSE;

/******************************************************************************/
/*----------------------------Local Function ---------------------------------*/
/******************************************************************************/
DEVICE_INITFUNC(apm32exx_dma_preinit);

/**
 * @brief preinitialize dma module,register dma devices
 */
static void dma_preinit(void)
{
    apm32exx_dma_preinit();
}

/******************************************************************************/
/*----------------------------Global Function --------------------------------*/
/******************************************************************************/
/**
 * @brief dma init function for each board
 */
bool dma_init(void)
{
    bool ret = 0U;

    if (FALSE == g_dma_initialized)
    {
        dma_preinit();
        if ((NULL != gp_dma_device) && (NULL != gp_dma_device->init))
        {
            gp_dma_device->init();
            g_dma_initialized = TRUE;
            ret = TRUE;
        }
    }

    return ret;
}

/**
 * @brief disable dma module
 */
bool dma_deinit(void)
{
    bool ret = FALSE;

    if ((TRUE == g_dma_initialized) &&
        (NULL != gp_dma_device) &&
        (NULL != gp_dma_device->deinit))
    {
        ret = gp_dma_device->deinit();
    }

    g_dma_initialized = FALSE;

    return ret;
}

/**
 * @brief function to register dma device
 *
 * @param p_dev device pointer
 *
 * @return NULL
 */
void dma_register(const dma_device_t *p_dev)
{
    if (p_dev)
    {
        gp_dma_device = p_dev;
    }
}
