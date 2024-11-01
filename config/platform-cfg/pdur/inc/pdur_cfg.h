#ifndef PDUR_CFG_H
#define PDUR_CFG_H

#include "Platform_Types.h"
#include "ComStack_Types.h"

#define PDUR_PDU_RX 0
#define PDUR_PDU_TX 1

#define PDUR_RX_PUDID_ON_OTA_REQ 0
#define PDUR_RX_PUDID_ON_OTA_RSP 1
#define PDUR_PDUIDS_CNT 2

typedef enum{
    PDUR_INTERTP = 0,
    PDUR_OTA,
    PDUR_ROUTES_CNT
} PduRoutes;

typedef struct {
    uint8 src;
    uint8 dest;
    uint8 dir;
    uint16 pduId;
} PduRPduType;

extern const PduRPduType PduRCfgTable[];

#endif
