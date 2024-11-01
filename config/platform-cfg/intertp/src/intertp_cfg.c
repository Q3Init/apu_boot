/*******************************************************************************
**                      Includes                                              **
*******************************************************************************/
#include "inter_tp.h"
#include "intertp.h"
/*******************************************************************************
**                      Imported Compiler Switch Check                        **
*******************************************************************************/

/*******************************************************************************
**                      Private Macro Definitions                             **
*******************************************************************************/

/*******************************************************************************
**                      Private Type Definitions                              **
*******************************************************************************/

/*******************************************************************************
**                      Private Function Declarations                         **
*******************************************************************************/

/*******************************************************************************
**                      Global Constant Definitions                           **
*******************************************************************************/
const InterTpPduType interTpPdusCfgTable[INTERTP_PDUS_CNT] =
{
    {.id = 0x11,.bus = BUS_UART1,.src = INTERTP_UART, .dest = INTERTP_PDUR,.dir = INTERTP_PDU_RX, .srcPduId = 0xFF                                ,.destPduId = INTERTP_RX_PDUID_ON_UART_11_DIAG_REQ},
    {.id = 0x22,.bus = BUS_UART1,.src = INTERTP_PDUR, .dest = INTERTP_UART,.dir = INTERTP_PDU_TX, .srcPduId = INTERTP_TX_PDUID_ON_UART_22_DIAG_RSP,.destPduId = 0xFF                                },
};
/*******************************************************************************
**                      Global Variable Definitions                           **
*******************************************************************************/

/*******************************************************************************
**                      Private Constant Definitions                          **
*******************************************************************************/

/*******************************************************************************
**                      Private Variable Definitions                          **
*******************************************************************************/

/*******************************************************************************
**                      Global Function Definitions                           **
*******************************************************************************/
/**
* InterTp_UartTransmit
*
* @param datas: tx datas
* @param dlc: Data length
* @param uartIndex: Number of serial drivers
* @return   ret: TRUE or FALSE
*
* @brief  Intertp maps new modules
*/
boolean InterTp_UartTransmit(const uint8 *datas,uint16 dlc,uint8 uartIndex)
{
	boolean ret = FALSE;
	ret = UartIf_Transmit(datas,dlc,uartIndex);
    return ret;
}

/**
* InterTp_IsRxEnable
*
* @return   none
*
* @brief  ota data This link is enabled
*/
boolean InterTp_IsRxEnable(void)
{
    boolean ret = TRUE;
    return ret;   
}
/*******************************************************************************
**                      Private Function Definitions                          **
*******************************************************************************/
