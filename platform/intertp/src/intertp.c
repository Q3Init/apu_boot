/*******************************************************************************
**                      Includes                                              **
*******************************************************************************/
#include "intertp.h"
#include <string.h>
/*******************************************************************************
**                      Imported Compiler Switch Check                        **
*******************************************************************************/

/*******************************************************************************
**                      Private Macro Definitions                             **
*******************************************************************************/
#define INTER_MODULES_CNT     1
#define INTER_TP_MSG_BYTE_CNT 16
#define INTER_TP_HEADER_CNT   1
#define INTER_TP_ID_CNT       1
#define INTER_TP_DLC_CNT      2
#define INTER_TP_XOR_CNT      2
#define INTER_TP_HEADER       0xEE
#define INTER_TP_RX_SIZE      512
/*******************************************************************************
**                      Private Type Definitions                              **
*******************************************************************************/
typedef struct
{
    uint8          step;
    uint8          fieldBytesCnt;
    InterTpMsgType msg;
} InterTpReceiveObjType;
typedef enum
{
    INTER_TP_WAIT_HEADER = 0,
    INTER_TP_WAIT_ID,
    INTER_TP_WAIT_DLC,
    INTER_TP_WAIT_DATA,
    INTER_TP_WAIT_XOR,
} InterTpStepType;
/*******************************************************************************
**                      Private Function Declarations                         **
*******************************************************************************/
static void   InterTp_lRxIndication( uint8 srcModule, const uint8* datas, uint16 len );
static uint16 InterTp_CalXor( const uint8* datas, uint8 dlc );
/*******************************************************************************
**                      Global Constant Definitions                           **
*******************************************************************************/

/*******************************************************************************
**                      Global Variable Definitions                           **
*******************************************************************************/

/*******************************************************************************
**                      Private Constant Definitions                          **
*******************************************************************************/
static const uint8 interTpHeadrPattern[ INTER_TP_HEADER_CNT ] =
    {
        INTER_TP_HEADER };
/*******************************************************************************
**                      Private Variable Definitions                          **
*******************************************************************************/
static InterTpReceiveObjType interTpReceiveObjs[ INTER_MODULES_CNT ];
static uint8                 interTpTransmitMsgBuf[ INTER_TP_MSG_BYTE_CNT ];
static boolean               InterTpNetRspReceived = FALSE;
/*******************************************************************************
**                      Global Function Definitions                           **
*******************************************************************************/

/**
 * InterTp_Init
 *
 * @return   none
 *
 * @brief  Intertp initialize
 */
void InterTp_Init( void )
{
    (void)memset( interTpReceiveObjs, 0, sizeof( interTpReceiveObjs ) );
    (void)memset( interTpTransmitMsgBuf, 0, sizeof( interTpTransmitMsgBuf ) );
    interTpReceiveObjs[ INTER_UART ].step = INTER_TP_WAIT_HEADER;
    InterTpNetRspReceived                 = FALSE;
}

/**
 * InterTp_UartRxIndication
 *
 * @param datas: uart datas
 * @param len: The length of the data
 * @return   none
 *
 * @brief  InterTp receives inter UART data
 */
void InterTp_UartRxIndication( uint8 uartIndex, const uint8* datas, uint16 len )
{
    InterTp_lRxIndication( uartIndex, datas, len );
}

/**
 * InterTp_UartTxConfirmation
 *
 * @return   none
 *
 * @brief  transmit interTp confirmation
 */
void InterTp_UartTxConfirmation( void )
{
    if ( InterTpNetRspReceived == TRUE )
    {
        InterTpNetRspReceived = FALSE;
        PduR_TxConfirmation();
    }
}

/**
 * InterTp_Transmit
 *
 * @param pduid: pud id
 * @param pduinfoPtr: Pdu information data
 * @return   ret TRUE or FALSE
 *
 * @brief  interTp transmit uartif confirmation
 */
boolean InterTp_Transmit( uint16 pduId, const uint8* datas, uint16 len )
{
    boolean ret = FALSE;
    uint16  crc = 0;
    if ( ( pduId < INTERTP_PDUS_CNT ) && ( interTpPdusCfgTable[ pduId ].dir == INTERTP_PDU_TX ) )
    {
        if ( interTpPdusCfgTable[ pduId ].src == INTERTP_PDUR )
        {
            interTpTransmitMsgBuf[ 0 ] = INTER_TP_HEADER;
            interTpTransmitMsgBuf[ 1 ] = interTpPdusCfgTable[ pduId ].id;
            interTpTransmitMsgBuf[ 2 ] = (uint8)( len >> 8 );
            interTpTransmitMsgBuf[ 3 ] = (uint8)( len );
            memcpy( &interTpTransmitMsgBuf[ 4 ], datas, len );
            crc                              = InterTp_CalXor( &interTpTransmitMsgBuf[ 0 ], len + 4 );
            interTpTransmitMsgBuf[ len + 4 ] = (uint8)( crc );
            interTpTransmitMsgBuf[ len + 5 ] = (uint8)( crc >> 8);
            if ( interTpPdusCfgTable[ pduId ].dest == INTERTP_UART )
            {
                ret = InterTp_UartTransmit( interTpTransmitMsgBuf, len + 6, interTpPdusCfgTable[ pduId ].bus );
            }
        }
    }
    if ( ret == TRUE )
    {
        if ( interTpPdusCfgTable[ pduId ].src == INTERTP_PDUR )
        {
            InterTpNetRspReceived = TRUE;
        }
    }
    return ret;
}
/*******************************************************************************
**                      Private Function Definitions                          **
*******************************************************************************/
/**
 * InterTp_lRxIndication
 *
 * @param srcModule: select mode
 * @param datas: extern datas
 * @param len: The length of the data
 * @return   none
 *
 * @brief  Receiving extern Packets
 */
