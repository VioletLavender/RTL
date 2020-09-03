/**************************************************************************//**
 * @file       ble_basicType.h
 * @brief      Provide the Definition of Basic Type and Basic Constant.
 *
 * @defgroup ble_basicType BLE Basic Type Definition
 * @{
 * @details    Basic type and constant definition. (rf_phy.h).
 * @}
 *
 *****************************************************************************/
#ifndef _BLE_BASICTYPE_H_
#define _BLE_BASICTYPE_H_


/**
 * @defgroup ble_asicType BLE Basic Type Definition
 * @{
 * @ingroup ble_basicType
 * @details Basic type definition.
 */
typedef unsigned char           Uint8;
typedef unsigned short int      Uint16;
typedef unsigned int            Uint32;
typedef signed char             Sint8;
typedef signed short int        Sint16;
typedef signed int              Sint32;

/** @} */

/*--------------------------------------*/
/* Basic constant definition            */
/*--------------------------------------*/
/**
 * @defgroup ble_basicConstant BLE Basic Constant Definition
 * @{
 * @ingroup ble_basicType
 * @details Basic constant definition.
 */
#define SUCCESS         0                   /**< Success.*/
#define FAIL            (!SUCCESS)          /**< Fail.*/

#define ACCEPT          0                   /**< Accept.*/
#define REJECT          (!ACCEPT)           /**< Reject.*/

//#define RESET           0                   /**< Reset.*/
//#define SET             (!RESET)            /**< Set.*/

#define OFF             0                   /**< Off.*/
#define ON              (!OFF)              /**< On.*/

#define NORMAL          0                   /**< Normal.*/
#define ABNORMAL        (!NORMAL)           /**< Abnormal.*/

#define NO              0                   /**< No.*/
#define YES             (!NO)               /**< Yes.*/

#define LOW             0                   /**< Low.*/
#define HIGH            (!LOW)              /**< High.*/

//#define BIT_SET         1                   /**< Bite set.*/
//#define BIT_CLEAR       0                   /**< Bit clear.*/

/** @} */


#endif // _BLE_BASICTYPE_H_


