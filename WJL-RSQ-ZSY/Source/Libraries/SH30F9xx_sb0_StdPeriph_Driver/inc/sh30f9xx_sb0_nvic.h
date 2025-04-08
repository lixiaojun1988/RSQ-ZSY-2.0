/**
  ******************************************************************************
  * @file    sh30f9xx_sb0_nvic.h
  * @author  sinowealth
  * @version V1.0.0
  * @date    2024-5-24  
  * @brief   This file contains all the functions prototypes for the NVIC firmware
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
#ifndef __SH30F9XX_SB0_NVIC_H
#define __SH30F9XX_SB0_NVIC_H

#ifdef __cplusplus
extern "C"
{
#endif

/* Includes ------------------------------------------------------------------*/
#include "sh30f9xx_sb0_libcfg.h"

/** @addtogroup SH30F9XX_SB0_STANDARD_LIB
  * @{
  */
 
/** @addtogroup NVIC_MODULE NVIC
  * @{
  */

/* Exported types ------------------------------------------------------------*/
/** @defgroup NVIC_Exported_Types NVIC Exported Types
  * @{
  */

/**
  * @brief Structure for NVIC initial
  */     
typedef struct
{
    IRQn_Type NVIC_IRQChannel;                  /*!< Specifies the IRQ channel to be enabled or disabled.
                                                   This parameter can be an enumerator of @ref IRQn_Type 
                                                   enumeration (please refer to sh30f9xx_sb0.h file) */
                                                   
    uint8_t NVIC_IRQChannelPriority;            /*!< Specifies the priority for the IRQ channel.This parameter 
                                                   can be 0 to 3.The smaller the value, the higher the priority */

    FunctionalState NVIC_IRQChannelCmd;    /*!< Specifies whether the IRQ channel defined in NVIC_IRQChannel
                                                   will be enabled or disabled. This parameter can be set either 
                                                   to SH_ENABLE or SH_DISABLE */
} NVIC_InitTypeDef;

/**
  * @}
  */    
    
/* Exported constants --------------------------------------------------------*/

/** @defgroup NVIC_Exported_Constants NVIC Exported Constants
  * @{
  */

#define NVIC_VectTab_RAM               ((uint32_t)0x20000000)
#define NVIC_VectTab_FLASH             ((uint32_t)0x00000000)

/**
  * @}
  */   

  
/* Exported functions --------------------------------------------------------*/

/** @defgroup NVIC_Exported_Functions NVIC Exported Functions
  * @{
  */


extern void NVIC_StructInit(NVIC_InitTypeDef *NVIC_InitStruct);

extern void NVIC_Init(NVIC_InitTypeDef *NVIC_InitStruct);

extern void NVIC_Configuration(IRQn_Type IRQChannel, uint8_t ChannelPriority);

extern void NVIC_SetVectorTable(uint32_t NVIC_VectTab, uint32_t Offset);

extern void SysTick_TimeConfig(uint16_t MilliSecond);

extern uint32_t SysTick_GetCounter(void);
/**
  * @}
  */
#ifdef __cplusplus
}
#endif

#endif /*__SH30F9XX_SB0_NVIC_H */

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT Sinowealth *****END OF FILE****/
