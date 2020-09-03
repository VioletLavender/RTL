/*----------------------------------------------------------------------------*/
/* This file implement MCU peripherals like: SPI for BLE                      */
/*----------------------------------------------------------------------------*/

#include "ble_basicType.h"
#include "mcu_definition.h"
#include "porting_spi.h"
#include "rf_phy.h"
#include "NuMicro.h"
#include "stdio.h"
#include "ble_cmd.h"

/******************************************************************************
 * Variables
 ******************************************************************************/




/******************************************************************************
 * Public Functions
 ******************************************************************************/
//Initial SPI clock and pin
void MCU_SpiInit(void)
{
    /* Select PCLK1 as the clock source of SPI0*/
    CLK_SetModuleClock(SPI0_MODULE, CLK_CLKSEL2_SPI0SEL_PCLK1, MODULE_NoMsk);
    /* Enable SPI0 peripheral clock */
    CLK_EnableModuleClock(SPI0_MODULE);
    /* Enable PDMA clock source */
    CLK_EnableModuleClock(PDMA_MODULE);

    /* MCU SPI pin initialization */
    // Setup SPI0 multi-function pins //
    // PA.3 is SPI0_SS,   PA.2 is SPI0_CLK,
    // PA.1 is SPI0_MISO, PA.0 is SPI0_MOSI
    SYS->GPA_MFPL = (SYS->GPA_MFPL & ~(SYS_GPA_MFPL_PA3MFP_Msk |
                                       SYS_GPA_MFPL_PA2MFP_Msk |
                                       SYS_GPA_MFPL_PA1MFP_Msk |
                                       SYS_GPA_MFPL_PA0MFP_Msk)) |
                    (SYS_GPA_MFPL_PA3MFP_SPI0_SS |
                     SYS_GPA_MFPL_PA2MFP_SPI0_CLK |
                     SYS_GPA_MFPL_PA1MFP_SPI0_MISO |
                     SYS_GPA_MFPL_PA0MFP_SPI0_MOSI);


    /* SPI master, clk=8M, mode 0, 8-bit, MSB first */
    SPI_Open(SPI0, SPI_MASTER, SPI_MODE_0, 8, SPI_CLK_FREQ);

    /* Enable the automatic hardware slave select function. Select the SS pin and configure as low-active */
    SPI_EnableAutoSS(SPI0, SPI_SS, SPI_SS_ACTIVE_LOW);
}



#pragma push
#pragma Otime
//#pragma Ospace

// Read RF 1 byte register
uint8_t SPI_1BYT_SetRx(uint8_t regAddr)
{
    uint32_t u32i;

    while(1)
    {
        InterruptDisable();
        if(SPI_IS_BUSY(SPI0)==0)      //if(SPI is free)
        {
            SPI0_ClearRxFIFO();
            SPI_WRITE_TX(SPI0, (regAddr|0x80));            //write 1st byte: (regAddr & 0x7F) | 0x80
            SPI_DISABLE_RX_PDMA(SPI0);
            if((regAddr&0x80))
            {
                SPI_WRITE_TX(SPI0, 0x01);                  //write 2nd byte: (regAddr & 0x80)>>7
            }
            else
            {
                SPI_WRITE_TX(SPI0, 0x00);
            }
            if(((PDMA_GET_INT_STATUS(PDMA))&(PDMA_INTSTS_TDIF_Msk|PDMA_INTSTS_ABTIF_Msk|PDMA_INTSTS_REQTOF0_Msk|PDMA_INTSTS_REQTOF1_Msk)))
            {
                u32i = (PDMA_GET_TD_STS(PDMA) & SPI_OPENED_CH);
                if(u32i)                                   //Check the PDMA transfer done flags
                {
                    PDMA_CLR_TD_FLAG(PDMA, u32i);          //Clear the PDMA transfer done flags
                }
            }
            break;
        }
        else
        {
            // printf("assert!");
        }
        InterruptEnable();
    }

    SPI_WRITE_TX(SPI0, 0xFF);        //1 more dummy byte
    SPI_WRITE_TX(SPI0, 0xF5);        //write 1 dummy byte to read 1 byte data

    while(SPI_GET_RX_FIFO_COUNT(SPI0)!=4);
    SPI_READ_RX(SPI0);
    SPI_READ_RX(SPI0);
    SPI_READ_RX(SPI0);
    u32i = SPI_READ_RX(SPI0);

    SPI0_ClearRxFIFO();
    InterruptEnable();
    return (uint8_t)u32i;
}

