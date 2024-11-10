#include "dma_cfg.h"

USART_BuffMsg_t DMA_USART1_RxMsg;
USART_BuffMsg_t DMA_USART1_TxMsg = {{11, 22}, 0};

DMASignalsCfg dmaSignalsCfgTable[DMA_SIGNALS_CNT] =
{
        {/* DMA1_CH4 in APM32 for USART1_TX*/
         .clock = RCM_AHB_PERIPH_DMA1,
         .channel = DMA1_Channel4,
         {.peripheralBaseAddr = USART1_DR_ADDRESS,
          .memoryBaseAddr = (uint32)DMA_USART1_TxMsg.DMA_USART_Buf,
          .dir = DMA_DIR_PERIPHERAL_DST,
          .bufferSize = 0,
          .peripheralInc = DMA_PERIPHERAL_INC_DISABLE,
          .memoryInc = DMA_MEMORY_INC_ENABLE,
          .peripheralDataSize = DMA_PERIPHERAL_DATA_SIZE_BYTE,
          .memoryDataSize = DMA_MEMORY_DATA_SIZE_BYTE,
          .loopMode = DMA_MODE_NORMAL,
          .priority = DMA_PRIORITY_HIGH,
          .M2M = DMA_M2MEN_DISABLE}
          },
        {/* DMA1_CH5 in APM32 for USART1_RX*/
         .clock = RCM_AHB_PERIPH_DMA1,
         .channel = DMA1_Channel5,
         {.peripheralBaseAddr = USART1_DR_ADDRESS,
          .memoryBaseAddr = (uint32)DMA_USART1_RxMsg.DMA_USART_Buf,
          .dir = DMA_DIR_PERIPHERAL_SRC,
          .bufferSize = 512,
          .peripheralInc = DMA_PERIPHERAL_INC_DISABLE,
          .memoryInc = DMA_MEMORY_INC_ENABLE,
          .peripheralDataSize = DMA_PERIPHERAL_DATA_SIZE_BYTE,
          .memoryDataSize = DMA_MEMORY_DATA_SIZE_BYTE,
          .loopMode = DMA_MODE_NORMAL,
          .priority = DMA_PRIORITY_HIGH,
          .M2M = DMA_M2MEN_DISABLE
          }
          }
};

