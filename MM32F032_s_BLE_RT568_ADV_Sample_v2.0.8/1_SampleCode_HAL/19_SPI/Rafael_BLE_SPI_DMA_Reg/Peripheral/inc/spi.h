/******************************************************************************
 * @file    spi.h
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
#ifndef __SPI_H__
#define __SPI_H__


#ifdef __cplusplus
extern "C" {
#endif


#undef  EXTERN


#ifdef  __SPI_C__
#define EXTERN
#else
#define EXTERN extern
#endif


/* Includes -----------------------------------------------------------------*/
#include "config.h"


/* Exported constants -------------------------------------------------------*/
/* Exported types -----------------------------------------------------------*/
/* Exported macro -----------------------------------------------------------*/


/* Exported functions -------------------------------------------------------*/
EXTERN void SPIx_Configure(SPI_TypeDef *SPIx);
EXTERN uint8_t SPIx_ReadWriteByte(SPI_TypeDef *SPIx, uint8_t Data);
EXTERN uint8_t SPIx_WriteByte(SPI_TypeDef *SPIx, uint8_t Data);
EXTERN void SPIx_DMA_TxData(SPI_TypeDef *SPIx, uint8_t *pBuf, uint32_t Len);
EXTERN void SPIx_DMA_RxData(SPI_TypeDef *SPIx, uint8_t *pBuf, uint32_t Len);


#ifdef __cplusplus
}
#endif


#endif


/******************* (C) COPYRIGHT 2020 ************************END OF FILE***/

