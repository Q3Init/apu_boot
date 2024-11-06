
/*******************************************************************************
**                      Includes                                              **
*******************************************************************************/
#include "iap.h"
#include "iap_cfg.h"
#include <string.h>
#include "pdur.h"
#include "rte.h"
/*******************************************************************************
**                      Imported Compiler Switch Check                        **
*******************************************************************************/

/*******************************************************************************
**                      Private Macro Definitions                             **
*******************************************************************************/
#define IAP_MSG_BYTE_CNT 16
#define IAP_RX_BUF_SIAZE 512
#define IAP_TX_BUF_SIAZE 512
#define SESSION_TIMER_CNT 5000
#define IAP_ACK                      0x55
#define IAP_SESSION_MISMATCH         0x01
#define IAP_DEPENDING                0x78

#define IAP_SET_TIMER(time) ((((time) + 5 / 2) / 5) + 1)
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

typedef enum {
    Defalut_session = 1,
    Programming_session,
    Extern_session,
} SessionType;

typedef struct
{
    PduInfoType *rxPdu;
    PduInfoType *txPdu;  
    uint16 rxPduLen;
    uint16 txPduLen;  
    IapBufStateType rxBufState;
    IapBufStateType txBufState;
    uint8 sessionLv;   
    uint16 sessionTick;   

    uint8 txCfirmReset;
    uint16 resetPerformTick;    
}IapRteType;

#define RXLEN iapRte.rxPduLen
#define TXLEN iapRte.txPduLen
/*******************************************************************************
**                      Private Function Declarations                         **
*******************************************************************************/
static void Iap_DefaultSession_Service(void);
static void Iap_ProgrammingSession_Service(void);
static void Iap_ExternSession_Service(void);
static void Iap_StopCommunication_Service(void);
static void Iap_Erase(void);
static void Iap_Downloadn(void);
static void Iap_Transing(void);
static void Iap_TransExit(void);
static void Iap_CheckApp(void);
static void Iap_SoftReset(void);
static void Iap_KeepSession(void);
static void Iap_PositiveResponse(void);
static void Iap_NegativeResponse(uint8 ack);
static void Iap_lAppendTx(uint8 data);
static void Iap_lHandleReset(void);
static void Iap_lHandleServ(void);
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
    RXLEN = 0;

    iapRte.txPdu = &iapTxPduInfo;
    iapRte.txPdu->datas = iapTxBuf;
    TXLEN = 0;

    iapRte.sessionLv = Defalut_session;
    iapRte.sessionTick = 0;

    iapRte.txCfirmReset = FALSE;
    iapRte.resetPerformTick = 0;
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
    if ( iapRte.sessionTick > 0) {
        iapRte.sessionTick--;
    } else {
        iapRte.sessionLv = Defalut_session;
    }
    Iap_lHandleServ();
    Iap_lHandleReset();
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
        RXLEN = len;
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
    if (iapRte.txCfirmReset == TRUE) {
        iapRte.txCfirmReset = FALSE;
        /* delay 50ms and perform reset */
        iapRte.resetPerformTick = IAP_SET_TIMER(50);
    }
}

