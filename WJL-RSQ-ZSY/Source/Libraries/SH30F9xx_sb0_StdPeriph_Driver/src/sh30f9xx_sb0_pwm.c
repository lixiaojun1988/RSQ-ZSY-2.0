/**
  ******************************************************************************
  * @file    sh30f9xx_sb0_pwm.c
  * @author  sinowealth
  * @version V1.0.0
  * @date    2024-5-24  
  * @brief   This file provides firmware functions to manage the PWM module
  *  
    @verbatim
  ==============================================================================
                         How to use this driver
  ==============================================================================
   [..]
     (#) Enable PWM Clock use RCC_APB0PeriphClockOnOff() function 
  
     (#) Fill PWM Init Structure member as default use PWM_StructInit() function
     
     (#) Config PWM Pin
         (++) Enable Pin Clock calling RCC_AHBPeriphClockOnOff() function 
         
         (++) Call GPIO_Init()function for config PWM pin(output mode, no-pullup, Push-Pull),if not use all member for PWM 
              pin config ,makesure call GPIO_StructInit() function for GPIO_Init function  paramter set default value
 
         (++) Call GPIO_PinAFConfig() for config pin as PWM function

     (#) Config PWM Clock ,period, duty, deadband etc use PWM_Init() function
     
     (#) Set PWM channelA/B output level by calling PWM_OutputMode() function

     (#) Enable PWM Module by calling PWM_OnOff() function
     
     (#) Use PWM interrupt by following step
     
         (++) Config PWM Interrupt use PWM_INTConfig() function
         
         (++) Enable Kernel PWM's Global interrupt by calling NVIC_EnableIRQ() function
         
         (++) reference startup asm file for PWM interrupt function name and rewrite this function 	
      
     (#) When PWM interrupt happen, use PWM_GetIntFlagStatus() function to get PWM interrupt flag

     (#) Call PWM_ClearIntFlag() function for clear PWM interrupt flag
  
  * @endverbatim
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
#include "sh30f9xx_sb0_pwm.h"

/** @addtogroup SH30F9XX_SB0_STANDARD_LIB
  * @{
  */

/** @defgroup PWM_MODULE PWM
  * @brief SH30F9XX SB0 PWM module driver library,reference this topic macro and function for drive PWM
  * @{
  */
  
/* Export function prototypes -----------------------------------------------*/

/** @defgroup PWM_Exported_Functions PWM Exported Functions
 *  @brief    This is PWM exported function code group
 *
  * @{
  */

/**
  * @brief  Deinitializes the PWMx peripheral registers to their default
  *         reset values 
  * @param  PWMx: where x can be 0 to 3 to select the PWM peripheral.
  * @retval None
  */
void PWM_Reset(PWM_TypeDef *PWMx)
{
    /* Check the parameters */
    assert_param(IS_PWM_MODULE(PWMx));

    if (PWMx == PWM0)
    {
        /* Reset PWM0 module */
        RCC_APB0PeriphReset(RCC_APB0_PWM0);
    }
    else if (PWMx == PWM1)
    {
        /* Reset PWM1 module */
        RCC_APB0PeriphReset(RCC_APB0_PWM1);
    }
    else if (PWMx == PWM2)
    {
        /* Reset PWM2 module */
        RCC_APB0PeriphReset(RCC_APB0_PWM2);
    }
    else if (PWMx == PWM3)
    {
        /* Reset PWM3 module */
        RCC_APB0PeriphReset(RCC_APB0_PWM3);
    }
    else{}
}

/**
  * @brief  Fills each PWM_InitStruct member with its default value.
  * @param  PWM_InitStruct : pointer to a @ref PWM_InitTypeDef
  *         structure which will be initialized.
  * @retval None
  */
void PWM_StructInit(PWM_InitTypeDef *PWM_InitStruct)
{
    PWM_InitStruct->PWM_Period = 0;
    PWM_InitStruct->PWM_Duty = 0;
    PWM_InitStruct->PWM_DeadBand = 0;
    PWM_InitStruct->PWM_CLKSource = TCK_PCLK0_DIV1;
}

/**
  * @brief  Initializes the PWMx peripheral according to the specified parameters 
  *         in the PWM_InitStruct.
  * @param  PWMx: where x can be  0 to 3 to select the PWM peripheral.
  * @param  PWM_InitStruct: pointer to a @ref PWM_InitTypeDef structure that contains 
  *         the configuration information for the specified PWM peripheral.
  * @retval None
  */
