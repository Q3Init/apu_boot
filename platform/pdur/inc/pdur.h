#ifndef PDUR_H
#define PDUR_H

#include "Platform_Types.h"
#include "ComStack_Types.h"
#include "pdur_cfg.h"

extern void PduR_RxIndication(uint16 pduId, const PduInfoType *pduInfoPtr);
extern void PduR_Transmit(uint16 pduId, const PduInfoType *pduInfoPtr);
extern void PduR_TxConfirmation(void);

#endif
