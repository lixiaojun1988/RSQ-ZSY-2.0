/**
  ******************************************************************************
  * @file    sh30f9xx_sb0_pwm.h
  * @author  sinowealth
  * @version V1.0.0
  * @date    2024-05-21  
  * @brief   This file contains all the functions prototypes for the PWM firmware

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
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __SH30f9xx_SB0_PWM_H
#define __SH30f9xx_SB0_PWM_H

#ifdef __cplusplus
extern "C"
{
#endif

/* Includes ------------------------------------------------------------------*/
#include "sh30f9xx_sb0_libcfg.h"

/** @addtogroup SH30F9XX_SB0_STANDARD_LIB
  * @{
  */
    
/** @addtogroup PWM_MODULE PWM
  * @{
  */

/** @defgroup PWM_Exported_Types PWM Exported Types
  * @{
  */
    
/** 
  * @brief PWM CLOCK 
  */    

typedef enum
{
    TCK_PCLK0_DIV1   = 0,   /*!< PCLK0 / 1   */
    TCK_PCLK0_DIV2   = 1,   /*!< PCLK0 / 2   */
    TCK_PCLK0_DIV4   = 2,   /*!< PCLK0 / 4   */
    TCK_PCLK0_DIV8   = 3,   /*!< PCLK0 / 8   */
    TCK_PCLK0_DIV16  = 4,   /*!< PCLK0 / 16  */
    TCK_PCLK0_DIV32  = 5,   /*!< PCLK0 / 32  */
    TCK_PCLK0_DIV64  = 6,   /*!< PCLK0 / 64  */
    TCK_PCLK0_DIV256 = 7,   /*!< PCLK0 / 256 */
} PWM_CLKDIV_TypeDef;    

/** 
  * @brief PWM fault output level 
  */    

typedef enum
{
    FLT_OUTPUT_HIGH_Z = 0,   /*!< high resistance state   */
    FLT_OUTPUT_LOW    = 2,   /*!< low level   */
    FLT_OUTPUT_HIGH   = 3,   /*!< high level   */
} PWM_FLTOutLevel_TypeDef;    

/**
 * @brief     PWM INT
 */
typedef enum
{
    PWM_INT_OVERFLOW = (uint16_t)PWM_CR_PWMIE_Msk,
    PWM_INT_FAULT    = (uint16_t)PWM_CR_EFLTIE_Msk,
} PWM_INT_TypeDef;

/**
 * @brief     PWM_CHANNEL
 */
typedef enum
{
    PWM_CHANNEL_A = (uint16_t)PWM_CR_PWMSA_Msk,
    PWM_CHANNEL_B = (uint16_t)PWM_CR_PWMSB_Msk,
} PWM_CHANNEL_TypeDef;

/**
  * @brief Structure for PWM initial
  */  
typedef struct
{
    PWM_CLKDIV_TypeDef PWM_CLKSource;  /*!< Specifie the PWM's clock sources.
                                                 This parameter can be 0 to 0x07*/
    uint16_t PWM_Period;                    /*!< Specifie the PWM period value. 
                                                This parameter can be 0 to 0xFFFF */
    uint16_t PWM_Duty;                      /*!< Specifie the PWM duty value. 
                                                 This parameter can be 0 to 0xFFFF */
    uint16_t PWM_DeadBand;                  /*!< Specifie the PWM dead band value. 
                                                 This parameter can be 0 to 0xFFFF */
} PWM_InitTypeDef;    
    
/**
  * @}
  */   

/* Exported constants --------------------------------------------------------*/

/** @defgroup PWM_Exported_Constants PWM Exported Constants
  * @{
  */

/**
  * @}
  */
  
/* Check Macro  ------------------------------------------------------------*/

/** @defgroup PWM_Parameter_Check_Macro PWM Parameter Check Macro
  * @{
  */

#define IS_PWM_INT(INT)            (((INT & (uint16_t)(~(PWM_CR_PWMIE_Msk|PWM_CR_EFLTIE_Msk))) == 0x0000) \
                                        && (INT != 0x0000))
                                        
