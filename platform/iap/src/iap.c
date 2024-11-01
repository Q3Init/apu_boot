
/*******************************************************************************
**                      Includes                                              **
*******************************************************************************/
#include "iap.h"
#include "iap_cfg.h"
#include <string.h>
#include "pdur.h"
/*******************************************************************************
**                      Imported Compiler Switch Check                        **
*******************************************************************************/

/*******************************************************************************
**                      Private Macro Definitions                             **
*******************************************************************************/
#define IAP_MSG_BYTE_CNT 16
#define IAP_RX_BUF_SIAZE 256
#define IAP_TX_BUF_SIAZE 256
#define IAP_ACK          0x55
#define IAP_NACK         0xAA
/*******************************************************************************
**                   Function like macro definitions                          **
*******************************************************************************/

/*******************************************************************************
**                      Private Type Definitions                              **
*******************************************************************************/
typedef enum
{
    IAP_CMD_DEF_SESSION  = 0xA1, 
    IAP_CMD_PRO_SESSION  = 0xA2, 
    IAP_CMD_EXT_SESSION  = 0xA3, 
    IAP_CMD_STOP_COMMUN  = 0xA4, 
    IAP_CMD_ERASE        = 0xA5, 
    IAP_CMD_DOWNLOAD     = 0xA6, 
    IAP_CMD_TRANSING     = 0xA7,
    IAP_CMD_TRANS_EXIT   = 0xA8,
    IAP_CMD_CHECK_APP    = 0xA9,
    IAP_CMD_SOFT_RESET   = 0xAA,
    IAP_CMD_KEEP_SESSION = 0x3E,
} IAP_CMD_t;

typedef enum {
    Iap_BUFFER_IDLE = 0,
    Iap_BUFFER_PROVIDED,
    Iap_BUFFER_FULL
} IapBufStateType;

typedef struct
{
    PduInfoType *rxPdu;
    PduInfoType *txPdu;  
    uint16 rxPduLen;
    uint16 txPduLen;  
    IapBufStateType rxBufState;
    IapBufStateType txBufState;          
}IapRteType;

/*******************************************************************************
**                      Private Function Declarations                         **
*******************************************************************************/
static void Iap_DefSession(void);
static void Iap_ProSession(void);
static void Iap_ExtSession(void);
static void Iap_StopCommun(void);
static void Iap_Erase(void);
static void Iap_Downloadn(void);
static void Iap_Transing(void);
static void Iap_TransExit(void);
static void Iap_CheckApp(void);
static void Iap_SoftReset(void);
static void Iap_KeepSession(void);
static void Iap_Transmit(uint8 ack);
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
IapRteType iapRte;
PduInfoType iapRxPduInfo;
PduInfoType iapTxPduInfo;
uint8 iapRxBuf[IAP_RX_BUF_SIAZE];
uint8 iapTxBuf[IAP_TX_BUF_SIAZE];
/*******************************************************************************
**                      Global Function Definitions                           **
*******************************************************************************/

/**
 * Iap_Init
 *
 * @return   none
 *
 * @brief Iap mainfunction
 */
void Iap_Init(void)
{
    (void)memset(&iapRxPduInfo, 0, sizeof(PduInfoType));
    (void)memset(&iapTxPduInfo, 0, sizeof(PduInfoType));
    (void)memset(iapRxBuf,0,sizeof(iapRxBuf));
    (void)memset(iapTxBuf,0,sizeof(iapTxBuf));
    (void)memset(&iapRte, 0, sizeof(IapRteType));
    iapRte.rxBufState = Iap_BUFFER_IDLE;
    iapRte.txBufState = Iap_BUFFER_IDLE;

    iapRte.rxPdu = &iapRxPduInfo;
    iapRte.rxPdu->datas = iapRxBuf;

    iapRte.txPdu = &iapTxPduInfo;
    iapRte.txPdu->datas = iapTxBuf;
}

/**
 * Iap_Function
 *
 * @return   none
 *
 * @brief Iap mainfunction
 */
