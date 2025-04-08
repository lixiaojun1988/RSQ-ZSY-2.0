/**
  ******************************************************************************
  * @file    sh30f9xx_sb0_syscfg.c
  * @author  sinowealth
  * @version V1.0.0
  * @date    2024-5-21    
  * @brief   This file provides firmware functions to manage the SYSCFG module
  *         
  @verbatim
  
  ==============================================================================
                         How to use this driver
  ==============================================================================
   [..]
   (#) Config BOD function use the following steps
       (++) Call SYSCFG_BODInit() function for BOD detect level congig
       
       (++) Call SYSCFG_BODOnOff() function for enable BOD function
       
       (++) Call SYSCFG_BODINTConfig() function for BOD interrupt purpose
       
       (++) Call SYSCFG_BODGetFlagStatus() function for get BOD flag
       
       (++) Call SYSCFG_BODIntClearFlag() function for clear BOD flag

   (#) Config LVR function use the following steps
       (++) Call SYSCFG_VLVRInit() function for LVR detect level congig
       
       (++) Call SYSCFG_VLVRConfig() function for enable LVR function

   (#) Call SYSCFG_EnterSLEEPMode for chip enter SLEEP Mode

   (#) Call SYSCFG_EnterSTOPMode for chip enter STOP Mode

   (#) Call SYSCFG_NMIInterruptTriggerConfig for trigger NMI interrupt source config

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
#include "sh30f9xx_sb0_syscfg.h"

/** @addtogroup SH30F9XX_SB0_STANDARD_LIB
  * @{
  */
 
/** @defgroup SYSCFG_MODULE SYSCFG 
  * @brief SH30F9XX SB0 SYSCFG module driver library,reference this topic macro and function for drive SYSCFG
  * @{
  */

/** @defgroup SYSCFG_Exported_Functions  SYSCFG Exported Functions 
  *
  * @brief    This is SYSCFG  exported function code group
  * @{
  */
  
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/**
  * @brief  Initialize BOD mode and voltage threshold.
  * @param  SYSCFG_BOD_Mode: Specifie the BOD mode.
  *          This parameter can be one of the following values:
  *         @arg @b SYSCFG_BOD_Mode_Rising
  *         @arg @b SYSCFG_BOD_Mode_Falling 
  *         @arg @b SYSCFG_BOD_Mode_RisingFalling
  * @param  SYSCFG_BOD_Level: Specifie the BOD voltage threshold.
  *          This parameter can be one of the following values:
  *         @arg @b SYSCFG_BOD_Level_2V85
  *         @arg @b SYSCFG_BOD_Level_3V00
  *         @arg @b SYSCFG_BOD_Level_3V15
  *         @arg @b SYSCFG_BOD_Level_3V30
  *         @arg @b SYSCFG_BOD_Level_3V45
  *         @arg @b SYSCFG_BOD_Level_3V60
  *         @arg @b SYSCFG_BOD_Level_3V75
  *         @arg @b SYSCFG_BOD_Level_3V90
  *         @arg @b SYSCFG_BOD_Level_4V05
  *         @arg @b SYSCFG_BOD_Level_4V20
  *         @arg @b SYSCFG_BOD_Level_4V35
  *         @arg @b SYSCFG_BOD_Level_4V50
  * @retval None
  */
void SYSCFG_BODInit(SYSCFGBODMode_TypeDef SYSCFG_BOD_Mode, SYSCFGBODLevel_TypeDef SYSCFG_BOD_Level)
{
    uint32_t temp = 0;

    /* Check the parameters */
    assert_param(IS_SYSCFG_BOD_MODE(SYSCFG_BOD_Mode));
    assert_param(IS_SYSCFG_BOD_LEVEL(SYSCFG_BOD_Level));

    SYSCFG->PWRCR.V32 &= ~(SYSCFG_PWRCR_BODMD_Msk | SYSCFG_PWRCR_VBOD_Msk);

    temp = ((uint32_t)SYSCFG_BOD_Mode << 5) | (uint32_t)SYSCFG_BOD_Level;
    SYSCFG->PWRCR.V32 |= temp;
}

