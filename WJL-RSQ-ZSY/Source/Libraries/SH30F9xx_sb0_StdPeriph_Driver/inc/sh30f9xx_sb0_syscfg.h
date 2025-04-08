/**
  ******************************************************************************
  * @file    sh30f9xx_sb0_syscfg.h
  * @author  sinowealth
  * @version V1.0.0
  * @date    2024-05-21  
  * @brief   This file contains all the functions prototypes for the SYSCFG firmware
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
#ifndef __SH30F9XX_SB0_SYSCFG_H
#define __SH30F9XX_SB0_SYSCFG_H

#ifdef __cplusplus
extern "C"
{
#endif

/* Includes ------------------------------------------------------------------*/
#include "sh30f9xx_sb0_libcfg.h"

/** @addtogroup SH30F9XX_SB0_STANDARD_LIB
  * @{
  */

/** @addtogroup SYSCFG_MODULE
  * @{
  */

/** @defgroup SYSCFG_Exported_Types SYSCFG Exported Types 
  * @{
  */
  
typedef enum
{
    SYSCFG_VLVR_Level_4V1 = 0x00,/*!< LVR Level  4.1 V */
    SYSCFG_VLVR_Level_3V7 = 0x01,/*!< LVR Level  3.7 V */
    SYSCFG_VLVR_Level_3V1 = 0x02,/*!< LVR Level  3.1 V */
} SYSCFG_VLVR_LEVEL_TypeDef;  


typedef enum
{
    SYSCFG_BOD_Mode_Rising = 0x00,         /*!< BOD detect Rising */
    SYSCFG_BOD_Mode_Falling = 0x01,        /*!< BOD detect Falling */    
    SYSCFG_BOD_Mode_RisingFalling = 0x02,  /*!< BOD detect Rising and Falling */
} SYSCFGBODMode_TypeDef; 


typedef enum
{
    SYSCFG_BOD_Level_2V85 = 0x03, /*!< BOD  Level  2.85 V */
    SYSCFG_BOD_Level_3V00 = 0x04, /*!< BOD  Level  3.00 V */
    SYSCFG_BOD_Level_3V15 = 0x05, /*!< BOD  Level  3.15 V */
    SYSCFG_BOD_Level_3V30 = 0x06, /*!< BOD  Level  3.30 V */
    SYSCFG_BOD_Level_3V45 = 0x07, /*!< BOD  Level  3.45 V */
    SYSCFG_BOD_Level_3V60 = 0x08, /*!< BOD  Level  3.60 V */
    SYSCFG_BOD_Level_3V75 = 0x09, /*!< BOD  Level  3.75 V */
    SYSCFG_BOD_Level_3V90 = 0x0A, /*!< BOD  Level  3.90 V */
    SYSCFG_BOD_Level_4V05 = 0x0B, /*!< BOD  Level  4.05 V */
    SYSCFG_BOD_Level_4V20 = 0x0C, /*!< BOD  Level  4.20 V */
    SYSCFG_BOD_Level_4V35 = 0x0D, /*!< BOD  Level  4.35 V */
    SYSCFG_BOD_Level_4V50 = 0x0E, /*!< BOD  Level  4.50 V */
} SYSCFGBODLevel_TypeDef; 


typedef enum
{
    SYSCFG_BOD_FLAG_BODIF = (uint8_t)0x01, /*!< BOD Interrupt Flag */
    SYSCFG_BOD_FLAG_BODF  = (uint8_t)0x02 /*!< BOD Flag */
} SYSCFG_BOD_FLAG_TypeDef;

typedef enum
{
    SYSCFG_SWJ_All_Function = 0x00,  /*!< SWJ  All Function*/
    SYSCFG_SWD_CLOSE_PinRemap = 0x01,/*!< SWJ  Close */
} SYSCFG_SWJCFG_TypeDef;


typedef enum
{
    SYSCFG_OSC_GPIO = 0x00,    /*!< OSC  Pin GPIO Function */
    SYSCFG_OSC_HCrystal = 0x01,/*!< OSC  Pin Cystal*/
    SYSCFG_OSC_ExtClock = 0x02,/*!< OSC  Pin  External Clock*/
} SYSCFG_OSCPin_TypeDef;






typedef enum
{
    SYSCFG_SLEEPEntry_WFI  = (uint8_t)0x00,/*!< Sleep wakeup WFI*/
    SYSCFG_SLEEPEntry_WFE  = (uint8_t)0x01 /*!< Sleep wakeup WFE*/
} SYSCFG_SLEEPEntry_TypeDef;


typedef enum
{
    SYSCFG_STOPEntry_WFI  = (uint8_t)0x00,/*!< Stop wakeup WFI*/
    SYSCFG_STOPEntry_WFE  = (uint8_t)0x01 /*!< Stop wakeup WFE*/
} SYSCFG_STOPEntry_TypeDef;