// Read RF 1 byte register or RX_FIFO in ISR
uint8_t SPI_1BYT_SetRx_Isr(uint8_t regAddr)
{
    uint32_t u32i;

    while(1)
    {
        if(SPI_IS_BUSY(SPI0)==0)
        {
            SPI0_ClearRxFIFO();
            SPI_WRITE_TX(SPI0, (regAddr|0x80));        //write 1st byte: (regAddr & 0x7F) | 0x80
            SPI_DISABLE_RX_PDMA(SPI0);
            if((regAddr&0x80))
            {
                SPI_WRITE_TX(SPI0, 0x01);              //write 2nd byte: (regAddr & 0x80)>>7
            }
            else
            {
                SPI_WRITE_TX(SPI0, 0x00);
            }
            if(((PDMA_GET_INT_STATUS(PDMA))&(PDMA_INTSTS_TDIF_Msk|PDMA_INTSTS_ABTIF_Msk|PDMA_INTSTS_REQTOF0_Msk|PDMA_INTSTS_REQTOF1_Msk)))
            {
                u32i = (PDMA_GET_TD_STS(PDMA) & SPI_OPENED_CH);
                if(u32i)                                //Check the PDMA transfer done flags
                {
                    PDMA_CLR_TD_FLAG(PDMA, u32i);       //Clear the PDMA transfer done flags
                }
            }
            break;
        }
        else
        {
            //printf("assert!");
        }
    }

    if(regAddr==RX_BUFFER_READ_PORT)   //read RX_FIFO
    {
        SPI_WRITE_TX(SPI0, 0xFF);                 //1 more dummy byte for MP read RX_FIFO
        SPI_WRITE_TX(SPI0, 0xFF);                 //1 more dummy byte for MP read RX_FIFO

        SPI_WRITE_TX(SPI0, 0xF5);                 //write 1 dummy byte to read 1 byte data

        while(SPI_GET_RX_FIFO_COUNT(SPI0)!=5);
        SPI_READ_RX(SPI0);
        SPI_READ_RX(SPI0);
        SPI_READ_RX(SPI0);
        SPI_READ_RX(SPI0);
    }
    else            //read register
    {
        SPI_WRITE_TX(SPI0, 0xFF);                 //1 more dummy byte for MP read register

        SPI_WRITE_TX(SPI0, 0xF5);                 //write 1 dummy byte to read 1 byte data

        while(SPI_GET_RX_FIFO_COUNT(SPI0)!=4);
        SPI_READ_RX(SPI0);
        SPI_READ_RX(SPI0);
        SPI_READ_RX(SPI0);
    }

    u32i = SPI_READ_RX(SPI0);
    SPI0_ClearRxFIFO();

    return (uint8_t)u32i;
}

// Write RF 1 byte register (not use for writing TX buffer)
void SPI_1BYT_SetTx(uint8_t regAddr, uint8_t u8SrcData)
{
    uint32_t u32i;

    while(1)
    {
        InterruptDisable();
        if(SPI_IS_BUSY(SPI0)==0)
        {
            if(((PDMA_GET_INT_STATUS(PDMA))&(PDMA_INTSTS_TDIF_Msk|PDMA_INTSTS_ABTIF_Msk|PDMA_INTSTS_REQTOF0_Msk|PDMA_INTSTS_REQTOF1_Msk)))
            {
                u32i = (PDMA_GET_TD_STS(PDMA) & SPI_OPENED_CH);
                if(u32i)                             //Check the PDMA transfer done flags
                {
                    PDMA_CLR_TD_FLAG(PDMA, u32i);    //Clear the PDMA transfer done flags
                }
            }
            break;
        }
        else
        {
            //printf("assert!");
        }
        InterruptEnable();
    }
    SPI_WRITE_TX(SPI0, (regAddr&0x7F));  //write 1st byte: regAddr & 0x7F

    if((regAddr&0x80))
    {
        SPI_WRITE_TX(SPI0, 0x01);        //write 2nd byte: (regAddr & 0x80) >> 7
    }
    else
    {
        SPI_WRITE_TX(SPI0, 0x00);
    }

    SPI_WRITE_TX(SPI0, u8SrcData);

    InterruptEnable();
}


