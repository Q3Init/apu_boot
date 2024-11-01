#ifndef COMSTACK_TYPES_H_
#define COMSTACK_TYPES_H_
#include "Platform_Types.h"

#define NTFRSLT_OK                        0x00
#define NTFRSLT_E_NOT_OK                  0x01

typedef struct {
    uint8 id;
    uint8 cmd;
    uint16 len;
    uint8 *datas;
} PduInfoType;

#endif

