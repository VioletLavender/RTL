/**************************************************************************//**
* @file       ble_cmd.h
* @brief      This file contains the functions of HOST to HCI interface.
*
*
* @defgroup ble_common BLE Common
* @{
* @details This file shows the common BLE definitions and functions. (ble_cmd.h, ble_event.h)
* @}
*****************************************************************************/

#ifndef _BLE_CMD_H_
#define _BLE_CMD_H_

//#include "host.h"
#include "blestack_status.h"
#include "ble_gapDef.h"
/**************************************************************************
 * Definitions
 **************************************************************************/
/** @defgroup ble_cmd_definition ble_cmd Definition
 * @{
 * @details Here shows the definitions in ble_cmd.h.
 * @ingroup ble_common
 * @}
 * @defgroup adv_definition BLE Advertising Definition
 * @{
 * @details Here shows the advertising related definition.
 * @ingroup ble_cmd_definition
 * @}
 * @defgroup scan_definition BLE Scan Definition
 * @{
 * @ingroup ble_cmd_definition
 * @details Here shows the scan related definition.
 * @}
 **************************************************************************/




/**
 * @ingroup ble_cmd_definition
 * @defgroup ble_cmd_cmdEvent BLE Command Event Definition
 * @{
 * @details BLE Command event definition.
 * @name BleCmdEvent
 * @brief Define the different events that could be received by the BleEventCallBack.
 * @{
 */
typedef Uint8 BleCmdEvent;

/** Null event.*/
#define BLECMD_EVENT_NULL                                 0x00

/** Event indicates the advertisement is enabled or disabled.
 * @note The parameter field is always NULL
*/
#define BLECMD_EVENT_ADV_COMPLETE                         0x01


/** Event indicates that receives the advertisement packets.*/
#define BLECMD_EVENT_SCAN_REPORT                          0x02

/** Event indicates the BLE connection is established.
 * @note The parameter field indicates the connection information though the
 * @ref BLE_Event_ConnParam Type
*/
#define BLECMD_EVENT_CONN_COMPLETE                        0x03

/** Event indicates the BLE connection has been canceled.
 * @note The parameter field is always NULL
*/
#define BLECMD_EVENT_CONN_CANCEL_COMPLETE                 0x04


/** Event indicates the Connection Parameter Update Response can be retrieved.
 * @note The parameter field indicates the connection information though the
 * @ref ble_cmd_connUpdateStatus Type \n
 * The event can be ignored, just wait for @ref BLECMD_EVENT_CONN_UPDATE_COMPLETE event
*/
#define BLECMD_EVENT_CONN_PARAMETER_UPDATE_RSP            0x05


/** Event indicates the BLE connection has been updated.
 * @note The parameter field indicates the connection information though the
 * @ref BLE_Event_ConnUpdateParam Type
*/
#define BLECMD_EVENT_CONN_UPDATE_COMPLETE                 0x06

/** Event indicates the BLE connection has been disconnected.
 * @note The parameter field indicates the connection information though the
 * @ref BLE_Event_DisconnParam Type
*/
#define BLECMD_EVENT_DISCONN_COMPLETE                     0x07

/** Event indicates that the Link Layer PHY update procedure is complete.
 * @note The parameter field indicates the connection information though the
 * @ref ble_cmd_blePhy Type
*/
#define BLECMD_EVENT_PHY_UPDATE_COMPLETE                  0x08

/** Event indicates the current Link Layer PHY can be retrieved.
 * @note The parameter field indicates the connection information though the
 * @ref ble_cmd_blePhy Type
*/
#define BLECMD_EVENT_PHY_READ_COMPLETE                    0x09

/** Event indicates the RSSI value can be retrieved.
 * @note The parameter field indicates the connection information though the
 * @ref BLE_Rssi_Param Type
*/
#define BLECMD_EVENT_READ_RSSI_COMPLETE                   0x0A


/** Event indicates the Security STK Generator Method can be retrieved.
 * @note The parameter field indicates the connection information though the
 * @ref BLE_Event_Stk_Gen_Method Type
*/
#define BLECMD_EVENT_STK_GEN_METHOD                       0x0B


/** Event indicates the PassKey Entry can be retrieved.
 * @note The parameter field indicates the connection information though the
 * @ref BLE_Event_PassKey_Confirm Type
*/
#define BLECMD_EVENT_PASSKEY_CONFIRM                      0x0C
/** @} */
/** @} */