typedef enum
{
    SYSCFG_NMI_TRIGGER_CSM   = (uint16_t)0x0100,/*!< NMI Trigger Source  CSM*/
    SYSCFG_NMI_TRIGGER_BOD   = (uint16_t)0x0080,/*!< NMI Trigger Source  BOD*/
    SYSCFG_NMI_TRIGGER_EXTI0 = (uint16_t)0x0040 /*!< NMI Trigger Source  EXTI0*/
} SYSCFG_NMI_TRIGGER_TypeDef;                      


typedef enum
{
    SRAM_Sector_0        = (uint16_t)0x0001, /*!< Sram Sector 0*/
    SRAM_Sector_1        = (uint16_t)0x0002, /*!< Sram Sector 1*/
    SRAM_Sector_2        = (uint16_t)0x0004, /*!< Sram Sector 2*/
    SRAM_Sector_3        = (uint16_t)0x0008, /*!< Sram Sector 3*/
    SRAM_Sector_4        = (uint16_t)0x0010, /*!< Sram Sector 4*/
    SRAM_Sector_5        = (uint16_t)0x0020, /*!< Sram Sector 5*/
    SRAM_Sector_6        = (uint16_t)0x0040, /*!< Sram Sector 6*/
    SRAM_Sector_7        = (uint16_t)0x0080, /*!< Sram Sector 7*/
    SRAM_Sector_8        = (uint16_t)0x0100, /*!< Sram Sector 8*/
    SRAM_Sector_9        = (uint16_t)0x0200, /*!< Sram Sector 9*/
    SRAM_Sector_10       = (uint16_t)0x0400, /*!< Sram Sector 10*/
    SRAM_Sector_11       = (uint16_t)0x0800, /*!< Sram Sector 11*/
    SRAM_Sector_12       = (uint16_t)0x1000, /*!< Sram Sector 12*/
    SRAM_Sector_13       = (uint16_t)0x2000, /*!< Sram Sector 13*/
    SRAM_Sector_14       = (uint16_t)0x4000, /*!< Sram Sector 14*/
    SRAM_Sector_15       = (uint16_t)0x8000  /*!< Sram Sector 15*/
} SRAM_Sector_TypeDef;


typedef enum
{
    DBG_Periph_WWDT   = (uint16_t)0x0080,/*!< SYSCFG DBG  Peripheral WWDT*/
    DBG_Periph_PCA    = (uint16_t)0x0100,/*!< SYSCFG DBG  Peripheral PCA*/
    DBG_Periph_TIM    = (uint16_t)0x0200,/*!< SYSCFG DBG  Peripheral TIM*/
    DBG_Periph_PWM    = (uint16_t)0x0400,/*!< SYSCFG DBG  Peripheral PWM*/
    DBG_Periph_UART   = (uint16_t)0x0800,/*!< SYSCFG DBG  Peripheral UART*/
    DBG_Periph_SPI    = (uint16_t)0x1000,/*!< SYSCFG DBG  Peripheral SPI*/
    DBG_Periph_TWI    = (uint16_t)0x2000 /*!< SYSCFG DBG  Peripheral TWI*/
} DBG_Periph_TypeDef;


typedef enum
{
    GPIOB_MODULE_CLOCK_IN_SCIOB = (uint8_t)0x00,/*!< GPIO Clock in SCIOB BUS*/
    GPIOB_MODULE_CLOCK_IN_AHB   = (uint8_t)0x01 /*!< GPIO Clock in AHB BUS*/
} GPIOB_MODULE_CLOCK_TypeDef;

/**
  * @}
  */


/** @defgroup SYSCFG_Parameter_Check_Macro SYSCFG Parameter Check Macro 
  * @{
  */

/**
  * @brief  Check is LVR level
  */
#define IS_SYSCFG_VLVR_LEVEL(LEVEL)      ((LEVEL == SYSCFG_VLVR_Level_4V1) \
                                              || (LEVEL == SYSCFG_VLVR_Level_3V7) \
                                              || (LEVEL == SYSCFG_VLVR_Level_3V1))

   
/**
  * @brief  Check is BOD mode
  */
#define IS_SYSCFG_BOD_MODE(MODE)         ((MODE == SYSCFG_BOD_Mode_Rising) \
                                              || (MODE == SYSCFG_BOD_Mode_Falling) \
                                              || (MODE == SYSCFG_BOD_Mode_RisingFalling))


/**
  * @brief  Check is BOD level
  */
#define IS_SYSCFG_BOD_LEVEL(BOD_LEVEL)   ((BOD_LEVEL == SYSCFG_BOD_Level_2V85)   \
                                              || (BOD_LEVEL == SYSCFG_BOD_Level_3V00) \
                                              || (BOD_LEVEL == SYSCFG_BOD_Level_3V15) \
                                              || (BOD_LEVEL == SYSCFG_BOD_Level_3V30) \
                                              || (BOD_LEVEL == SYSCFG_BOD_Level_3V45) \
                                              || (BOD_LEVEL == SYSCFG_BOD_Level_3V60) \
                                              || (BOD_LEVEL == SYSCFG_BOD_Level_3V75) \
                                              || (BOD_LEVEL == SYSCFG_BOD_Level_3V90) \
                                              || (BOD_LEVEL == SYSCFG_BOD_Level_4V05) \
                                              || (BOD_LEVEL == SYSCFG_BOD_Level_4V20) \
                                              || (BOD_LEVEL == SYSCFG_BOD_Level_4V35) \
                                              || (BOD_LEVEL == SYSCFG_BOD_Level_4V50))
                                              