// Write RF 1 byte register in ISR (not use for writing TX buffer)
void SPI_1BYT_SetTx_Isr(uint8_t regAddr, uint8_t u8SrcData)
{
    uint32_t u32i;
    while(1)
    {
        if(SPI_IS_BUSY(SPI0)==0)
        {
            if(((PDMA_GET_INT_STATUS(PDMA))&(PDMA_INTSTS_TDIF_Msk|PDMA_INTSTS_ABTIF_Msk|PDMA_INTSTS_REQTOF0_Msk|PDMA_INTSTS_REQTOF1_Msk)))
            {
                u32i = (PDMA_GET_TD_STS(PDMA) & SPI_OPENED_CH);
                if(u32i)                             //Check the PDMA transfer done flags
                {
                    PDMA_CLR_TD_FLAG(PDMA, u32i);    //Clear the PDMA transfer done flags
                }
            }
            break;
        }
        else
        {
            // printf("assert!");
        }
    }
    SPI_WRITE_TX(SPI0, (regAddr&0x7F));     //write 1st byte: regAddr & 0x7F
    if((regAddr&0x80))
    {
        SPI_WRITE_TX(SPI0, 0x01);           //write 2nd byte: (regAddr & 0x80) >> 7
    }
    else
    {
        SPI_WRITE_TX(SPI0, 0x00);
    }

    SPI_WRITE_TX(SPI0, u8SrcData);

}

// Write RF 2 byte registers in ISR
void SPI_2BYT_SetTx_Isr(uint8_t regAddr, uint8_t *u8SrcAddr)
{
    uint32_t u32i;

    while(1)
    {
        if(SPI_IS_BUSY(SPI0)==0)
        {
            if(((PDMA_GET_INT_STATUS(PDMA))&(PDMA_INTSTS_TDIF_Msk|PDMA_INTSTS_ABTIF_Msk|PDMA_INTSTS_REQTOF0_Msk|PDMA_INTSTS_REQTOF1_Msk)))
            {
                u32i = (PDMA_GET_TD_STS(PDMA) & SPI_OPENED_CH);
                if(u32i)                             //Check the PDMA transfer done flags
                {
                    PDMA_CLR_TD_FLAG(PDMA, u32i);    //Clear the PDMA transfer done flags
                }
            }
            break;
        }
        else
        {
            //printf("assert!");
        }
    }
    SPI_WRITE_TX(SPI0, (regAddr&0x7F));    //write 1st byte: regAddr & 0x7F

    if((regAddr&0x80))
    {
        SPI_WRITE_TX(SPI0, 0x01);          //write 2nd byte: (regAddr & 0x80) >> 7
    }
    else
    {
        SPI_WRITE_TX(SPI0, 0x00);
    }

    if(regAddr==TX_BUFFER_WRITE_PORT)                   //if (writing to TX_buffer)
    {
        uint32_t ram_start_addr;

        ram_start_addr = BleTxFIFOAddr_Get();
        SPI_WRITE_TX(SPI0, (ram_start_addr & 0xFF));       //write 3rd byte: ram_start_addr & 0xFF
        SPI_WRITE_TX(SPI0, (ram_start_addr & 0x0100)>>8);  //write 4th byte: (ram_start_addr & 0x0100)>>8 | (b7<<7),  b7=1/0 means fill payload/header
    }
    SPI_WRITE_TX(SPI0, *(u8SrcAddr));
    SPI_WRITE_TX(SPI0, *(u8SrcAddr+1));
}

/*------------- SPI_PDMA ---------------*/
//SPI PDMA init
void SPI_PDMA_Init(void)
{

    //Reset PDMA module
    SYS_ResetModule(PDMA_RST);

    //Enable PDMA channels
    PDMA_Open(PDMA, SPI_OPENED_CH);

    //Single request type. SPI only support PDMA single request type.
    PDMA_SetBurstType(PDMA, SPI_MASTER_TX_DMA_CH, PDMA_REQ_SINGLE, 0);
    PDMA_SetBurstType(PDMA, SPI_MASTER_RX_DMA_CH, PDMA_REQ_SINGLE, 0);

    //Set source/destination attributes
    PDMA_SetTransferAddr(PDMA, SPI_MASTER_TX_DMA_CH, NULL, PDMA_SAR_INC, (uint32_t)&SPI0->TX, PDMA_DAR_FIX);
    PDMA_SetTransferAddr(PDMA, SPI_MASTER_RX_DMA_CH, (uint32_t)&SPI0->RX, PDMA_SAR_FIX, NULL, PDMA_DAR_INC);

    //Set request source; set basic mode.
    PDMA_SetTransferMode(PDMA, SPI_MASTER_TX_DMA_CH, PDMA_SPI0_TX, FALSE, 0);
    PDMA_SetTransferMode(PDMA, SPI_MASTER_RX_DMA_CH, PDMA_SPI0_RX, FALSE, 0);

    //Disable table interrupt
    PDMA->DSCT[SPI_MASTER_TX_DMA_CH].CTL |= PDMA_DSCT_CTL_TBINTDIS_Msk;
    PDMA->DSCT[SPI_MASTER_RX_DMA_CH].CTL |= PDMA_DSCT_CTL_TBINTDIS_Msk;

}



