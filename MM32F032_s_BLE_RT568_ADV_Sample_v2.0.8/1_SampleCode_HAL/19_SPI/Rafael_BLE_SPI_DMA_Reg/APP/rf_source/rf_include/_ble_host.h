/**************************************************************************//**
* @file       _ble_host.h
* @brief
*
* @copyright  All rights reserved. \n
*             (C) 2020 Rafael Microelectronics Co. Ltd. Taiwan, R.O.C.
*
*
*****************************************************************************/

#ifndef __BLE_HOST_H_
#define __BLE_HOST_H_


#include "ble_basicType.h"
#include "ble_cmd.h"

#define SIZE_PACKET_MAX             251  /**< BLE Max Packet Size. \n  maping to L2CAPBuf.DataBuf size
                                            PDU(39 bytes max.)+CRC(3 bytes)*/
#define SIZE_KEYBUFFER_MAX          42  /**< BLE Max Packet Size. \n
                                            PDU(39 bytes max.)+CRC(3 bytes)*/

//Both a GATT client and server implementations shall support an ATT_MTU not less than the default value
#define SIZE_DEFAULT_ATT_MTU                                23      //the default value, BLE
#define SIZE_MINIMUM_ENCRYPTION_KEY                         16
#define SIZE_AES_KEY                                        16

/** @brief BLE device parameter.
*/
typedef struct BLE_Device_Param
{
    uint8_t         ble_deviceChipId;     /** Chip Id */
    BLE_Addr_Param  ble_deviceAddr_param; /** BLE device address*/
} BLE_Device_Param;

extern BLE_Device_Param ble_device_param;


#endif // __BLE_HOST_H_


