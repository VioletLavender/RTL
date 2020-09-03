/******************************************************************************
 * @file    main.c
 * @author  MM32 AE Team
 * @version V2.2
 * @date    13-August-2020
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
#define __MAIN_C__


/* Includes -----------------------------------------------------------------*/
#include "main.h"
#include "rf_phy.h"
#include "ble_cmd.h"
#include "hci.h"
#include "LL.h"
#include "_rafael_phy.h"

/* Includes -----------------------------------------------------------------*/
#include "systick.h"
#include "uart.h"
#include "spi.h"

/* Includes -----------------------------------------------------------------*/
#include "bsp_irq_gpio.h"
#include "bsp_spi_ble.h"


/* Private typedef ----------------------------------------------------------*/
/* Private define -----------------------------------------------------------*/
/* Private macro ------------------------------------------------------------*/
/* Private variables --------------------------------------------------------*/

uint8_t HCI_SET_ADV_PARA[] = //HCI command (raw data)
{
    0x01, 0x06, 0x20, 0x0F,
    0xA0, 0x00, 0xA0, 0x00,
    0x00, 0x00, 0x00, 0x66,
    0x55, 0x44, 0x33, 0x22,
    0x11, 0x07, 0x00,
};

uint8_t HCI_SET_ADV_DATA[] = //HCI command (raw data)
{
    0x01, 0x08, 0x20, 0x0D,
	
    0x0C, ////scan data start for this(RAW DATA 1)
	
	0x0B, 
	
	0x02, 
	
	
	0x01,
    0x05, 
	
	0x03, 
	
	0x02, 
	
	0x12,
    0x18, 
	
	0x03, 
	
	0x19, 
	
	0x40,
    0x01,
};

uint8_t HCI_SET_SCN_DATA[] = //HCI command (raw data)
{
    0x01, 0x09, 0x20, 0x0F,
	
  0x0E, //scan data start for this(RAW DATA 2)
	
	0x0D, 0x09,
  
	0x52,
	0x61,
	0x66,
	0x61,
	0x65,
	0x6C,   //"Rafael"
  0x2D,
	0x37,
	0x32,
	0x36,
	0x38,
	0x39,   //"_72689"
	0x39,
	0x30

};

uint8_t HCI_SET_ADV_EN[] = //HCI command (raw data)
{
    0x01, 0x0A, 0x20, 0x01,
    0x01,
};

/* Private function prototypes ----------------------------------------------*/
/* Private functions --------------------------------------------------------*/

#if defined(ewarm) || defined(DOXYGEN)
void SysCtlDelay(uint32_t ui32Count)
{
    __asm("    subs    r0, #1\n"
          "    bne.n   SysCtlDelay\n"
          "    bx      lr");
}
#endif

#if defined(codered) || defined(gcc) || defined(sourcerygxx)
void __attribute__((naked))
SysCtlDelay(uint32_t ui32Count)
{
    __asm("    subs    r0, #1\n"
          "    bne     SysCtlDelay\n"
          "    bx      lr");
}
#endif


extern uint32_t SystemCoreClock ;



#if defined(rvmdk) || defined(__ARMCC_VERSION)
__asm void
SysCtlDelay(uint32_t ui32Count)
{
    subs    r0, #1;
    bne     SysCtlDelay;
    bx      lr;
}
#endif


void delay_us(u16 us)
{
    u32 xus = us;
    SysCtlDelay(xus * (SystemCoreClock / 4000000));
}

void delay_ms(u16 ms)
{
    u32 xms = ms;
    SysCtlDelay(xms * (1000 * (SystemCoreClock / 4000000)));
}

