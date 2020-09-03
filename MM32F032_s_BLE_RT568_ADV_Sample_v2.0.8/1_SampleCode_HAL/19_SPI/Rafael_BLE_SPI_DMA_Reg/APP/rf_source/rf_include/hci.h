#ifndef _HCI_H_
#define _HCI_H_

#include "config.h"
#include "ble_cmd.h"
#include "_ble_host.h"
#include "knl_pblc.h"

#ifdef _HCI_NEW_

//Four kinds of HCI packets that can be sent via the UART Transport Layer
//HCI__001
#define HCI_PKT_COMMAND                         0x01
#define HCI_PKT_ACL_DATA                        0x02
#define HCI_PKT_SYNCHRONOUS_DATA                0x03
#define HCI_PKT_EVENT                           0x04

//LL__023
#define HCI_VERSION                             BLE_VERSION     //HCI__002
#define HCI_REVISION_L                          0x01
#define HCI_REVISION_H                          0x00
#define LMP_VERSION                             BLE_VERSION     //HCI__003
#define LMP_VERSION_SUBVER_L                    0x00
#define LMP_VERSION_SUBVER_H                    0x00

//HCI__016


//HCI__032
#define HCI_ADDR_TYPE_PUBLIC                        0x00
#define HCI_ADDR_TYPE_RANDOM                        0x01
//#define HCI_ADDR_TYPE_RESOLVABLE_PRIVATE_PUBLIC     0x02
#define HCI_ADDR_TYPE_RESOLVABLE_PRIVATE_RANDOM     0x03

#define HCI_ROLE_MASTER                             0x00
#define HCI_ROLE_SLAVE                              0x01
#endif

typedef uint8_t HCIStatus;
//HCI__008


//HCI__009
#ifdef _HCI_NEW_
//LL__022
#define HCI_ADV_TYPE_ADV_IND                                            0x00
#define HCI_ADV_TYPE_ADV_DIRECT_IND_HIGH                                0x01
#define HCI_ADV_TYPE_ADV_SCAN_IND                                       0x02
#define HCI_ADV_TYPE_ADV_NONCONN_IND                                    0x03
#define HCI_ADV_TYPE_ADV_DIRECT_IND_LOW                                 0x04
#define HCI_ADV_TYPE_EVT_SCAN_RSP                                       0x04    //HCI__031
#define HCI_ADV_TYPE_EVT_UNKNOWN                                        0x05    //Jeffrey defined
#endif

#define SIZE_UART_BUFFER                            255



extern Uint8 Event_Mask[5];
extern Uint8 Event_Mask2[1];
extern Uint8 LE_Event_Mask[3];

extern Uint8 SIZE_HCI_COMMAND_LE_CONTROLLER_PARAMETERS[(30+2)];

extern Uint8 TAB_HCI_PKT_LTH_TABLE[5];

extern Uint8 TAB_LMP_FEATURES_MASK[8];

extern void initUart(void);

typedef void (*MHCCallBack)(BleCmdEvent event,
                            void *param);

#ifdef _DEBUG_MSG_USER_
extern void msg2uart(Uint8 * CodeStr, Uint8 *HexStr, Uint8 HexStrLength);
#endif

#endif   //#define _HCI_H_
