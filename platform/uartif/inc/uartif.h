#ifndef UARTIF_H_
#define UARTIF_H_

#include "Platform_Types.h"
#include "uartif_cfg.h"

extern void UartIf_Init(void);
extern void UartIf_MainFunction(void);
extern void UartIf_RxIndicaiton(const uint8 *datas, uint16 dlc, uint8 uartIndex);
extern boolean UartIf_Transmit(const uint8 *datas, uint16 dlc, uint8 uartIndex);
#endif
