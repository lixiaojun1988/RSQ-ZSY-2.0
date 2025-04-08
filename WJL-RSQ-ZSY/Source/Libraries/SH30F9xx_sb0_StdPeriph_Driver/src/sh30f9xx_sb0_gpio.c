/**
  ******************************************************************************
  * @file    sh30f9xx_sb0_gpio.c
  * @author  sinowealth
  * @version V1.0.0
  * @date    2024-5-21  
  * @brief   This file provides firmware functions to manage the GPIO module 
  *  
    @verbatim
  ==============================================================================
                         How to use this driver
  ==============================================================================
 [..]
   (#) Enable the GPIO clock using the following function : RCC_AHBPeriphClockOnOff().

   (#) Configure the GPIO pin(s) using GPIO_StructInit().
       (++) Configure the IO mode using "Mode" member from GPIO_InitTypeDef structure
       (++) Activate Pull-up resistor using "Pull" member from GPIO_InitTypeDef
            structure.
       (++) In case of Output or alternate function mode selection: the driving ability is
            configured through "GPIO_ODrv" member from GPIO_InitTypeDef structure.

   (#) To get the level of a pin configured in input mode use GPIO_ReadInputDataBit().

   (#) To set/reset the level of a pin configured in output mode use
       GPIO_SetBits()/GPIO_ResetBits().

   (#) To lock pin configuration until next reset use GPIO_PinLock().

   (#) call GPIO_PinAFConfig() function for pin use as GPIO or alternate function
   
   (#) call GPIO_Reset() function for reset pin configuration
    @endverbatim
  ******************************************************************************
  * @attention
  *
  * SINOWEALTH IS SUPPLYING THIS SOFTWARE FOR USE EXCLUSIVELY SH_ON SINOWEALTH'S 
  * MICROCONTROLLER PRODUCTS. IT IS PROTECTED UNDER APPLICABLE COPYRIGHT LAWS. 
  * THIS SOFTWARE IS FOR GUIDANCE IN ORDER TO SAVE TIME. AS A RESULT, SINOWEALTH 
  * SHALL NOT BE HELD LIABLE FOR ANY DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES 
  * WITH RESPECT TO ANY CLAIMS ARISING FROM THE CONTENT OF SUCH FIRMWARE AND/OR
  * THE USE MADE BY CUSTOMERS OF THE CODING INFORMATION CONTAINED HEREIN IN 
  * CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2024 Sinowealth</center></h2>
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "sh30f9xx_sb0_gpio.h"
/** @addtogroup SH30F9XX_SB0_STANDARD_LIB
  * @{
  */

/** @defgroup GPIO_MODULE GPIO 
  * @brief SH30F9XX SB0 GPIO module driver library,reference this topic macro and function for drive GPIO
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/


/** @defgroup GPIO_Exported_Functions GPIO Exported Functions
  *
  * @brief    This is GPIO  exported function code group
  * @{
  */

/**
  * @brief  Reset the GPIOx peripheral registers to their default reset values.
  * @note   By default,the GPIO Pins are configured in input floating mode(except JTAG pins).
  * @param  GPIOx: Where x can be A..D to select the GPIO peripheral.
  * @retval None
  */
void GPIO_Reset(GPIO_TypeDef *GPIOx)
{
    /* Check the parameters */
    assert_param(IS_GPIO_ALL_PERIPH(GPIOx));

    GPIOx->LCKR.V32   = 0x5AA50000;
    GPIOx->MODER.V32  = (uint32_t)0x00;
    GPIOx->AFRH.V32   = (uint32_t)0x00;
    GPIOx->AFRL.V32   = (uint32_t)0x00;
    GPIOx->PUPDR.V32  = (uint32_t)0x00;
    GPIOx->OTYPER.V32 = (uint32_t)0x00;
    GPIOx->ODRVR.V32  = (uint32_t)0x00;
    GPIOx->TTLEN.V32  = (uint32_t)0x00;
    GPIOx->ODR = (uint32_t)0x00;
}

/**
  * @brief  Initialize the GPIOx peripheral according to the specified parameters in
  *         the GPIO_InitStruct.
  * @param  GPIOx: Where x can be A..D to select the GPIO peripheral.
  * @param  GPIO_InitStruct: Pointer to a @ref GPIO_InitTypeDef structure that contains
  *         the configuration information for the specified GPIO peripheral.
  * @retval None
  */
