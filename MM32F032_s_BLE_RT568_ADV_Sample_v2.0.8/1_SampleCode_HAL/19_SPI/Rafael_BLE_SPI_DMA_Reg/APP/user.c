/**************************************************************************//**
 * @file     user.c
 * @version  V0.0
 * $Revision: 01 $
 * @brief
 *           Demonstrate how to use LIB pre-build function to start and stop a BLE
 *           connection.
 * @note
 *
 ******************************************************************************/
#include <stdio.h>
#include "BleAppSetting.h"
#include "rf_phy.h"
#include "porting_misc.h"
#include "ble_cmd.h"
#include "hci.h"
#include "LL.h"

const uint8_t HCI_SET_ADV_PARA[] = //HCI command (raw data)
{
    0x01, 0x06, 0x20, 0x0F,
    0xA0, 0x00, 0xA0, 0x00,
    0x00, 0x00, 0x00, 0x66,
    0x55, 0x44, 0x33, 0x22,
    0x11, 0x07, 0x00,
};

const uint8_t HCI_SET_ADV_DATA[] = //HCI command (raw data) 
{
    0x01, 0x08, 0x20, 0x0D,
    0x0C, 0x0B, 0x02, 0x01,
    0x05, 0x03, 0x02, 0x0D,
    0x18, 0x03, 0x19, 0x40,
    0x03,
};

const uint8_t HCI_SET_SCN_DATA[] = //HCI command (raw data) 
{
    0x01, 0x09, 0x20, 0x0F,
    0x0E, 0x0D, 0x09, 
    0x52, 0x61, 0x66, 0x61, 0x65, 0x6C,   //"Rafael"
    0x2D, 0x37, 0x32, 0x36, 0x38, 0x32,   //"_72682"
};

const uint8_t HCI_SET_ADV_EN[] = //HCI command (raw data) 
{
    0x01, 0x0A, 0x20, 0x01,
    0x01,
};

#pragma push
//#pragma Otime
#pragma Ospace


void BleApp_Main(void)
{
    extern Uint8 Ch_ADV_Ch_Hop_Table[4];
    extern LL_Adv LL_Adv_Para;
    extern Uint8 LL_Msg_AdvScnConn;    

    Knl_CodeCpy((uint8_t *)&LL_Adv_Para.Adv_Para.LL_Adv_Interval_Min, (uint8_t *)&HCI_SET_ADV_PARA[4], LEN_HCLL_LE_SET_ADV_PARAM_PARA);
    LL_Adv_Para.Adv_Para.LL_Adv_Type = LE_ADV_TYPE_ADV_SCAN_IND;
    LL_Adv_Para.Adv_Para.LL_Adv_Interval_Min = (LL_Adv_Para.Adv_Para.LL_Adv_Interval_Min*5);
    Ch_ADV_Ch_Hop_Table[0] = 1;
    Ch_ADV_Ch_Hop_Table[1] = 2;
    Ch_ADV_Ch_Hop_Table[2] = 3;
    Ch_ADV_Ch_Hop_Table[3] = 0;

    LL_Adv_Para.Adv_Para.LL_Adv_Data_Length = 18;
    Knl_CodeCpy(LL_Adv_Para.Adv_Para.LL_Adv_Data, &HCI_SET_ADV_DATA[5], 18);        
   
    LL_Adv_Para.Adv_Para.LL_ScanRsp_Data_Length = 0x0E;
    Knl_CodeCpy(LL_Adv_Para.Adv_Para.LL_ScanRsp_Data, &HCI_SET_SCN_DATA[5], 0x0E);
    
    InterruptDisable();
    LL_Msg_AdvScnConn |= 0x80;
    LL_Adv_Para.Adv_Para.LL_AdvMap_ID = Ch_ADV_Ch_Hop_Table[0];
    LL_Adv_Para.Adv_Para.LL_Adv_Interval_Max = 0;
    LL_Adv_Para.Adv_Para.LL_AdvConn_ID = 4;
    InterruptEnable();

} //end of BleApp_Main()

#pragma pop

