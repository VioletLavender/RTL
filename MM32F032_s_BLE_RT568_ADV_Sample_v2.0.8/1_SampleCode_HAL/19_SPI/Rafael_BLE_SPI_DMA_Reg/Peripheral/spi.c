/******************************************************************************
 * @file    spi.c
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
#define __SPI_C__


/* Includes -----------------------------------------------------------------*/
#include "spi.h"


/* Private typedef ----------------------------------------------------------*/
/* Private define -----------------------------------------------------------*/
/* Private macro ------------------------------------------------------------*/


/* Private variables --------------------------------------------------------*/
uint8_t SPI_DMATxFlag = 0;
uint8_t SPI_DMARxFlag = 0;


/* Private function prototypes ----------------------------------------------*/
/* Private functions --------------------------------------------------------*/


/* Exported variables -------------------------------------------------------*/
/* Exported function prototypes ---------------------------------------------*/


/*
 *------------------------------------------------
 *          NSS     SCK     MISO    MOSI    AF
 *------------------------------------------------
 *   SPI1   PA4     PA5     PA6     PA7     AF0
 *   SPI2   PB12    PB13    PB14    PB15    AF0
 *------------------------------------------------
 */


/******************************************************************************
 * @brief
 * @param
 * @retval
 * @attention
******************************************************************************/
void SPIx_Configure(SPI_TypeDef *SPIx)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    SPI_InitTypeDef  SPI_InitStructure;

    if (SPIx == SPI1)
    {
        /* Enable SPI1 Clock */
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);

        /* SPI2 Configuration */
        SPI_StructInit(&SPI_InitStructure);
        SPI_InitStructure.SPI_Mode              = SPI_Mode_Master;
        SPI_InitStructure.SPI_DataSize          = SPI_DataSize_8b;
        SPI_InitStructure.SPI_DataWidth         = 8;
        SPI_InitStructure.SPI_CPOL              = SPI_CPOL_Low;
        SPI_InitStructure.SPI_CPHA              = SPI_CPHA_1Edge;
        SPI_InitStructure.SPI_NSS               = SPI_NSS_Soft;
        SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_2;
        SPI_InitStructure.SPI_FirstBit          = SPI_FirstBit_MSB;
        SPI_Init(SPI1, &SPI_InitStructure);

        /* Enable SPI1 */
        SPI_Cmd(SPI1, ENABLE);

        /* Enable GPIOA Clock */
        RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA,    ENABLE);
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);

        /* Enable SPI1 GPIO Alternate Function */
        GPIO_PinAFConfig(GPIOA, GPIO_PinSource4, GPIO_AF_0);    /* PA4 SPI1_NSS  */
        GPIO_PinAFConfig(GPIOA, GPIO_PinSource5, GPIO_AF_0);    /* PA5 SPI1_SCK  */
        GPIO_PinAFConfig(GPIOA, GPIO_PinSource6, GPIO_AF_0);    /* PA6 SPI1_MISO */
        GPIO_PinAFConfig(GPIOA, GPIO_PinSource7, GPIO_AF_0);    /* PA7 SPI1_MOSI */

        /* SPI1_NSS(PA4) SPI1_SCK(PA5) And SPI1_MOSI(PA7) Configuration */
        GPIO_StructInit(&GPIO_InitStructure);
        GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_7;
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
        GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF_PP;
        GPIO_Init(GPIOA, &GPIO_InitStructure);

        /* SPI1_MISO(PA6) Configuration */
        GPIO_StructInit(&GPIO_InitStructure);
        GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_6;
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
        GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IPU;
        GPIO_Init(GPIOA, &GPIO_InitStructure);
    }
    else if (SPIx == SPI2)
    {
        /* Enable SPI2 Clock */
        RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2, ENABLE);

        /* SPI2 Configuration */
        SPI_StructInit(&SPI_InitStructure);
        SPI_InitStructure.SPI_Mode              = SPI_Mode_Master;
        SPI_InitStructure.SPI_DataSize          = SPI_DataSize_8b;
        SPI_InitStructure.SPI_DataWidth         = 8;
        SPI_InitStructure.SPI_CPOL              = SPI_CPOL_Low;
        SPI_InitStructure.SPI_CPHA              = SPI_CPHA_1Edge;
        SPI_InitStructure.SPI_NSS               = SPI_NSS_Soft;
        SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_2;
        SPI_InitStructure.SPI_FirstBit          = SPI_FirstBit_MSB;
        SPI_Init(SPI2, &SPI_InitStructure);

        /* Enable SPI2 */
        SPI_Cmd(SPI2, ENABLE);

        /* Enable GPIOB Clock */
        RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB,    ENABLE);
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);

        /* Enable SPI2 GPIO Alternate Function */
        GPIO_PinAFConfig(GPIOB, GPIO_PinSource12, GPIO_AF_0);   //---PB12 SPI2_NSS
        GPIO_PinAFConfig(GPIOB, GPIO_PinSource13, GPIO_AF_0);   //---PB13 SPI2_SCK
        GPIO_PinAFConfig(GPIOB, GPIO_PinSource14, GPIO_AF_0);   //---PB14 SPI2_MISO
        GPIO_PinAFConfig(GPIOB, GPIO_PinSource15, GPIO_AF_0);   //---PB15 SPI2_MOSI

        /* SPI2_NSS(PB12) SPI2_SCK(PB13) And SPI2_MOSI(PB15) Configuration */
        GPIO_StructInit(&GPIO_InitStructure);
        GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_15;
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
        GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF_PP;
        GPIO_Init(GPIOB, &GPIO_InitStructure);

        /* SPI2_MISO(PB14) Configuration */
        GPIO_StructInit(&GPIO_InitStructure);
        GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_14;
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
        GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_FLOATING ;//GPIO_Mode_FLOATING; //GPIO_Mode_IPU
        GPIO_Init(GPIOB, &GPIO_InitStructure);
    }
    else
    {
    }
}