/**
  * @brief  Enable or disable the specified BOD interrupt.
  * @param  NewState: New state of BOD interrupt.
  *          This parameter can be one of the following values:
  *          @arg @b SH_ENABLE  
  *          @arg @b SH_DISABLE 
  * @retval None
  */
void SYSCFG_BODINTConfig(FunctionalState NewState)
{
    /* Check the parameters */
    assert_param(IS_FUNCTION_STATE(NewState));

    if (NewState != SH_DISABLE)
    {
        SYSCFG->PWRCR.V32 |= SYSCFG_PWRCR_BODIE_Msk;
    }
    else
    {
        SYSCFG->PWRCR.V32 &= ~SYSCFG_PWRCR_BODIE_Msk;
    }
}
/**
  * @brief  Initialize LVR voltage threshold.
  * @param  SYSCFG_VLVR_Level: Specifie the LVR voltage threshold.
  *          This parameter can be one of the following values:
  *          @arg @b  SYSCFG_VLVR_Level_4V1
  *          @arg @b  SYSCFG_VLVR_Level_3V7
  *          @arg @b  SYSCFG_VLVR_Level_3V1
  * @retval None
  */
void SYSCFG_VLVRInit(SYSCFG_VLVR_LEVEL_TypeDef SYSCFG_VLVR_Level)
{
    uint32_t temp = 0;
    /* Check the parameters */
    assert_param(IS_SYSCFG_VLVR_LEVEL(SYSCFG_VLVR_Level));

    SYSCFG->PWRCR.V32 &= ~(SYSCFG_PWRCR_VLVR_Msk);

    temp = ((uint32_t)SYSCFG_VLVR_Level << 9);
    SYSCFG->PWRCR.V32 |= temp;
}

/**
  * @brief  Enable or disable the specified LVR function.
  * @param  NewState: New state of LVR function.
  *          This parameter can be one of the following values:
  *          @arg @b SH_ENABLE  
  *          @arg @b SH_DISABLE 
  * @retval None
  */
void SYSCFG_VLVRConfig(FunctionalState NewState)
{
    /* Check the parameters */
    assert_param(IS_FUNCTION_STATE(NewState));

    if (NewState != SH_DISABLE)
    {
        SYSCFG->PWRCR.V32 |= SYSCFG_PWRCR_LVREN_Msk;
    }
    else
    {
        SYSCFG->PWRCR.V32 &= ~SYSCFG_PWRCR_LVREN_Msk;
    }
}

/**
  * @brief  Open or close BOD module.
  * @param  OnOffState: state of the BOD
  *          This parameter can one of the following values:
  *            @arg @b SH_ON   
  *            @arg @b SH_OFF  
  * @retval None
  */
void SYSCFG_BODOnOff(CmdState OnOffState)
{
    /* Check the parameters */
    assert_param(IS_CMD_STATE(OnOffState));

    if (OnOffState != SH_OFF)
    {
        SYSCFG->PWRCR.V32 |= SYSCFG_PWRCR_BODEN_Msk;
    }
    else
    {
        SYSCFG->PWRCR.V32 &= ~SYSCFG_PWRCR_BODEN_Msk;
    }
}

/**
  * @brief  Check whether the specified BOD flag is set or not.
  * @param  BOD_Flag: specifies the flag to check.
  *          This parameter can be one of the following values:
  *            @arg @b SYSCFG_BOD_FLAG_BODIF: BOD interrupt pending flag.
  *            @arg @b SYSCFG_BOD_FLAG_BODF: BOD output flag.
  * @retval New state of BOD Flag(SH_SET or SH_RESET).
  */