void GPIO_Init(GPIO_TypeDef *GPIOx, GPIO_InitTypeDef *GPIO_InitStruct)
{
    uint32_t pinPos = 0x00, pos = 0x00, currentPin = 0x00;

    /* Check the parameters */
    assert_param(IS_GPIO_ALL_PERIPH(GPIOx));
    assert_param(IS_GPIO_PIN(GPIO_InitStruct->GPIO_Pin));
    assert_param(IS_GPIO_MODE(GPIO_InitStruct->GPIO_Mode));
    assert_param(IS_GPIO_PUPD(GPIO_InitStruct->GPIO_PuPd));

    /* Configure the port pins */
    while ((GPIO_InitStruct->GPIO_Pin >> pinPos) != 0x00)
    {
        pos = ((uint32_t)0x01) << pinPos;
        /* Get the port pins position */
        currentPin = (GPIO_InitStruct->GPIO_Pin) & pos;

        if (currentPin == pos)
        {
            GPIOx->MODER.V32 &= ~((uint16_t)GPIO_MODER_MODER0_Msk << (pinPos));
            GPIOx->MODER.V32 |= (((uint32_t)GPIO_InitStruct->GPIO_Mode) << pinPos);

            if (GPIO_InitStruct->GPIO_Mode == GPIO_Mode_OUT)
            {
                /* Check the output driver parameters */
                assert_param(IS_GPIO_ODRV(GPIO_InitStruct->GPIO_ODrv));

                /*Output driver configuration */
                GPIOx->ODRVR.V32 &= ~((uint32_t)GPIO_ODRVR_ODRVR_SINK_Pos << (pinPos * 2));
                GPIOx->ODRVR.V32 |= ((uint32_t)(GPIO_InitStruct->GPIO_ODrv) << (pinPos * 2));

                /* Output type parameter check */
                assert_param(IS_GPIO_OTYPE(GPIO_InitStruct->GPIO_OType));

                /* Output type configuration */
                GPIOx->OTYPER.V32 &= ~((uint16_t)GPIO_OTYPER_OT_Msk << pinPos);
                GPIOx->OTYPER.V32 |= (uint16_t)(((uint16_t)GPIO_InitStruct->GPIO_OType) << pinPos);
            }

            /* Pull-up Pull-down configuration   GPIO_CFG_PUPDR_PHDR0_Msk  */
            GPIOx->PUPDR.V32 &= ~((uint32_t)GPIO_PUPDR_PHDR0_Msk << (pinPos * 2));
            GPIOx->PUPDR.V32 |= (((uint32_t)GPIO_InitStruct->GPIO_PuPd) << (pinPos * 2));
        }

        pinPos++;
    }
}

/**
  * @brief  Fills each GPIO_InitStruct member with its default value.
  * @param  GPIO_InitStruct : pointer to a @ref GPIO_InitTypeDef structure which will be initialized.
  * @retval None
  */
void GPIO_StructInit(GPIO_InitTypeDef *GPIO_InitStruct)
{
    /* Reset GPIO init structure parameters values */
    GPIO_InitStruct->GPIO_Pin   = GPIO_Pin_All;
    GPIO_InitStruct->GPIO_Mode  = GPIO_Mode_IN;
    GPIO_InitStruct->GPIO_ODrv  = GPIO_ODrv_NORMAL;
    GPIO_InitStruct->GPIO_OType = GPIO_OType_PP;
    GPIO_InitStruct->GPIO_PuPd  = GPIO_PuPd_NOPULL;
}

/**
  * @brief  Select whether CMOS or TTL level for GPIO Pins.
  * @param  GPIOx: Where x can be A..D to select the GPIO peripheral.
  *         But only GPIOA and GPIOB are available.
  * @param  GPIO_Pin: Specifie the port bit to be set TTL or CMOS.
  *         This parameter can be any combination of GPIO_Pin_x where x can be 0 to 15.      
  * @param  GPIO_BitLevel: Specifie the level for selected GPIO Pins.
  *          This parameter can be one of the following values:
  *           @arg @b GPIO_BitLevel_CMOS
  *           @arg @b GPIO_BitLevel_TTL
  * @retval None
  */
void GPIO_PinTTLConfig(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin, uint8_t GPIO_BitLevel)
{
    /* Check the parameters */
    assert_param(IS_GPIO_ALL_PERIPH(GPIOx));
    assert_param(IS_GPIO_PIN(GPIO_Pin));
    assert_param(IS_GPIO_BIT_LEVEL(GPIO_BitLevel));

    if (GPIO_BitLevel != GPIO_BitLevel_CMOS)
    {
        GPIOx->TTLEN.V32 |= GPIO_Pin;
    }
    else
    {
        GPIOx->TTLEN.V32 &= ~GPIO_Pin;
    }
}

