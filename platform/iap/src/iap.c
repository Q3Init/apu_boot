
/*******************************************************************************
**                      Includes                                              **
*******************************************************************************/
#include "iap.h"
#include "iap_cfg.h"
#include <string.h>
#include "pdur.h"
#include "rte.h"
#include "bootcheck.h"
#include "flash.h"
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
#define IAP_ACK                                 0x55
#define IAP_SESSION_MISMATCH                    0x01
#define IAP_FUNCTION_MISMATCH                   0x02
#define IAP_ERASE_ERROR                         0x03
#define IAP_ERASE_ADDRESS_ERROR                 0x04
#define IAP_SEQUENCE_OF_APP_BLOCK_ERROR         0x05
#define IAP_WRITE_EEROR                         0x06
#define IAP_DEPENDING                           0x78

#define IAP_SET_TIMER(time) ((((time) + 5 / 2) / 5) + 1)
/* APP bankA start address */
#define APP_BANKA_START_ADDRESS (0x08011000)
#define APP_BANK_SIZE           (0x4B000)
/*******************************************************************************
**                   Function like macro definitions                          **
*******************************************************************************/

/*******************************************************************************
**                      Private Type Definitions                              **
*******************************************************************************/
typedef enum
{
    IAP_CMD_DEF_SESSION  = 0x01, 
    IAP_CMD_PRO_SESSION  = 0x02, 
    IAP_CMD_EXT_SESSION  = 0x03, 
    IAP_CMD_STOP_COMMUN  = 0x04, 
    IAP_CMD_ERASE        = 0x05, 
    IAP_CMD_DOWNLOAD     = 0x06, 
    IAP_CMD_TRANSING     = 0x07,
    IAP_CMD_TRANS_EXIT   = 0x08,
    IAP_CMD_CHECK_APP    = 0x09,
    IAP_CMD_SOFT_RESET   = 0x0A,
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

    uint16 app_block;    
    uint32 app_address;
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
                Iap_NegativeResponse(IAP_FUNCTION_MISMATCH);
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
        return;
    }
    iapRte.sessionLv = Programming_session;
    iapRte.sessionTick = IAP_SET_TIMER(SESSION_TIMER_CNT);
    Iap_lAppendTx(IAP_ACK);
    Iap_PositiveResponse();
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
    if (iapRte.sessionLv == Defalut_session) {
        Iap_NegativeResponse(IAP_SESSION_MISMATCH);
        return;
    }
    Iap_lAppendTx(IAP_ACK);
    Iap_PositiveResponse();
}

static void Iap_Erase(void)
{
    std_return_t ret = E_NOK;
    uint32 app_start_addr = 0;
    memcpy(&app_start_addr,&iapRte.rxPdu->datas[1],sizeof(uint32));
    if (iapRte.sessionLv == Defalut_session) {
        Iap_NegativeResponse(IAP_SESSION_MISMATCH);
        return;
    }
    ret = earse_integrity_app_flag();
    if (ret != E_OK) {
        Iap_NegativeResponse(IAP_ERASE_ERROR);
        return;
    }
    if (app_start_addr == APP_BANKA_START_ADDRESS) {
        ret = flash_erase(INTERNAL_PFLASH_DRIVER_INDEX,APP_BANKA_START_ADDRESS,APP_BANK_SIZE);
        if (ret != E_OK) {
            Iap_NegativeResponse(IAP_ERASE_ERROR);
            return;
        }
    } else {
        Iap_NegativeResponse(IAP_ERASE_ADDRESS_ERROR);
        return;
    }
    Iap_lAppendTx(IAP_ACK);
    Iap_PositiveResponse();
}

static void Iap_Downloadn(void)
{
    if (iapRte.sessionLv == Defalut_session) {
        Iap_NegativeResponse(IAP_SESSION_MISMATCH);
        return;
    }
    iapRte.app_block = 0;
    iapRte.app_address = INTEGRITY_APP_FLAG_ADDRESS;
    Iap_lAppendTx(IAP_ACK);
    Iap_PositiveResponse();
}

static void Iap_Transing(void)
{
    std_return_t ret = E_NOK;
    uint32 write_data;
    uint32 write_address = iapRte.app_address;
    uint16 download_appblock = (uint16)iapRte.rxPdu->datas[1] + (uint16)(iapRte.rxPdu->datas[1] << 8);
    if (iapRte.sessionLv == Defalut_session) {
        Iap_NegativeResponse(IAP_SESSION_MISMATCH);
        return;
    }
    if ((iapRte.app_block + 1) == download_appblock) {
        for (uint16 index = 0;index <= ((iapRte.rxPdu->len - 3) / 4);index++) {
            memcpy(&write_data,&iapRte.rxPdu->datas[index+3],4);
            ret = flash_write(INTERNAL_PFLASH_DRIVER_INDEX,write_address,(iapRte.rxPdu->len - 3),&write_data);
            if (ret != E_OK) {
                Iap_NegativeResponse(IAP_WRITE_EEROR);
                return;
            } else {
                write_address+=4;
                index+=4;
            }
        }
        iapRte.app_address += (iapRte.rxPdu->len - 3);
        iapRte.app_block++;
    } else {
        Iap_NegativeResponse(IAP_SEQUENCE_OF_APP_BLOCK_ERROR);
        return;
    }
    Iap_lAppendTx(IAP_ACK);
    Iap_PositiveResponse();
}

static void Iap_TransExit(void)
{
    if (iapRte.sessionLv == Defalut_session) {
        Iap_NegativeResponse(IAP_SESSION_MISMATCH);
        return;
    }
    Iap_lAppendTx(IAP_ACK);
    Iap_PositiveResponse();
}

static void Iap_CheckApp(void)
{
    if (iapRte.sessionLv == Defalut_session) {
        Iap_NegativeResponse(IAP_SESSION_MISMATCH);
        return;
    }
    Iap_lAppendTx(IAP_ACK);
    Iap_PositiveResponse();
}

static void Iap_SoftReset(void)
{
    if (iapRte.sessionLv == Defalut_session) {
        Iap_NegativeResponse(IAP_SESSION_MISMATCH);
        return;
    }
    iapRte.txCfirmReset = TRUE;
    Iap_lAppendTx(IAP_ACK);
    Iap_PositiveResponse();
}

static void Iap_KeepSession(void)
{
    iapRte.sessionTick = IAP_SET_TIMER(SESSION_TIMER_CNT);
    iapRte.rxBufState = Iap_BUFFER_IDLE;
    memset(iapRxBuf,0,sizeof(iapRxBuf));
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