FlagStatus SYSCFG_BODGetFlagStatus(SYSCFG_BOD_FLAG_TypeDef BOD_Flag)
{
    FlagStatus bitStatus;

    /* Check the parameters */
    assert_param(IS_SYSCFG_BOD_FLAG(BOD_Flag));

    if ((SYSCFG->PWRSR.V32 & BOD_Flag) != SH_RESET)
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
  * @brief  Clear BOD pending bit.
  * @retval None
  */
void SYSCFG_BODIntClearFlag(void)
{
    /* Clear BODIF bit */
    SYSCFG->PWRSR.V32 &= ~SYSCFG_PWRSR_BODIF_Msk;
}


/**
  * @brief  Enters SLEEP mode.
  * @param  SYSCFG_SLEEPEntry: specifies if SLEEP mode in entered with WFI or WFE instruction.
  *          This parameter can be one of the following values:
  *            @arg @b SYSCFG_SLEEPEntry_WFI: enter STOP mode with WFI instruction
  *            @arg @b SYSCFG_SLEEPEntry_WFE: enter STOP mode with WFE instruction
  * @retval None
  */
void SYSCFG_EnterSLEEPMode(SYSCFG_SLEEPEntry_TypeDef SYSCFG_SLEEPEntry)
{
    /* Check the parameters */
    assert_param(IS_SYSCFG_SLEEP_ENTRY(SYSCFG_SLEEPEntry));

    /* Select SLEEP mode entry */
    if (SYSCFG_SLEEPEntry == SYSCFG_SLEEPEntry_WFI)
    {
        /* Request Wait For Interrupt */
        __WFI();
    }
    else
    {
        /* Request Wait For Event */
        __SEV();
        __WFE();
        __WFE();
    }
}

/**
  * @brief  Enters STOP mode.
  *   
  * @note   In Stop mode, all I/O pins keep the same state as in Run mode.
  * @note   When exiting Stop mode by issuing an interrupt or a wakeup event, 
  *         the HSI RC oscillator is selected as system clock.
  * @param  SYSCFG_STOPEntry: specifies if STOP mode in entered with WFI or WFE instruction.
  *          This parameter can be one of the following values:
  *            @arg @b SYSCFG_STOPEntry_WFI: enter STOP mode with WFI instruction
  *            @arg @b SYSCFG_STOPEntry_WFE: enter STOP mode with WFE instruction
  * @retval None
  */
void SYSCFG_EnterSTOPMode(SYSCFG_STOPEntry_TypeDef SYSCFG_STOPEntry)
{
    /* Check the parameters */
    assert_param(IS_SYSCFG_SLOP_ENTRY(SYSCFG_STOPEntry));

    /* Set SLEEPDEEP bit of Cortex System Control Register */
    SCB->SCR |= SCB_SCR_SLEEPDEEP_Msk;

    /* Select STOP mode entry */
    if (SYSCFG_STOPEntry == SYSCFG_STOPEntry_WFI)
    {
        /* Request Wait For Interrupt */
        __WFI();
    }
    else
    {
        /* Request Wait For Event */
        __SEV();
        __WFE();
        __WFE();
    }
    /* Reset SLEEPDEEP bit of Cortex System Control Register */
    SCB->SCR &= (uint32_t) ~((uint32_t)SCB_SCR_SLEEPDEEP_Msk);
}

/**
  * @brief  Configure NMI interrupt trigger source.
  * @param  TriggerSource: Specifie NMI interrupt trigger source.
  *         This parameter can be any combination of the following values:
  *            @arg @b SYSCFG_NMI_TRIGGER_CSM: CSM trigger NMI interrupt.
  *            @arg @b SYSCFG_NMI_TRIGGER_BOD: BOD trigger NMI interrupt.
  *            @arg @b SYSCFG_NMI_TRIGGER_EXTI0: EXTI0 trigger NMI interrupt. 
  * @param  NewState: New state of NMI interrupt trigger source.
  *         This parameter can be any combination of the following values:
  *            @arg @b SH_ENABLE
  *            @arg @b SH_DISABLE
  * @retval None
  */
void SYSCFG_NMIInterruptTriggerConfig(SYSCFG_NMI_TRIGGER_TypeDef TriggerSource, FunctionalState NewState)
{
    uint32_t temp = 0;

    /* Check the parameters */
    assert_param(IS_SYSCFG_NMI_TRIGGER_SOURCE(TriggerSource));
    assert_param(IS_FUNCTION_STATE(NewState));

    temp = SYSCFG->SAFR.V32 & (uint32_t)0x0000FFFF;

    if (NewState != SH_DISABLE)
    {
        temp |= ((uint32_t)0x5AA5 << 16) | TriggerSource;
        SYSCFG->SAFR.V32 = temp;
    }
    else
    {
        temp &= ~TriggerSource;
        temp |= ((uint32_t)0x5AA5 << 16);
        SYSCFG->SAFR.V32 = temp;
    }
}


/**
  * @brief  Specifie SWJ pin alternate function.
  * @param  SWJPin_Mode: specifie the value for SWJ pin mode.
  *          This parameter can be one of the @ref SYSCFG_SWJCFG_TypeDef enum values:
  *            @arg @b SYSCFG_SWJ_All_Function
  *            @arg @b SYSCFG_SWD_CLOSE_PinRemap
  * @retval None
  */
void SYSCFG_SWJPinConfig(SYSCFG_SWJCFG_TypeDef SWJPin_Mode)
{
    uint32_t temp = 0;

    /* Check the parameters */
    assert_param(IS_SYSCFG_SWJ_PIN_MODE(SWJPin_Mode));

    temp = SYSCFG->SAFR.V32 & (~SYSCFG_SAFR_SWJCFG_Msk);

    temp |= (uint32_t)(SWJPin_Mode << 5);
    SYSCFG->SAFR.V32 = ((uint32_t)0x5AA5 << 16) | temp;
}

/**
  * @brief  Specifie XTAL pin And XTALX alternate
  * @param  OSCPin_Mode: specifie the value for XTAL pin mode.
  *         This parameter can be one of the @ref SYSCFG_OSCPin_TypeDef enum values:
  *         @arg @b SYSCFG_OSC_GPIO
  *         @arg @b SYSCFG_OSC_HCrystal
  *         @arg @b SYSCFG_OSC_ExtClock
  * @retval None
  */
void SYSCFG_OSCPinConfig(SYSCFG_OSCPin_TypeDef OSCPin_Mode)
{
    uint32_t temp = 0;

    /* Check the parameter */
    assert_param(IS_SYSCFG_OSC_PIN_MODE(OSCPin_Mode));

    temp = SYSCFG->SAFR.V32 & (~SYSCFG_SAFR_OSCCFG_Msk);

    temp |= (uint32_t)OSCPin_Mode;
    SYSCFG->SAFR.V32 = ((uint32_t)0x5AA5 << 16) | temp;
}


/**
  * @brief  Lock or unlock SRAM sectors.
  * @param  SectorNum: specifie SRAM sectors to be locked or unlocked.
  *          This parameter can be any combination of the following values:
  *            @arg @b SRAM_Sector_0
  *            @arg @b SRAM_Sector_1
  *            @arg @b SRAM_Sector_2
  *            @arg @b SRAM_Sector_3
  *            @arg @b SRAM_Sector_4
  *            @arg @b SRAM_Sector_5
  *            @arg @b SRAM_Sector_6
  *            @arg @b SRAM_Sector_7
  *            @arg @b SRAM_Sector_8 
  *            @arg @b SRAM_Sector_9 
  *            @arg @b SRAM_Sector_10
  *            @arg @b SRAM_Sector_11
  *            @arg @b SRAM_Sector_12
  *            @arg @b SRAM_Sector_13
  *            @arg @b SRAM_Sector_14
  *            @arg @b SRAM_Sector_15
  * @param  NewState: new state of selected SRAM sectors.
  *          This parameter can be one of the following values:
  *            @arg @b SH_ENABLE  to lock the selected sectors
  *            @arg @b SH_DISABLE to unlock the selected sectors
  * @retval None
  */
void SYSCFG_SRAMLockConfig(SRAM_Sector_TypeDef SectorNum, FunctionalState NewState)
{
    uint32_t temp = 0;

    /* Check the parameters */
    assert_param(IS_SYSCFG_SRAM_SECTOR(SectorNum));
    assert_param(IS_FUNCTION_STATE(NewState));

    if (NewState != SH_DISABLE)
    {
        SYSCFG->SRAMLOCK.V32 |= ((uint32_t)0x5AA5 << 16) | SectorNum;
    }
    else
    {
        temp = SYSCFG->SRAMLOCK.V32 & (~SectorNum);
        SYSCFG->SRAMLOCK.V32 = ((uint32_t)0x5AA5 << 16) | temp;
    }
}



/**
  * @brief  Enable or disable peripherals clock in debug mode.
  * @param  DBG_Periph: specifie peripherals to be enable or disable.
  *          This parameter can be any combination of the following values:
  *            @arg @b DBG_Periph_WWDT 
  *            @arg @b DBG_Periph_PCA  
  *            @arg @b DBG_Periph_TIM  
  *            @arg @b DBG_Periph_PWM  
  *            @arg @b DBG_Periph_UART 
  *            @arg @b DBG_Periph_SPI  
  *            @arg @b DBG_Periph_TWI  
  * @param  NewState: new state of the selected peripherals.
  *          This parameter can be one of the following values:
  *            @arg @b SH_ENABLE
  *            @arg @b SH_DISABLE
  * @retval None
  */
void SYSCFG_DBGPeriphConfig(DBG_Periph_TypeDef DBG_Periph, FunctionalState NewState)
{
    uint32_t temp = 0;

    /* Check the parameters */
    assert_param(IS_DBG_PERIPH(DBG_Periph));
    assert_param(IS_FUNCTION_STATE(NewState));

    if (NewState != SH_DISABLE)
    {
        SYSCFG->DBGCR.V32 |= ((uint32_t)0x5AA5 << 16) | DBG_Periph;
    }
    else
    {
        temp = SYSCFG->DBGCR.V32 & (~DBG_Periph);
        SYSCFG->DBGCR.V32 = ((uint32_t)0x5AA5 << 16) | temp;
    }
}

/**
  * @brief  Enable or disable low power clock in debug mode.
  * @param  NewState: new state of the low power mode clock.
  *          This p@b arameter can be one of the following values:
  *           @arg @b SH_ENABLE
  *           @arg @b SH_DISABLE
  * @retval None
  */
void SYSCFG_DBGLowPowerConfig(FunctionalState NewState)
{
    uint32_t temp = 0;

    /* Check the parameters */
    assert_param(IS_FUNCTION_STATE(NewState));

    if (NewState != SH_DISABLE)
    {
        SYSCFG->DBGCR.V32 |= ((uint32_t)0x5AA5 << SYSCFG_DBGCR_LOCK_Pos) | ((uint32_t)0x01 << SYSCFG_DBGCR_DBG_STOP_Pos);
    }
    else
    {
        temp = SYSCFG->DBGCR.V32 & (~((uint32_t)0x01 << SYSCFG_DBGCR_DBG_STOP_Pos));
        SYSCFG->DBGCR.V32 = ((uint32_t)0x5AA5 << SYSCFG_DBGCR_LOCK_Pos) | temp;
    }
}

/**
  * @brief  GPIO module mount bus configuration register.
  * @param  GPIOB_Clock_Source: GPIO module mount bus select bit.
  *         This parameter can be one of the following values:
  *           @arg @b GPIOB_MODULE_CLOCK_IN_SCIOB
  *           @arg @b GPIOB_MODULE_CLOCK_IN_AHB
  * @retval None
  */
void SYSCFG_GPIOBCRConfig(GPIOB_MODULE_CLOCK_TypeDef GPIOB_Clock_Source)
{
    /* Check the parameters */
    assert_param(IS_GPIOB_CLOCK_SOURCE(GPIOB_Clock_Source));

    SYSCFG->GPIOBCR.V32 = ((uint32_t)0x5AA5 << 16) | GPIOB_Clock_Source;
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
