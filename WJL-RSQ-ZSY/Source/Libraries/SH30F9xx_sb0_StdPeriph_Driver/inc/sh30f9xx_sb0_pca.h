/**
  ******************************************************************************
  * @file    sh30f9xx_sb0_pca.h
  * @author  sinowealth
  * @version V1.0.0
  * @date    2024-5-24  
  * @brief   This file contains all the functions prototypes for the PCA firmware
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
#ifndef __SH30F9XX_SB0_PCA_H
#define __SH30F9XX_SB0_PCA_H

#ifdef __cplusplus
 extern "C" 
 {
#endif

/* Includes ------------------------------------------------------------------*/
#include "sh30f9xx_sb0_libcfg.h"

/** @addtogroup SH30F9XX_SB0_STANDARD_LIB
  * @{
  */

/** @addtogroup PCA_MODULE PCA
  * @{
  */

/** @defgroup PCA_Exported_Types PCA Exported Types
  * @{
  */

/**   
  * @brief PCA_Clock_Source
  */      
typedef enum 
{
    PCLK0            =    0,     /*!< Pclock0   */
    ECIxNEGATIVELY   =    1,     /*!< Ecix falling edge */
    LSICLK           =    2,     /*!< LSI 128K RC */
    LSECLK           =    3,     /*!< LSE 32K */
    EXTHIGHOSCDIV8   =    4,     /*!< HSE DIV8 */
    TIM0OVERFLOW     =    5,     /*!< Timer 0 overflow */
} PCA_CLOCK_SOURCE_TypeDef;

/**   
  * @brief PCA_Counter_Mode
  */ 
typedef enum 
{
    SINGLEEDGE      =    0,     /*!< The counter counts up */
    DOUBLEEDGE      =    1,     /*!< Counter middle alignment */
} PCA_COUNTERMODE_TypeDef;

/**   
  * @brief PCA_Filtering_Time only in mode0
  */ 
typedef enum 
{
    FILTERING_NONE        =    0,     /*!< No filtering function */
    FILTERING_8PCLK0      =    1,     /*!< The filtering time is 8*PCLK0 */
    FILTERING_16PCLK0     =    2,     /*!< The filtering time is 16*PCLK0 */
    FILTERING_32PCLK0     =    3,     /*!< The filtering time is 32*PCLK0 */
} PCA_FILTERING_TIMER_TypeDef;

/**   
  * @brief PCA_Mode
  */ 
typedef enum 
{
    CAPTUREMODE      =    0,     /*!< capture mode   */
    COMPAREMODE      =    1,     /*!< compare mode */
    FREQOUTPUTMODE   =    2,     /*!< Frequency output */
    PWMMODE          =    3,     /*!< PWM output */
} PCA_MODEL_TypeDef;

/**   
  * @brief PCA_Capture_Mode
  */ 
typedef enum 
{
    RISING_EDGE_TRIGGER      =    0,     /*!< Rising edge trigger    */
    FALLING_EDGE_TRIGGER     =    2,     /*!< Falling edge trigger   */
    ARBITRARY_EDGE_TRIGGER   =    3,     /*!< Arbitrary edge trigger */
} PCA_CAPMODE_TypeDef;

/**   
  * @brief PCA_Compare_Mode
  */ 
typedef enum 
{
    CONTINUOUS      =    0,     /*!< Continuous compare */
    SINGLE          =    2,     /*!< Single compare */
} PCA_COMPAREMODE_TypeDef;

/**   
  * @brief PCA_Init_Output_Level
  */ 
typedef enum 
{
    INITAL_HIGH       =    0,     /*!< The initial level is high */
    INITAL_LOW        =    1,     /*!< The initial level is low */
} PCA_INITAL_LEVEL_TypeDef;

/**   
  * @brief PCA_PWM_Mode
  */ 
typedef enum 
{
    PWM_8BIT             =    0,     /*!< 8 bit PWM  */
    PWM_16BIT            =    1,     /*!< 16 bit PWM  */
    PHASE_PWM_16BIT      =    2,     /*!< 16 bit phase correction PWM */
    PHASEFREQ_PWM_16BIT  =    3,     /*!< 16 bit phase frequency correction PWM */
} PCA_PWMMODEL_TypeDef;

/**   
  * @brief PCA_PWM_Active_Level
  */ 
typedef enum  
{
    ACTIVE_LOW         =    0,     /*!< The active level is low */
    ACTIVE_HIGH        =    1,     /*!< The active level is high */
} PCA_ACTIVE_LEVEL_TypeDef;

/**   
  * @brief PCA_Channel
  */
typedef enum 
{
    PCA_CHANNEL_A      =    0,     /*!< PCA Channel A */
    PCA_CHANNEL_B      =    1,     /*!< PCA Channel B */
} PCA_CHANNEL_TypeDef;

/**
  * @brief Structure for CFGR register
  */ 
