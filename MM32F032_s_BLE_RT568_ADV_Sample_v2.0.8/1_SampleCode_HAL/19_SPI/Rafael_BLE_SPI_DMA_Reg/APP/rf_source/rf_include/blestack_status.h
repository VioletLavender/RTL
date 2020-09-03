/**************************************************************************//**
* @file       blestack_status.h
* @brief      This file contains the functions of HOST to HCI interface.
*
*
* @defgroup ble_common BLE Common
* @{
* @details This file shows the common BLE definitions and functions. (ble_cmd.h, ble_event.h)
* @}
*****************************************************************************/

#ifndef _BLESTACK_STATUS_H_
#define _BLESTACK_STATUS_H_

#include "ble_basicType.h"


/**
 * @ingroup ble_stack_definition
 * @defgroup ble_Stack_Status BLE Stack Error Code Definition
 * @{
 * @details BLE Stack error code definition.
 * @name BleStackStatus
 * @brief Define different BleStackStatus type.
 * @{
 */
typedef Uint8 BleStackStatus;

/** Successful command */
#define BLESTACK_STATUS_SUCCESS                           0x00

/** Busy */
#define BLESTACK_STATUS_ERR_BUSY                          0x01

/** Invalid command */
#define BLESTACK_STATUS_ERR_INVALID_CMD                   0x02

/** Invalid Parameter */
#define BLESTACK_STATUS_ERR_INVALID_PARAM                 0x03

/** Command timer Busy */
#define BLESTACK_STATUS_ERR_TIMER_BUSY                    0x04

/**Command feature not supported */
#define BLESTACK_STATUS_ERR_NOT_SUPPORTED                 0x05

/** Profile client configuration disable */
#define BLESTACK_STATUS_ERR_CLIENT_CONFIGURATION_DISABLE  0x06

/** Stack state is free indicates that LL&Host task is NOT running. */
#define BLESTACK_STATUS_FREE                              0x07
/** @} */
/** @} */


#endif // _BLESTACK_STATUS_H_