void PWM_Init(PWM_TypeDef *PWMx, PWM_InitTypeDef *PWM_InitStruct)
{
    /* check the parameters */
    assert_param(IS_PWM_ALL_PERIPH(PWMx));
    assert_param(IS_PWMCLKSOURCE_DIV(PWM_InitStruct->PWM_CLKSource));

    PWM_UNLOCK();
    PWMx->PWMPR = PWM_InitStruct->PWM_Period;
    PWMx->PWMDR = PWM_InitStruct->PWM_Duty;
    PWMx->PWMDTR = PWM_InitStruct->PWM_DeadBand;

    PWMx->CR.BIT.TCK = PWM_InitStruct->PWM_CLKSource;
    PWM_LOCK();
}

/**
  * @brief  Open or close the PWM peripheral.
  * @param  PWMx: where x can be 0 to 3 to select the PWM peripheral.
  * @param  OnOffState: state of the PWM peripheral.
  *         This parameter can be one of the following values:
  *            @arg @b SH_ON
  *            @arg @b SH_OFF
  * @retval None
  */
void PWM_OnOff(PWM_TypeDef *PWMx, CmdState OnOffState)
{
    /* check the parameters */
    assert_param(IS_PWM_ALL_PERIPH(PWMx));
    assert_param(IS_CMD_STATE(OnOffState));

    PWM_UNLOCK();
    if (OnOffState != SH_OFF)
    {
        PWMx->CR.V32 |= PWM_CR_PWMEN_Msk;
    }
    else
    {
        PWMx->CR.V32 &= (~PWM_CR_PWMEN_Msk);
    }
    PWM_LOCK();
}

/**
  * @brief  Set the pwm peripheral cycle.
  * @param  PWMx: where x can be  0 to 3 to select the PWM peripheral.
  * @param  prescale: set pwm Clock period(PWM Cycle).
  * @retval None
  */
void PWM_SetCycle(PWM_TypeDef *PWMx, uint16_t prescale)
{
    /* check the parameters */
    assert_param(IS_PWM_ALL_PERIPH(PWMx));

    PWM_UNLOCK();
    PWMx->PWMPR = prescale;
    PWM_LOCK();
}

/**
  * @brief  Set the pwm peripheral duty cycle.
  * @param  PWMx: where x can be  0 to 3 to select the PWM peripheral.
  * @param  duty: set pwm duty.
  * @retval None
  */
void PWM_SetDuty(PWM_TypeDef *PWMx, uint16_t duty)
{
    /* check the parameters */
    assert_param(IS_PWM_ALL_PERIPH(PWMx));

    PWM_UNLOCK();
    PWMx->PWMDR = duty;
    PWM_LOCK();
}

/**
  * @brief  Check whether the specified PWM interrupt flag is set or not.
  * @param  PWMx: where x can be 0 to 3 to select the PWM peripheral.
  * @retval The new state of PWM's flag.
  *         can be one of the following values:
  *         @arg @b SH_SET
  *         @arg @b SH_RESET
  */
