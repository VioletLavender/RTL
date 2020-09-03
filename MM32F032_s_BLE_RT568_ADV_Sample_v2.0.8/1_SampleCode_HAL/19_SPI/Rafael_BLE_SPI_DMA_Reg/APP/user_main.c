/**************************************************************************//**
 * @file     main.c
 * @version  V0.9
 * $Revision: 01 $
 * @brief
 *           Demonstrate BLE operation.
 *           Includes the basic initialization and the loop for kernel(BLE) operations.
 * @note
 *
 ******************************************************************************/
#include <stdio.h>
#include "rf_phy.h"
#include "porting_spi.h"
#include "porting_misc.h"
#include "mcu_definition.h"



#pragma push
#pragma Ospace

/*!
   \brief Initial neccessary peripheral on MCU.
*/
void SYS_Init(void)
{
    /* Unlock protected registers */
    SYS_UnlockReg();

    /*---------------------------------------------------------------------------------------------------------*/
    /* Init System Clock                                                                                       */
    /*---------------------------------------------------------------------------------------------------------*/
    //GPIO_SetMode(PF, (BIT2|BIT3), GPIO_MODE_INPUT);
    // Enable HIRC
    CLK_EnableXtalRC(CLK_PWRCTL_HIRCEN_Msk);
    // Waiting for HIRC clock ready
    CLK_WaitClockReady(CLK_STATUS_HIRCSTB_Msk);


    /* Set core clock as PLL_CLOCK from PLL */
    //CLK_SetCoreClock(CPU_CLOCK_RATE);  //48MHz for M0, 64MHz for M4
    CLK_SetHCLK(CLK_CLKSEL0_HCLKSEL_HIRC, CLK_CLKDIV0_HCLK(1));

    CLK->PCLKDIV = CLK_PCLKDIV_APB0DIV_DIV2 | CLK_PCLKDIV_APB1DIV_DIV2;  //48/2=24MHz

    //debug print use UART0
    /* Select HIRC as the clock source of UART0 */
    CLK_SetModuleClock(UART0_MODULE, CLK_CLKSEL1_UART0SEL_HIRC, CLK_CLKDIV0_UART0(1));

    /* Enable UART peripheral clock */
    CLK_EnableModuleClock(UART0_MODULE);

    /*---------------------------------------------------------------------------------------------------------*/
    /* Init I/O Multi-function                                                                                 */
    /*---------------------------------------------------------------------------------------------------------*/
    /* Set PB multi-function pins for UART0 RXD=PB.12 and TXD=PB.13 */
    SYS->GPB_MFPH = (SYS->GPB_MFPH & ~(SYS_GPB_MFPH_PB12MFP_Msk | SYS_GPB_MFPH_PB13MFP_Msk)) |     //It is also VCOM TX/RX port
                    (SYS_GPB_MFPH_PB12MFP_UART0_RXD | SYS_GPB_MFPH_PB13MFP_UART0_TXD);

    /* Update System Core Clock */
    /* User can use SystemCoreClockUpdate() to calculate SystemCoreClock and CyclesPerUs automatically. */
    SystemCoreClockUpdate();
    /* Lock protected registers */
    SYS_LockReg();
}

void RF_Open()
{
    /* Wait RF PHY stable */
    CLK_SysTickDelay(25000);//延时函数延时us

    /* Initialize Gpio reset pin */
    MCU_GpioResetInit();//用于设置RF芯片的复位引脚

    /* Do Gpio Reset */
    MCU_GpioReset();//标志位延时函数
	
    CLK_SysTickDelay(50000);     //HW 32K clk count 15ms, but need to consider 32K deviation & MCU HIRC deviation

    /* SPI IO remapping */
    RF_SpiIoMapping();

    /* initial SPI PDMA */
    SPI_PDMA_Init();

    /* Initialize RF PHY */
    RF_Init();                   //EnableGpioInterrupt in the end of this function
}


/*!
   \brief main loop for initialization and BLE kernel
*/
int main(void)
{
    extern BleStackStatus Ble_Kernel_Root(void);
    extern void BleApp_Main(void);
    extern void BleApp_Init(void);

    /* Init System, IP clock and multi-function I/O. */
    SYS_Init();

    /* Enable the BLE RF PHY */
    RF_Open();

    /* Open UART0 for debug */

    UART_Open(UART0, 115200);
    printf("-------------------\n");
    printf("  BLE Start.....\n");
    printf("-------------------\n");

    printf("Chip_ID=0x%x\n",ChipId_Get());


    BleApp_Main();

    while(1)
    {

    }
}
#pragma pop