void BleApp_Main(void)
{
    extern uint8_t Ch_ADV_Ch_Hop_Table[4];
    extern LL_Adv LL_Adv_Para;
    extern uint8_t LL_Msg_AdvScnConn;

    Knl_CodeCpy((uint8_t *)&LL_Adv_Para.Adv_Para.LL_Adv_Interval_Min, (uint8_t *)&HCI_SET_ADV_PARA[4], LEN_HCLL_LE_SET_ADV_PARAM_PARA);
    LL_Adv_Para.Adv_Para.LL_Adv_Type = LE_ADV_TYPE_ADV_SCAN_IND;
    LL_Adv_Para.Adv_Para.LL_Adv_Interval_Min = (LL_Adv_Para.Adv_Para.LL_Adv_Interval_Min * 5);
    Ch_ADV_Ch_Hop_Table[0] = 1;
    Ch_ADV_Ch_Hop_Table[1] = 2;
    Ch_ADV_Ch_Hop_Table[2] = 3;
    Ch_ADV_Ch_Hop_Table[3] = 0;
//HCI_SET_ADV_DATA[5]=sizeof(HCI_SET_ADV_DATA)-6;
	
    LL_Adv_Para.Adv_Para.LL_Adv_Data_Length = sizeof(HCI_SET_ADV_DATA)+1;
	
    Knl_CodeCpy(LL_Adv_Para.Adv_Para.LL_Adv_Data, &HCI_SET_ADV_DATA[5], LL_Adv_Para.Adv_Para.LL_Adv_Data_Length);
	
		HCI_SET_SCN_DATA[5]=(sizeof(HCI_SET_SCN_DATA)-6);//cp
	
    LL_Adv_Para.Adv_Para.LL_ScanRsp_Data_Length = HCI_SET_SCN_DATA[5]+3;
		
    Knl_CodeCpy(LL_Adv_Para.Adv_Para.LL_ScanRsp_Data, &HCI_SET_SCN_DATA[5], LL_Adv_Para.Adv_Para.LL_ScanRsp_Data_Length);//used to set scan data

    InterruptDisable();
		
    LL_Msg_AdvScnConn |= 0x80;
		
    LL_Adv_Para.Adv_Para.LL_AdvMap_ID = Ch_ADV_Ch_Hop_Table[0];
		
    LL_Adv_Para.Adv_Para.LL_Adv_Interval_Max = 0;
    LL_Adv_Para.Adv_Para.LL_AdvConn_ID = 4;
    InterruptEnable();
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

    delay_us(1);

    //Address 1th byte
    SPIData = regAddr & 0x7F;
    for (SPICount = 0; SPICount < 8; SPICount++)          // Prepare to clock out the Address byte
    {
        if (SPIData & 0x80)                                 // Check for a 1
            SPI_MOSI = 1;                                     // and set the MOSI line appropriately
        else
            SPI_MOSI = 0;

        delay_us(1);                                    // delay half clk cycle
        SPI_CK = 1;                                         // Toggle the clock line
        delay_us(1);
        SPI_CK = 0;
        SPIData <<= 1;                                      // Rotate to get the next bit
    }                                                     // and loop back to send the next bit
    // Repeat for the Data byte
    //Address 2nd byte
    SPIData = (regAddr & 0x80) >> 7;
    for (SPICount = 0; SPICount < 8; SPICount++)          // Prepare to clock out the Address byte
    {
        if (SPIData & 0x80)                                 // Check for a 1
            SPI_MOSI = 1;                                     // and set the MOSI line appropriately
        else
            SPI_MOSI = 0;

        delay_us(1);                                     // delay half clk cycle
        SPI_CK = 1;                                         // Toggle the clock line
        delay_us(1);
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

        delay_us(1);
        SPI_CK = 1;
        delay_us(1);
        SPI_CK = 0;
        SPIData <<= 1;
    }

    delay_us(1);

    SPI_CS = 1;
    SPI_MOSI = 0;
}

/******************************************************************************
 * @brief    SPI IO mapping. Must do this after Power ON
 * @param
 * @retval
 * @attention
******************************************************************************/
void SPI_GPIO_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    /* GPIOB Peripheral clock enable */
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB | RF_IRQ_CLOCK, ENABLE);
    /*Be careful not to set 5 pins as HIGH, it will trigger HW mapping SPI pin*/
    GPIO_StructInit(&GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin   = RF_IRQ_PIN | GPIO_Pin_12 | GPIO_Pin_13 |  \
                                    GPIO_Pin_14 | GPIO_Pin_15;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    GPIO_Init(RF_IRQ_PORT, &GPIO_InitStructure);
}

