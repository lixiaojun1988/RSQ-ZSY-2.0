/**
  ******************************************************************************
  * @file    sh30f9xx_sb0_crc.h
  * @author  sinowealth
  * @version V1.0.0
  * @date    2024-5-28 
  * @brief   This file contains all the functions prototypes for the CRC firmware
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
#ifndef __SH30F9XX_SB0_CRC_H
#define __SH30F9XX_SB0_CRC_H

#ifdef __cplusplus
extern "C"
{
#endif

/* Includes ------------------------------------------------------------------*/
#include "sh30f9xx_sb0_libcfg.h"

/** @addtogroup SH30F9XX_SB0_STANDARD_LIB
  * @{
  */

/** @addtogroup CRC_MODULE CRC
  * @{
  */

/** @defgroup CRC_Exported_Types CRC Exported Types
  * @{
  */
    

/*!  CRC Mode CONSTANT */
typedef enum 
{
    CRC_MODE_32     =   0,     /*!< 32 bits CRC (POLY: 04C11DB7) */
    CRC_MODE_16     =   1,     /*!< 16 bits CRC (POLY: 8005) */
    CRC_MODE_CITT   =   2,     /*!< 16 bits CRC (POLY: 1021) */
    CRC_MODE_8      =   3,     /*!< 8 bits CRC (POLY: 7) */
} CRC_MODE_TypeDef;

/*!  CRC Input Mode CONSTANT */
typedef enum 
{
    CRC_INPUT_WORD      =   0,  /*!< One Word One Calculate */
    CRC_INPUT_HALFWORD  =   1,  /*!< Half Word One Calculate */
    CRC_INPUT_BYTE      =   2,  /*!< One Byte One Calculate */
} CRC_INPUT_TypeDef;

/**  
  * @brief Structure for CRC initial
  */    
typedef struct
{   
    uint32_t Reload           :1;   /*!< Reload the Initial value to calculate unit: SH_ENABLE or SH_DISABLE */
    uint32_t rev1             :7;   /*!< reserved */
    uint32_t Mode             :2;   /*!< POLY mode selection: CRC_MODE_32, CRC_MODE_16,CRC_MODE_CITT,CRC_MODE_8*/
    uint32_t ComplementInput  :1;   /*!< complement the input value  then to calculate : SH_ENABLE or SH_DISABLE*/
    uint32_t BitRevertInput   :1;   /*!< reverse the input value by bit then to calculate: SH_ENABLE or SH_DISABLE */
    uint32_t ByteRevertInput  :1;   /*!< reverse the input value by byte then to calculate: SH_ENABLE or SH_DISABLE */
    uint32_t ComplementOutput :1;   /*!< complement the result value : SH_ENABLE or SH_DISABLE */
    uint32_t BitRevertOutput  :1;   /*!< reverse the result value by bit : SH_ENABLE or SH_DISABLE  */
    uint32_t ByteRevertOutput :1;   /*!< reverse the result value by byte : SH_ENABLE or SH_DISABLE */
    uint32_t rev              :16;  /*!< reserved for address*/
    
    uint32_t InitialVal;            /*!< Inital Value */
} CRC_InitTypeDef;

/**
  * @}
  */

/* Check Macro ------------------------------------------------------------*/

/** @defgroup CRC_Parameter_Check_Macro CRC Parameter Check Macro
  * @{
  */

/*!  check CRC mode selection */
#define IS_CRC_MODE(mode)          (((mode) == CRC_MODE_32)     \
                                        || ((mode) == CRC_MODE_16)   \
                                        || ((mode) == CRC_MODE_CITT) \
                                        || ((mode) == CRC_MODE_8))

/*!  check CRC input mode selection*/
#define IS_CRC_INPUTTYPE(type)     (((type) == CRC_INPUT_WORD)   \
                                        || ((type) == CRC_INPUT_HALFWORD)   \
                                        || ((type) == CRC_INPUT_BYTE))

/**
  * @}
  */

/* Export Function Macro ------------------------------------------------------------*/
/** @defgroup CRC_Export_Func_Macro CRC Export Func Macro
  * @{
  */

/*! get last CRC value */
#define CRC_GetCRC()                (CRC->DR)

/*! reload CRC init value */
#define CRC_Reload()                (CRC->CR.BIT.RELOAD = SH_SET)

/**
  * @}
  */

/* Exported functions --------------------------------------------------------*/

/** @defgroup CRC_Exported_Functions CRC Exported Functions
  * @{
  */  

extern void CRC_Reset(void);

extern void CRC_Init(CRC_InitTypeDef *CRC_InitStruct);

extern void CRC_StructInit(CRC_InitTypeDef *CRC_InitStruct);

extern uint32_t CRC_CalcCRC(uint32_t Data, CRC_INPUT_TypeDef InputType);

extern uint32_t CRC_CalcBlockCRC(const uint8_t *InBuffer, uint32_t Count, CRC_INPUT_TypeDef InputType);

/**
  * @}
  */

#ifdef __cplusplus
}
#endif

#endif /*__SH30F9XX_SB0_CRC_H */

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT Sinowealth *****END OF FILE****/
