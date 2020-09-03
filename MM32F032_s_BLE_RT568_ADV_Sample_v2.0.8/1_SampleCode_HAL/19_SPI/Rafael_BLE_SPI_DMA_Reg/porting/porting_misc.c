/*----------------------------------------------------------------------------*/
/* This file implement MCU peripherals like: UART, GPIO, etc for BLE          */
/*----------------------------------------------------------------------------*/
#include "porting_misc.h"
#include "porting_spi.h"
#include "rf_phy.h"
#include "NuMicro.h"
#include "ble_cmd.h"

/******************************************************************************
 * Public Functions
 ******************************************************************************/
/*-------------------- GPIO peripheral ---------------------*/
void MCU_GpioResetInit(void)
{

    GPIO_SetMode(PC, BIT1, GPIO_MODE_OUTPUT);

    RESET_RF = 1;  //RESET_RF defined in porting_misc.h//IRQ¸´Î»Òý½ÅPC1
}

void MCU_GpioReset(void)
{
    //Do Reset: pulse low
    RESET_RF = 1;
    CLK_SysTickDelay(1000);       //1ms

    RESET_RF = 0;
    CLK_SysTickDelay(1000);       //1ms

    RESET_RF = 1;
}

//Assign GPIO INT pin, LED pin
void MCU_GpioPinInit(void)
{
    GPIO_SetMode(PF, BIT15, GPIO_MODE_INPUT);
}

void MCU_GpioIntEnable(void)
{
    // Configure PF.15 as Input mode and enable interrupt by rising edge trigger
    GPIO_SetMode(PF, BIT15, GPIO_MODE_INPUT);

    if(ChipId_Get()==MP_A1)
    {
        GPIO_EnableInt(PF, 15, GPIO_INT_RISING);
    }
    else
    {
        GPIO_EnableInt(PF, 15, GPIO_INT_HIGH);
    }
    NVIC_EnableIRQ(GPIO_PCPDPEPF_IRQn);


}


void MCU_GpioIntDisable(void)
{
    GPIO_DisableInt(PF, 15);
    NVIC_DisableIRQ(GPIO_PCPDPEPF_IRQn);          //disable irq in nvic
}


/* GPIO Interrupt Handler */
#pragma push
#pragma Otime
void GPCDEF_IRQHandler(void) __irq
{
    volatile uint32_t temp;
    extern void LL_GPIO_Isr(void);

    //Clear MCU GPIO Int status
    if(GPIO_GET_INT_FLAG(PF, BIT15))
    {
        GPIO_CLR_INT_FLAG(PF, BIT15);
    }
    else
    {
        // Un-expected interrupt. Just clear all PF interrupts
        temp = PF->INTSRC;
        PF->INTSRC = temp;
    }

    LL_GPIO_Isr();      // Put this on the top following the clearing MCU GPIO Int status process.

}
#pragma pop

/*------------------- SPI GPIO for I/O mapping --------------*/
/**************************************************************************************
 * spiWriteReg
 *
 * Writes to an 8-bit register with the SPI port
 **************************************************************************************/
void spiGpioDelay(void)
{
    CLK_SysTickDelay(1);   //one 1M cycle=1us
}