#define IS_PWM_CHANNEL(CHANNEL)    (((CHANNEL & (uint16_t)(~(PWM_CR_PWMSA_Msk|PWM_CR_PWMSB_Msk))) == 0x0000) \
                                        && (CHANNEL != 0x0000))                                        

/*!  check pwm clock source */
#define IS_PWMCLKSOURCE_DIV(div)   (((div) == TCK_PCLK0_DIV1)    \
                                        || ((div) == TCK_PCLK0_DIV2)  \
                                        || ((div) == TCK_PCLK0_DIV4)  \
                                        || ((div) == TCK_PCLK0_DIV8) \
                                        || ((div) == TCK_PCLK0_DIV16)  \
                                        || ((div) == TCK_PCLK0_DIV32) \
                                        || ((div) == TCK_PCLK0_DIV64) \
                                        || ((div) == TCK_PCLK0_DIV256))

/*!  check fault output level  */
#define IS_PWMFLTOutLevel(level)   (((level) == FLT_OUTPUT_HIGH_Z)    \
                                        || ((level) == FLT_OUTPUT_LOW)  \
                                        || ((level) == FLT_OUTPUT_HIGH))
                                        
#define IS_PWM_ALL_PERIPH(PERIPH)  ((PERIPH == PWM0) || (PERIPH == PWM1) \
                                        || (PERIPH == PWM2) || (PERIPH == PWM3))  
/*! check PWM module pointer */
#define IS_PWM_MODULE(m)           (m == PWM0 || m == PWM1 || m == PWM2 || m == PWM3)

/**
  * @}
  */

/* Export Function Macro ------------------------------------------------------------*/
/** @defgroup PWM_Export_Func_Macro PWM Export Func Macro
  * @{
  */
  
#define PWM_UNLOCK()                    (PWMx->PWMLOCK = 0x5AA5)
#define PWM_LOCK()                      (PWMx->PWMLOCK = 0) 

/**
  * @}
  */

/* Exported functions --------------------------------------------------------*/

/** @defgroup PWM_Exported_Functions PWM Exported Functions
  * @{
  */

extern void PWM_Reset(PWM_TypeDef *PWMx);

extern void PWM_StructInit(PWM_InitTypeDef *PWM_InitStruct);

extern void PWM_Init(PWM_TypeDef *PWMx, PWM_InitTypeDef *PWM_InitStruct);

extern void PWM_OnOff(PWM_TypeDef *PWMx, CmdState OnOffState);

extern void PWM_SetCycle(PWM_TypeDef *PWMx, uint16_t prescale);

extern void PWM_SetDuty(PWM_TypeDef *PWMx, uint16_t duty);

extern void PWM_FaultPinLevel(PWM_TypeDef *PWMx, LevelStatus levelState);

extern void PWM_FaultPinFunction(PWM_TypeDef *PWMx, FunctionalState NewState);

extern FlagStatus PWM_GetIntFlagStatus(PWM_TypeDef *PWMx);

extern void PWM_ClearIntFlag(PWM_TypeDef *PWMx);

extern FlagStatus PWM_GetFLTFlagStatus(PWM_TypeDef *PWMx);

extern void PWM_ClearFLTFlag(PWM_TypeDef *PWMx);

extern void PWM_INTConfig(PWM_TypeDef *PWMx, PWM_INT_TypeDef PWMIntSource, FunctionalState NewState);

extern void PWM_OutputMode(PWM_TypeDef *PWMx, PWM_CHANNEL_TypeDef PWM_Channel, LevelStatus levelState);

extern FlagStatus PWM_GetFLTSStatus(PWM_TypeDef *PWMx);

extern void PWM_ClearFLTSStatus(PWM_TypeDef *PWMx);

extern void PWM_FaultPinDebounce(PWM_TypeDef *PWMx, uint8_t Debtime);

extern void PWM_FLTOutputMode(PWM_TypeDef *PWMx, PWM_CHANNEL_TypeDef PWM_Channel, PWM_FLTOutLevel_TypeDef Pinlevel);

/**
  * @}
  */


#ifdef __cplusplus
}
#endif

#endif /*__SH30F9XX_SB0_PWM_H */

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT Sinowealth *****END OF FILE****/