//wait PDMA finish operation
uint32_t SPI_PDMA_waitFinish(void)
{
    uint32_t u32i;

    while(SPI_IS_BUSY(SPI0));
    u32i = PDMA_GET_INT_STATUS(PDMA);               //Get interrupt status
    if((u32i&(PDMA_INTSTS_TDIF_Msk|PDMA_INTSTS_ABTIF_Msk|PDMA_INTSTS_REQTOF0_Msk|PDMA_INTSTS_REQTOF1_Msk)))
    {
        if(u32i & PDMA_INTSTS_TDIF_Msk)             //Check the PDMA transfer done interrupt flag
        {
            u32i = (PDMA_GET_TD_STS(PDMA) & SPI_OPENED_CH);
            if(u32i)                                //Check the PDMA transfer done flags
            {
                PDMA_CLR_TD_FLAG(PDMA, u32i);       //Clear the PDMA transfer done flags
            }
            u32i = SUCCESS;
        }
        else if(u32i & (PDMA_INTSTS_REQTOF0_Msk | PDMA_INTSTS_REQTOF1_Msk))             //Check the DMA time-out interrupt flag
        {
            PDMA->INTSTS = u32i & (PDMA_INTSTS_REQTOF0_Msk | PDMA_INTSTS_REQTOF1_Msk);  //Clear the time-out flag
            u32i = FAIL;
        }
        else                                        //Check the DMA transfer abort interrupt flag, (ui32 & PDMA_INTSTS_ABTIF_Msk)
        {
            u32i = PDMA_GET_ABORT_STS(PDMA);        //Get the target abort flag
            PDMA_CLR_ABORT_FLAG(PDMA, u32i);        //Clear the target abort flag
            u32i = FAIL;
        }
    }
    else
    {
        u32i = SUCCESS;
    }
    return u32i;
}


//Because of the set-and-forget SPI DMA behavior, user should avoid modification of the source data
//SPI PDMA read RF in ISR
void SPI_PDMA_SetRx_Isr(uint8_t regAddr, uint32_t u32DstAddr, uint32_t u32TransCount)
{
    uint32_t u32i;
    extern const Uint8 TAB_ZERO_128[];

    while(1)
    {
        if(SPI_IS_BUSY(SPI0)==0)
        {
            if(((PDMA_GET_INT_STATUS(PDMA))&(PDMA_INTSTS_TDIF_Msk|PDMA_INTSTS_ABTIF_Msk|PDMA_INTSTS_REQTOF0_Msk|PDMA_INTSTS_REQTOF1_Msk)))
            {
                u32i = (PDMA_GET_TD_STS(PDMA) & SPI_OPENED_CH);
                if(u32i)                           //Check the PDMA transfer done flags
                {
                    PDMA_CLR_TD_FLAG(PDMA, u32i);  //Clear the PDMA transfer done flags
                }
            }
            break;
        }
    }
    SPI0_ClearRxFIFO();
    SPI_WRITE_TX(SPI0, ((regAddr & 0x7F)|0x80));  //1st byte (regAddr & 0x7F) | 0x80
    if((regAddr&0x80))
    {
        SPI_WRITE_TX(SPI0, 0x01);                 //2nd byte (regAddr & 0x80)>>7
    }
    else
    {
        SPI_WRITE_TX(SPI0, 0x00);
    }

    if(regAddr==RX_BUFFER_READ_PORT)   //read RX_FIFO
    {
        SPI_WRITE_TX(SPI0, 0xFF);                   //1 more dummy byte for MP read RX_FIFO
        SPI_WRITE_TX(SPI0, 0xFF);                   //1 more dummy byte for MP read RX_FIFO
    }
    else            //read register
    {
        SPI_WRITE_TX(SPI0, 0xFF);                   //1 more dummy byte for MP read register
    }

    SPI_DISABLE_RX_PDMA(SPI0);
    PDMA->DSCT[SPI_MASTER_TX_DMA_CH].SA = (uint32_t)TAB_ZERO_128; //In SPI read, Master write dummy data to generate SPI clock

    PDMA->DSCT[SPI_MASTER_TX_DMA_CH].CTL &= ~(PDMA_DSCT_CTL_TXCNT_Msk | PDMA_DSCT_CTL_TXWIDTH_Msk | PDMA_DSCT_CTL_OPMODE_Msk);
    PDMA->DSCT[SPI_MASTER_TX_DMA_CH].CTL |= ((PDMA_WIDTH_8|PDMA_OP_BASIC) | ((u32TransCount - 1UL) << PDMA_DSCT_CTL_TXCNT_Pos));
    SPI_TRIGGER_TX_PDMA(SPI0);
    PDMA->DSCT[SPI_MASTER_RX_DMA_CH].DA = u32DstAddr;
    PDMA->DSCT[SPI_MASTER_RX_DMA_CH].CTL &= ~(PDMA_DSCT_CTL_TXCNT_Msk | PDMA_DSCT_CTL_TXWIDTH_Msk | PDMA_DSCT_CTL_OPMODE_Msk);
    PDMA->DSCT[SPI_MASTER_RX_DMA_CH].CTL |= ((PDMA_WIDTH_8|PDMA_OP_BASIC) | ((u32TransCount - 1UL) << PDMA_DSCT_CTL_TXCNT_Pos));

    if(regAddr==RX_BUFFER_READ_PORT)   //read RX_FIFO
    {
        while(SPI_GET_RX_FIFO_COUNT(SPI0)!=4);
        SPI_READ_RX(SPI0);
        SPI_READ_RX(SPI0);
        SPI_READ_RX(SPI0);
        SPI_READ_RX(SPI0);
    }
    else   //read register
    {
        while(SPI_GET_RX_FIFO_COUNT(SPI0)!=3);
        SPI_READ_RX(SPI0);
        SPI_READ_RX(SPI0);
        SPI_READ_RX(SPI0);
    }

    SPI_TRIGGER_RX_PDMA(SPI0);
}


