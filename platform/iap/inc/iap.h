#ifndef IAP_H_
#define IAP_H_

#include "ComStack_Types.h"
#include "Platform_Types.h"

extern void Iap_Init(void);
extern void Iap_MainFunction(void);
extern uint8 Iap_ProvideRxBuffer(uint16 len,PduInfoType **pdu);
extern uint8 Iap_ProvideTxBuffer(uint16 len, PduInfoType **pdu);
extern void Iap_RxIndication(uint8 rslt);
extern void Iap_TxConfirmation(void);

#endif