/** Event indicates the Authentication status can be retrieved.
 * @note The parameter field indicates the connection information though the
 * @ref BLE_Event_Auth_Status Type
*/
#define BLECMD_EVENT_AUTH_STATUS                          0x0D
/** @} */
/** @} */

/** Event indicates the scan is enabled or disabled.
 * @note The parameter field is always NULL
*/
#define BLECMD_EVENT_SCAN_COMPLETE                        0x0E
/** @} */
/** @} */

/** Event indicates the create connection.
 * @note The parameter field indicates the connection information though the
 * @ref BLE_Event_CreateConn_Param Type
*/
#define BLECMD_EVENT_CREATE_CONNECTION                    0x0F
/** @} */
/** @} */


/**
 * @ingroup ble_cmd_definition
 * @defgroup ble_cmd_disconnReason BLE Disconnect Reason Definition
 * @{
 * @details BLE disconnect reason definition.
 * @name BleDisConnReason
 * @brief Define the BLE disconnect reason.
 * @{
 */
typedef Uint8 BleDisConnReason;
#define BLEDISCONNREASON_AUTHENTICATION_FAILURE                                       (0x05u) /**< Authentication failure. */
#define BLEDISCONNREASON_PIN_OR_KEY_MISSING                                           (0x06u) /**< Encryption key is missing or not saved on the remote device. */
#define BLEDISCONNREASON_CONNECTION_TIMEOUT                                           (0x08u) /**< Connection timeout. */
#define BLEDISCONNREASON_REMOTE_USER_TERMINATED_CONNECTION                            (0x13u) /**< Disconnection requested by the remote device users. */
#define BLEDISCONNREASON_REMOTE_DEVICE_TERMINATED_CONNECTION_DUE_TO_LOW_RESOURCES     (0x14u) /**< Disconnection by the remote device due to low resources. */
#define BLEDISCONNREASON_REMOTE_DEVICE_TERMINATED_CONNECTION_DUE_TO_POWER_OFF         (0x15u) /**< Disconnection by the remote device due to power off. */
#define BLEDISCONNREASON_CONNECTION_TERMINATED_BY_LOCAL_HOST                          (0x16u) /**< Disconnection by the local device Host stack. */
#define BLEDISCONNREASON_UNSUPPORTED_REMOTE_FEATURE_UNSUPPORTED_LMP_FEATURE           (0x1Au) /**< Unsupported Remote Feature. */
#define BLEDISCONNREASON_LMP_RESPONSE_TIMEOUT_LL_RESPONSE_TIMEOUT                     (0x22u) /**< LMP Response Timeout. */
#define BLEDISCONNREASON_PAIRING_WITH_UNIT_KEY_NOT_SUPPORTED                          (0x29u) /**< Pairing with Unit Key Unsupported. */
#define BLEDISCONNREASON_UNACCEPTABLE_CONNECTION_INTERVAL                             (0x3Bu) /**< Connection Interval Unacceptable. */
#define BLEDISCONNREASON_CONNECTION_TERMINATED_DUE_TO_MIC_FAILURE                     (0x3Du) /**< Connection Terminated due to MIC Failure. */
#define BLEDISCONNREASON_CONNECTION_FAILED_TO_BE_ESTABLISHED                          (0x3Eu) /**< Connection Failed to be Established. */
/** @} */
/** @} */




/**
 * @ingroup ble_cmd_definition
 * @defgroup ble_cmd_connRole BLE Connection Role Definition
 * @{
 * @details BLE connection role.
 * @name BleConnRole
 * @brief Define BLE connection role.
 * @{
 */
typedef Uint8 BleConnRole;
#define BLEROLE_MASTER                      (0x00u) /**< Master role. */
#define BLEROLE_SLAVE                       (0x01u) /**< Slave role. */
/** @} */
/** @} */


/**
 * @defgroup ble_cmd_advChannelMap BLE Advertisement Channel Definition
 * @{
 * @ingroup adv_definition
 * @details BLE advertisement channel definition.
 * @name BleAdvChannelMap
 * @brief The definition for advertising channel.
 * @{
 */
typedef Uint8 BleAdvChannelMap;

/** Channel 37 */
#define ADV_CHANNEL_37         0x01

/** Channel 38 */
#define ADV_CHANNEL_38         0x02

/** Channel 39 */
#define ADV_CHANNEL_39         0x04

