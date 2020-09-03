#ifndef __PORTING_SPI_H__
#define __PORTING_SPI_H__

#include "mcu_definition.h"  //include "stdint.h" for uint16_t  

/******************************************************************************
 * Define
 ******************************************************************************/

#define SPI_MASTER_TX_DMA_CH    3
#define SPI_MASTER_RX_DMA_CH    4
#define SPI_OPENED_CH   ((1 << SPI_MASTER_TX_DMA_CH) | (1 << SPI_MASTER_RX_DMA_CH))
#define DATA_COUNT              64
#define SPI_CLK_FREQ            12000000

#define SPI0_ClearRxFIFO() (SPI0->FIFOCTL |= SPI_FIFOCTL_RXFBCLR_Msk)
/******************************************************************************
 * Functions
 ******************************************************************************/
void MCU_SpiInit(void);

extern uint8_t SPI_1BYT_SetRx(uint8_t regAddr);
extern uint8_t SPI_1BYT_SetRx_Isr(uint8_t regAddr);
extern void SPI_1BYT_SetTx(uint8_t regAddr, uint8_t u8SrcData);
extern void SPI_1BYT_SetTx_Isr(uint8_t regAddr, uint8_t u8SrcData);
extern void SPI_2BYT_SetTx_Isr(uint8_t regAddr, uint8_t *u8SrcAddr);

void SPI_PDMA_Init(void);
extern uint32_t SPI_PDMA_waitFinish(void);
extern void SPI_PDMA_SetRx_Isr(uint8_t regAddr, uint32_t u32DstAddr, uint32_t u32TransCount);
extern void SPI_PDMA_SetTx(uint8_t regAddr, uint32_t u32SrcAddr, uint32_t u32TransCount);


#endif  //__PORTING_SPI_H__

