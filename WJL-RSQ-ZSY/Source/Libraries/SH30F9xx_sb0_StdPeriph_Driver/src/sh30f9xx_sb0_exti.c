/**
  ******************************************************************************
  * @file    sh30f9xx_sb0_exti.c
  * @author  sinowealth
  * @version V1.0.0
  * @date    2024-5-24  
  * @brief   This file provides firmware functions to manage the EXTI module 
  *  
    @verbatim
  ==============================================================================
                         How to use this driver
  ==============================================================================
   [..]
     (#) Enable EXTI Clock use RCC_APB0PeriphClockOnOff() function 
    
     (#) Fill EXTI Init Structure member as default use EXTI_StructInit() function
     
     (#) Config EXTI Pin
         (++) Enable Pin Clock calling RCC_AHBPeriphClockOnOff() function 
         
         (++) Call GPIO_Init()function for config EXTI pin(input mode, pullup),if not use all member for EXTI 
              pin config ,makesure call GPIO_StructInit() function for GPIO_Init function  paramter set default value

         (++) Call GPIO_PinAFConfig() for config pin as GPIO function

     (#) Config EXTI interrupt mode, trigger level or edge, clock div etc use EXTI_Init() function

     (#) Use EXTI interrupt by following step

         (++) Enable Kernel EXTI's Global interrupt by calling NVIC_EnableIRQ() function
         
         (++) reference startup asm file for EXTI interrupt function name and rewrite this function 	
      
     (#) When EXTI interrupt happen, use EXTI_GetPendingFlagStatus() function to get EXTI interrupt flag

     (#) Call EXTI_ClearPendingFlag() function for clear EXTI interrupt flag
   
  *  @endverbatim
  *
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
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "sh30f9xx_sb0_exti.h"

/** @addtogroup SH30F9XX_SB0_STANDARD_LIB
  * @{
  */

/** @defgroup EXTI_MODULE EXTI
  * @brief SH30F9XX SB0 EXTI module driver library,reference this topic macro and function for drive EXTI
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/* Export function prototypes -----------------------------------------------*/

/** @defgroup EXTI_Exported_Functions EXTI Exported Functions
 *  @brief    This is EXTI exported function code group
 *
  * @{
  */
  
/**
  * @brief  Reset the EXTI peripheral registers to their default reset values.
  * @retval None
  */
void EXTI_Reset(void)
{ 
    RCC_APB0PeriphReset(RCC_APB0_EXTI);
}

/**
  * @brief  Initializes the EXTI peripheral according to the specified
  *         parameters in the EXTI_InitStruct.
  * @param  EXTI_InitStruct: pointer to a @ref EXTI_InitTypeDef structure
  *         that contains the configuration information for the EXTI peripheral.
  * @retval None
  */