/******************************************************************************
 * @brief
 * @param
 * @retval
 * @attention
******************************************************************************/
void SPIx_DMA_Configure(DMA_Channel_TypeDef                  *Channel,
                        uint32_t                              DMA_MemoryBaseAddr,
                        uint32_t                              DMA_BufferSize,
                        DMA_memory_incremented_mode_TypeDef   DMA_MemoryInc
                       )
{
    DMA_InitTypeDef DMA_InitStructure;

    /* Enable DMA1 Clock */
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);

    if (Channel == DMA1_Channel2)
    {
        /* DMA Channel Disable */
        DMA_Cmd(DMA1_Channel2, DISABLE);

        /* DMA1 Channel2(SPI1_RX) Configturation */
        DMA_DeInit(DMA1_Channel2);
        DMA_StructInit(&DMA_InitStructure);
        DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t) & (SPI1->RXREG);
        DMA_InitStructure.DMA_MemoryBaseAddr     = DMA_MemoryBaseAddr;
        DMA_InitStructure.DMA_DIR                = DMA_DIR_PeripheralSRC;
        DMA_InitStructure.DMA_BufferSize         = DMA_BufferSize;
        DMA_InitStructure.DMA_PeripheralInc      = DMA_PeripheralInc_Disable;
        DMA_InitStructure.DMA_MemoryInc          = DMA_MemoryInc;
        DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
        DMA_InitStructure.DMA_MemoryDataSize     = DMA_MemoryDataSize_Byte;
        DMA_InitStructure.DMA_Mode               = DMA_Mode_Normal;
        DMA_InitStructure.DMA_Priority           = DMA_Priority_VeryHigh;
        DMA_InitStructure.DMA_M2M                = DMA_M2M_Disable;
        DMA_InitStructure.DMA_Auto_reload        = DMA_Auto_Reload_Disable;
        DMA_Init(DMA1_Channel2, &DMA_InitStructure);

        /* Enable DMA Channel TC Interrupt */
        DMA_ITConfig(DMA1_Channel2, DMA_IT_TC, ENABLE);

        /* DMA Channel Enable */
        DMA_Cmd(DMA1_Channel2, ENABLE);
    }
    else if (Channel == DMA1_Channel3)
    {
        /* DMA Channel Disable */
        DMA_Cmd(DMA1_Channel3, DISABLE);

        /* DMA1 Channel3(SPI1_TX) Configturation */
        DMA_DeInit(DMA1_Channel3);
        DMA_StructInit(&DMA_InitStructure);
        DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t) & (SPI1->TXREG);
        DMA_InitStructure.DMA_MemoryBaseAddr     = DMA_MemoryBaseAddr;
        DMA_InitStructure.DMA_DIR                = DMA_DIR_PeripheralDST;
        DMA_InitStructure.DMA_BufferSize         = DMA_BufferSize;
        DMA_InitStructure.DMA_PeripheralInc      = DMA_PeripheralInc_Disable;
        DMA_InitStructure.DMA_MemoryInc          = DMA_MemoryInc;
        DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
        DMA_InitStructure.DMA_MemoryDataSize     = DMA_MemoryDataSize_Byte;
        DMA_InitStructure.DMA_Mode               = DMA_Mode_Normal;
        DMA_InitStructure.DMA_Priority           = DMA_Priority_VeryHigh;
        DMA_InitStructure.DMA_M2M                = DMA_M2M_Disable;
        DMA_InitStructure.DMA_Auto_reload        = DMA_Auto_Reload_Disable;
        DMA_Init(DMA1_Channel3, &DMA_InitStructure);

        /* Enable DMA Channel TC Interrupt */
        DMA_ITConfig(DMA1_Channel3, DMA_IT_TC, ENABLE);

        /* DMA Channel Enable */
        DMA_Cmd(DMA1_Channel3, ENABLE);
    }
    else if (Channel == DMA1_Channel4)
    {
        /* DMA Channel Disable */
        DMA_Cmd(DMA1_Channel4, DISABLE);

        /* DMA1 Channel4(SPI2_RX) Configturation */
        DMA_DeInit(DMA1_Channel4);
        DMA_StructInit(&DMA_InitStructure);
        DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t) & (SPI2->RXREG);
        DMA_InitStructure.DMA_MemoryBaseAddr     = DMA_MemoryBaseAddr;
        DMA_InitStructure.DMA_DIR                = DMA_DIR_PeripheralSRC;
        DMA_InitStructure.DMA_BufferSize         = DMA_BufferSize;
        DMA_InitStructure.DMA_PeripheralInc      = DMA_PeripheralInc_Disable;
        DMA_InitStructure.DMA_MemoryInc          = DMA_MemoryInc;
        DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
        DMA_InitStructure.DMA_MemoryDataSize     = DMA_MemoryDataSize_Byte;
        DMA_InitStructure.DMA_Mode               = DMA_Mode_Normal;
        DMA_InitStructure.DMA_Priority           = DMA_Priority_VeryHigh;
        DMA_InitStructure.DMA_M2M                = DMA_M2M_Disable;
        DMA_InitStructure.DMA_Auto_reload        = DMA_Auto_Reload_Disable;
        DMA_Init(DMA1_Channel4, &DMA_InitStructure);