//Because of the set-and-forget SPI DMA behavior, user should avoid modification of the source data
//SPI PDMA write RF
void SPI_PDMA_SetTx(uint8_t regAddr, uint32_t u32SrcAddr, uint32_t u32TransCount)
{
    uint32_t u32i;

    while(1)
    {
        if(SPI_IS_BUSY(SPI0)==0)
        {
            if(((PDMA_GET_INT_STATUS(PDMA))&(PDMA_INTSTS_TDIF_Msk|PDMA_INTSTS_ABTIF_Msk|PDMA_INTSTS_REQTOF0_Msk|PDMA_INTSTS_REQTOF1_Msk)))
            {
                u32i = (PDMA_GET_TD_STS(PDMA) & SPI_OPENED_CH);
                if(u32i)                               //Check the PDMA transfer done flags
                {
                    PDMA_CLR_TD_FLAG(PDMA, u32i);      //Clear the PDMA transfer done flags
                }
            }
            break;
        }
    }
    PDMA->DSCT[SPI_MASTER_TX_DMA_CH].SA = u32SrcAddr;
    SPI_WRITE_TX(SPI0, (regAddr&0x7F));       //write 1st byte: regAddr & 0x7F

    if((regAddr&0x80))
    {
        SPI_WRITE_TX(SPI0, 0x01);             //write 2nd byte: (regAddr & 0x80) >> 7
    }
    else
    {
        SPI_WRITE_TX(SPI0, 0x00);
    }

    if(regAddr==TX_BUFFER_WRITE_PORT)                             //if (writing to TX_buffer)
    {
        uint32_t ram_start_addr;

        ram_start_addr = BleTxFIFOAddr_Get();
        SPI_WRITE_TX(SPI0, (ram_start_addr & 0xFF));                 //write 3rd byte: ram_start_addr & 0xFF
        SPI_WRITE_TX(SPI0, ((ram_start_addr & 0x0100)>>8) | 0x80);   //write 4th byte: (ram_start_addr & 0x0100)>>8 | (b7<<7),  b7=1/0 means fill payload/header
    }

    PDMA->DSCT[SPI_MASTER_TX_DMA_CH].CTL &= ~(PDMA_DSCT_CTL_TXCNT_Msk | PDMA_DSCT_CTL_TXWIDTH_Msk | PDMA_DSCT_CTL_OPMODE_Msk);
    PDMA->DSCT[SPI_MASTER_TX_DMA_CH].CTL |= ((PDMA_WIDTH_8|PDMA_OP_BASIC) | ((u32TransCount - 1UL) << PDMA_DSCT_CTL_TXCNT_Pos));

    SPI_TRIGGER_TX_PDMA(SPI0);  //Enable SPI master DMA function
}


#pragma pop