/******************************************************************************
 * @brief
 * @param
 * @retval
 * @attention  SPI IO mapping. Must do this after Power ON
******************************************************************************/
void RF_SpiIoMapping(void)
{
    /*
     *------------------------------------------------
     *          NSS     SCK     MISO    MOSI   IRQ  AF
     *------------------------------------------------
     *   SPI2   PB12    PB13    PB14    PB15   PB8  AF0
     *------------------------------------------------
     */
    /*(1) Here set all CLK,CS,MOSI,NISO,INT as output */
    SPI_GPIO_Init();
    SPI_CS = 1;
    SPI_CK = 0;
    SPI_MOSI = 0;
    SPI_MISO = 0;
    DEFAULT_INT = 0;

    //(2) Write R248, R249 GPIO select
    //Write R248=8'b10,001,000, R249=8'b0,100,011,0
    //GPIO0[2:0]=0 - CS
    //GPIO1[2:0]=1 - CLK
    //GPIO2[2:0]=2 - MOSI
    //GPIO3[2:0]=3 - MISO
    //GPIO4[2:0]=4 - INT
    delay_us(25);
    spiGpioWriteReg(248, 0x88);
    delay_us(25);
    spiGpioWriteReg(249, 0x46);
    delay_us(25);

    /* (3) Output all pin as HIGH, last 10ms(>1ms). trigger HW take effect */
    SPI_GPIO_Init();
    SPI_CS = 1;
    SPI_CK = 1;
    SPI_MOSI = 1;
    SPI_MISO = 1;
    DEFAULT_INT = 1;
    SysTick_DelayMs(10);  //Delay 10ms

    //(4) Init GPIO & SPI
    MCU_GpioPinInit();        //Set GPIO interrupt pin as input
    /* Init SPI_BLE : SPI master, clk=12M, mode 0, 8-bit, MSB first  */
    BSP_SPI_BLE_Init();

    SPI_1BYT_SetTx(249, (0x46 | 0x80));  //set RF MISO, INT as output

    SPI_1BYT_SetTx(53, 0xC0); //To gurantee DC/DC power on when set R40=0x90

    //enable LDO
    SPI_1BYT_SetTx(40, 0xC0);// 截取正常波形中是 0x90 而不是 0xc0
    SysTick_DelayMs(25);        //Put delay after LDO_enable, or set register may have strange behavior!

    //enable chip
    SPI_1BYT_SetTx(53, 0x80); //Enable chip
    SysTick_DelayMs(10);   //Put delay after chip_enable, or set register may have strange behavior!
}

/******************************************************************************
 * @brief
 * @param
 * @retval
 * @attention
******************************************************************************/
static void RF_Open()
{    
    GPIO_InitTypeDef GPIO_InitStructure;

    /* Wait RF PHY stable ,delay 25ms */
    SysTick_DelayMs(25);

    /* RF_RESET_CLOCK  enable */
    RCC_AHBPeriphClockCmd(RF_RESET_CLOCK, ENABLE);
    /* Configure RF_RESET_PIN in output pushpull mode */
    GPIO_StructInit(&GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin   = RF_RESET_PIN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;
    GPIO_Init(RF_RESET_PORT, &GPIO_InitStructure);

    /* Reset RF by using RF_RESET_PIN */
    GPIO_SetBits(RF_RESET_PORT, RF_RESET_PIN);
    SysTick_DelayMs(1);
    GPIO_ResetBits(RF_RESET_PORT, RF_RESET_PIN);
    SysTick_DelayMs(1);
    GPIO_SetBits(RF_RESET_PORT, RF_RESET_PIN);


    SysTick_DelayMs(50);

    /* SPI IO remapping */
    RF_SpiIoMapping();

    /* Initialize RF PHY */
    RF_Init();       /* EnableGpioInterrupt in the end of this function */
}

/* Exported variables -------------------------------------------------------*/
/* Exported function prototypes ---------------------------------------------*/


/******************************************************************************
 * @brief
 * @param
 * @retval
 * @attention
******************************************************************************/
int main(void)
{
    /* Init System Clock 48M , HCLK2 = 24M SPICLK = 12M */
    /* Init SysTick timer 1ms for SysTick_DelayMs */
    SysTick_Init(1000);

    /* Config UART1 with parameter(115200, N, 8, 1) for printf */
    UARTx_Configure(DEBUG_UART, 115200, UART_WordLength_8b, UART_StopBits_1,  \
                    UART_Parity_No);

    /* Enable the BLE RF PHY */
    RF_Open();

    printf("\r\nRafael_BLE_MM32 Demo run. Now is %s %s\r\n", __DATE__, __TIME__);
    printf("-------------------\r\n");
    printf("  BLE Start.....\r\n");
    printf("-------------------\r\n");

    printf("Chip_ID=0x%x\r\n", SPI_1BYT_SetRx(RFIP_REG_0));

    /* SPI BLE Demo run */
    BleApp_Main();

    while (1)
    {

    }
}


/******************* (C) COPYRIGHT 2020 ************************END OF FILE***/

