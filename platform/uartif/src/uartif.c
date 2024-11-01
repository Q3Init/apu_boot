/*
* Copyright (c) File,SHENZHEN MINIEYE INNOVATION TECHNOLOGY Co.,Ltd.
* All Rights Reserved.
* Dept.: Software Dept.
* File: UartIf.c
* Description: Uart interface module source file, relevant to Uart and InterTp module.
* Creator: WangDingwen
* History:
* 2021-10-20 WangDingwen: Draft Version
* 2021-12-17 YangJiaxin & LuTianQing: Annotated edition
*/

/*******************************************************************************
**                      Includes                                              **
*******************************************************************************/
#include <string.h>
#include "uartif.h"
#include "intertp.h"
#if UART_CNT > 0
/*******************************************************************************
**                      Imported Compiler Switch Check                        **
*******************************************************************************/

/*******************************************************************************
**                      Private Macro Definitions                             **
*******************************************************************************/

/*******************************************************************************
**                   Function like macro definitions                          **
*******************************************************************************/

/*******************************************************************************
**                      Private Type Definitions                              **
*******************************************************************************/
typedef struct {
    uint16 header; /* Buffer head position */
    uint16 tail;   /* Buffer tail position */
    uint16 deepth; /* Fifo size */
    uint8 *datas;
    
} UartIfFifoType;
/*******************************************************************************
**                      Private Function Declarations                         **
*******************************************************************************/
static uint16 UartIf_GetFifoLen(const UartIfFifoType *fifo);
static uint16 UartIf_lReadRxDatas(uint8 *buf, uint16 dlc, uint8 uartIndex);
static void UartIf_lMainFunctionRx(void);
static void UartIf_lMainFunctionTx(void);
/*******************************************************************************
**                      Global Constant Definitions                           **
*******************************************************************************/

/*******************************************************************************
**                      Global Variable Definitions                           **
*******************************************************************************/

/*******************************************************************************
**                      Private Constant Definitions                          **
*******************************************************************************/

/*******************************************************************************
**                      Private Variable Definitions                          **
*******************************************************************************/
static uint8 uartIfRxDatas[UART_CNT][UARTIF_RX_FIFO_SIZE];
static uint8 uartIfTxDatas[UART_CNT][UARTIF_TX_FIFO_SIZE];

static uint8 uartIfRxBuf[UART_CNT][UARTIF_RX_HDL_SIZE_EACH_CYCLE];
static uint8 uartIfTxBuf[UART_CNT][UARTIF_TX_HDL_SIZE_EACH_CYCLE];

static UartIfFifoType uartIfRxFifo[UART_CNT];
static UartIfFifoType uartIfTxFifo[UART_CNT];
/*******************************************************************************
**                      Global Function Definitions                           **
*******************************************************************************/

/**
* UartIf_Init
*
* @return   none
*
* @brief  Initialize the FIFO of uARTIf
*/
void UartIf_Init(void)
{
    uint8 uartIndex;
    (void)memset(uartIfRxFifo, 0, sizeof(uartIfRxFifo));
    (void)memset(uartIfTxFifo, 0, sizeof(uartIfRxFifo));
    (void)memset(uartIfRxBuf, 0, sizeof(uartIfRxBuf));
    (void)memset(uartIfTxBuf, 0, sizeof(uartIfTxBuf));

    for (uartIndex = 0; uartIndex < UART_CNT;uartIndex++) {
        uartIfRxFifo[uartIndex].datas = uartIfRxDatas[uartIndex];
        uartIfRxFifo[uartIndex].deepth = UARTIF_RX_FIFO_SIZE;

        uartIfTxFifo[uartIndex].datas = uartIfTxDatas[uartIndex];
        uartIfTxFifo[uartIndex].deepth = UARTIF_TX_FIFO_SIZE;
    }
    
}

/**
* UartIf_MainFunction
*
* @return   none
*
* @brief  uartif main function rx/tx
*/
void UartIf_MainFunction(void)
{
    UartIf_lMainFunctionRx();
    UartIf_lMainFunctionTx();
}

/**
* UartIf_RxIndicaiton
*
* @param datas : intertp datas
* @param dlc : datas length 
* @return   none
*
* @brief  uartif rx indication
*/
void UartIf_RxIndicaiton(const uint8 *datas, uint16 dlc, uint8 uartIndex)
{
    uint16 byte_i;
    uint16 len = UartIf_GetFifoLen(&uartIfRxFifo[uartIndex]);
    
    if (uartIfRxFifo[uartIndex].deepth < (len + dlc + 1)) {
        if (uartIfRxFifo[uartIndex].deepth <= (1 + len)) {
            len = 0;
        } else {
            len = (uartIfRxFifo[uartIndex].deepth - 1) - len; 
        }
    } else {
        len = dlc;
    }
    
    for (byte_i = 0; byte_i < len; byte_i++) { 
        uartIfRxFifo[uartIndex].datas[uartIfRxFifo[uartIndex].tail] = datas[byte_i]; /* Read from tail of uartFIFo */
        if ((uartIfRxFifo[uartIndex].tail + 1) <  uartIfRxFifo[uartIndex].deepth) {
            uartIfRxFifo[uartIndex].tail++;
        } else {
            uartIfRxFifo[uartIndex].tail = 0;
        }
    }
}