void EXTI_Init(EXTI_InitTypeDef *EXTI_InitStruct)
{
    uint8_t pinPosition = 0;
    uint32_t EXTI_Linex = 0;
    uint32_t tempAddress = 0;

    /* Check the parameters */
    assert_param(IS_EXTI_LINE(EXTI_InitStruct->EXTI_Line));
    assert_param(IS_EXTI_MODE(EXTI_InitStruct->EXTI_Mode));
    assert_param(IS_EXTI_TRIGGER(EXTI_InitStruct->EXTI_Trigger));
    assert_param(IS_EXTI_SAMPLE_CYCLE(EXTI_InitStruct->EXTI_SampleCycle));
    assert_param(IS_EXTI_SAMPLE_CLOCK_DIV(EXTI_InitStruct->EXTI_SampleClockDivision));

    tempAddress = (uint32_t)EXTI_BASE;

    if (EXTI_InitStruct->EXTI_LineCmd != SH_DISABLE)
    {
        /* clear select exti line enable bit */
        EXTI->IMR &= (~EXTI_InitStruct->EXTI_Line);
        EXTI->EMR &= (~EXTI_InitStruct->EXTI_Line);

        tempAddress += EXTI_InitStruct->EXTI_Mode;
        *(__IO uint32_t *)tempAddress |= EXTI_InitStruct->EXTI_Line;

        EXTI->TMSR &= (~EXTI_InitStruct->EXTI_Line);
        EXTI->RTSR &= (~EXTI_InitStruct->EXTI_Line);
        EXTI->FTSR &= (~EXTI_InitStruct->EXTI_Line);

        if (EXTI_InitStruct->EXTI_Trigger >= EXTI_Trigger_HighLevel)
        {
            EXTI->TMSR |= EXTI_InitStruct->EXTI_Line;
        }
        else
        {
            EXTI->TMSR &= (~EXTI_InitStruct->EXTI_Line);
        }

        if ((EXTI_InitStruct->EXTI_Trigger == EXTI_Trigger_Rising_Falling) \
           || (EXTI_InitStruct->EXTI_Trigger == EXTI_Trigger_HighLevel_LowLevel))
        {
            EXTI->RTSR |= EXTI_InitStruct->EXTI_Line;
            EXTI->FTSR |= EXTI_InitStruct->EXTI_Line;
        }
        else
        {
            tempAddress = (uint32_t)EXTI_BASE;
            tempAddress += (EXTI_InitStruct->EXTI_Trigger & 0x1F);
            *(__IO uint32_t *)tempAddress |= EXTI_InitStruct->EXTI_Line;
        }
        for (pinPosition = 0; pinPosition < 16; pinPosition++)
        {
            EXTI_Linex = ((uint32_t)0x01) << pinPosition;
            if ((EXTI_InitStruct->EXTI_Line & EXTI_Linex) == EXTI_Linex)
            {
                if (pinPosition < 0x05)
                {
                    EXTI->SAMP.V32 &= (~(((uint32_t)0x0F) << (pinPosition * 4)));
                    EXTI->SAMP.V32 |= (((uint32_t)EXTI_InitStruct->EXTI_SampleCycle \
                                       | ((uint32_t)EXTI_InitStruct->EXTI_SampleClockDivision << 2)) \
                                       << (pinPosition * 4));
                }
            }
        }
    }
    else
    {
        tempAddress += EXTI_InitStruct->EXTI_Mode;
        *(__IO uint32_t *)tempAddress &= (~EXTI_InitStruct->EXTI_Line);
    }
}

/**
  * @brief  Fills each EXTI_InitStruct member with its reset value.
  * @param  EXTI_InitStruct: pointer to a @ref EXTI_InitTypeDef structure which will.
  *         be initialized.
  * @retval None
  */
void EXTI_StructInit(EXTI_InitTypeDef *EXTI_InitStruct)
{
    EXTI_InitStruct->EXTI_Line = EXTI_LineNull;
    EXTI_InitStruct->EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStruct->EXTI_Trigger = EXTI_Trigger_Falling;
    EXTI_InitStruct->EXTI_SampleCycle = EXTI_SampleCycle_1;
    EXTI_InitStruct->EXTI_SampleClockDivision = EXTI_SampleClockDivision_1;
    EXTI_InitStruct->EXTI_LineCmd = SH_DISABLE;
}

/**
  * @brief  Select the GPIO pin used as EXTI Line.
  * @param  EXTI_PortSourceGPIOx: select the GPIO port to be used as source for exti lines.
  *         This parameter can be EXTI_PortSourceGPIOx @ref EXTI_Port_Type. 
  * @param  EXTI_PinSourcex: specifie the EXTI line to be configured.
  *         This parameter can be EXTI_PinSourcex @ref EXTI_PIN_Type.
  * @retval None
  */
void EXTI_PinConfig(EXTI_Port_TypeDef EXTI_PortSourceGPIOx, EXTI_PIN_TypeDef EXTI_PinSourcex)
{
    /* Check the parameters */
    assert_param(IS_EXTI_PORT_SOURCE(EXTI_PortSourceGPIOx));
    assert_param(IS_EXTI_PIN_SOURCE(EXTI_PinSourcex));

    if (EXTI_PinSourcex > EXTI_PinSource7)
    {
        EXTI->CFGH.V32 &= (~(((uint32_t)0x0F) << ((EXTI_PinSourcex - EXTI_PinSource8) * 4)));
        EXTI->CFGH.V32 |= ((EXTI_PortSourceGPIOx) << ((EXTI_PinSourcex - EXTI_PinSource8) * 4));
    }
    else
    {
        EXTI->CFGL.V32 &= (~(((uint32_t)0x0F) << (EXTI_PinSourcex * 4)));
        EXTI->CFGL.V32 |= ((EXTI_PortSourceGPIOx) << (EXTI_PinSourcex * 4));
    }
}