FlagStatus PWM_GetIntFlagStatus(PWM_TypeDef *PWMx)
{
    FlagStatus bitStatus;

    /* check the parameters */
    assert_param(IS_PWM_ALL_PERIPH(PWMx));

    if ((PWMx->PWMINTF.V32 & PWM_PWMINTF_PWMIF_Msk) != SH_RESET)
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
  * @brief  Clear the PWMx's pending flag.
  * @param  PWMx: where x can be 0 to 3 to select the PWM peripheral.
  * @retval None
  */
void PWM_ClearIntFlag(PWM_TypeDef *PWMx)
{
    /* check the parameters */
    assert_param(IS_PWM_ALL_PERIPH(PWMx));
    PWM_UNLOCK();
    PWMx->PWMINTF.V32 |= PWM_PWMINTF_PWMIFC_Msk;
    PWM_LOCK();
}
/**
  * @brief  Check whether the specified PWM fault flag is set or not.
  * @param  PWMx: where x can be 0 to 3 to select the PWM peripheral.
  * @retval The new state of PWM's flag.
  *         can be one of the following values:
  *         @arg @b SH_SET
  *         @arg @b SH_RESET
  */
FlagStatus PWM_GetFLTFlagStatus(PWM_TypeDef *PWMx)
{
    FlagStatus bitStatus;

    /* check the parameters */
    assert_param(IS_PWM_ALL_PERIPH(PWMx));

    if ((PWMx->PWMINTF.V32 & PWM_PWMINTF_FLTIF_Msk) != SH_RESET)
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
  * @brief  Clear the PWMx's fault flag.
  * @param  PWMx: where x can be 0 to 3 to select the PWM peripheral.
  * @retval None
  */
void PWM_ClearFLTFlag(PWM_TypeDef *PWMx)
{
    /* check the parameters */
    assert_param(IS_PWM_ALL_PERIPH(PWMx));
    PWM_UNLOCK();
    PWMx->PWMINTF.V32 |= PWM_PWMINTF_FLTIFC_Msk;
    PWM_LOCK();
}

/**
  * @brief  Enable or disable the specified PWM's Overflow interrupts.
  * @param  PWMx: where x can be 0 to 3 to select the PWM peripheral.
  * @param  PWMIntSource: PWM Interrupt source.
  *         This parameter can be one of the following values:
  *            @arg @b PWM_INT_OVERFLOW
  *            @arg @b PWM_INT_FAULT
  * @param  NewState: New state of PWM interrupt.
  *         This parameter can be one of the following values:
  *            @arg @b SH_ENABLE
  *            @arg @b SH_DISABLE
  * @retval None
  */
void PWM_INTConfig(PWM_TypeDef *PWMx, PWM_INT_TypeDef PWMIntSource, FunctionalState NewState)
{
    /* Check the parameters */
    assert_param(IS_PWM_ALL_PERIPH(PWMx));
    assert_param(IS_PWM_INT(PWMIntSource));
    assert_param(IS_FUNCTION_STATE(NewState));

    PWM_UNLOCK();
    if (NewState != SH_DISABLE)
    {
        PWMx->CR.V32 |= PWMIntSource;
    }
    else
    {
        PWMx->CR.V32 &= (~PWMIntSource);
    }
    PWM_LOCK();
}

/**
  * @brief  The specified PWM Channel output level set.
  * @param  PWMx: where x can be 0 to 3 to select the PWM peripheral.
  * @param  PWM_Channel: PWM Channel.
  *         This parameter can be one of the following values:
  *            @arg @b PWM_CHANNEL_A
  *            @arg @b PWM_CHANNEL_B
  * @param  levelState: PWM Drive Level.
  *         This parameter can be one of the following values:
  *            @arg @b SH_LOW
  *            @arg @b SH_HIGH
  * @retval None
  */
void PWM_OutputMode(PWM_TypeDef *PWMx, PWM_CHANNEL_TypeDef PWM_Channel, LevelStatus levelState)
{
    /* Check the parameters */
    assert_param(IS_PWM_ALL_PERIPH(PWMx));
    assert_param(IS_PWM_CHANNEL(PWM_Channel));
    assert_param(IS_LEVEL_STATE(levelState));

    PWM_UNLOCK();
    if (PWM_Channel & PWM_CHANNEL_A)
    {
        if (levelState != SH_HIGH)
        {
            PWMx->CR.BIT.PWMSA = 1;
        }
        else
        {
            PWMx->CR.BIT.PWMSA = 0;
        }
    }
    if (PWM_Channel & PWM_CHANNEL_B)
    {
        if (levelState != SH_HIGH)
        {
            PWMx->CR.BIT.PWMSB = 0;
        }
        else
        {
            PWMx->CR.BIT.PWMSB = 1;
        }
    }
    PWM_LOCK();
}

/**
  * @brief  Check whether the specified PWM fault FLTS status is set or not.
  * @param  PWMx: where x can be 0 to 3 to select the PWM peripheral.
  * @retval The new state of PWM's FLTS status,
  *         can be one of the following values:
  *         @arg @b SH_SET
  *         @arg @b SH_RESET
  */
FlagStatus PWM_GetFLTSStatus(PWM_TypeDef *PWMx)
{
    FlagStatus bitStatus;

    /* check the parameters */
    assert_param(IS_PWM_ALL_PERIPH(PWMx));

    if ((PWMx->PWMINTF.V32 & PWM_PWMINTF_FLTS_Msk) != SH_RESET)
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
  * @brief  Clear the PWMx's fault status.
  * @param  PWMx: where x can be 0 to 3 to select the PWM peripheral.
  * @retval None
  * @note   When fault level disappeared, FLTS can be clear.
  */
void PWM_ClearFLTSStatus(PWM_TypeDef *PWMx)
{
    /* check the parameters */
    assert_param(IS_PWM_ALL_PERIPH(PWMx));
    PWM_UNLOCK();
    PWMx->PWMINTF.V32 |= PWM_PWMINTF_FLTSC_Msk;
    PWM_LOCK();
}

/**
  * @brief  Set PWM peripheral Fault Pin electrical level to Close PWM Output.
  * @param  PWMx: where x can be 0 to 3 to select the PWM peripheral.
  * @param  levelState: level of the Fault Pin.
  *         This parameter can be one of the following values:
  *            @arg @b SH_LOW
  *            @arg @b SH_HIGH
  * @retval None
  */
void PWM_FaultPinLevel(PWM_TypeDef *PWMx, LevelStatus levelState)
{
    /* check the parameters */
    assert_param(IS_PWM_ALL_PERIPH(PWMx));
    assert_param(IS_LEVEL_STATE(levelState));

    PWM_UNLOCK();
    if (levelState != SH_HIGH)
    {
        PWMx->CR.V32 &= (~PWM_CR_FLTC_Msk);
    }
    else
    {
        PWMx->CR.V32 |= PWM_CR_FLTC_Msk;
    }
    PWM_LOCK();
}

/**
  * @brief  Enable or disable PWM peripheral Fault Pin Function.
  * @param  PWMx: where x can be 0 to 3 to select the PWM peripheral.
  * @param  NewState: new state of the Fault Fin function.
  *         This parameter can be one of the following values:
  *            @arg @b SH_ENABLE
  *            @arg @b SH_DISABLE
  * @retval None
  */
void PWM_FaultPinFunction(PWM_TypeDef *PWMx, FunctionalState NewState)
{
    /* check the parameters */
    assert_param(IS_PWM_ALL_PERIPH(PWMx));
    assert_param(IS_FUNCTION_STATE(NewState));

    PWM_UNLOCK();
    if (NewState != SH_DISABLE)
    {
        PWMx->CR.V32 |= PWM_CR_EFLT_Msk;
    }
    else
    {
        PWMx->CR.V32 &= (~PWM_CR_EFLT_Msk);
    }
    PWM_LOCK();
}

/**
  * @brief  Set PWM peripheral fault pin debounce.
  * @param  PWMx: where x can be 0 to 3 to select the PWM peripheral.
  * @param  Debtime: can be set 0 to 63, the data means as follows:
  *          @arg @b 0 means PCLK0/1 debounce
  *          @arg @b 1 means PCLK0/2 debounce
  *          @arg @b 2 means PCLK0/3 debounce
  *          @arg @b     ...
  *          @arg @b 63 means PCLK0/64 debounce
  * @retval None
  */
void PWM_FaultPinDebounce(PWM_TypeDef *PWMx, uint8_t Debtime)
{
    /* check the parameters */
    assert_param(IS_PWM_ALL_PERIPH(PWMx));
    assert_param(Debtime > 63);

    PWM_UNLOCK();
    PWMx->CR.BIT.FLTDEB = Debtime;
    PWM_LOCK();
}

/**
  * @brief  The specified PWM Channel fault output level set.
  * @param  PWMx: where x can be 0 to 3 to select the PWM peripheral.
  * @param  PWM_Channel: PWM Channel.
  *         This parameter can be one of the following values:
  *            @arg @b PWM_CHANNEL_A
  *            @arg @b PWM_CHANNEL_B
  * @param  Pinlevel: PWM Drive Level.
  *         This parameter can be one of the following values:
  *            @arg @b FLT_OUTPUT_HIGH_Z
  *            @arg @b FLT_OUTPUT_LOW
  *            @arg @b FLT_OUTPUT_HIGH
  * @retval None
  */
void PWM_FLTOutputMode(PWM_TypeDef *PWMx, PWM_CHANNEL_TypeDef PWM_Channel, PWM_FLTOutLevel_TypeDef Pinlevel)
{
    /* Check the parameters */
    assert_param(IS_PWM_ALL_PERIPH(PWMx));
    assert_param(IS_PWM_CHANNEL(PWM_Channel));
    assert_param(IS_PWMFLTOutLevel(Pinlevel));

    PWM_UNLOCK();
    if (PWM_Channel & PWM_CHANNEL_A)
    {
       PWMx->CR.BIT.FOUTA = Pinlevel;
    }
    else if (PWM_Channel & PWM_CHANNEL_B)
    {
       PWMx->CR.BIT.FOUTB = Pinlevel;
    }
    PWM_LOCK();
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
