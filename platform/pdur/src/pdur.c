/*******************************************************************************
**                      Includes                                              **
*******************************************************************************/
#include "pdur.h"
#include "iap.h"
#include "string.h"
#include "intertp.h"

PduInfoType *pdur_rxpdu;
PduInfoType *pdur_txpdu;

/**
 * PduR_RxIndication
 * 
 * @param[in]  pduId:ID of the operation
 * @param[in]  pduInfoPtr:The corresponding block
 * @return     none
 * @brief      The PDUR receives commands
 */
void PduR_RxIndication(uint16 pduId, const PduInfoType *pduInfoPtr)
{
    if ((pduId < PDUR_PDUIDS_CNT) && (PduRCfgTable[pduId].dir == PDUR_PDU_RX)) {
        if (PduRCfgTable[pduId].dest == PDUR_OTA) {
            if (Iap_ProvideRxBuffer(pduInfoPtr->len,&pdur_rxpdu) == E_OK) {
                (void)memcpy(pdur_rxpdu,pduInfoPtr,sizeof(PduInfoType));
                Iap_RxIndication(NTFRSLT_OK);
            } else {
                Iap_RxIndication(NTFRSLT_E_NOT_OK);
            }
        }
    }
}

/**
 * PduR_Transmit
 * 
 * @param[in]  pduId:ID of the operation
 * @param[in]  pduInfoPtr:The corresponding block
 * @return     none
 * @brief      The PDUR sends data
 */
void PduR_Transmit(uint16 pduId, const PduInfoType *pduInfoPtr)
{
    if ((pduId < PDUR_PDUIDS_CNT) && (PduRCfgTable[pduId].dir == PDUR_PDU_TX)) {
        if (PduRCfgTable[pduId].dest == PDUR_INTERTP) {
            if (Iap_ProvideTxBuffer(pduInfoPtr->len,&pdur_txpdu) == E_OK) {
                (void)memcpy(pdur_txpdu,pduInfoPtr,sizeof(PduInfoType));
                InterTp_Transmit(PduRCfgTable[pduId].pduId,pdur_txpdu->datas,pdur_txpdu->len);
            }
        } else {
            /* nothing to do */
        }
    }
}

/**
 * PduR_TxConfirmation
 * 
 * @return     none
 * @brief      The PDUR sends data
 */
void PduR_TxConfirmation(void)
{
    Iap_TxConfirmation();
}