void spiGpioWriteReg(const unsigned char regAddr, const unsigned char regData)
{

    unsigned char SPICount;                               // Counter used to clock out the data
    unsigned char SPIData;                                // Define a data structure for the SPI data.

    SPI_CS = 1;                                           // Make sure we start with /CS high
    SPI_CK = 0;                                           // and CK low

    SPI_CS = 0;                                           // Set /CS low to start the SPI cycle 25nS
    // Although SPIData could be implemented as an "int", resulting in one
    // loop, the routines run faster when two loops are implemented with
    // SPIData implemented as two "char"s.

    spiGpioDelay();

    //Address 1th byte
    SPIData = regAddr & 0x7F;
    for (SPICount = 0; SPICount < 8; SPICount++)          // Prepare to clock out the Address byte
    {
        if (SPIData & 0x80)                                 // Check for a 1
            SPI_MOSI = 1;                                     // and set the MOSI line appropriately
        else
            SPI_MOSI = 0;

        spiGpioDelay();                                     // delay half clk cycle
        SPI_CK = 1;                                         // Toggle the clock line
        spiGpioDelay();
        SPI_CK = 0;
        SPIData <<= 1;                                      // Rotate to get the next bit
    }                                                     // and loop back to send the next bit
    // Repeat for the Data byte
    //Address 2nd byte
    SPIData = (regAddr & 0x80)>>7;
    for (SPICount = 0; SPICount < 8; SPICount++)          // Prepare to clock out the Address byte
    {
        if (SPIData & 0x80)                                 // Check for a 1
            SPI_MOSI = 1;                                     // and set the MOSI line appropriately
        else
            SPI_MOSI = 0;

        spiGpioDelay();                                     // delay half clk cycle
        SPI_CK = 1;                                         // Toggle the clock line
        spiGpioDelay();
        SPI_CK = 0;
        SPIData <<= 1;                                      // Rotate to get the next bit
    }

    //Data
    SPIData = regData;                                    // Preload the data to be sent with Data
    for (SPICount = 0; SPICount < 8; SPICount++)          // Prepare to clock out the Data
    {
        if (SPIData & 0x80)
            SPI_MOSI = 1;
        else
            SPI_MOSI = 0;

        spiGpioDelay();
        SPI_CK = 1;
        spiGpioDelay();
        SPI_CK = 0;
        SPIData <<= 1;
    }

    spiGpioDelay();

    SPI_CS = 1;
    SPI_MOSI = 0;
}


void SPI_GPIO_Init(void)
{
    GPIO_SetMode(PA, BIT2, GPIO_MODE_OUTPUT);
    GPIO_SetMode(PA, BIT0, GPIO_MODE_OUTPUT);
    GPIO_SetMode(PA, BIT1, GPIO_MODE_OUTPUT);
    GPIO_SetMode(PA, BIT3, GPIO_MODE_OUTPUT);
    GPIO_SetMode(PF, BIT15, GPIO_MODE_OUTPUT);  //PF15
}

//SPI IO mapping. Must do this after Power ON
void RF_SpiIoMapping(void)
{
    //(1) Set all SPI GPIO pin as output. Be careful not to set 5 pins as HIGH, it will trigger HW mapping SPI pin
    SPI_GPIO_Init();         //Here set all CLK,CS,MOSI,NISO,INT as output
    SPI_CS=1;
    SPI_CK=0;
    SPI_MOSI=0;
    SPI_MISO=0;
    DEFAULT_INT=0;
    //(2) Write R248, R249 GPIO select
    //Write R248=8'b10,001,000, R249=8'b0,100,011,0
    //GPIO0[2:0]=0 - CS
    //GPIO1[2:0]=1 - CLK
    //GPIO2[2:0]=2 - MOSI
    //GPIO3[2:0]=3 - MISO
    //GPIO4[2:0]=4 - INT
    CLK_SysTickDelay(50);
    spiGpioWriteReg(248, 0x88);
    CLK_SysTickDelay(50);
    spiGpioWriteReg(249, 0x46);
    CLK_SysTickDelay(50);

    //(3) Output all pin as HIGH, last 10ms(>1ms). trigger HW take effect
    SPI_GPIO_Init();          //Set all as output
    SPI_CS=1;
    SPI_CK=1;
    SPI_MOSI=1;
    SPI_MISO=1;
    DEFAULT_INT=1;
    CLK_SysTickDelay(10000);  //Delay 10ms
    //(4) Init GPIO & SPI
    MCU_GpioPinInit();        //Set GPIO interrupt pin as input
    MCU_SpiInit();            //Intial SPI pin, change MISO direction as INPUT

    SPI_1BYT_SetTx(249, (0x46 | 0x80));  //set RF MISO, INT as output

    //manual control
    SPI_1BYT_SetTx(53, 0xC0); //To gurantee DC/DC power on when set R40=0x90

    //enable LDO
    SPI_1BYT_SetTx(40, 0xC0);
    CLK_SysTickDelay(25000);        //Put delay after LDO_enable, or set register may have strange behavior!

    //enable chip
    SPI_1BYT_SetTx(53, 0x80); //Enable chip
    CLK_SysTickDelay(10000);        //Put delay after chip_enable, or set register may have strange behavior!
}