typedef struct
{
    uint32_t ClkSel                   : 3;  /*!< CLOCK SELECT*/
    uint32_t rev1                     : 3;  /*!< reserved*/
    uint32_t PCACounterMode           : 1;  /*!< counts up or  Counter middle alignment */
    uint32_t rev2                     : 5;  /*!< reserved */
    uint32_t CCR0UPDATAEN             : 1;  /*!< Comparison module 0 immediately updates the enable bit */
    uint32_t CCR1UPDATAEN             : 1;  /*!< Comparison module 1 immediately updates the enable bit */
    uint32_t rev3                     : 1;  /*!< reserved*/
    uint32_t PUEUPDATAEN              : 1;  /*!< PCA The cycle value immediately updates the enable bit */
    uint32_t ECFSel                   : 2;  /*!< Input signal filtering time (only valid in mode0 mode) */
    uint32_t rev4                     : 14; /*!< reserved*/
} PCA_CFGR_TypeDef;

/**
  * @brief Structure for CCMR register
  */ 
typedef struct
{
    uint32_t rev5                     : 2;  /*!< reserved */
    uint32_t PCA_TCP                  : 1;  /*!< PCA edge signale indicate*/    
    uint32_t PCA_CEN                  : 1;  /*!< Compare OR capture module function: SH_ENABLE or SH_DISABLE*/    
    uint32_t FunctionSelect           : 2;  /*!< PCA secondary mode select*/    
    uint32_t ModeSelect               : 2;  /*!< PCA mode select:CAPTUREMODE,COMPARE,FREQOUTPUTMODE,PWMMODE*/    
    uint32_t PCA_CC                   : 1;  /*!< Clear counter enable bit when capture occurs*/
    uint32_t rev6                     : 23; /*!< reserved*/
} PCA_CCMR_TypeDef;


/**
  * @brief Structure for PCA initial
  */ 
typedef struct
{
    /* for CFGR register */
    PCA_CFGR_TypeDef PCA_CFGR;

    /* for FORCE register */
    uint32_t PCA_FORCE;

    /* for CNT register */
    uint32_t PCA_CNT;

    /* for PSC register */
    uint32_t PCA_PSC;

    /* for PR register */
    uint32_t PCA_PR;
    
    /* for CPM0 register */
    PCA_CCMR_TypeDef PCA_CCMR0;

    /* for CPM1 register */
    PCA_CCMR_TypeDef PCA_CCMR1;
    
    /* for CPR0 register */
    uint32_t PCA_CCR0;

    /* for CPR1 register */
    uint32_t PCA_CCR1;

} PCA_InitTypeDef;

/**
  * @}
  */

/**   
  * @brief PCA_Flags
  */ 
typedef enum
{
    PCA_FLAG_CC0IF = (uint16_t)PCA_SR_CC0IF_Msk,           /*!< */
    PCA_FLAG_CC1IF = (uint16_t)PCA_SR_CC1IF_Msk,           /*!< */
    PCA_FLAG_PIF   = (uint16_t)PCA_SR_PIF_Msk,             /*!< */
    PCA_FLAG_CIF   = (uint16_t)PCA_SR_CIF_Msk,             /*!< */
} PCA_FLAG_TypeDef;

/**   
  * @brief PCA Interrupt
  */
typedef enum
{
    PCA_COUNTER_OVERFLOW_INT = (uint16_t)PCA_CFGR_CIE_Msk, /*!< */
    PCA_COUNTER_CYCLE_INT    = (uint16_t)PCA_CFGR_PIE_Msk, /*!< */
} PCA_INT_TypeDef;

/* Exported constants --------------------------------------------------------*/

/** @defgroup PCA_Exported_Constants PCA Exported Constants
  * @{
  */

/**
  * @}
  */

/* Check Macro ------------------------------------------------------------*/

/** @defgroup PCA_Parameter_Check_Macro PCA Parameter Check Macro
  * @{
  */

/*!  check PCA Interrupt flag */
#define IS_PCA_GET_ONE_FLAG(FLAG)         ((FLAG == PCA_FLAG_CC0IF)   \
                                          ||(FLAG == PCA_FLAG_CC1IF)  \
                                          ||(FLAG == PCA_FLAG_PIF)    \
                                          ||(FLAG == PCA_FLAG_CIF))
                                     
/*!  check PCA Interrupt SOURCE */
#define IS_PCA_INT_SOURCE(SOURCE)         ((SOURCE == PCA_COUNTER_OVERFLOW_INT) \
                                          || (SOURCE == PCA_COUNTER_CYCLE_INT))                                     
                                     
                                     

/*!  check PCA clock source */
#define IS_PCACLKSOURCE_DIV(div)          (((div) == PCLK0)              \
                                          || ((div) == ECIxNEGATIVELY)   \
                                          || ((div) == LSICLK)           \
                                          || ((div) == LSECLK)           \
                                          || ((div) == EXTHIGHOSCDIV8)   \
                                          || ((div) == TIM0OVERFLOW))

/*!  check PCA Counter Mode */
#define IS_PCACOUNTERMODE_DIV(div)        (((div) == SINGLEEDGE) || ((div) == DOUBLEEDGE))


