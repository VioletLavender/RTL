#ifndef __PORTING_MISC_H__
#define __PORTING_MISC_H__

#include "BleAppSetting.h"
#include "NuMicro.h"

#ifdef _HCI_HW_    //In normal mode, DO NOT define _HCI_HW_
#include "ble_basicType.h"
#endif

//For SPI IO re-mapping (could change by SIP MCU-RF pin connection)
#if (_BOARD_SELECTION_==_BOARD_NUVOTON_M031_SIP_)
#define SPI_IO_ORDER      6   //6(M031 SIP)
#elif (_BOARD_SELECTION_==_BOARD_NUVOTON_M031TD2AE_QFN33_)
//#define SPI_IO_ORDER      7   //7(daughter board, M0SI<->MISO)
#define SPI_IO_ORDER      1   //7(daughter board, M0SI<->MISO)
#else
#define SPI_IO_ORDER      1   //1(normal)
#endif

#if (SPI_IO_ORDER==1)
//GPIO_sel order(0:CS, 1:CK, 2:MOSI, 3:MISO, 4:INT)
#define SPI_CS            PA3
#define SPI_CK            PA2
#define SPI_MOSI          PA0
#define SPI_MISO          PA1
#if (_BOARD_SELECTION_ == _BOARD_NUVOTON_M031SE_)
#define DEFAULT_INT       PA5  //PA5 INT GPIO pin
#elif (_BOARD_SELECTION_ == _BOARD_NUVOTON_M032SE3AE_)
#define DEFAULT_INT       PA5  //PA5 INT GPIO pin
#elif (_BOARD_SELECTION_ == _BOARD_NUVOTON_M487JIDAE_B3_) //M487_EVK3
#define DEFAULT_INT       PC9  //PC9 INT GPIO pin
#elif (_BOARD_SELECTION_ == _BOARD_NUVOTON_M031TD2AE_QFN33_)
#define DEFAULT_INT       PF15 //PF15 INT GPIO pin
#elif (_BOARD_SELECTION_ == _BOARD_NUVOTON_M031_SIP_)
#define DEFAULT_INT       PC2 //PC2 INT GPIO pin
#endif    //#if(_BOARD_SELECTION_ == _BOARD_NUVOTON_M031SE_)

#elif (SPI_IO_ORDER==6)  //M031 SIP order
//GPIO_sel order(0:INT, 1:CS, 2:CK, 3:MOSI, 4:MISO)
#if (_BOARD_SELECTION_==_BOARD_NUVOTON_M031_SIP_)
#define SPI_CS            PC2
#define SPI_CK            PD3
#define SPI_MOSI          PD2
#define SPI_MISO          PD1
#define DEFAULT_INT       PD0
#else
#define SPI_CS            PA5
#define SPI_CK            PA3
#define SPI_MOSI          PA2
#define SPI_MISO          PA1
#define DEFAULT_INT       PA0
#endif //(_BOARD_SELECTION_==xxxxx)

#elif (SPI_IO_ORDER==7)  //M031TD2AE_QFN33 daughter board

#define SPI_CS            PA3
#define SPI_CK            PA2
#define SPI_MOSI          PA1
#define SPI_MISO          PA0  //MOSI<->MISO
#define DEFAULT_INT       PF15 //PF15 INT GPIO pin

#endif //(SPI_IO_ORDER)


#if ((_BOARD_SELECTION_ == _BOARD_NUVOTON_M031SE_) || (_BOARD_SELECTION_ == _BOARD_NUVOTON_M032SE3AE_))
#define  RESET_RF   PA4
#elif (_BOARD_SELECTION_ == _BOARD_NUVOTON_M031TD2AE_QFN33_)
#define  RESET_RF   PC1
#elif (_BOARD_SELECTION_ == _BOARD_NUVOTON_M031_SIP_)
#define  RESET_RF   PA12
#else
#define  RESET_RF   PC10  //M487 
#endif


void MCU_GpioResetInit(void);
void MCU_GpioReset(void);

void MCU_GpioPinInit(void);
void MCU_GpioIntEnable(void);
void MCU_GpioIntDisable(void);
//void System_PowerDown(void);

/** SPI IO mapping.
 *  Must do this after Power ON and MCU GPIO initialed.
 *
 * @ingroup rf_phy_function
 *
 */
void RF_SpiIoMapping(void);


#ifdef _DEBUG_PINS_
void Debug_Pins_Init(void);
#endif

extern void _CLK_Idle(void);

#endif  //__PORTING_MISC_H__