/** Channel 37 and Channel 38 */
#define ADV_CHANNEL_37_38      (ADV_CHANNEL_37 | ADV_CHANNEL_38)

/** Channel 37 and Channel 39 */
#define ADV_CHANNEL_37_39      (ADV_CHANNEL_37 | ADV_CHANNEL_39)

/** Channel 38 and Channel 39 */
#define ADV_CHANNEL_38_39      (ADV_CHANNEL_38 | ADV_CHANNEL_39)

/** Channel 37 and Channel 38 and channel 39 */
#define ADV_CHANNEL_ALL        (ADV_CHANNEL_37 | ADV_CHANNEL_38 | ADV_CHANNEL_39)
/** @} */
/** @} */


/**
 * @defgroup ble_cmd_advFilter BLE Advertisement Filter Policy Definition
 * @{
 * @ingroup adv_definition
 * @details BLE advertisement filter policy definition.
 * @name BleAdvFilterPolicy
 * @brief The definition for advertising filter policy.
 * @{
 */
typedef Uint8 BleAdvFilterPolicy;

/** Filter scan requests and connect requests from any device */
#define ADV_FILTER_POLICY_ACCEPT_ALL               0x00

/** Filter scan requests with whitelist,connect requests with from any . */
#define ADV_FILTER_POLICY_ACCEPT_SCAN_REQ_WL       0x01

/** Filter scan requests from any, connect requests with whitelist. */
#define ADV_FILTER_POLICY_ACCEPT_CONN_REQ_WL       0x02

/** Filter both scan and connect requests with whitelist. */
#define ADV_FILTER_POLICY_ACCEPT_SCAN_CONN_REQ_WL  0x03
/** @} */
/** @} */


/**
 * @defgroup ble_cmd_advType BLE Advertising Type Definition
 * @{
 * @ingroup adv_definition
 * @details BLE advertising type definition.
 * @name BleAdvType
 * @brief  Advertising type for LE set advertising parameter.
 * @{
 */
typedef Uint8 BleAdvType;

#define ADV_TYPE_ADV_IND            0x00    /**< Connectable and scannable undirected advertising. */
#define ADV_TYPE_ADV_DIRECT_IND     0x01    /**< Connectable directed advertising. */
#define ADV_TYPE_SCAN_IND           0x02    /**< Scanable undirected advertising. */
#define ADV_TYPE_ADV_NONCONN_IND    0x03    /**< Non-Connectable undirected advertising. */
#define ADV_TYPE_SCAN_RSP           0x04    /**< Scan Response. */
/** @} */
/** @} */



/**
 * @defgroup ble_cmd_scanType BLE Scan Type Definition
 * @{
 * @ingroup scan_definition
 * @details BLE scan type definition.
 * @name BleScanType
 * @brief  Scan type for LE set scan request.
 * @{
 */
typedef Uint8 BleScanType;
#define SCAN_TYPE_PASSIVE           0x00 /**< Passive scan type. */
#define SCAN_TYPE_ACTIVE            0x01 /**< Active scan type. */
/** @} */
/** @} */


/**
 * @defgroup ble_cmd_scanFilterPolicy BLE Scan Filter Policy Definition
 * @{
 * @ingroup scan_definition
 * @details BLE scan filter policy definition.
 * @name BleScanFilterPolicy
 * @brief  The definition for scan filter policy.
 * @{
 */
typedef Uint8 BleScanFilterPolicy;

/** Filter advertisement packets except directed advertising and scan response from any device */
#define SCAN_FILTER_POLICY_ACCEPT_ALL               0x00

/** Filter advertisement from the device which is in the white list . */
#define ADV_FILTER_POLICY_ACCEPT_WL                 0x01
/** @} */
/** @} */


/**
 * @ingroup ble_cmd_definition
 * @defgroup ble_cmd_addrType BLE Address Type Definition
 * @{
 * @details BLE address type.
 * @name BleAddrType
 * @brief Define different BLE address type.
 * @{
 */
typedef Uint8 BleAddrType;
#define PUBLIC_ADDR                     0x00  /**< Public BLE address.  */
#define RANDOM_ADDR                     0x01  /**< Random BLE address.  */
#define PUBLIC_IDENTITY_ADDR            0x02  /**< Corresponds to Resloved Private Address. */
#define RANDOM_IDENTITY_ADDR            0x03  /**< Corresponds to Resloved Private Address. */

