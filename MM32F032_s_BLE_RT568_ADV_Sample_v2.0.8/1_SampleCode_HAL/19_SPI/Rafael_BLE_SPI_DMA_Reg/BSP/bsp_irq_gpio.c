/******************************************************************************
 * @file    bsp_irq_gpio.c
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
#define __bsp_irq_gpio_C__


/* Includes -----------------------------------------------------------------*/
#include "bsp_irq_gpio.h"


/* Private typedef ----------------------------------------------------------*/
/* Private define -----------------------------------------------------------*/
/* Private macro ------------------------------------------------------------*/
/* Private variables --------------------------------------------------------*/
/* Private function prototypes ----------------------------------------------*/
/* Private functions --------------------------------------------------------*/


/* Exported variables -------------------------------------------------------*/
/* Exported function prototypes ---------------------------------------------*/

/******************************************************************************
 * @brief    Assign GPIO INT pin
 * @param
 * @retval
 * @attention
******************************************************************************/
void MCU_GpioPinInit(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    /* RF_IRQ_CLOCK enable */
    RCC_AHBPeriphClockCmd(RF_IRQ_CLOCK, ENABLE);
    /*Be careful not to set 5 pins as HIGH, it will trigger HW mapping SPI pin*/
    GPIO_StructInit(&GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin   = RF_IRQ_PIN ;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_FLOATING ;//GPIO_Mode_IPD ; //GPIO_Mode_FLOATING
    GPIO_Init(RF_IRQ_PORT, &GPIO_InitStructure);
}

void MCU_GpioIntEnable(void)
{
    NVIC_InitTypeDef  NVIC_InitStructure;
    EXTI_InitTypeDef  EXTI_InitStructure;

    // Configure PB8 as Input mode and enable interrupt by rising edge trigger
    MCU_GpioPinInit();
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
    //IRQ - pb8
    SYSCFG_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource8);

    EXTI_InitStructure.EXTI_Line = EXTI_Line8;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);

    NVIC_InitStructure.NVIC_IRQChannel = EXTI4_15_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}


void MCU_GpioIntDisable(void)
{
    NVIC_InitTypeDef  NVIC_InitStructure;
    EXTI_InitTypeDef  EXTI_InitStructure;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
    //IRQ - pb8
    SYSCFG_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource8);

    EXTI_InitStructure.EXTI_Line = EXTI_Line8;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
    EXTI_InitStructure.EXTI_LineCmd = DISABLE;
    EXTI_Init(&EXTI_InitStructure);

    NVIC_InitStructure.NVIC_IRQChannel = EXTI4_15_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = DISABLE;
    NVIC_Init(&NVIC_InitStructure);
}


/* GPIO Interrupt Handler */

void EXTI4_15_IRQHandler(void)
{
    extern void LL_GPIO_Isr(void);
    extern void delay_us(u16 us);

    if (EXTI_GetITStatus(EXTI_Line8) != RESET)
    {
        EXTI_ClearITPendingBit(EXTI_Line8);  // Key modification!!! Put this here
        while (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_8) == 1)
        {
            LL_GPIO_Isr();      // Put this on the top following the clearing MCU GPIO Int status process.
            Tiny_Delay(1);
        }
        //EXTI_ClearITPendingBit(EXTI_Line8);
    }

//    LL_GPIO_Isr();      // Put this on the top following the clearing MCU GPIO Int status process.
}

/******************* (C) COPYRIGHT 2020 ************************END OF FILE***/