void Iap_MainFunction(void)
{
    if ( iapRte.rxBufState == Iap_BUFFER_FULL && iapRte.txBufState == Iap_BUFFER_IDLE) {
        switch ( iapRte.rxPdu->cmd ) {
            case IAP_CMD_DEF_SESSION:
                Iap_DefSession();
                break;
            case IAP_CMD_PRO_SESSION:
                Iap_ProSession();
                break;
            case IAP_CMD_EXT_SESSION:
                Iap_ExtSession();
                break;
            case IAP_CMD_STOP_COMMUN:
                Iap_StopCommun();
                break;
            case IAP_CMD_ERASE:
                Iap_Erase();
                break;
            case IAP_CMD_DOWNLOAD:
                Iap_Downloadn();
                break;
            case IAP_CMD_TRANSING:
                Iap_Transing();
                break;
            case IAP_CMD_TRANS_EXIT:
                Iap_TransExit();
                break;
            case IAP_CMD_CHECK_APP:
                Iap_CheckApp();
                break;
            case IAP_CMD_SOFT_RESET:
                Iap_SoftReset();
                break;
            case IAP_CMD_KEEP_SESSION:
                Iap_KeepSession();
                break;
            default:
                break;
        }
    }
}

/**
* Iap_ProvideRxBuffer
*
* @param len: pdu message len
* @param pdu: pdu message
* @return ret: E_OK or E_NOK
*
* @brief  Iap provide rx buffer
*/
uint8 Iap_ProvideRxBuffer(uint16 len,PduInfoType **pdu)
{
    uint8 ret = E_OK;
    if (((iapRte.rxBufState != Iap_BUFFER_IDLE) && (iapRte.rxBufState != Iap_BUFFER_PROVIDED)) ||
        (iapRte.txBufState != Iap_BUFFER_IDLE) ||
        (len == 0) ||
        (len > IAP_RX_BUF_SIAZE) ||
        (pdu == NULL)) {
            ret = E_NOK;
    } else {
        *pdu = iapRte.rxPdu;
        iapRte.rxPduLen = len;
        iapRte.rxBufState = Iap_BUFFER_PROVIDED;
    }
    return ret;
}

/**
* Iap_ProvideTxBuffer
*
* @param len: pdu message len
* @param pdu: pdu message
* @return ret: E_OK or E_NOK
*
* @brief  Dcm provide tx buffer
*/
uint8 Iap_ProvideTxBuffer(uint16 len, PduInfoType **pdu)
{
    uint8 ret = E_OK;
    if ((iapRte.txBufState != Iap_BUFFER_FULL) ||
       (len > IAP_TX_BUF_SIAZE) ||
       (pdu == NULL)) {
        ret = E_NOK;
    } else {
        *pdu = &iapTxPduInfo;
        iapRte.txBufState = Iap_BUFFER_PROVIDED;
    }
    return ret;
}

/**
* Iap_RxIndication
*
* @return   none
*
* @brief  DCM RX buffer status
*/
void Iap_RxIndication(uint8 rslt)
{
    if (rslt == NTFRSLT_OK) {
        iapRte.rxBufState = Iap_BUFFER_FULL;
    }
}

/**
* Iap_TxConfirmation
*
* @return   none
*
* @brief  Iap TX buffer status
*/
void Iap_TxConfirmation(void)
{
    iapRte.txBufState = Iap_BUFFER_IDLE;
}

/*******************************************************************************
**                      Private Function Definitions                         **
*******************************************************************************/

static void Iap_DefSession(void)
{
}
static void Iap_ProSession(void)
{
}
static void Iap_ExtSession(void)
{
    iapTxPduInfo.len = 0x01;
    Iap_Transmit(IAP_ACK);
}
static void Iap_StopCommun(void)
{
}
static void Iap_Erase(void)
{
}
static void Iap_Downloadn(void)
{
}
static void Iap_Transing(void)
{
}
static void Iap_TransExit(void)
{
}
static void Iap_CheckApp(void)
{
}

static void Iap_SoftReset(void)
{
}
static void Iap_KeepSession(void)
{
}

static void Iap_Transmit(uint8 ack)
{
    if (iapRte.txBufState == Iap_BUFFER_IDLE) {
        iapRte.rxBufState = Iap_BUFFER_IDLE;
        iapRte.txPdu->cmd = iapRte.rxPdu->cmd;
        iapRte.txPdu->datas[0] = ack;
        iapRte.txBufState = Iap_BUFFER_FULL;
        PduR_Transmit(PDUR_RX_PUDID_ON_OTA_RSP,iapRte.txPdu);
    }
}
