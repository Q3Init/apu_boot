/******************************************************************************/
/*---------------------------    Include      --------------------------------*/
/******************************************************************************/
#include "uart_type.h"
#include "uart_cfg.h"
#include "dma_cfg.h"
#include "uartif.h"

/******************************************************************************/
/*----------------------------Local  Function --------------------------------*/
/******************************************************************************/
/**
 * @brief initialize APM32E10xx GPIO interface
 */
static std_return_t apm32e10xx_uart_init(void)
{
    bool ret = TRUE;

    uint8 uartIndex;

    for (uartIndex = 0; uartIndex < UART_SIGNALS_CNT; uartIndex++)
    {
        /* USART configuration */
        USART_Config((uartSignalsCfgTable + uartIndex)->uart, (USART_Config_T *)&((uartSignalsCfgTable + uartIndex)->mcal_config_uart));

        if ((uartSignalsCfgTable + uartIndex)->irq_type != 0)
        {
            USART_EnableInterrupt((uartSignalsCfgTable + uartIndex)->uart, (USART_INT_T)(uartSignalsCfgTable + uartIndex)->irq_type);
            USART_ClearStatusFlag((uartSignalsCfgTable + uartIndex)->uart, USART_FLAG_RXBNE);
            NVIC_EnableIRQRequest((uartSignalsCfgTable + uartIndex)->irq,
                                  (uartSignalsCfgTable + uartIndex)->preemptionPriority, (uartSignalsCfgTable + uartIndex)->subPriority);
        }
        /* Enable USART */
        USART_Enable((uartSignalsCfgTable + uartIndex)->uart);
    }

    USART_EnableDMA(USART1, USART_DMA_TX_RX);

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
static bool apm32e10xx_write_buffer(uint8_t channel_id, void *data, uint16_t count)
{
    bool ret = FALSE;
    if (channel_id == INTERNAL_UART0_DRIVER_INDEX)
    {
        DMA_ConfigDataNumber(DMA1_Channel4, count);

        DMA1_Channel4->CHCFG |= (1 << 7);

        DMA1_Channel4->CHMADDR = (uint32_t)data;
        DMA_Disable(DMA1_Channel5);
        DMA_Enable(DMA1_Channel4);

        ret = TRUE;
    }
    if (channel_id == INTERNAL_UART1_DRIVER_INDEX)
    {
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
void apm32exx_uart_preinit(void)
{
    uart_register(&apm32e10xx_uart_device);
}

/**
 * @brief
 * @param
 * @retval
 */
uint8 enter_cnt;
void USART1_IRQHandler(void)
{

    uint16 dataLen = 0;
    if (USART_ReadStatusFlag(uartSignalsCfgTable[0].uart, USART_FLAG_TXC) != RESET)
    { 
        uartSignalsCfgTable[0].uart->STS;
        uartSignalsCfgTable[0].uart->DATA;
        DMA_ClearStatusFlag(DMA1_FLAG_TC4);
        USART_ClearStatusFlag(uartSignalsCfgTable[0].uart, USART_FLAG_TXC);
        UartIf_TxConfirmation();
        DMA_Disable(DMA1_Channel4);
        DMA_Enable(DMA1_Channel5);
    }

    if (USART_ReadStatusFlag(uartSignalsCfgTable[0].uart, USART_FLAG_IDLE) != RESET)
    {
        uartSignalsCfgTable[0].uart->STS;
        uartSignalsCfgTable[0].uart->DATA;

        DMA_Disable(DMA1_Channel5);
        DMA_USART1_RxMsg.DMA_USART_Len = 512 - DMA_ReadDataNumber(DMA1_Channel5);

        dataLen = (DMA_USART1_RxMsg.DMA_USART_Buf[2] << 8) + (DMA_USART1_RxMsg.DMA_USART_Buf[3]);

        if (DMA_USART1_RxMsg.DMA_USART_Len == (dataLen + 6))
        {
            enter_cnt++;
            UartIf_RxIndicaiton(DMA_USART1_RxMsg.DMA_USART_Buf, DMA_USART1_RxMsg.DMA_USART_Len, 0);
        }
        
        DMA1_Channel5->CHCFG |= (1 << 7);
        DMA1_Channel5->CHMADDR = (uint32_t)DMA_USART1_RxMsg.DMA_USART_Buf;
        DMA_ConfigDataNumber(DMA1_Channel5, 512);
        DMA_Enable(DMA1_Channel5);
    }
}