/**
* UartIf_TxConfirmation
*
* @return   none
*
* @brief  interTp transmit uartif confirmation
*/
void UartIf_TxConfirmation(void)
{
    InterTp_UartTxConfirmation();
}

/**
* UartIf_Transmit
*
* @param datas: 
* @param dlc: 
* @return   ret TRUE or FALSE
*
* @brief  uartif transmit
*/
boolean UartIf_Transmit(const uint8 *datas, uint16 dlc, uint8 uartIndex)
{
    boolean ret = FALSE;
    uint16 byte_i;
    uint16 len = UartIf_GetFifoLen(&uartIfTxFifo[uartIndex]);
    if (uartIfTxFifo[uartIndex].deepth > (dlc + len + 1)) {
        for (byte_i = 0; byte_i < dlc; byte_i++) {
            uartIfTxFifo[uartIndex].datas[uartIfTxFifo[uartIndex].tail] = datas[byte_i]; /* Start with tail of uartFifo */
            if ((uartIfTxFifo[uartIndex].tail + 1) <  uartIfTxFifo[uartIndex].deepth) {
                uartIfTxFifo[uartIndex].tail++;
            } else {
                uartIfTxFifo[uartIndex].tail = 0;
            }
        }
        ret = TRUE;
    }
    return ret; 
}

/*******************************************************************************
**                      Private Function Definitions                          **
*******************************************************************************/
/**
* UartIf_lMainFunctionRx
*
* @return   none
*
* @brief  uartif main function 
*/
static void UartIf_lMainFunctionRx(void)
{
    uint8 uartIndex;
    uint16 len;
    for(uartIndex = 0; uartIndex < UART_CNT; uartIndex++) {
        len = UartIf_lReadRxDatas(uartIfRxBuf[uartIndex], UARTIF_RX_HDL_SIZE_EACH_CYCLE,uartIndex); /* Read data length */
        if (len > 0) {
            InterTp_UartRxIndication(uartIndex,uartIfRxBuf[uartIndex], len);
        }
        
    }
    
}

/**
* UartIf_lMainFunctionTx
*
* @return   none
*
* @brief  uartif main function tx
*/
static void UartIf_lMainFunctionTx(void)
{
    uint8 uartIndex;
    uint16 byte_i;
    uint16 len;
    for(uartIndex = 0; uartIndex < UART_CNT; uartIndex++) {
        len = UartIf_GetFifoLen(&uartIfTxFifo[uartIndex]);
        if (len == 0) {
            continue;
        }
        if (len > UARTIF_TX_HDL_SIZE_EACH_CYCLE) {
            len = UARTIF_TX_HDL_SIZE_EACH_CYCLE;
        }
        for (byte_i = 0; byte_i < len; byte_i++) {
            uartIfTxBuf[uartIndex][byte_i] = uartIfTxFifo[uartIndex].datas[uartIfTxFifo[uartIndex].header];
            if ((uartIfTxFifo[uartIndex].header + 1) <  uartIfTxFifo[uartIndex].deepth) {
                uartIfTxFifo[uartIndex].header++;
            } else {
                uartIfTxFifo[uartIndex].header = 0;
            }
        }
        if (len > 0) {
            UartIfCfg_Write(uartIndex, uartIfTxBuf[uartIndex], len);
        }
    }
}

/**
* UartIf_lReadRxDatas
*
* @param buf: uartif rx buffer
* @param dlc: The length of the data read
* @return   read data len
*
* @brief  Initialize the FIFO of uARTIf
*/
static uint16 UartIf_lReadRxDatas(uint8 *buf, uint16 dlc, uint8 uartIndex)
{
    uint16 byte_i;
    uint16 len = UartIf_GetFifoLen(&uartIfRxFifo[uartIndex]); /* Gets the length of the FIFO */
    if (len > dlc) {
        len = dlc;
    }
    for (byte_i = 0; byte_i < len; byte_i++) {
        buf[byte_i] = uartIfRxFifo[uartIndex].datas[uartIfRxFifo[uartIndex].header];
        if ((uartIfRxFifo[uartIndex].header + 1) <  uartIfRxFifo[uartIndex].deepth) {
            uartIfRxFifo[uartIndex].header++;
        } else {
            uartIfRxFifo[uartIndex].header = 0;
        }
    }
    return len;
}

/**
* UartIf_GetFifoLen
*
* @param fifo: uartif fifo
* @return   fifo len
*
* @brief  Initialize the FIFO of uARTIf
*/
static uint16 UartIf_GetFifoLen(const UartIfFifoType *fifo)
{
    uint16 len;
    if (fifo->header <= fifo->tail) {
        len = fifo->tail - fifo->header;
    } else {
        len = fifo->tail + fifo->deepth - fifo->header;
    }
    return len;
}
#endif

