/******************************************************************************
 * @file    bsp_spi_ble.c
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
#define __bsp_spi_ble_C__


/* Includes -----------------------------------------------------------------*/
#include "spi.h"
#include "bsp_spi_ble.h"
#include "rf_phy.h"

/* Private typedef ----------------------------------------------------------*/
/* Private define -----------------------------------------------------------*/
/* Private macro ------------------------------------------------------------*/


/* Private variables --------------------------------------------------------*/
uint8_t SPI_FLASH_Buffer[SPI_FLASH_SECTOR_SIZE];


/* Private function prototypes ----------------------------------------------*/
/* Private functions --------------------------------------------------------*/


/* Exported variables -------------------------------------------------------*/
/* Exported function prototypes ---------------------------------------------*/

extern void SPIx_DMA_Configure(DMA_Channel_TypeDef                  *Channel,
                        uint32_t                              DMA_MemoryBaseAddr,
                        uint32_t                              DMA_BufferSize,
                        DMA_memory_incremented_mode_TypeDef   DMA_MemoryInc
) ;
/******************************************************************************
 * @brief       
 * @param       
 * @retval      
 * @attention   
******************************************************************************/
void BSP_SPI_BLE_Init(void)
{
    uint8_t Data[10] = {0xFF} ;
    uint8_t pBuf[10] = {0xFF} ;
    /* Set SPI_BLE CS Pin High Level */
    BSP_SPI_BLE_CS_H();

    /* SPI Configuration */
    SPIx_Configure(SPI2);

    /* Enable SPI BLE TX And RX */
    BSP_SPI_BLE_TXEN();
    BSP_SPI_BLE_RXEN();
    /* initial SPI PDMA */
//    SPI_PDMA_Init();
    SPIx_DMA_Configure(DMA1_Channel4, (uint32_t)Data, 0, DMA_MemoryInc_Disable);
    SPIx_DMA_Configure(DMA1_Channel5, (uint32_t)pBuf, 0, DMA_MemoryInc_Disable);
}


// Read RF 1 byte register
uint8_t SPI_1BYT_SetRx(uint8_t regAddr)
{
    uint32_t u32i;

    InterruptDisable();
    
    SPI2->NSSR &= ~SPI_NSSR_NSS ;//BSP_SPI_BLE_CS_L();// 
////    SPI0_ClearRxFIFO();
    SPIx_ReadWriteByte(SPI2, (regAddr|0x80));            //write 1st byte: (regAddr & 0x7F) | 0x80

    if((regAddr&0x80))
    {
        SPIx_ReadWriteByte(SPI2, 0x01);                  //write 2nd byte: (regAddr & 0x80)>>7
    }
    else
    {
        SPIx_ReadWriteByte(SPI2, 0x00);
    }
    
    SPIx_ReadWriteByte(SPI2, 0xFF);        //1 more dummy byte
    u32i = SPIx_ReadWriteByte(SPI2, 0xF5);        //write 1 dummy byte to read 1 byte data

//    while(SPI_GetFlagStatus(SPI2,SPI_FLAG_RXAVL_4BYTE)==RESET);  
//    SPI_ReceiveData(SPI2);
//    SPI_ReceiveData(SPI2);
//    SPI_ReceiveData(SPI2);
//    u32i = SPI_ReceiveData(SPI2);

////    SPI0_ClearRxFIFO();
    SPI2->NSSR |= SPI_NSSR_NSS ;//BSP_SPI_BLE_CS_H();
    
    InterruptEnable();
    return (uint8_t)u32i;
}