/** @} */
/** @} */


/**
 * @ingroup ble_cmd_definition
 * @defgroup ble_cmd_blePhy BLE PHY Definition
 * @{
 * @details BLE PHY definition.
 * @name BlePhy
 * @brief  The definition of BLE PHY.
 * @{
 */
typedef Uint8 BlePhy;
#define BLE_PHY_1M         0x01   /**< 1Mbps  */
#define BLE_PHY_2M         0x02   /**< 2Mbps  */
/** @} */
/** @} */



/**
 * @ingroup ble_cmd_definition
 * @defgroup ble_cmd_bleTxPower BLE TX Power Level Definition
 * @{
 * @details BLE TX power level definition.
 * @name BleTxPower
 * @brief  The definition of BLE TX Power Level.
 * @{
 */
typedef Sint8 BleTxPowerLevel;
#define TX_POWER_0_DBM          0     /**< 0 dBm.   */
#define TX_POWER_4_DBM          4     /**< +4 dBm.  */
#define TX_POWER_8_DBM          8     /**< +8 dBm.  */
/** @} */
/** @} */
#define TX_POWER_10_DBM         10


/**
 * @ingroup ble_cmd_definition
 * @defgroup ble_cmd_bleState BLE State Definition
 * @{
 * @details BLE State Definition.
 * @name BleState
 * @brief  The definition of BLE State.
 * @{
 */
typedef Uint8 BleMode;
#define STATE_BLE_STANDBY           0x00    /**< Stanby mode.        */
#define STATE_BLE_ADVERTISING       0x01    /**< Advertising mode.   */
#define STATE_BLE_CONNECTION        0x02    /**< Connection mode.    */
#define STATE_BLE_SCANNING          0x03    /**< Scanning mode.      */
#define STATE_BLE_INITIATING        0x04    /**< Initialating mode.  */
/** @} */
/** @} */


/**
 * @ingroup ble_cmd_definition
 * @defgroup ble_cmd_rfMode BLE RF Mode Definition
 * @{
 * @details BLE RF mode definition.
 * @name BleRFMode
 * @brief  The definition of BLE RF Mode.
 * @{
 */
typedef Uint8 BleRF_Mode;
#define BLERFMODE_ACTIVE           0x00 /**< RF is in active mode. */
#define BLERFMODE_SLEEP            0x01 /**< RF entered sleep mode. */
/** @} */
/** @} */

/**
 * @ingroup ble_cmd_definition
 * @defgroup ble_cmd_STK_GenMethod BLE STK Generator Method Definition
 * @{
 * @details BLE STK Generator Method definition.
 * @name BleSTK_GenMethod
 * @brief  The definition of BLE STK Generator Method.
 * @{
 */
typedef Uint8 BleSTK_GenMethod;
#define PASSKEY_ENTRY            0x01  /**< Entry only.   */
#define PASSKEY_DISPLAY          0x02  /**< Display only. */
/** @} */
/** @} */


/**
 * @ingroup ble_cmd_definition
 * @defgroup ble_cmd_connUpdateStatus BLE Connect Update Status Definition
 * @{
 * @details BLE Connect Update Status definition.
 * @name BleConnUpdateStatus
 * @brief  The definition of BLE Connect Update Status.
 * @{
 */
typedef Uint8 BleConnUpdateStatus;
#define CMD_SUCCESS              0x00  /**< Success.  */
#define CMD_REJECTED             0x01  /**< Rejected. */
#define CMD_TIMEOUT              0x02  /**< Timeout.  */
/** @} */
/** @} */


/**
 * @ingroup ble_cmd_definition
 * @defgroup ble_cmd_Auth_Status BLE Authentication Status Definition
 * @{
 * @details BLE Authentication Status definition.
 * @name BleAuthenticationStatus
 * @brief  The definition of BLE Authentication Status.
 * @{
 */
