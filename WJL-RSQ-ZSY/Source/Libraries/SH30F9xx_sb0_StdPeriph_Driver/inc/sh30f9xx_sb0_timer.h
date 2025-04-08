/**
  ******************************************************************************
  * @file    sh30f9xx_sb0_timer.h
  * @author  sinowealth
  * @version V1.0.0
  * @date    2024-05-21  
  * @brief   This file contains all the functions prototypes for the TIMER firmware
  *          library.
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
#ifndef __SH30F9XX_SB0_TIMER_H
#define __SH30F9XX_SB0_TIMER_H

#ifdef __cplusplus
extern "C"
{
#endif

/* Includes ------------------------------------------------------------------*/
#include "sh30f9xx_sb0_libcfg.h"

/** @addtogroup SH30F9XX_SB0_STANDARD_LIB
  * @{
  */

/** @addtogroup TIM_MODULE
  * @{
  */
  
  
/** @defgroup TIM_Exported_Types TIM Exported Types 
  * @{
  */
  

/** 
  * @brief   TIM  fliter time select parameter  
  */
typedef enum 
{
    TIM_ECF_FLITER_TIME_NONE   = (uint16_t)0x0000,/*!< Filter Time None */
    TIM_ECF_FLITER_TIME_8PCLK  = (uint16_t)0x1000,/*!< Filter Time 8 * PCLK */
    TIM_ECF_FLITER_TIME_16PCLK = (uint16_t)0x2000,/*!< Filter Time 16 * PCLK */
    TIM_ECF_FLITER_TIME_32PCLK = (uint16_t)0x3000 /*!< Filter Time 32 * PCLK */
} TIM_ECF_FLITER_Time_TypeDef;



/** 
  * @brief   TIM  clock source select parameter  
  */
typedef enum 
{
    TIM_CLKSource_PCLK  = (uint16_t)0x0000,/*!< Clock source is PCLK*/
    TIM_CLKSource_INPUT = (uint16_t)0x0010,/*!< Clock source is INPUT*/
    TIM_CLKSource_LSI_4 = (uint16_t)0x0020,/*!< Clock source is LSI/4*/
} TIM_CLK_Source_TypeDef;



/** 
  * @brief   TIM  OP mode select parameter  
  */
typedef enum 
{
    TIM_OPMode_Continue  = (uint16_t)0x0000,/*!< Timer Operation Mode Continue*/
    TIM_OPMode_OnePulse  = (uint16_t)0x0008 /*!< Timer Operation Mode OnePulse*/
} TIM_OP_Mode_TypeDef;




/** 
  * @brief   TIM  TC mode select parameter  
  */
typedef enum 
{
    TIM_TCMode_Disable   = (uint16_t)0x0000, /*!< Timer compare function disable */
    TIM_TCMode_Enable    = (uint16_t)0x0400  /*!< Timer compare function enable*/
} TIM_TC_Mode_TypeDef;

/** 
  * @brief   Timer Init structure definition  
  */
typedef struct
{
    uint32_t TIM_Prescaler;                         /*!< Specifie the TIM prescaler value.
                                                        This parameter can be 0 to 0xFFFF */

    uint32_t TIM_Period;                            /*!< Specifie the TIM period value.
                                                        This parameter can be 0 to 0xFFFF */

    TIM_CLK_Source_TypeDef TIM_CLKSource;      /*!< Specifie the TIM's clock sources.
                                                        This parameter can be a value of @ref TIM_CLK_Source_TypeDef */

    TIM_OP_Mode_TypeDef TIM_OPMode;            /*!< Specifie the TIM OP mode.
                                                        This parameter can be a value of @ref TIM_OP_Mode_TypeDef */

    TIM_TC_Mode_TypeDef TIM_TCMode;            /*!< Specifie the Compare output.
                                                        This parameter can be a value of @ref TIM_TC_Mode_TypeDef */

    TIM_ECF_FLITER_Time_TypeDef TIM_ECFFilter; /*!< Specifie the Compare output.
                                                        This parameter can be a value of @ref TIM_ECF_FLITER_Time_TypeDef */
} TIM_InitTypeDef;

/**
  * @}
  */



/** @defgroup TIM_Parameter_Check_Macro TIM Parameter Check Macro 
  * @{
  */

/**
  * @brief  Check is ECF pin filter time 
  */
#define IS_TIM_FILTER_TIMER(TIMER) ((TIMER == TIM_ECF_FILTER_TIME_NONE)     \
                                        || (TIMER == TIM_ECF_FILTER_TIME_8PCLK)  \
                                        || (TIMER == TIM_ECF_FILTER_TIME_16PCLK) \
                                        || (TIMER == TIM_ECF_FILTER_TIME_32PCLK))
/**
  * @brief  Check is TIM perioheral
  */                                    
#define IS_TIM_ALL_PERIPH(PERIPH)  ((PERIPH == (void *)TIM0) || (PERIPH == (void *)TIM1)  \
                                        || (PERIPH == (void *)TIM2) || (PERIPH == (void *)TIM3))                                   
  

/**
  * @brief  Check is TIM clock source  
  */
#define IS_TIM_CLK_SOURCE(SOURCE)  ((SOURCE == TIM_CLKSource_PCLK) \
                                        || (SOURCE == TIM_CLKSource_LSI_4) \
                                        || (SOURCE == TIM_CLKSource_INPUT))
                                    
/**
* @brief  Check is TIM operation mode  
*/                                   
#define IS_TIM_OP_MODE(MODE)       ((MODE == TIM_OPMode_Continue) \
                                        || (MODE == TIM_OPMode_OnePulse))

/**
  * @brief  Check is TIM compare mode 
  */
#define IS_TIM_TC_MODE(MODE)       ((MODE == TIM_TCMode_Enable) \
                                        || (MODE == TIM_TCMode_Disable))



/**
  * @}
  */
  
  

/* Exported types ------------------------------------------------------------*/

/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions --------------------------------------------------------*/

/** @addtogroup TIM_Exported_Functions
  * @{
  */
extern void TIM_Reset(void *TIMx);

extern void TIM_StructInit(TIM_InitTypeDef *TIM_InitStruct);

extern void TIM_Init(void *TIMx, TIM_InitTypeDef *TIM_InitStruct);

extern void TIM_INTConfig(void *TIMx, FunctionalState NewState);

extern void TIM_OnOff(void *TIMx, CmdState OnOffState);

extern void TIM_OutPutConfig(void *TIMx, FunctionalState NewState);

extern void TIM_CM0EventTrigger(void *TIMx, FunctionalState NewState);

extern void TIM_ExtPeripheralsTrigger(void *TIMx, FunctionalState NewState);

extern FlagStatus TIM_GetFlagStatus(void *TIMx);

extern void TIM_ClearFlag(void *TIMx);

extern uint32_t TIM_GetCounter(void *TIMx);

extern uint32_t TIM_GetPrescaler(void *TIMx);

extern uint32_t TIM_GetPeriod(void *TIMx);

extern void TIM_SetCounter(void *TIMx, uint32_t Counter);

extern void TIM_SetPrescaler(void *TIMx, uint32_t Prescaler);

extern void TIM_SetPeriod(void *TIMx, uint32_t Period);

#ifdef __cplusplus
}
#endif

#endif /*__SH30F9XX_SB0_TIMER_H */

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
