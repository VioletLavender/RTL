/**************************************************************************//**
* @file       rf_phy.h
* @brief      This file provides the declaration that RF IC control needed.
*
* @defgroup rf_phy RF PHY
* @{
* @details  Provides the declaration that RF IC control needed. (rf_phy.h).
* @}
*****************************************************************************/

#ifndef __RF_PHY_H__
#define __RF_PHY_H__

#include "config.h"
#include "ble_cmd.h"


/**************************************************************************
 * Definitions
 **************************************************************************/
/** @defgroup rf_phy_def RF PHY Definition.
 * @{
 * @details Here shows the definitions of RF PHY.
 * @ingroup rf_phy
 * @}
 */

/** @defgroup rf_phy_def_chipId Chip ID Definition
 * @{
 * @ingroup rf_phy_def
 */
#define MP_A1      102u                  /**< chip_id = 102 (0x66). */
#define MP_A2      103u                  /**< chip_id = 103 (0x67). */
/** @} */


/** @defgroup rf_phy_def_TRxWritePort TX/RX Buffer Write Port Definition
 * @{
 * @ingroup rf_phy_def
 */
#define RFIP_REG_254            254U          /**< Register 254. */
#define RFIP_REG_255            255U          /**< Register 255. */


#define TX_BUFFER_WRITE_PORT    RFIP_REG_254  /**< TX Buffer Write Port. */
#define RX_BUFFER_READ_PORT     RFIP_REG_255  /**< RX Buffer Write Port. */
/** @} */



//========== SW Feature options =========//
// will be moved to _rafael_phy.h in SDK
//==========================================

/** use RF timer as LL timerU
 * @ingroup rf_phy_def
 */
#define _TMR_USE_INTERNAL_      //Use RF internal RTC as timer
#ifndef _HCI_HW

/** Enable Auto-Sleep Function by LL.
 * @ingroup rf_phy_def
 */
#define _PWR_DOWN_BY_LL_        //Un-define it to stop the auto-sleep function by LL.

//#ifndef _APP_CURRENT_TEST_
//#define _PWR_DOWN_BY_LL_        //Un-define it to stop the auto-sleep function by LL.
//#endif

#endif


#define _SW_PATCH_MP_

#define DUR_LL_WAKEUP_MIN       1600    //1600*125u=200ms


/** Enable WDT Reset.
 * @ingroup rf_phy_def
 */
//#define _HW_PRG_RESET_


/** LL Power-Saving Duration. (1600*125u=200ms)
 * @ingroup rf_phy_def
 */
#define DUR_LL_WAKEUP_MIN                 1600    //1600*125u=200ms, LL power-saving duration


/** Free Time Period to the Next Event over 10ms will make the IC Go Sleep. (example: 80*125us=10ms)
 * @ingroup rf_phy_def
 */
#define DUR_PWR_DOWN_PERIOD_FCTR_125MULT  80   //125*n (us), example: 80*125us=10ms, free time period to the next event over 10ms will make the IC go sleep.
//================================================



/**************************************************************************
 * Functions
 **************************************************************************/

/** @defgroup rf_phy_function RF PHY Function
 * @{
 * @details Here shows the functions of RF PHY.
 * @ingroup rf_phy
 * @}
 */

/** Initializes the BLE stack.
 *  Must initial BLE stack after @ref RF_SpiIoMapping.
 *
 * @ingroup rf_phy_function
 */
void RF_Init(void);


/** Enable BLE Kernel.
 *  Must do it after @ref RF_SpiIoMapping and @ref RF_Init.
 *
 * @ingroup rf_phy_function
 * @note If no any BLE Stack (LL&Host) task is running then run user application task. \n
 * The task priority is LL>Host>User_app.
 *
 * @retval  BLESTACK_STATUS_FREE     : Stack (LL&Host task) is not running, can run user application task.
 * @retval  BLESTACK_STATUS_ERR_BUSY : Stack (LL&Host task) is running, can NOT run user application task.
 */
BleStackStatus Ble_Kernel_Root(void);


/** Get BLE LL Timeline.
 *  Timeline unit is 125us
 *
 * @ingroup rf_phy_function
 */
uint32_t LLTimeline_Get(void); // will be moved to _rafael_phy.h in SDK



/** Get BLE Device Chip ID
 *
 *  This function is used to get BLE Device Chip ID.
 *
 * @ingroup rf_phy_function
 *
 * @return  @ref rf_phy_def_chipId "Chip ID"
 */
uint8_t ChipId_Get(void);



/** Get TX FIFI Address
 *
 *  This function is used to get TX FIFO RAM start address.
 *
 * @ingroup rf_phy_function
 *
 * @return  TX FIFO address.
 */
uint32_t BleTxFIFOAddr_Get(void);



/** Get Current BLE Device RF Mode
 *
 *  This function is used to get current RF mode.
 *
 * @ingroup rf_phy_function
 *
 * @retval  BLERFMODE_ACTIVE : The BLE device RF is in active mode.
 * @retval  BLERFMODE_SLEEP  : The BLE device RF is in sleep mode.
 */
BleRF_Mode BleRFMode_Get(void);




#endif