typedef Uint8 BleAuthStatus;
#define AUTH_SUCCESS                                    0x00  /**< Success.  */
#define AUTH_ERR_PASSKEY_ENTRY_FAILED                   0x01  /**< Passkey entry failed.  */
#define AUTH_ERR_OOB_NOT_AVAILABLE                      0x02  /**< Out of Band Key not available.  */
#define AUTH_ERR_AUTHENTICATION_REQUIREMENTS            0x03  /**< Error authentication requirement.  */
#define AUTH_ERR_CONFIRM_VALUE_FAILED                   0x04  /**< Confirm value failed.  */
#define AUTH_ERR_PAIRING_NOT_SUPPORTED                  0x05  /**< Pairing not supported.  */
#define AUTH_ERR_ENCRYPTION_KEY_SIZE                    0x06  /**< Encryption key size.  */
#define AUTH_ERR_COMMAND_NOT_SUPPORTED                  0x07  /**< Command not supported.  */
#define AUTH_ERR_UNSPECIFIED_REASON                     0x08  /**< Unspecified reason.  */
#define AUTH_ERR_REPEATED_ATTEMPTS                      0x09  /**< Too little time has elapsed since last attempt.  */
#define AUTH_ERR_INVALID_PARAMETERS                     0x0A  /**< Invalid parameters.  */
#define AUTH_ERR_DHKEY_CHECK_FAILED                     0x0B  /**< DHKey check failed.  */
#define AUTH_ERR_NUMERIC_COMPARISON_FAILED              0x0C  /**< Numeric comparison failed.  */
#define AUTH_ERR_BR_EDR_PAIRING_IN_PROGRESS             0x0D  /**< BR/EDR paring in progress.  */
#define AUTH_ERR_CROSS_TRANS_KEY_GEN_NOT_ALLOWED        0x0E  /**< BR/EDR Link Key cannot be used for LE keys.  */
#define AUTH_ERR_TIMEOUT                                0x0F  /**< Procedure time out.  */
#define AUTH_ERR_PIN_OR_KEY_MISSING                     0x10  /**< Bonding PIN / KEY Missing.  */
/** @} */
/** @} */



/**
 * @ingroup ble_cmd_definition
 * @defgroup ble_cmd_PhyUpdateStatus BLE Phy Update Status Definition
 * @{
 * @details BLE Phy Update Status definition.
 * @name BlePhyUpdateStatus
 * @brief  The definition of BLE Phy Update Status.
 * @{
 */
typedef Uint8 BlePhyUpdateStatus;
#define PHY_UPDATE_SUCCESS                         0x00  /**< Success.  */
#define PHY_UPDATE_REMOTE_FEATURE_UNSUPPORTED      0x1A  /**< Command Cancel Due to Remote feature Unsupported.  */
#define PHY_UDDATE_TRANSACTION_COLLISION           0x2A  /**< Command Cancel Due to Transaction Collision. */

/** @} */
/** @} */



/**
 * @ingroup ble_cmd_definition
 * @defgroup ble_cmd_Adv_Complete_Event_Status BLE ADV Complete Event Status Definition
 * @{
 * @details BLE ADV Complete Event Status definition.
 * @name BleAdvCompleteStatus
 * @brief  The definition of BLE ADV Complete Event Status.
 * @{
 */
typedef Uint8 BleAdvCompleteStatus;
#define ADV_ENABLE_CMD_SUCCESS                   0x00  /**< Success.  */
#define CMD_REJECTED_DUE_TO_LIMITED_RESOURCES    0x0D  /**< Rejected. */

/** @} */
/** @} */



/**
 * @ingroup ble_cmd_definition
 * @defgroup ble_cmd_IOCaps BLE IO Capabilities Definition
 * @{
 * @details BLE IO Capabilities definition.
 * @name BleIOCaps
 * @brief  The definition of BLE IO Capabilities.
 * @{
 */
typedef Uint8 IOCaps;
#define DISPLAY_ONLY                      0x00   /**< Display Only.              */
#define DISPLAY_YESNO                     0x01   /**< Display and Yes/No Entry.  */
#define KEYBOARD_ONLY                     0x02   /**< Keyboard Only.             */
#define NOINPUT_NOOUTPUT                  0x03   /**< No IO Capabilities.        */
#define KEYBOARD_DISPLAY                  0x04   /**< Keyboard and Display.      */
/** @} */
/** @} */


/**
 * @ingroup ble_cmd_definition
 * @defgroup ble_cmd_BondingFlags BLE Bonding Flags Definition
 * @{
 * @details BLE Bonding Flags definition.
 * @name BondingFlags
 * @brief  The definition of BLE Bonding Flags.
 * @{
 */
typedef Uint8 BondingFlags;
#define NO_BONDING                        0x00   /**< NO Bonding.  */
#define BONDING                           0x01   /**< Bonding.     */
/** @} */
/** @} */