/**
  * @brief  Check is BOD flag
  */
#define IS_SYSCFG_BOD_FLAG(FLAG)         ((FLAG == SYSCFG_BOD_FLAG_BODIF) \
                                              || (FLAG == SYSCFG_BOD_FLAG_BODF))
                                   

/**
  * @brief  Check is SLEEP entry
  */
#define IS_SYSCFG_SLEEP_ENTRY(ENTRY)     ((ENTRY == SYSCFG_SLEEPEntry_WFI) \
                                              || (ENTRY == SYSCFG_SLEEPEntry_WFE))


/**
  * @brief  Check is STOP entry
  */
#define IS_SYSCFG_SLOP_ENTRY(ENTRY)      ((ENTRY == SYSCFG_STOPEntry_WFI) \
                                              || (ENTRY == SYSCFG_STOPEntry_WFE))


/**
  * @brief  Check is SWJ pin mode 
  */
#define IS_SYSCFG_SWJ_PIN_MODE(MODE)     ((MODE == SYSCFG_SWJ_All_Function) \
                                              || (MODE == SYSCFG_SWD_CLOSE_PinRemap))


/**
  * @brief  Check is osc pin mode 
  */
#define IS_SYSCFG_OSC_PIN_MODE(MODE)     ((MODE == SYSCFG_OSC_GPIO) \
                                              || (MODE == SYSCFG_OSC_HCrystal) \
                                              || (MODE == SYSCFG_OSC_ExtClock))
      

/**
  * @brief  Check is NMI trigger source  
  */
#define IS_SYSCFG_NMI_TRIGGER_SOURCE(SOURCE)  (((SOURCE & (uint16_t)0xFE3F) == 0x00) \
                                                   && (SOURCE != 0x00))


/**
  * @brief  Check is sram sector 
  */
#define IS_SYSCFG_SRAM_SECTOR(LOCK_SECTOR)    (LOCK_SECTOR != 0x0000)


/**
  * @brief  Check is DGB peripheral 
  */
#define IS_DBG_PERIPH(PERIPH)            (((PERIPH & (uint16_t)0xC003) == 0x00) \
                                              && (PERIPH != 0x00))

/**
  * @brief  Check is Lowpower mode 
  */
#define IS_DBG_LOWPOWER_MODE(MODE)       (((MODE & (uint16_t)0xFFFC) == 0x00) \
                                              && (MODE != 0x00))

/**
  * @brief  Check is clock source
  */
#define IS_GPIOB_CLOCK_SOURCE(SRC)       ((SRC == 0x00) || (SRC == 0x01))


/**
  * @}
  */
  
  
  
  
/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions --------------------------------------------------------*/

/** @addtogroup SYSCFG_Exported_Functions
  * @{
  */
extern void SYSCFG_BODInit(SYSCFGBODMode_TypeDef SYSCFG_BOD_Mode, SYSCFGBODLevel_TypeDef SYSCFG_BOD_Level);  

extern void SYSCFG_BODINTConfig(FunctionalState NewState);

extern void SYSCFG_VLVRInit(SYSCFG_VLVR_LEVEL_TypeDef SYSCFG_VLVR_Level);

extern void SYSCFG_VLVRConfig(FunctionalState NewState);

extern void SYSCFG_BODOnOff(CmdState OnOffState);

extern FlagStatus SYSCFG_BODGetFlagStatus(SYSCFG_BOD_FLAG_TypeDef BOD_Flag);

extern void SYSCFG_BODIntClearFlag(void);

extern void SYSCFG_EnterSLEEPMode(SYSCFG_SLEEPEntry_TypeDef SYSCFG_SLEEPEntry);

extern void SYSCFG_EnterSTOPMode(SYSCFG_STOPEntry_TypeDef SYSCFG_STOPEntry);

extern void SYSCFG_NMIInterruptTriggerConfig(SYSCFG_NMI_TRIGGER_TypeDef TriggerSource, FunctionalState NewState);

extern void SYSCFG_SWJPinConfig(SYSCFG_SWJCFG_TypeDef SWJPin_Mode);

extern void SYSCFG_OSCPinConfig(SYSCFG_OSCPin_TypeDef OSCPin_Mode);

extern void SYSCFG_SRAMLockConfig(SRAM_Sector_TypeDef SectorNum, FunctionalState NewState);

extern void SYSCFG_DBGPeriphConfig(DBG_Periph_TypeDef DBG_Periph, FunctionalState NewState);

extern void SYSCFG_DBGLowPowerConfig(FunctionalState NewState);

extern void SYSCFG_GPIOBCRConfig(GPIOB_MODULE_CLOCK_TypeDef GPIOB_Clock_Source);

#ifdef __cplusplus
}
#endif

#endif /* __SH30F9XX_SB0_SYSCFG_H */



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