// Read RF 1 byte register or RX_FIFO in ISR
uint8_t SPI_1BYT_SetRx_Isr(uint8_t regAddr)
{
    uint32_t u32i;
    
    SPI2->NSSR &= ~SPI_NSSR_NSS ;//BSP_SPI_BLE_CS_L();// 
////    SPI0_ClearRxFIFO();
    SPIx_ReadWriteByte(SPI2, (regAddr|0x80));        //write 1st byte: (regAddr & 0x7F) | 0x80
    if((regAddr&0x80))
    {
        SPIx_ReadWriteByte(SPI2, 0x01);              //write 2nd byte: (regAddr & 0x80)>>7
    }
    else
    {
        SPIx_ReadWriteByte(SPI2, 0x00);
    }
        

    if(regAddr==RX_BUFFER_READ_PORT)   //read RX_FIFO
    {
        SPIx_ReadWriteByte(SPI2, 0xFF);                 //1 more dummy byte for MP read RX_FIFO
        SPIx_ReadWriteByte(SPI2, 0xFF);                 //1 more dummy byte for MP read RX_FIFO

        u32i = SPIx_ReadWriteByte(SPI2, 0xF5);                 //write 1 dummy byte to read 1 byte data

//        while((SPI2 ->CSTAT & 0x00000F00)!=0x00000500);
//        SPI_ReceiveData(SPI2);
//        SPI_ReceiveData(SPI2);
//        SPI_ReceiveData(SPI2);
//        SPI_ReceiveData(SPI2);
    }
    else            //read register
    {
        SPIx_ReadWriteByte(SPI2, 0xFF);                 //1 more dummy byte for MP read register

        u32i = SPIx_ReadWriteByte(SPI2, 0xF5);                 //write 1 dummy byte to read 1 byte data

//        while(SPI_GetFlagStatus(SPI2,SPI_FLAG_RXAVL_4BYTE)==RESET); 
//        SPI_ReceiveData(SPI2);
//        SPI_ReceiveData(SPI2);
//        SPI_ReceiveData(SPI2);
    }

//    u32i = SPI_ReceiveData(SPI2);
////    SPI0_ClearRxFIFO();
    SPI2->NSSR |= SPI_NSSR_NSS ;//BSP_SPI_BLE_CS_H();
    
    return (uint8_t)u32i;
}


// Write RF 1 byte register (not use for writing TX buffer)
void SPI_1BYT_SetTx(uint8_t regAddr, uint8_t u8SrcData)
{
    InterruptDisable();
    
    SPI2->NSSR &= ~SPI_NSSR_NSS ;//BSP_SPI_BLE_CS_L();// 
    
    SPIx_ReadWriteByte(SPI2, (regAddr&0x7F));  //write 1st byte: regAddr & 0x7F

    if((regAddr&0x80))
    {
        SPIx_ReadWriteByte(SPI2, 0x01);        //write 2nd byte: (regAddr & 0x80) >> 7
    }
    else
    {
        SPIx_ReadWriteByte(SPI2, 0x00);
    }

    SPIx_ReadWriteByte(SPI2, u8SrcData);
    
    SPI2->NSSR |= SPI_NSSR_NSS ;//BSP_SPI_BLE_CS_H();
    InterruptEnable();
}

// Write RF 1 byte register in ISR (not use for writing TX buffer)
void SPI_1BYT_SetTx_Isr(uint8_t regAddr, uint8_t u8SrcData)
{      
    SPI2->NSSR &= ~SPI_NSSR_NSS ;//BSP_SPI_BLE_CS_L();//
    
    SPIx_ReadWriteByte(SPI2, (regAddr&0x7F));  //write 1st byte: regAddr & 0x7F

    if((regAddr&0x80))
    {
        SPIx_ReadWriteByte(SPI2, 0x01);        //write 2nd byte: (regAddr & 0x80) >> 7
    }
    else
    {
        SPIx_ReadWriteByte(SPI2, 0x00);
    }

    SPIx_ReadWriteByte(SPI2, u8SrcData);
    
    SPI2->NSSR |= SPI_NSSR_NSS ;//BSP_SPI_BLE_CS_H();
}

// Write RF 2 byte registers in ISR
void SPI_2BYT_SetTx_Isr(uint8_t regAddr, uint8_t *u8SrcAddr)
{
    SPI2->NSSR &= ~SPI_NSSR_NSS ;//BSP_SPI_BLE_CS_L();//
    
    SPIx_ReadWriteByte(SPI2, (regAddr&0x7F));    //write 1st byte: regAddr & 0x7F

    if((regAddr&0x80))
    {
        SPIx_ReadWriteByte(SPI2, 0x01);          //write 2nd byte: (regAddr & 0x80) >> 7
    }
    else
    {
        SPIx_ReadWriteByte(SPI2, 0x00);
    }

    if(regAddr==TX_BUFFER_WRITE_PORT)                   //if (writing to TX_buffer)
    {
        uint32_t ram_start_addr;

        ram_start_addr = BleTxFIFOAddr_Get();
        SPIx_ReadWriteByte(SPI2, (ram_start_addr & 0xFF));       //write 3rd byte: ram_start_addr & 0xFF
        SPIx_ReadWriteByte(SPI2, (ram_start_addr & 0x0100)>>8);  //write 4th byte: (ram_start_addr & 0x0100)>>8 | (b7<<7),  b7=1/0 means fill payload/header
    }
    SPIx_ReadWriteByte(SPI2, *(u8SrcAddr));
    SPIx_ReadWriteByte(SPI2, *(u8SrcAddr+1));
    
    SPI2->NSSR |= SPI_NSSR_NSS ;//BSP_SPI_BLE_CS_H();
}