/**
 * @ingroup ble_cmd_definition
 * @defgroup ble_cmd_Create_Conn Enable_Event_Status BLE Create Connection Event Status Definition
 * @{
 * @details BLE Create Connection Event Status definition.
 * @name BleCreateConnStatus
 * @brief  The definition of BLE Create Connection Event Status.
 * @{
 */
typedef Uint8 BleCreateConnStatus;
#define COMMAND_SUCCESS                              0x00  /**< Success.  */
#define COMMAND_DISALLOWED                           0x0C  /**< Disallowed. */
#define COMMAND_REJECTED_DUE_TO_LIMITED_RESOURCES    0x0D  /**< Rejected. */

/** @} */
/** @} */
/**************************************************************************
 * Structures
 **************************************************************************/
/** @defgroup ble_cmd_structure ble_cmd Structure
 * @{
 * @details Here shows the structures in ble_cmd.h.
 * @ingroup ble_common
 * @}
 **************************************************************************/

/**
 * @brief BLE address.
 * @ingroup ble_cmd_structure
*/
typedef struct BLE_Addr_Param
{

    /**
     * @ref ble_cmd_addrType "BLE address type"
     */
    BleAddrType addrType;

    /** BLE address 48-bit in LSB */
    Uint8 addr[SIZE_BLE_ADDR];

} BLE_Addr_Param;




/** @brief BLE advertising parameter.
 * @ingroup ble_cmd_structure
*/
typedef struct BLE_Adv_Param
{
    /**
     * @ref ble_cmd_addrType "BLE advertising type"
     */
    BleAdvType Adv_Type;

    /** Minimum advertising interval. \n
      *  @note Advertising interval = value * 0.625ms \n
      *        Advertising interval Range = 20ms ~ 10.24s
      */
    Uint16 Adv_Interval_Min;

    /** Maximum advertising  interval.
      *  @note Advertising interval = value * 0.625ms \n
      *        Advertising interval Range = 20ms ~ 10.24s
      */
    Uint16 Adv_Interval_Max;


    /** The BLE address and BLE address type for directed advertising */
    BLE_Addr_Param Adv_DirectAddr_Param;

    /**
     * @ref adv_definition "Advertising channel map"
     */
    BleAdvChannelMap    Adv_Channel_Map;

    /**
     * @ref ble_cmd_advFilter "Advertising filter policy"
     */
    BleAdvFilterPolicy  Adv_Filter_Policy;
} BLE_Adv_Param;



/** @brief BLE scan parameter.
 * @ingroup ble_cmd_structure
*/
typedef struct BLE_Scan_Param
{

    /**
     * @ref ble_cmd_scanType "BLE scan type"
     */
    BleScanType           Scan_Type;

    /** Scan interval.
      *  @note Scan interval = value * 0.625ms \n
      *        Scan interval Range = 2.5ms ~ 10.24s
      */
    Uint16                Scan_Interval;

    /** Scan window.
      *  @note Scan window = value * 0.625ms \n
      *        Scan window Range = 2.5ms ~ 10.24s
      */
    Uint16                Scan_Window;

    /**
     * @ref ble_cmd_scanFilterPolicy "BLE scan filter policy"
     */
    BleScanFilterPolicy   Scan_FilterPolicy;

} BLE_Scan_Param;



/** @brief BLE connection parameter.
 * @ingroup ble_cmd_structure
*/
typedef struct BLE_Conn_Param
{

    /** Minimum connection interval.
      *  @note Connection interval = value * 1.25ms \n
      *        Connection interval Range = 7.5ms ~ 4s
      */
    Uint16 Conn_IntervalMin;

    /** Maximum connection interval.
    *  @note Connection interval = value * 1.25ms \n
    *        Connection interval Range = 7.5ms ~ 4s
    */
    Uint16 Conn_IntervalMax;

    /** Slave latency for the connection in number of connection event.
     *  @note Latency Range = 0x0000 to 0x01F3
     */
    Uint16 Conn_Latency;

    /** Connection supervision timeout.
     *  @note Supervision timeout = value * 10ms \n
     *        Supervision timeout Range = 100ms ~ 32s
     */
    Uint16 Conn_SvisionTimeout;

} BLE_Conn_Param;


/** @brief BLE connection update parameter.
 * @ingroup ble_cmd_structure
*/
typedef struct BLE_ConnUpdate_Param
{
    Uint8           connId;           /**< Connection id. */
    BLE_Conn_Param  connUpdateParam;  /**< Connection parameter. */

} BLE_ConnUpdate_Param;


