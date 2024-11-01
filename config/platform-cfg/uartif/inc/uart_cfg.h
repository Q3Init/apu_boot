#ifndef UARTIF_CFG_H_
#define UARTIF_CFG_H_
/*******************************************************************************
**                      Includes                                              **
*******************************************************************************/
#include "Platform_Types.h"
/*******************************************************************************
**                      Common Published Information                          **
*******************************************************************************/

/*******************************************************************************
**                     Configuration options                                  **
*******************************************************************************/

/*******************************************************************************
**                      Global Macro Definitions                              **
*******************************************************************************/
/* fifo size */
#define UARTIF_RX_FIFO_SIZE 1024
#define UARTIF_TX_FIFO_SIZE 1024
/* once data size */
#define UARTIF_RX_HDL_SIZE_EACH_CYCLE 512
#define UARTIF_TX_HDL_SIZE_EACH_CYCLE 512
/* Uart Index */
#define UART1 0
#define UART_CNT 1
/*******************************************************************************
**                      Global Type Definitions                               **
*******************************************************************************/

/*******************************************************************************
**                      Global Constant Declarations                          **
*******************************************************************************/

/*******************************************************************************
**                      Global Variable Declarations                          **
*******************************************************************************/

/*******************************************************************************
**                      Global Function Declarations                          **
*******************************************************************************/
extern void UartIfCfg_Write(uint8 module, const uint8 *datas, uint16 len);
/*******************************************************************************
**                      Global Inline Function Definitions                    **
*******************************************************************************/
#endif