//        /* Enable DMA Channel TC Interrupt */
//        DMA_ITConfig(DMA1_Channel4, DMA_IT_TC, ENABLE);

        /* DMA Channel Enable */
//        DMA_Cmd(DMA1_Channel4, ENABLE);
    }
    else if (Channel == DMA1_Channel5)
    {
        /* DMA Channel Disable */
        DMA_Cmd(DMA1_Channel5, DISABLE);

        /* DMA1 Channel5(SPI2_TX) Configturation */
        DMA_DeInit(DMA1_Channel5);
        DMA_StructInit(&DMA_InitStructure);
        DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t) & (SPI2->TXREG);
        DMA_InitStructure.DMA_MemoryBaseAddr     = DMA_MemoryBaseAddr;
        DMA_InitStructure.DMA_DIR                = DMA_DIR_PeripheralDST;
        DMA_InitStructure.DMA_BufferSize         = DMA_BufferSize;
        DMA_InitStructure.DMA_PeripheralInc      = DMA_PeripheralInc_Disable;
        DMA_InitStructure.DMA_MemoryInc          = DMA_MemoryInc;
        DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
        DMA_InitStructure.DMA_MemoryDataSize     = DMA_MemoryDataSize_Byte;
        DMA_InitStructure.DMA_Mode               = DMA_Mode_Normal;
        DMA_InitStructure.DMA_Priority           = DMA_Priority_VeryHigh;
        DMA_InitStructure.DMA_M2M                = DMA_M2M_Disable;
        DMA_InitStructure.DMA_Auto_reload        = DMA_Auto_Reload_Disable;
        DMA_Init(DMA1_Channel5, &DMA_InitStructure);