static void InterTp_lRxIndication( uint8 srcModule, const uint8* datas, uint16 len )
{
    InterTpReceiveObjType* objPtr;
    uint16                 i;
    uint8                  data;
    uint16                 pduId;
    uint8                  rxDatas[ INTER_TP_RX_SIZE ];
    uint8                  xor_buf[ INTER_TP_RX_SIZE ];
    PduInfoType            pdu;
    pdu.datas = rxDatas;
    if ( srcModule >= INTER_MODULES_CNT )
    {
        return;
    }
    objPtr = ( interTpReceiveObjs + srcModule );
    for ( i = 0; i < len; i++ ) 
    {
        data = datas[ i ];
        xor_buf[i] = datas[ i ];
        switch ( objPtr->step )
        {
            case INTER_TP_WAIT_HEADER:
                if ( interTpHeadrPattern[ objPtr->fieldBytesCnt ] == data )
                {
                    objPtr->fieldBytesCnt++;
                    if ( objPtr->fieldBytesCnt == INTER_TP_HEADER_CNT )
                    {
                        objPtr->msg.id.val    = 0;
                        objPtr->fieldBytesCnt = 0;
                        objPtr->step          = INTER_TP_WAIT_ID;
                    }
                }
                else
                {
                    objPtr->fieldBytesCnt = 0;
                }
                break;
            case INTER_TP_WAIT_ID:
                objPtr->msg.id.buf[ objPtr->fieldBytesCnt ] = data;
                objPtr->fieldBytesCnt++;
                if ( objPtr->fieldBytesCnt == INTER_TP_ID_CNT )
                {
                    objPtr->fieldBytesCnt = 0;
                    objPtr->step          = INTER_TP_WAIT_DLC;
                }
                break;
            case INTER_TP_WAIT_DLC:
                objPtr->msg.dlc.buf[ objPtr->fieldBytesCnt ] = data;
                objPtr->fieldBytesCnt++;
                if ( objPtr->fieldBytesCnt == INTER_TP_DLC_CNT )
                {
					objPtr->msg.dlc.val = ( objPtr->msg.dlc.val << 8| objPtr->msg.dlc.val >> 8 ) ;
                    objPtr->fieldBytesCnt = 0;
                    objPtr->step          = INTER_TP_WAIT_DATA;
                }
                break;
            case INTER_TP_WAIT_DATA:
                objPtr->msg.datas[ objPtr->fieldBytesCnt ] = data;
                objPtr->fieldBytesCnt++;
                if ( objPtr->fieldBytesCnt == objPtr->msg.dlc.val )
                {
                    objPtr->fieldBytesCnt = 0;
                    objPtr->step          = INTER_TP_WAIT_XOR;
                }
                break;
            case INTER_TP_WAIT_XOR:
                objPtr->msg.Xor.p_buf[ objPtr->fieldBytesCnt ] = data;
                objPtr->fieldBytesCnt++;
                if ( objPtr->fieldBytesCnt == INTER_TP_XOR_CNT )
                {
                    if ( objPtr->msg.Xor.val == InterTp_CalXor( xor_buf, (objPtr->msg.dlc.val + 4)) )
                    {
                        for ( pduId = 0; pduId < INTERTP_RX_PDUS_CNT; pduId++ )
                        {
                            if ( interTpPdusCfgTable[ pduId ].id == ( objPtr->msg.id.val ) )
                            {
                                pdu.id = objPtr->msg.id.val;
                                pdu.len = objPtr->msg.dlc.val;
                                (void)memcpy( pdu.datas, objPtr->msg.datas, pdu.len );
                                if ( interTpPdusCfgTable[ pduId ].dest == INTERTP_PDUR )
                                {
                                    if ( InterTp_IsRxEnable( ) == TRUE )
                                    {
                                        PduR_RxIndication( interTpPdusCfgTable[pduId].destPduId, &pdu );
                                    }
                                }
                            }
                        }
                    }
                    objPtr->fieldBytesCnt = 0;
                    objPtr->step          = INTER_TP_WAIT_HEADER;
                }
                break;
            default:
                objPtr->fieldBytesCnt = 0;
                objPtr->step          = INTER_TP_WAIT_HEADER;
                break;
        }
    }
}
/**
 * InterTp_CalXor
 *
 * @param datas: Data that needs to be calculated
 * @param dlc: The length of the data
 * @return   The return value is CRC
 *
 * @brief  The data calculated crc
 */
static uint16 InterTp_CalXor( const uint8* datas, uint8 dlc )
{
    uint16 xor = 0;
    xor        = CRC16( datas, dlc );
    return xor;
}