/** @brief BLE IO Capabilities.
 * @ingroup ble_cmd_structure
*/
typedef struct BLE_IO_Caps
{
    IOCaps          IO_Param;         /**< IO Capabilities. */

} BLE_IO_Caps;




/** @brief BLE Bonding Flags.
 * @ingroup ble_cmd_structure
*/
typedef struct BLE_Bonding_Flags
{
    BondingFlags         Flags;         /**< Bonding Flags. */

} BLE_Bonding_Flags;




/**
 * @brief BLE Event Connection Parameter.
 * @ingroup ble_cmd_structure
*/
typedef struct BLE_Event_ADV_Complete_Param
{
    BleAdvCompleteStatus  status;               /**< @ref ble_cmd_Adv_Complete_Event_Status "Event Status." */

} BLE_Event_ADV_Complete_Param;




/**
 * @brief BLE Event Connection Parameter.
 * @ingroup ble_cmd_structure
*/
typedef struct BLE_Event_ConnParam
{
    Uint8              connId;               /**< Connection id. */
    BleConnRole        connRole;             /**< @ref ble_cmd_connRole "Connection role" */
    BLE_Addr_Param     peerAddr;             /**< Remote device address type and address. */
    Uint16             connInterval;         /**< The current link connection interval value. */
    Uint16             connLatency;          /**< The current link connection latency value. */
    Uint16             supervisionTimeout;   /**< The current link connection supervision timeout value. */

} BLE_Event_ConnParam;


/**
 * @brief BLE Event Disconnection Parameter.
 * @ingroup ble_cmd_structure
*/
typedef struct BLE_Event_DisconnParam
{
    Uint8              connId;               /**< Connection id. */
    BleDisConnReason   disconnectReson;      /**< @ref ble_cmd_disconnReason "Disconnection reason". */

} BLE_Event_DisconnParam;




/**
 * @brief BLE Event Connection Update Parameter.
 * @ingroup ble_cmd_structure
*/
typedef struct BLE_Event_ConnUpdateParam
{
    Uint8               connId;               /**< Connection id. */
    BleConnUpdateStatus status;               /**< @ref ble_cmd_connUpdateStatus "Event Status." */
    Uint16              connInterval;         /**< The current link connection interval value. */
    Uint16              connLatency;          /**< The current link connection latency value. */
    Uint16              supervisionTimeout;   /**< The current link connection supervision timeout value. */

} BLE_Event_ConnUpdateParam;




/** @brief BLE RSSI value.
 * @ingroup ble_cmd_structure
*/
typedef struct BLE_Rssi_Param
{
    Uint8 connID; /**< Connection id. */
    Sint8 rssi;   /**< RSSI value. */
} BLE_Rssi_Param;





/**
 * @brief BLE Event STK Generator Method.
 * @ingroup ble_cmd_structure
*/
typedef struct BLE_Event_Stk_Gen_Method
{
    Uint8              connId;          /**< Connection id. */
    BleSTK_GenMethod   Stk_Gen_Method;  /**< @ref ble_cmd_STK_GenMethod "STK generate method." */

} BLE_Event_Stk_Gen_Method;




/**
 * @brief BLE Event PassKey Confirm.
 * @ingroup ble_cmd_structure
*/
typedef struct BLE_Event_PassKey_Confirm
{
    Uint8              connId;   /**< Connection id. */
} BLE_Event_PassKey_Confirm;




/**
 * @brief BLE Event Authentication Status.
 * @ingroup ble_cmd_structure
*/
typedef struct BLE_Event_Auth_Status
{
    Uint8              connId;   /**< Connection id. */
    BleAuthStatus      status;   /**< @ref ble_cmd_Auth_Status "Event Status." */

} BLE_Event_Auth_Status;




/**
 * @brief BLE Event Phy Update Complete Parameters.
 * @ingroup ble_cmd_structure
*/
typedef struct BLE_Event_Phy_Update_Param
{
    BlePhyUpdateStatus status;   /**< Phy update status. */
    Uint8              connId;   /**< Connection id. */
    BlePhy             tx_Phy;   /**< Tx Phy. */
    BlePhy             rx_Phy;   /**< Rx Phy. */

} BLE_Event_Phy_Update_Param;


#endif // _BLE_CMD_H_