//        /* Enable DMA Channel TC Interrupt */
//        DMA_ITConfig(DMA1_Channel5, DMA_IT_TC, ENABLE);

        /* DMA Channel Enable */
//        DMA_Cmd(DMA1_Channel5, ENABLE);
    }
    else
    {
    }
}


/******************************************************************************
 * @brief
 * @param
 * @retval
 * @attention
******************************************************************************/
void SPIx_DMA_InterruptConfigure(SPI_TypeDef *SPIx)
{
    NVIC_InitTypeDef NVIC_InitStructure;

    if (SPIx == SPI1)
    {
        /* DMA1 Channel2~3 Interrupt Configuration */
        NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel2_3_IRQn;
        NVIC_InitStructure.NVIC_IRQChannelPriority = 2;
        NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
        NVIC_Init(&NVIC_InitStructure);
    }
    else if (SPIx == SPI2)
    {
        /* DMA1 Channel4~5 Interrupt Configuration */
        NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel4_5_IRQn;
        NVIC_InitStructure.NVIC_IRQChannelPriority = 2;
        NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
        NVIC_Init(&NVIC_InitStructure);
    }
    else
    {
    }
}
/******************************************************************************
 * @brief
 * @param
 * @retval
 * @attention
******************************************************************************/
uint8_t SPIx_WriteByte(SPI_TypeDef *SPIx, uint8_t Data)
{
    uint32_t Timeout = 0;
 
    /* Send Data */
    SPIx->TDR = Data; //SPI_SendData(SPIx, Data);
 
    /* Wait For Transmitter Empty Flag */
    while (!(SPIx->SR & SPI_FLAG_TXEPT)) //while(!SPI_GetFlagStatus(SPIx, SPI_FLAG_TXEPT))
    {
        if (Timeout++ > 0xFFFF) break;
    }
   return 0;
}

/******************************************************************************
 * @brief
 * @param
 * @retval
 * @attention
******************************************************************************/
uint8_t SPIx_ReadWriteByte(SPI_TypeDef *SPIx, uint8_t Data)
{
    uint32_t Timeout = 0;

    /* Send Data */
    SPIx->TDR = Data; //SPI_SendData(SPIx, Data);

    /* Wait For Transmitter Empty Flag */
    while (!(SPIx->SR & SPI_FLAG_TXEPT)) //while(!SPI_GetFlagStatus(SPIx, SPI_FLAG_TXEPT))
    {
        if (Timeout++ > 0xFFFF) break;
    }

    Timeout = 0;

    /* Wait ForReceive 1 Byte Available Data Flag */
    while (!(SPIx->SR & SPI_FLAG_RXAVL)) //while(!SPI_GetFlagStatus(SPIx, SPI_FLAG_RXAVL))
    {
        if (Timeout++ > 0xFFFF) break;
    }

    /* Return Received Data */
    return SPIx->RDR ;//SPI_ReceiveData(SPIx);
}

void DMA_Set_MemoryInc_Enable(DMA_Channel_TypeDef *channel, FunctionalState NewState)
{
    if (NewState == ENABLE)
    {
        channel->CCR |= DMA_MemoryInc_Enable;
    }
    else
    {
        channel->CCR  &= ~(DMA_MemoryInc_Enable);
    }
}