/**
  * @brief  Lock GPIO pins configuration registers.
  * @param  GPIOx: Where x can be A..D to select the GPIO peripheral.
  * @param  GPIO_Pin: Specifie the port bit to be locked.
  *          This parameter can be any combination of GPIO_Pin_x where x can be 0 to 15.
  * @param  GPIO_BitLock: Lock or unlock specified port pins.
  *          This parameter can be one of the following values:
  *            @arg @b GPIO_BitLock_Disable
  *            @arg @b GPIO_BitLock_Enable
  * @retval None
  */
void GPIO_PinLock(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin, uint8_t GPIO_BitLock)
{
    uint32_t temp = 0x00;

    /* Check the parameters */
    assert_param(IS_GPIO_ALL_PERIPH(GPIOx));
    assert_param(IS_GPIO_PIN(GPIO_Pin));
    assert_param(IS_GPIO_BIT_LOCK(GPIO_BitLock));

    temp = GPIOx->LCKR.V32;

    if (GPIO_BitLock != GPIO_BitLock_Disable)
    {
        temp |= GPIO_Pin;
    }
    else
    {
        temp &= ~GPIO_Pin;
    }

    GPIOx->LCKR.V32 = ((uint32_t)0x5AA5 << 16) | temp;
}




/**
  * @brief  Read the specified input port pin.
  * @param  GPIOx: Where x can be A..D to select the GPIO peripheral.
  * @param  GPIO_Pin: specifie the port pin to read.
  *          This parameter can be one of GPIO_Pin_x where x can be 0 to 15.
  * @retval The input port pin value.
  */
uint8_t GPIO_ReadInputDataBit(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin)
{
    uint8_t bitstatus = 0x00;

    /* Check the parameters */
    assert_param(IS_GPIO_ALL_PERIPH(GPIOx));
    assert_param(IS_GET_GPIO_PIN(GPIO_Pin));

    if ((GPIOx->IDR & GPIO_Pin) != (uint32_t)Bit_RESET)
    {
        bitstatus = (uint8_t)Bit_SET;
    }
    else
    {
        bitstatus = (uint8_t)Bit_RESET;
    }
    return bitstatus;
}

/**
  * @brief  Reads the specified GPIO input data port.
  * @param  GPIOx: where x can be A..D to select the GPIO peripheral.
  * @retval GPIO input data port value.
  */
uint16_t GPIO_ReadInputData(GPIO_TypeDef *GPIOx)
{
    /* Check the parameters */
    assert_param(IS_GPIO_ALL_PERIPH(GPIOx));

    return ((uint16_t)GPIOx->IDR);
}

/**
  * @brief  Read the specified output data port bit.
  * @param  GPIOx: where x can be A..D to select the GPIO peripheral.
  * @param  GPIO_Pin: specifies the port bit to read.
  *          This parameter can be one of GPIO_Pin_x where x can be 0 to 15.
  * @retval The output port pin value.
  */
uint8_t GPIO_ReadOutputDataBit(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin)
{
    uint8_t bitstatus = 0x00;

    /* Check the parameters */
    assert_param(IS_GPIO_ALL_PERIPH(GPIOx));
    assert_param(IS_GET_GPIO_PIN(GPIO_Pin));

    if ((GPIOx->ODR & GPIO_Pin) != (uint32_t)Bit_RESET)
    {
        bitstatus = (uint8_t)Bit_SET;
    }
    else
    {
        bitstatus = (uint8_t)Bit_RESET;
    }
    return bitstatus;
}

/**
  * @brief  Read the specified GPIO output data port.
  * @param  GPIOx: where x can be A..D to select the GPIO peripheral.
  * @retval GPIO output data port value.
  */
uint16_t GPIO_ReadOutputData(GPIO_TypeDef *GPIOx)
{
    /* Check the parameters */
    assert_param(IS_GPIO_ALL_PERIPH(GPIOx));

    return ((uint16_t)GPIOx->ODR);
}

/**
  * @brief  Set the selected data port bits.
  * @note   This functions uses GPIOx_BSRR register to allow atomic read/modify 
  *         accesses. In this way, there is no risk of an IRQ occurring between
  *         the read and the modify access.
  * @param  GPIOx: where x can be A..D to select the GPIO peripheral.
  * @param  GPIO_Pin: specifie the port bits to be written.
  *          This parameter can be any combination of GPIO_Pin_x where x can be 0 to 15.
  * @retval None
  */
void GPIO_SetBits(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin)
{
    /* Check the parameters */
    assert_param(IS_GPIO_ALL_PERIPH(GPIOx));
    assert_param(IS_GPIO_PIN(GPIO_Pin));

    GPIOx->BSRR.V32 = GPIO_Pin;
}

/**
  * @brief  Reset the selected data port bits.
  * @param  GPIOx: where x can be A..D to select the GPIO peripheral.
  * @param  GPIO_Pin: specifies the port bits to be written.
  *          This parameter can be any combination of GPIO_Pin_x where x can be 0 to 15.
  * @retval None
  */