/*!  check PCA filtering time Mode */
#define IS_FILTERING_TIMER_TYPE(type)     (((type) == FILTERING_NONE)      \
                                          || ((type) == FILTERING_8PCLK0)   \
                                          || ((type) == FILTERING_16PCLK0)  \
                                          || ((type) == FILTERING_32PCLK0))

/*!  check PCA Mode */
#define IS_PCAMODE_DIV(div)               (((div) == CAPTUREMODE)        \
                                          || ((div) == COMPAREMODE)      \
                                          || ((div) == FREQOUTPUTMODE)   \
                                          || ((div) == PWMMODE)) 

/*!  check PCA Capture Mode */
#define IS_PCACAPMODE_DIV(div)            (((div) == RISING_EDGE_TRIGGER)        \
                                          || ((div) == FALLING_EDGE_TRIGGER)     \
                                          || ((div) == ARBITRARY_EDGE_TRIGGER)) 


/*!  check PCA compare Mode */
#define IS_PCACOMPAREMODE_DIV(div)        (((div) == CONTINUOUS) || ((div) == SINGLE))



/*!  check PCA initial level */
#define IS_INITAL_LEVEL(level)            (((level) == INITAL_HIGH) || ((level) == INITAL_LOW))



/*!  check PCA PWM Mode */
#define IS_PWMMODEL_DIV(div)              (((div) == PWM_8BIT)           \
                                          || ((div) == PWM_16BIT)        \
                                          || ((div) == PHASE_PWM_16BIT)  \
                                          || ((div) == PHASEFREQ_PWM_16BIT)) 


/*!  check PCA active level */
#define IS_ACTIVE_LEVEL(level)            (((level) == ACTIVE_LOW) || ((level) == ACTIVE_HIGH))



/*!  check PCA Channel */
#define IS_PCACHANEEL_CHANNEL(channel)    (((channel) == PCA_CHANNEL_A) \
                                          || ((channel) == PCA_CHANNEL_B))

/*!  check PCA peripheral */
#define IS_PCA_ALL_PERIPH(PERIPH)         ((PERIPH == PCA0)   \
                                          || (PERIPH == PCA1) \
                                          || (PERIPH == PCA2) \
                                          || (PERIPH == PCA3))

/*!  check PCA cascade peripheral */
#define IS_PCA_CASCADE_PERIPH(PERIPH)     ((PERIPH == PCA0) || (PERIPH == PCA2))

/**
  * @}
  */ 

/* Export Function Macro ------------------------------------------------------------*/
/** @defgroup PCA_Export_Func_Macro PCA Export Func Macro
  * @{
  */

#define PCA_UNLOCK()                    (PCAx->LCKR = 0x33CC)
#define PCA_LOCK()                      (PCAx->LCKR = 0) 

/**
  * @}
  */
  
/* Exported functions --------------------------------------------------------*/

/** @defgroup PCA_Exported_Functions PCA Exported Functions
  * @{
  */
  
extern void PCA_Reset(PCA_TypeDef *PCAx);

extern void PCA_StructInit(PCA_InitTypeDef *PCA_InitStruct);
    
extern void PCA_Init(PCA_TypeDef *PCAx, const PCA_InitTypeDef *PCA_InitStruct);

extern void PCA_ClearFlag(PCA_TypeDef *PCAx, PCA_FLAG_TypeDef PCA_IntFlag);

extern void PCA_ClearAllFlag(PCA_TypeDef *PCAx);

extern void PCA_OnOff(PCA_TypeDef *PCAx, CmdState OnOffState);

extern void PCA_CascadeOnOff(PCA_TypeDef *PCAx, CmdState OnOffState);

extern void PCA_FreqOutValue(PCA_TypeDef *PCAx, PCA_CHANNEL_TypeDef PCAChannel, uint8_t Value);

extern void PCA_ForceMatchControl(PCA_TypeDef *PCAx, PCA_CHANNEL_TypeDef PCAChannel, CmdState OnOffState);

extern void PCA_SetPRValue(PCA_TypeDef *PCAx, uint32_t Value);

extern void PCA_SetCCRValue(PCA_TypeDef *PCAx, PCA_CHANNEL_TypeDef PCAChannel, uint32_t Value);

extern uint32_t PCA_GetCaptureValue(PCA_TypeDef *PCAx,PCA_CHANNEL_TypeDef PCAChannel);

extern FlagStatus PCA_GetFlagStatus(PCA_TypeDef *PCAx, PCA_FLAG_TypeDef PCA_IntFlag);

extern FlagStatus PCA_GetCapterLevelStatus(PCA_TypeDef *PCAx, PCA_CHANNEL_TypeDef PCAChannel);

extern void PCA_INTConfig(PCA_TypeDef *PCAx, PCA_INT_TypeDef IntSource, FunctionalState NewState);

extern void PCA_CapINTConfig(PCA_TypeDef *PCAx, PCA_CHANNEL_TypeDef PCAChannel, FunctionalState NewState);

/**
  * @}
  */
#ifdef __cplusplus
}
#endif

/**
  * @}
  */ 

/**
  * @}
  */ 
#endif /*__SH30F9XX_SB0_PCA_H */
/************************ (C) COPYRIGHT Sinowealth *****END OF FILE****/