void DMA_Set_MemoryAddress(DMA_Channel_TypeDef *channel, u32 address)
{
    channel->CMAR = address;
}
void DMA_Set_TransmitLen(DMA_Channel_TypeDef *channel, u16 length)
{
    channel->CNDTR = length;
}
void DRV_DMA_TransmitPacket(DMA_Channel_TypeDef *ch, u32 addr, u32 len)
{
    DMA_Cmd(ch, DISABLE);
    DMA_Set_MemoryAddress(ch, addr);
    DMA_Set_TransmitLen(ch, len);
    DMA_Cmd(ch, ENABLE);
}

/******************************************************************************
 * @brief
 * @param
 * @retval
 * @attention
******************************************************************************/
void SPIx_DMA_TxData(SPI_TypeDef *SPIx, uint8_t *pBuf, uint32_t Len)
{
    uint32_t Data[2];

    /* Clear DMA Rx & Tx Flag */
    SPI_DMARxFlag = 0;
    SPI_DMATxFlag = 0;

    if (SPIx == SPI1)
    {
        SPIx_DMA_Configure(DMA1_Channel2, (uint32_t)Data, Len, DMA_MemoryInc_Disable);
        SPIx_DMA_Configure(DMA1_Channel3, (uint32_t)pBuf, Len, DMA_MemoryInc_Enable);

        SPIx_DMA_InterruptConfigure(SPI1);

        /* Enable SPI1 */
        SPI_DMACmd(SPI1, ENABLE);

        /* Wait Receive And Transmit Complete */
        while (SPI_DMARxFlag == 0);
        while (SPI_DMATxFlag == 0);

        /* Disable SPI */
        SPI_DMACmd(SPI1, DISABLE);
    }
    else if (SPIx == SPI2)
    {
        DMA1_Channel5->CCR &= ~DMA_CCR_EN ;
        DMA1_Channel5->CCR |= DMA_MemoryInc_Enable;// DMA_Set_MemoryInc_Enable(DMA1_Channel5, ENABLE);
        DMA1_Channel5->CMAR = (u32)pBuf;// DRV_DMA_TransmitPacket(DMA1_Channel5, (u32)pBuf, Len);
        DMA1_Channel5->CNDTR = Len;
        DMA1_Channel5->CCR |= DMA_CCR_EN ;

        DMA1_Channel4->CCR &= ~DMA_CCR_EN ;
        DMA1_Channel4->CCR  &= ~(DMA_MemoryInc_Enable);// DMA_Set_MemoryInc_Enable(DMA1_Channel4, DISABLE);
        DMA1_Channel4->CMAR = (u32)Data;// DRV_DMA_TransmitPacket(DMA1_Channel4, (u32)Data, Len);
        DMA1_Channel4->CNDTR = Len;
        DMA1_Channel4->CCR |= DMA_CCR_EN ;
        
        /* Enable SPI2 */
        SPI2->GCR |= SPI_GCR_DMAEN ; //    SPI_DMACmd(SPI2, ENABLE);
        /* Wait DMA TC4 Flag And Clear It */
        while (!(DMA1->ISR & DMA1_FLAG_TC4)) ; //    while(!DMA_GetFlagStatus(DMA1_FLAG_TC4));
        DMA1->IFCR = DMA1_FLAG_TC4;   // DMA_ClearFlag(DMA1_FLAG_TC4);

        /* Wait DMA TC5 Flag And Clear It */
        while (!(DMA1->ISR & DMA1_FLAG_TC5)) ; //    while(!DMA_GetFlagStatus(DMA1_FLAG_TC5));
        DMA1->IFCR = DMA1_FLAG_TC5;  //    DMA_ClearFlag(DMA1_FLAG_TC5);

        /* Disable SPI */
        SPI2->GCR &= ~SPI_GCR_DMAEN ; //  SPI_DMACmd(SPI2, DISABLE);//
    }
    else
    {
    }
}