void GPIO_ResetBits(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin)
{
    /* Check the parameters */
    assert_param(IS_GPIO_ALL_PERIPH(GPIOx));
    assert_param(IS_GPIO_PIN(GPIO_Pin));

    GPIOx->BSRR.V32 = (uint32_t)GPIO_Pin << 16;
}

/**
  * @brief  Set or clear the selected data port bit.
  * @param  GPIOx: where x can be A..D to select the GPIO peripheral.
  * @param  GPIO_Pin: specifie the port bit to be written.
  *          This parameter can be one of GPIO_Pin_x where x can be 0 to 15.
  * @param  BitVal: specifie the value to be written to the selected bit.
  *          This parameter can be one of the BitAction enum values:
  *            @arg @b Bit_RESET: to clear the port pin
  *            @arg @b Bit_SET: to set the port pin
  * @retval None
  */
void GPIO_WriteBit(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin, BitAction BitVal)
{
    /* Check the parameters */
    assert_param(IS_GPIO_ALL_PERIPH(GPIOx));
    assert_param(IS_GET_GPIO_PIN(GPIO_Pin));
    assert_param(IS_GPIO_BIT_ACTION(BitVal));

    if (BitVal != Bit_RESET)
    {
        GPIOx->BSRR.V32 = GPIO_Pin;
    }
    else
    {
        GPIOx->BSRR.V32 = (uint32_t)GPIO_Pin << 16;
    }
}

/**
  * @brief  Write data to selected GPIO data port. This function is used for debugging. 
  *          It is not recommended for actual projects. Other functions can be used instead.
  * @param  GPIOx: where x can be A..D to select the GPIO peripheral.
  * @param  PortVal: specifie the value to be written to the port output data register.
  * @retval None
  */
void GPIO_Write(GPIO_TypeDef *GPIOx, uint16_t PortVal)
{
    /* Check the parameters */
    assert_param(IS_GPIO_ALL_PERIPH(GPIOx));

    GPIOx->ODR = PortVal;
}

/**
  * @brief  Toggle the specified GPIO pins.
  * @param  GPIOx: where x can be A..D to select the GPIO peripheral.
  * @param  GPIO_Pin: specifie the pins to be toggled.
  *          This parameter can be any combination of GPIO_Pin_x where x can be 0 to 15.
  * @retval None
  */
void GPIO_ToggleBits(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin)
{
    /* Check the parameters */
    assert_param(IS_GPIO_ALL_PERIPH(GPIOx));

    if(GPIO_ReadOutputDataBit(GPIOx, GPIO_Pin) != Bit_RESET)
    {
        GPIOx->BSRR.V32 = (uint32_t)GPIO_Pin << 16;
    }
    else
    {
        GPIOx->BSRR.V32 = GPIO_Pin;
    }
}



/**
  * @brief  Changes the mapping of the specified pin.
  * @param  GPIOx: where x can be A..D to select the GPIO peripheral.
  * @param  GPIO_Pin: specifies the pin for the alternate function.
  *         This parameter can be one of GPIO_Pin_x where x can be (0..15).
  * @param  GPIO_AF: selects the pin to used as Alternate function.
  *         This parameter can be one of the following values:
  *            @arg @b parameter of GPIO_AF_Type.
  * @retval None
  */
void GPIO_PinAFConfig(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin, GPIO_AF_TypeDef GPIO_AF)
{
    uint8_t pinPos = 0;
    uint16_t currentPin = 0;

    /* Check the parameters */
    assert_param(IS_GPIO_ALL_PERIPH(GPIOx));
    assert_param(IS_GPIO_PIN(GPIO_Pin));
    assert_param(IS_GPIO_AF(GPIO_AF));
    /* Configure the port pins */
    while ((GPIO_Pin >> pinPos) != 0x00)
    {
        /* Get the current GPIO Pin position */
        currentPin = (uint16_t)0x01 << pinPos;
        if ((GPIO_Pin & currentPin) == currentPin)
        {
            if (pinPos > 0x07)
            {
                GPIOx->AFRH.V32 &= (~((uint32_t)0x0F << ((pinPos & 0x07) * 4)));
                GPIOx->AFRH.V32 |= ((uint32_t)GPIO_AF << ((pinPos & 0x07) * 4));
            }
            else
            {
                GPIOx->AFRL.V32 &= (~((uint32_t)0x0F << (pinPos * 4)));
                GPIOx->AFRL.V32 |= ((uint32_t)GPIO_AF << (pinPos * 4));
            }
        }
        pinPos++;
    }
}
/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT Sinowealth *****END OF FILE****/