/*******************************************************************************
**                      Private Function Definitions                         **
*******************************************************************************/
/**
* Iap_lHandleServ
*
* @return none
*
* @brief  Iap service handler function
*/
static void Iap_lHandleServ(void)
{
    if ( iapRte.rxBufState == Iap_BUFFER_FULL && iapRte.txBufState == Iap_BUFFER_IDLE) {
        switch ( iapRte.rxPdu->datas[0] ) {
            case IAP_CMD_DEF_SESSION:
                Iap_DefaultSession_Service();
                break;
            case IAP_CMD_PRO_SESSION:
                Iap_ProgrammingSession_Service();
                break;
            case IAP_CMD_EXT_SESSION:
                Iap_ExternSession_Service();
                break;
            case IAP_CMD_STOP_COMMUN:
                Iap_StopCommunication_Service();
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

static void Iap_DefaultSession_Service(void)
{
    iapRte.sessionLv = Defalut_session;
    iapRte.sessionTick = 0;
    Iap_lAppendTx(IAP_ACK);
    Iap_PositiveResponse();
}

static void Iap_ProgrammingSession_Service(void)
{
    if (iapRte.sessionLv != Extern_session) {
        Iap_NegativeResponse(IAP_SESSION_MISMATCH);
    }
    iapRte.sessionLv = Programming_session;
    iapRte.sessionTick = IAP_SET_TIMER(SESSION_TIMER_CNT);
    Iap_NegativeResponse(IAP_DEPENDING);
}

static void Iap_ExternSession_Service(void)
{
    iapRte.sessionLv = Extern_session;
    iapRte.sessionTick = IAP_SET_TIMER(SESSION_TIMER_CNT);
    Iap_lAppendTx(IAP_ACK);
    Iap_PositiveResponse();
}
static void Iap_StopCommunication_Service(void)
{
    if (iapRte.sessionLv != Extern_session) {
        Iap_NegativeResponse(IAP_SESSION_MISMATCH);
    }
    Iap_lAppendTx(IAP_ACK);
    Iap_PositiveResponse();
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
    if (iapRte.sessionLv != Extern_session) {
        Iap_NegativeResponse(IAP_SESSION_MISMATCH);
    }
    Iap_lAppendTx(IAP_ACK);
    Iap_PositiveResponse();
}

static void Iap_CheckApp(void)
{
    if (iapRte.sessionLv != Extern_session) {
        Iap_NegativeResponse(IAP_SESSION_MISMATCH);
    }
    Iap_lAppendTx(IAP_ACK);
    Iap_PositiveResponse();
}

static void Iap_SoftReset(void)
{
    if (iapRte.sessionLv != Extern_session) {
        Iap_NegativeResponse(IAP_SESSION_MISMATCH);
    }
    iapRte.txCfirmReset = TRUE;
    Iap_lAppendTx(IAP_ACK);
    Iap_PositiveResponse();
}

static void Iap_KeepSession(void)
{
    iapRte.sessionTick = IAP_SET_TIMER(SESSION_TIMER_CNT);
}

static void Iap_PositiveResponse(void)
{
    if (iapRte.txBufState == Iap_BUFFER_IDLE) {
        iapRte.rxBufState = Iap_BUFFER_IDLE;
        iapRte.txBufState = Iap_BUFFER_FULL;
        iapRte.txPdu->len = TXLEN;
        iapRte.txPdu->datas[0] = iapRte.rxPdu->datas[0] + 0x40;
        PduR_Transmit(PDUR_RX_PUDID_ON_OTA_RSP,iapRte.txPdu);
        TXLEN = 0;
    }
}

static void Iap_NegativeResponse(uint8 ack)
{
    if (iapRte.txBufState == Iap_BUFFER_IDLE) {
        iapRte.rxBufState = Iap_BUFFER_IDLE;
        iapRte.txBufState = Iap_BUFFER_FULL;
        iapRte.txPdu->len = 0x03;
        iapRte.txPdu->datas[0] = 0x7F;
        iapRte.txPdu->datas[1] = iapRte.rxPdu->datas[0];
        iapRte.txPdu->datas[2] = ack;
        PduR_Transmit(PDUR_RX_PUDID_ON_OTA_RSP,iapRte.txPdu);
    }
}

/**
* Iap_lAppendTx
*
* @param data:data
* @return none
*
* @brief  iap append tx data
*/
static void Iap_lAppendTx(uint8 data)
{
    if (TXLEN + 1 < IAP_TX_BUF_SIAZE) {
        TXLEN++;
        iapRte.txPdu->datas[TXLEN] = data;
    }
}

/**
* Iap_lHandleReset
*
* @return none
*
* @brief reset handle
*/
static void Iap_lHandleReset(void)
{
    if (iapRte.resetPerformTick > 1) {
        iapRte.resetPerformTick--;
    }
    if (iapRte.resetPerformTick == 1) {
        iapRte.resetPerformTick = 0;
        Rte_McuPerformReset(); /* System reset, reset MCU */
    }
}
