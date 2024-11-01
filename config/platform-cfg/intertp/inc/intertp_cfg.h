#ifndef INTERTP_CFG_H_
#define INTERTP_CFG_H_
/*******************************************************************************
**                      Includes                                              **
*******************************************************************************/
#include "Platform_Types.h"
#include "ComStack_Types.h"
#include "uartif.h"

/*******************************************************************************
**                      Common Published Information                          **
*******************************************************************************/

/*******************************************************************************
**                     Configuration options                                  **
*******************************************************************************/

/*******************************************************************************
**                      Global Macro Definitions                              **
*******************************************************************************/
/* src and dest*/
#define INTERTP_PDUR 0
#define INTERTP_UART 1
/* bus type */
#define BUS_UART1 0
/* dir:Tx and Rx */
#define INTERTP_PDU_RX 0
#define INTERTP_PDU_TX 1
/* pduId */
#define INTERTP_RX_PDUID_ON_UART_11_DIAG_REQ 0
#define INTERTP_TX_PDUID_ON_UART_22_DIAG_RSP 1
/* pduId rx cnt */
#define INTERTP_RX_PDUS_CNT 1
/* pduId tx cnt */
#define INTERTP_TX_PDUS_CNT 1
/* pduId rx and tx cnt */
#define INTERTP_PDUS_CNT (INTERTP_RX_PDUS_CNT + INTERTP_TX_PDUS_CNT)
/*******************************************************************************
**                      Global Type Definitions                               **
*******************************************************************************/
typedef struct 
{
    uint8 id;
    uint8 bus;
    uint8 src;
    uint8 dest;
    uint8 srcPduId;
    uint8 destPduId;
    uint8 dir;
}InterTpPduType;
/*******************************************************************************
**                      Global Constant Declarations                          **
*******************************************************************************/
extern const InterTpPduType interTpPdusCfgTable[];
/*******************************************************************************
**                      Global Variable Declarations                          **
*******************************************************************************/

/*******************************************************************************
**                      Global Function Declarations                          **
*******************************************************************************/
extern boolean InterTp_UartTransmit(const uint8 *datas,uint16 dlc,uint8 uartIndex);
extern boolean InterTp_IsRxEnable(void);
/*******************************************************************************
**                      Global Inline Function Definitions                    **
*******************************************************************************/
#endif
