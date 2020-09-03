/******************************************************************************
 * @file    bsp_spi_flash.h
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
#ifndef __BSP_SPI_FLASH_H__
#define __BSP_SPI_FLASH_H__


#ifdef __cplusplus
extern "C" {
#endif


#undef  EXTERN


#ifdef  __BSP_SPI_FLASH_C__
#define EXTERN
#else
#define EXTERN extern
#endif


/* Includes -----------------------------------------------------------------*/
#include "config.h"


/* Exported constants -------------------------------------------------------*/
#define SPI_FLASH_PAGE_SIZE     256
#define SPI_FLASH_SECTOR_SIZE   4096


/* Exported types -----------------------------------------------------------*/


/* Exported macro -----------------------------------------------------------*/
#define BSP_SPI_BLE_CS_H()    SPI_CSInternalSelected(SPI2, DISABLE)
#define BSP_SPI_BLE_CS_L()    SPI_CSInternalSelected(SPI2, ENABLE)

#define BSP_SPI_BLE_RXEN()    SPI_BiDirectionalLineConfig(SPI2, SPI_Direction_Rx)
#define BSP_SPI_BLE_TXEN()    SPI_BiDirectionalLineConfig(SPI2, SPI_Direction_Tx)


/* Exported functions -------------------------------------------------------*/
EXTERN void BSP_SPI_BLE_Init(void);
EXTERN void BSP_SPI_BLE_Entry(void);

EXTERN uint8_t SPI_1BYT_SetRx(uint8_t regAddr);
EXTERN uint8_t SPI_1BYT_SetRx_Isr(uint8_t regAddr);
EXTERN void SPI_1BYT_SetTx(uint8_t regAddr, uint8_t u8SrcData);
EXTERN void SPI_1BYT_SetTx_Isr(uint8_t regAddr, uint8_t u8SrcData);
EXTERN void SPI_2BYT_SetTx_Isr(uint8_t regAddr, uint8_t *u8SrcAddr);

EXTERN uint32_t SPI_PDMA_waitFinish(void);
EXTERN void SPI_PDMA_SetRx_Isr(uint8_t regAddr, uint32_t u32DstAddr, uint32_t u32TransCount);
EXTERN void SPI_PDMA_SetTx(uint8_t regAddr, uint32_t u32SrcAddr, uint32_t u32TransCount);

#ifdef __cplusplus
}
#endif


#endif


/******************* (C) COPYRIGHT 2020 ************************END OF FILE***/

