/**
  ******************************************************************************
  * @file    sh30f9xx_sb0_hwdt.c
  * @author  sinowealth
  * @version V1.0.0
  * @date    2024-5-21    
  * @brief   This file provides firmware functions to manage the HWDT module 
  *         
  @verbatim
  ==============================================================================
                         How to use this driver
  ==============================================================================
 [..]
   (#) Cofing HWDT period  by calling HWDT_ConfigPeriod() function
   
   (#) Call HWDT_OnOff() function for enable or disable hwdt function in STOP/SLEEP mode
      
   (#) Clear HWDT counter by calling HWDT_ReloadCounter() function 
   
   (#) Call  HWDT_GetFlagStatus() function to get HWDT set flag status 
   

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
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "sh30f9xx_sb0_hwdt.h"

/** @addtogroup SH30F9XX_SB0_STANDARD_LIB
  * @{
  */

/** @defgroup HWDT_MODULE HWDT
  * @brief SH30F9XX SB0 HWDT module driver library,reference this topic macro and function for drive HWDT
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/



/** @defgroup HWDT_Exported_Functions HWDT Exported Functions
  *
  *  @brief    This is HWDT  exported function code group
  * @{
  */
  
/**
  * @brief  Config HWDT Reload period
  * @param  ReloadPeriod:  Specifies the HWDT Reload period,This parameter can be one of the following values:
  *            @arg @b HWDT_ReloadPeriod_16MS  :  HWDT Reload period 16MS
  *            @arg @b HWDT_ReloadPeriod_64MS  :  HWDT Reload period 64MS
  *            @arg @b HWDT_ReloadPeriod_128MS :  HWDT Reload period 128MS
  *            @arg @b HWDT_ReloadPeriod_256MS :  HWDT Reload period 256MS
  *            @arg @b HWDT_ReloadPeriod_512MS :  HWDT Reload period 512MS
  *            @arg @b HWDT_ReloadPeriod_1024MS:  HWDT Reload period 1024MS
  *            @arg @b HWDT_ReloadPeriod_1536MS:  HWDT Reload period 1536MS
  *            @arg @b HWDT_ReloadPeriod_2048MS:  HWDT Reload period 2048MS
  * @retval None
  */
void HWDT_ConfigPeriod(HWDT_ReloadPeriod_TypeDef ReloadPeriod)
{
    /* Check the parameters */
    assert_param(IS_HWDT_RELOAD_PERIOD(ReloadPeriod));

    HWDT->CR.V32 = ((0x5AA5 << HWDT_CR_LOCK_Pos) | (0x00 << HWDT_CR_HWDTRLR_Pos));
    HWDT->CR.V32 |= ((0x5AA5 << HWDT_CR_LOCK_Pos)| (ReloadPeriod << HWDT_CR_HWDTRLR_Pos));
}

/**
  * @brief  Config HWDT ON or OFF in STOP/SLEEP Mode
  * @param  OnOffState: specifies the HWDT On/Off state in STOP/SLEEP Mode.
  *         This parameter can be one of the following values:
  *            @arg @b SH_ON
  *            @arg @b SH_OFF
  * @retval None
  */
void HWDT_OnOff(CmdState OnOffState)
{
    /* Check the parameters */
    assert_param(IS_CMD_STATE(OnOffState));
    
    if(OnOffState == SH_ON)
    {
        HWDT->CR.V32 |= ((0x5AA5 << HWDT_CR_LOCK_Pos) | (0x00 << HWDT_CR_HWDTEN_Pos));
    }
    else if(OnOffState == SH_OFF)
    {
        HWDT->CR.V32 |= ((0x5AA5 << HWDT_CR_LOCK_Pos) | (0x3C << HWDT_CR_HWDTEN_Pos));
    }
}

/**
  * @brief  Reloads HWDT counter with value defined in the reload register
  * @retval None
  */
void HWDT_ReloadCounter(void)
{
    HWDT->CLR = 0xAAAA;
}

/**
  * @brief  Checks whether the specified HWDT flag is set or not.
  * @retval The new state of HWDT_FLAG (SH_SET or SH_RESET).
  */
FlagStatus HWDT_GetFlagStatus(void)
{
    /* Return the flag status */
    return (RCC->RSTSTR.BIT.HWDTRSTF ? SH_SET : SH_RESET);
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