/******************************************************************************
 * @brief
 * @param
 * @retval
 * @attention
******************************************************************************/
void SPIx_DMA_RxData(SPI_TypeDef *SPIx, uint8_t *pBuf, uint32_t Len)
{
    volatile uint32_t Data[2] = {0xFFFFFFFF, 0xFFFFFFFF};

    /* Clear DMA Rx & Tx Flag */
    SPI_DMARxFlag = 0;
    SPI_DMATxFlag = 0;

    if (SPIx == SPI1)
    {
        SPIx_DMA_Configure(DMA1_Channel2, (uint32_t)pBuf, Len, DMA_MemoryInc_Enable);
        SPIx_DMA_Configure(DMA1_Channel3, (uint32_t)Data, Len, DMA_MemoryInc_Disable);

        SPIx_DMA_InterruptConfigure(SPI1);

        /* Enable SPI1 */
        SPI_DMACmd(SPI1, ENABLE);

        /* Wait Receive And Transmit Complete */
        while (SPI_DMARxFlag == 0);
        while (SPI_DMATxFlag == 0);

        /* Disable SPI */
        SPI_DMACmd(SPI1, DISABLE);
    }
    else if (SPIx == SPI2)
    {
        DMA1_Channel5->CCR &= ~DMA_CCR_EN ;
        DMA1_Channel5->CCR  &= ~(DMA_MemoryInc_Enable);// DMA_Set_MemoryInc_Enable(DMA1_Channel5, DISABLE);
        DMA1_Channel5->CMAR = (u32)Data;// DRV_DMA_TransmitPacket(DMA1_Channel5, (u32)Data, Len);
        DMA1_Channel5->CNDTR = Len;
        DMA1_Channel5->CCR |= DMA_CCR_EN ;

        DMA1_Channel4->CCR &= ~DMA_CCR_EN ;
        DMA1_Channel4->CCR |= DMA_MemoryInc_Enable;// DMA_Set_MemoryInc_Enable(DMA1_Channel4, ENABLE);
        DMA1_Channel4->CMAR = (u32)pBuf;// DRV_DMA_TransmitPacket(DMA1_Channel4, (u32)pBuf, Len);
        DMA1_Channel4->CNDTR = Len;
        DMA1_Channel4->CCR |= DMA_CCR_EN ;
        /* Enable SPI2 */
        SPI2->GCR |= SPI_GCR_DMAEN ; //    SPI_DMACmd(SPI2, ENABLE);

        /* Wait DMA TC4 Flag And Clear It */
        while (!(DMA1->ISR & DMA1_FLAG_TC4)) ; //    while(!DMA_GetFlagStatus(DMA1_FLAG_TC4));
        DMA1->IFCR = DMA1_FLAG_TC4;   // DMA_ClearFlag(DMA1_FLAG_TC4);

        /* Wait DMA TC5 Flag And Clear It */
        while (!(DMA1->ISR & DMA1_FLAG_TC5)) ; //    while(!DMA_GetFlagStatus(DMA1_FLAG_TC5));
        DMA1->IFCR = DMA1_FLAG_TC5;  //    DMA_ClearFlag(DMA1_FLAG_TC5);

        /* Disable SPI */
        SPI2->GCR &= ~SPI_GCR_DMAEN ; //    SPI_DMACmd(SPI2, DISABLE);
    }
    else
    {
    }
}


/******************************************************************************
 * @brief
 * @param
 * @retval
 * @attention
******************************************************************************/
void DMA1_Channel2_3_IRQHandler(void)
{
    if (DMA_GetITStatus(DMA1_IT_TC2) != RESET)
    {
        SPI_DMARxFlag = 1;

        DMA_ClearITPendingBit(DMA1_IT_TC2);
    }

    if (DMA_GetITStatus(DMA1_IT_TC3) != RESET)
    {
        SPI_DMATxFlag = 1;

        DMA_ClearITPendingBit(DMA1_IT_TC3);
    }
}


/******************************************************************************
 * @brief
 * @param
 * @retval
 * @attention
******************************************************************************/
void DMA1_Channel4_5_IRQHandler(void)
{
    if (DMA_GetITStatus(DMA1_IT_TC4) != RESET)
    {
        SPI_DMARxFlag = 1;

        DMA_ClearITPendingBit(DMA1_IT_TC4);
    }

    if (DMA_GetITStatus(DMA1_IT_TC5) != RESET)
    {
        SPI_DMATxFlag = 1;

        DMA_ClearITPendingBit(DMA1_IT_TC5);
    }
}


/******************* (C) COPYRIGHT 2020 ************************END OF FILE***/