/**
  * @brief  Generates a Software interrupt on selected EXTI line.
  * @param  EXTI_Line: specifies the EXTI line on which the software interrupt
  *         will be generated.
  *         This parameter can be any combination of the following values:
  *            @arg @b EXTI_Line0
  *            @arg @b EXTI_Line1
  *            @arg @b EXTI_Line2
  *            @arg @b EXTI_Line3
  *            @arg @b EXTI_Line4
  *            @arg @b EXTI_Line5
  *            @arg @b EXTI_Line6
  *            @arg @b EXTI_Line7
  *            @arg @b EXTI_Line8
  *            @arg @b EXTI_Line9
  *            @arg @b EXTI_Line10
  *            @arg @b EXTI_Line11
  *            @arg @b EXTI_Line12
  *            @arg @b EXTI_Line13
  *            @arg @b EXTI_Line14
  *            @arg @b EXTI_Line15
  * @retval None
  */
void EXTI_GenerateSWInterrupt(EXTI_Line_Typedef EXTI_Line)
{
    /* Check the parameters */
    assert_param(IS_EXTI_LINE(EXTI_Line));

    EXTI->SWIER = EXTI_Line;
}

/**
  * @brief  Checks whether the specified EXTI line flag is set or not.
  * @param  EXTI_Line: specifies the EXTI line flag to check.
  *         This parameter can be any combination of the following values:
  *            @arg @b EXTI_Line0
  *            @arg @b EXTI_Line1
  *            @arg @b EXTI_Line2
  *            @arg @b EXTI_Line3
  *            @arg @b EXTI_Line4
  *            @arg @b EXTI_Line5
  *            @arg @b EXTI_Line6
  *            @arg @b EXTI_Line7
  *            @arg @b EXTI_Line8
  *            @arg @b EXTI_Line9
  *            @arg @b EXTI_Line10
  *            @arg @b EXTI_Line11
  *            @arg @b EXTI_Line12
  *            @arg @b EXTI_Line13
  *            @arg @b EXTI_Line14
  *            @arg @b EXTI_Line15
  * @retval Flag Status
  *         can be one of the following values:
  *         @arg @b SH_SET
  *         @arg @b SH_RESET
  */
FlagStatus EXTI_GetPendingFlagStatus(EXTI_Line_Typedef EXTI_Line)
{
    FlagStatus bitStatus;

    /* Check the parameters */
    assert_param(IS_EXTI_GET_ONE_LINE(EXTI_Line));

    if ((EXTI->PR.V32 & EXTI_Line) != (uint32_t)SH_RESET)
    {
        bitStatus = SH_SET;
    }
    else
    {
        bitStatus = SH_RESET;
    }
    return bitStatus;
}

/**
  * @brief  Clears the EXTI's line pending flags.
  * @param  EXTI_Line: specifies the EXTI lines flags to clear.
  *         This parameter can be any combination of the following values:
  *            @arg @b EXTI_Line0
  *            @arg @b EXTI_Line1
  *            @arg @b EXTI_Line2
  *            @arg @b EXTI_Line3
  *            @arg @b EXTI_Line4
  *            @arg @b EXTI_Line5
  *            @arg @b EXTI_Line6
  *            @arg @b EXTI_Line7
  *            @arg @b EXTI_Line8
  *            @arg @b EXTI_Line9
  *            @arg @b EXTI_Line10
  *            @arg @b EXTI_Line11
  *            @arg @b EXTI_Line12
  *            @arg @b EXTI_Line13
  *            @arg @b EXTI_Line14
  *            @arg @b EXTI_Line15
  * @retval None
  */
void EXTI_ClearPendingFlag(EXTI_Line_Typedef EXTI_Line)
{
    /* Check the parameters */
    assert_param(IS_EXTI_LINE(EXTI_Line));

    EXTI->PR.V32 |= (uint32_t)(EXTI_Line << 16);
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