//wait PDMA finish operation
uint32_t SPI_PDMA_waitFinish(void)
{
    uint32_t u32i;

    u32i = SUCCESS;
    return u32i;
}


//Because of the set-and-forget SPI DMA behavior, user should avoid modification of the source data
//SPI PDMA read RF in ISR
void SPI_PDMA_SetRx_Isr(uint8_t regAddr, uint32_t u32DstAddr, uint32_t u32TransCount)
{
    
    extern  uint8_t TAB_ZERO_128[];//const
    
    SPI2->NSSR &= ~SPI_NSSR_NSS ;//BSP_SPI_BLE_CS_L();// 
    
//    SPI0_ClearRxFIFO();
    SPIx_ReadWriteByte(SPI2,((regAddr & 0x7F)|0x80));  //1st byte (regAddr & 0x7F) | 0x80
    if((regAddr&0x80))
    {
        SPIx_ReadWriteByte(SPI2, 0x01);                 //2nd byte (regAddr & 0x80)>>7
    }
    else
    {
        SPIx_ReadWriteByte(SPI2, 0x00);
    }

    if(regAddr==RX_BUFFER_READ_PORT)   //read RX_FIFO
    {
        SPIx_ReadWriteByte(SPI2, 0xFF);                   //1 more dummy byte for MP read RX_FIFO
        SPIx_ReadWriteByte(SPI2, 0xFF);                   //1 more dummy byte for MP read RX_FIFO
    }
    else            //read register
    {
        SPIx_ReadWriteByte(SPI2, 0xFF);                   //1 more dummy byte for MP read register
    }

//    /* Enable SPI TX DMA function */  或者依旧开启TX_DMA 只是把while去掉
//    SPIx_DMA_TxData(SPI2,(uint8_t *)TAB_ZERO_128,u32TransCount-1UL);
   
//    if(regAddr==RX_BUFFER_READ_PORT)   //read RX_FIFO
//    {
//        while(SPI_GetFlagStatus(SPI2,SPI_FLAG_RXAVL_4BYTE)==RESET);
//        SPI_ReceiveData(SPI2);
//        SPI_ReceiveData(SPI2);
//        SPI_ReceiveData(SPI2);
//        SPI_ReceiveData(SPI2);
//    }
//    else   //read register
//    {
//        while((SPI2 ->CSTAT & 0x00000F00)!=0x00000300);
//        SPI_ReceiveData(SPI2);
//        SPI_ReceiveData(SPI2);
//        SPI_ReceiveData(SPI2);
//    }
    /* Enable SPI RX DMA function */
    SPIx_DMA_RxData(SPI2,(uint8_t *)u32DstAddr,u32TransCount-1UL); //u32TransCount-1UL ?
    
    SPI2->NSSR |= SPI_NSSR_NSS ;//BSP_SPI_BLE_CS_H();
}

//Because of the set-and-forget SPI DMA behavior, user should avoid modification of the source data
//SPI PDMA write RF
void SPI_PDMA_SetTx(uint8_t regAddr, uint32_t u32SrcAddr, uint32_t u32TransCount)
{
    SPI2->NSSR &= ~SPI_NSSR_NSS ;//BSP_SPI_BLE_CS_L();// 
    
    SPIx_ReadWriteByte(SPI2, (regAddr&0x7F));       //write 1st byte: regAddr & 0x7F

    if((regAddr&0x80))
    {
        SPIx_ReadWriteByte(SPI2, 0x01);             //write 2nd byte: (regAddr & 0x80) >> 7
    }
    else
    {
        SPIx_ReadWriteByte(SPI2, 0x00);
    }

    if(regAddr==TX_BUFFER_WRITE_PORT)                             //if (writing to TX_buffer)
    {
        uint32_t ram_start_addr;

        ram_start_addr = BleTxFIFOAddr_Get();
        SPIx_ReadWriteByte(SPI2, (ram_start_addr & 0xFF));                 //write 3rd byte: ram_start_addr & 0xFF
        SPIx_ReadWriteByte(SPI2, ((ram_start_addr & 0x0100)>>8) | 0x80);   //write 4th byte: (ram_start_addr & 0x0100)>>8 | (b7<<7),  b7=1/0 means fill payload/header
    }

    /* Enable SPI TX DMA function */ 
    SPIx_DMA_TxData(SPI2,(uint8_t *)u32SrcAddr,u32TransCount-1UL); //u32TransCount-1UL ?
    
    SPI2->NSSR |= SPI_NSSR_NSS ;//BSP_SPI_BLE_CS_H();//
}


/******************* (C) COPYRIGHT 2020 ************************END OF FILE***/

