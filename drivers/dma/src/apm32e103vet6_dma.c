/******************************************************************************/
/*---------------------------    Include      --------------------------------*/
/******************************************************************************/
#include "dma_type.h"
#include "dma_cfg.h"

/******************************************************************************/
/*----------------------------Local  Function --------------------------------*/
/******************************************************************************/
/**
 * @brief initialize APM32E10xx GPIO interface
 */
static std_return_t apm32e10xx_dma_init(void)
{
    bool ret = TRUE;

    uint8 dmaIndex;
    for (dmaIndex = 0; dmaIndex < DMA_SIGNALS_CNT; dmaIndex++)
    {
        /* Enable DMA Clock */
        RCM_EnableAHBPeriphClock((dmaSignalsCfgTable + dmaIndex)->clock);

        /* Enable DMA channel */
        DMA_Config((dmaSignalsCfgTable + dmaIndex)->channel, (DMA_Config_T *)&((dmaSignalsCfgTable + dmaIndex)->mcal_config_dma));

        /* Enable DMA */
        //        DMA_Enable((dmaSignalsCfgTable + dmaIndex)->channel);
    }

    DMA_Disable(DMA1_Channel4); // Tx
    DMA_Enable(DMA1_Channel5);  // Rx

    return ret;
}

/**
 * @brief de-initialize APM32E10xx GPIO interface
 */
static std_return_t apm32e10xx_dma_deinit(void)
{
    bool ret = TRUE;

    return ret;
}

/******************************************************************************/
/*-------------------  Local Variable Definitions    -------------------------*/
/******************************************************************************/
static const dma_device_t apm32e10xx_dma_device =
    {
        .init = apm32e10xx_dma_init,
        .deinit = apm32e10xx_dma_deinit,
};
/******************************************************************************/
/*----------------------------Global Function --------------------------------*/
/******************************************************************************/
/**
 * @brief register s32k3xx dma device to dma wrapper
 */
void s32k3xx_dma_preinit(void)
{
    dma_register(&apm32e10xx_dma_device);
}
