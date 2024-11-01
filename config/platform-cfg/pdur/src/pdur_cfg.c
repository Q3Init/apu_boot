#include "pdur_cfg.h"

const PduRPduType PduRCfgTable[PDUR_PDUIDS_CNT] = 
{
    {.src = PDUR_INTERTP,.dest = PDUR_OTA,    .dir = PDUR_PDU_RX,.pduId = PDUR_RX_PUDID_ON_OTA_REQ},
    {.src = PDUR_OTA,    .dest = PDUR_INTERTP,.dir = PDUR_PDU_TX,.pduId = PDUR_RX_PUDID_ON_OTA_RSP},
};
