#ifndef _BLEAPPSETTING_H_
#define _BLEAPPSETTING_H_

#include "config.h"

#define _HCI_NEW_        //define in SDK
//#define _HCI_HW_       //remove in SDK. However, it is a must for test
//#define _HCI_VIRTUAL_  //if it defined. hci is virtual.
//#define _HCI_HW_MSG_   //remove in SDK. (msg2uart)


// Define BLE Version
//#define BLE_VERSION             0x06    //Bluetooth Core Specification 4.0
//#define BLE_VERSION             0x07    //Bluetooth Core Specification 4.1
//#define BLE_VERSION             0x08    //Bluetooth Core Specification 4.2
#define BLE_VERSION             0x09    //Bluetooth Core Specification 5.0
//#define BLE_VERSION             0x0A    //Bluetooth Core Specification 5.1
#define BLE_COMPANY_ID_L        0x64    //HCI__004
#define BLE_COMPANY_ID_H        0x08

// Settings
#define MAX_SIZE_ATT_VALUE_OVER_MTU_WR      45    //Maximum Attribute Data Size

#endif
