/******************************************************************************
 * @file    config.h
 * @author  King
 * @version V1.00
 * @date    20-May-2020
 * @brief   ......
 ******************************************************************************
 * @attention
 * 
 * THE EXISTING FIRMWARE IS ONLY FOR REFERENCE, WHICH IS DESIGNED TO PROVIDE
 * CUSTOMERS WITH CODING INFORMATION ABOUT THEIR PRODUCTS SO THEY CAN SAVE
 * TIME. THEREFORE, MINDMOTION SHALL NOT BE LIABLE FOR ANY DIRECT, INDIRECT OR
 * CONSEQUENTIAL DAMAGES ABOUT ANY CLAIMS ARISING OUT OF THE CONTENT OF SUCH
 * HARDWARE AND/OR THE USE OF THE CODING INFORMATION CONTAINED HEREIN IN
 * CONNECTION WITH PRODUCTS MADE BY CUSTOMERS.
 * <H2><CENTER>&COPY; COPYRIGHT 2020 MINDMOTION </CENTER></H2>
******************************************************************************/


/* Define to prevent recursive inclusion ------------------------------------*/
#ifndef __CONFIG_H__
#define __CONFIG_H__


#ifdef __cplusplus
extern "C" {
#endif


/* Includes -----------------------------------------------------------------*/
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>


/* Includes -----------------------------------------------------------------*/
#include "HAL_conf.h"
#include "HAL_device.h"

/* Exported typedef -------------------------------------------------------*/
typedef struct _16_Bits_Struct
{
    u16 bit0 : 1;
    u16 bit1 : 1;
    u16 bit2 : 1;
    u16 bit3 : 1;
    u16 bit4 : 1;
    u16 bit5 : 1;
    u16 bit6 : 1;
    u16 bit7 : 1;
    u16 bit8 : 1;
    u16 bit9 : 1;
    u16 bit10 : 1;
    u16 bit11 : 1;
    u16 bit12 : 1;
    u16 bit13 : 1;
    u16 bit14 : 1;
    u16 bit15 : 1;
} Bits_16_TypeDef;

/* Exported constants -------------------------------------------------------*/
#define DEBUG_UART      UART1   /* Used for printf */

#define _MCU_ARM_CORTEX_   //no actual use, just for InterruptDisable
#define Tiny_Delay(x)    delay_us(x)//SysTick_DelayMs(x)

#ifndef BIT
#define BIT(x)    (1 << (x))
#endif

#define RF_IRQ_PORT     GPIOB
#define RF_IRQ_PIN      GPIO_Pin_8
#define RF_IRQ_CLOCK    RCC_AHBPeriph_GPIOB

#define RF_RESET_PORT   GPIOB
#define RF_RESET_PIN    GPIO_Pin_9
#define RF_RESET_CLOCK  RCC_AHBPeriph_GPIOB

#define SPI2_PORT_OUT    ((Bits_16_TypeDef *)(&(GPIOB->ODR)))
#define SPI_CS            (SPI2_PORT_OUT->bit12)
#define SPI_CK            (SPI2_PORT_OUT->bit13)
#define SPI_MOSI          (SPI2_PORT_OUT->bit15)
#define SPI_MISO          (SPI2_PORT_OUT->bit14)
#define RF_IRQ_PORT_OUT  ((Bits_16_TypeDef *)(&(RF_IRQ_PORT->ODR)))
#define DEFAULT_INT       (RF_IRQ_PORT_OUT->bit8)

/* Platform Select */
#define _BOARD_NUVOTON_M031TD2AE_QFN33_ 3   //M031 daughter board
#define _BOARD_SELECTION_              _BOARD_NUVOTON_M031TD2AE_QFN33_

/* Ble App Setting */
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

/* InterruptEnable */
#ifdef _MCU_ARM_CORTEX_
#define InterruptDisable      __disable_irq
#define InterruptEnable       __enable_irq
#endif

#define E_SUCCESS (0)

#ifndef NULL
#define NULL      (0)                 ///< NULL pointer
#endif

#define TRUE      (1UL)               ///< Boolean true, define to use in API parameters or return value
#define FALSE     (0UL)               ///< Boolean false, define to use in API parameters or return value


#ifdef __cplusplus
}
#endif


#endif


/******************* (C) COPYRIGHT 2020 ************************END OF FILE***/

