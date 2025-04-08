/**
  ******************************************************************************
  * @file    sh30f9xx_sb0_gpio.h
  * @author  sinowealth
  * @version V1.0.0
  * @date    2024-5-21  
  * @brief   This file contains all the functions prototypes for the GPIO firmware
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
#ifndef  __SH30F9XX_SB0_GPIO_H
#define  __SH30F9XX_SB0_GPIO_H

#ifdef  __cplusplus
extern "C"
{
#endif

/* Includes ------------------------------------------------------------------*/
#include "sh30f9xx_sb0_libcfg.h"

/** @addtogroup SH30F9XX_SB0_STANDARD_LIB
  * @{
  */

/** @addtogroup GPIO_MODULE
  * @{
  */
  

/** @defgroup GPIO_Exported_Types GPIO Exported Types 
  * @{
  */
  
/** 
  * @brief  GPIO Configuration Mode enumeration 
  */
typedef enum
{ 
    GPIO_Mode_IN   = 0x00, /*!< GPIO Input Mode */
    GPIO_Mode_OUT  = 0x01  /*!< GPIO Output Mode */
} GPIOMode_TypeDef;


/** 
  * @brief  GPIO Output type enumeration 
  */
typedef enum
{ 
    GPIO_OType_PP = 0x00, /*!< GPIO Output Type Push-Pull */
    GPIO_OType_OD = 0x01  /*!< GPIO Output Type Open-Drain */
} GPIOOType_TypeDef;
  
/** 
  * @brief  GPIO Output driving ability enumeration 
  */  
typedef enum
{ 
    GPIO_ODrv_NORMAL     = 0x00, /*!< GPIO output weak driver */
    GPIO_ODrv_VERYSTRONG = 0x01  /*!< GPIO outputstrong driver */
} GPIOODrv_TypeDef;



/** 
  * @brief  GPIO Configuration PullUp  enumeration 
  */ 
typedef enum
{ 
    GPIO_PuPd_NOPULL = 0x00, /*!< GPIO No pull up  */
    GPIO_PuPd_UP     = 0x01  /*!< GPIO pull up */
} GPIOPuPd_TypeDef;



/** 
  * @brief  GPIO Bit SH_SET and Bit SH_RESET enumeration 
  */ 
typedef enum
{ 
    Bit_RESET = 0, /*!< GPIO bit reset  */
    Bit_SET        /*!< GPIO bit set  */
} BitAction;  


/** 
  * @brief   GPIO Alternate Function Mapping Type
  */
typedef enum
{
    PXX_AF1   = 1,   PXX_AF2  ,   PXX_AF3  ,   PXX_AF4  ,   PXX_AF5  ,   PXX_AF6   ,   PXX_AF7     ,   PXX_AF8 ,   PXX_AF9   ,   PXX_AF10 ,
    PA0_GPIO  = 1,   PA0_TXD3 ,   PA0_RXD3 ,   PA0_AF4  ,   PA0_AF5  ,   PA0_SS1   ,   PA0_PCA0A   ,   PA0_T0  ,   PA0_AF9   ,   PA0_AN0  ,
    PA1_GPIO  = 1,   PA1_AF2  ,   PA1_RXD3 ,   PA1_AF4  ,   PA1_AF5  ,   PA1_MISO1 ,   PA1_PCA0B   ,   PA1_T1  ,   PA1_FLT2  ,   PA1_AN1  ,
    PA2_GPIO  = 1,   PA2_TXD0 ,   PA2_AF3  ,   PA2_SDA0 ,   PA2_SCL0 ,   PA2_MOSI1 ,   PA2_PCA3B   ,   PA2_T2  ,   PA2_FLT1  ,   PA2_AN2  ,
    PA3_GPIO  = 1,   PA3_TXD0 ,   PA3_AF3  ,   PA3_AF4  ,   PA3_SCL0 ,   PA3_SCK1  ,   PA3_AF7     ,   PA3_T3  ,   PA3_PWM1B ,   PA3_AF10 ,
    PA4_GPIO  = 1,   PA4_AF2  ,   PA4_RXD0 ,   PA4_SDA0 ,   PA4_SCL0 ,   PA4_SCK1  ,   PA4_PCA0ECI ,   PA4_T0  ,   PA4_PWM1A ,   PA4_AN3  ,
    PA5_GPIO  = 1,   PA5_TXD1 ,   PA5_AF3  ,   PA5_SDA0 ,   PA5_SCL0 ,   PA5_MOSI1 ,   PA5_PCA3ECI ,   PA5_AF8 ,   PA5_PWM1B ,   PA5_AF10 ,
    PA6_GPIO  = 1,   PA6_TXD2 ,   PA6_AF3  ,   PA6_AF4  ,   PA6_AF5  ,   PA6_AF6   ,   PA6_PCA3A   ,   PA6_AF8 ,   PA6_FLT0  ,   PA6_AF10 ,
    PA7_GPIO  = 1,   PA7_AF2  ,   PA7_RXD2 ,   PA7_AF4  ,   PA7_AF5  ,   PA7_AF6   ,   PA7_PCA3B   ,   PA7_AF8 ,   PA7_FLT0  ,   PA7_AF10 ,
    PA8_GPIO  = 1,   PA8_TXD4 ,   PA8_AF3  ,   PA8_AF4  ,   PA8_AF5  ,   PA8_SS1   ,   PA8_PCA3ECI ,   PA8_AF8 ,   PA8_PWM0B ,   PA8_AF10 ,
    PA9_GPIO  = 1,   PA9_TXD1 ,   PA9_RXD4 ,   PA9_AF4  ,   PA9_AF5  ,   PA9_AF6   ,   PA9_PCA3A   ,   PA9_AF8 ,   PA9_PWM0B ,   PA9_AN18 ,
    PA10_GPIO = 1,   PA10_TXD1,   PA10_RXD1,   PA10_AF4 ,   PA10_AF5 ,   PA10_SS1  ,   PA10_PCA3B  ,   PA10_AF8,   PA10_AF9  ,   PA10_AF10,
    PA11_GPIO = 1,   PA11_TXD1,   PA11_RXD1,   PA11_AF4 ,   PA11_AF5 ,   PA11_SS0  ,   PA11_PCA3A  ,   PA11_T1 ,   PA11_AF9  ,   PA11_AF10,
    PA12_GPIO = 1,   PA12_TXD5,   PA12_RXD5,   PA12_AF4 ,   PA12_AF5 ,   PA12_AF6  ,   PA12_PCA3ECI,   PA12_T2 ,   PA12_FLT3 ,   PA12_AN17,
    PA13_GPIO = 1,   PA13_AF2 ,   PA13_AF3 ,   PA13_AF4 ,   PA13_SCL0,   PA13_SS0  ,   PA13_PCA0ECI,   PA13_T3 ,   PA13_PWM3A,   PA13_AN19,
    PA14_GPIO = 1,   PA14_AF2 ,   PA14_AF3 ,   PA14_SDA0,   PA14_AF5 ,   PA14_AF6  ,   PA14_PCA0A  ,   PA14_T0 ,   PA14_PWM3B,   PA14_AN20,
    PA15_GPIO = 1,   PA15_TXD3,   PA15_RXD3,   PA15_AF4 ,   PA15_SCL1,   PA15_AF6  ,   PA15_PCA0B  ,   PA15_AF8,   PA15_PWM2A,   PA15_AF10,
    PB0_GPIO  = 1,   PB0_TXD3 ,   PB0_RXD3 ,   PB0_SDA1 ,   PB0_AF5  ,   PB0_SS0   ,   PB0_PCA1ECI ,   PB0_AF8 ,   PB0_PWM2B ,   PB0_AN27 ,
    PB1_GPIO  = 1,   PB1_AF2  ,   PB1_RXD3 ,   PB1_AF4  ,   PB1_SCL0 ,   PB1_SS0   ,   PB1_PCA1A   ,   PB1_T3  ,   PB1_PWM2A ,   PB1_AN28 ,
    PB2_GPIO  = 1,   PB2_TXD3 ,   PB2_AF3  ,   PB2_SDA0 ,   PB2_AF5  ,   PB2_AF6   ,   PB2_PCA1B   ,   PB2_T0  ,   PB2_FLT2  ,   PB2_AF10 ,
    PB3_GPIO  = 1,   PB3_AF2  ,   PB3_RXD3 ,   PB3_AF4  ,   PB3_AF5  ,   PB3_SS1   ,   PB3_PCA1A   ,   PB3_T1  ,   PB3_FLT0  ,   PB3_AF10 ,
    PB4_GPIO  = 1,   PB4_TXD1 ,   PB4_AF3  ,   PB4_AF4  ,   PB4_SCL0 ,   PB4_SCK1  ,   PB4_PCA1A   ,   PB4_AF8 ,   PB4_PWM0A ,   PB4_AF10 ,
    PB5_GPIO  = 1,   PB5_AF2  ,   PB5_RXD1 ,   PB5_SDA0 ,   PB5_AF5  ,   PB5_MOSI1 ,   PB5_PCA1B   ,   PB5_AF8 ,   PB5_PWM0B ,   PB5_AF10 ,
    PB6_GPIO  = 1,   PB6_TXD2 ,   PB6_AF3  ,   PB6_SDA1 ,   PB6_AF5  ,   PB6_MISO1 ,   PB6_PCA0A   ,   PB6_T0  ,   PB6_PWM0A ,   PB6_AF10 ,
    PB7_GPIO  = 1,   PB7_AF2  ,   PB7_RXD2 ,   PB7_AF4  ,   PB7_SCL1 ,   PB7_MISO0 ,   PB7_PCA0B   ,   PB7_T1  ,   PB7_AF9   ,   PB7_AN4  ,
    PB8_GPIO  = 1,   PB8_AF2  ,   PB8_AF3  ,   PB8_SDA0 ,   PB8_SCL0 ,   PB8_MOSI0 ,   PB8_PCA0A   ,   PB8_T0  ,   PB8_PWM0A ,   PB8_AN5  ,
    PB9_GPIO  = 1,   PB9_TXD5 ,   PB9_RXD5 ,   PB9_SDA0 ,   PB9_SCL0 ,   PB9_SCK0  ,   PB9_PCA0B   ,   PB9_T1  ,   PB9_FLT0  ,   PB9_AF10 ,
    PB10_GPIO = 1,   PB10_AF2 ,   PB10_AF3 ,   PB10_AF4 ,   PB10_AF5 ,   PB10_SS0  ,   PB10_PCA0ECI,   PB10_T2 ,   PB10_PWM0A,   PB10_AF10,
    PB11_GPIO = 1,   PB11_AF2 ,   PB11_AF3 ,   PB11_AF4 ,   PB11_AF5 ,   PB11_SS1  ,   PB11_AF7    ,   PB11_T3 ,   PB11_PWM0B,   PB11_AF10,
    PB12_GPIO = 1,   PB12_TXD5,   PB12_RXD5,   PB12_SDA1,   PB12_SCL1,   PB12_SCK1 ,   PB12_PCA1ECI,   PB12_T0 ,   PB12_PWM0A,   PB12_AF10,
    PB13_GPIO = 1,   PB13_TXD5,   PB13_RXD5,   PB13_SDA1,   PB13_SCL1,   PB13_MOSI1,   PB13_PCA1A  ,   PB13_AF8,   PB13_PWM0B,   PB13_AF10,
    PB14_GPIO = 1,   PB14_TXD4,   PB14_RXD4,   PB14_AF4 ,   PB14_AF5 ,   PB14_MISO1,   PB14_PCA1B  ,   PB14_AF8,   PB14_PWM0A,   PB14_AF10,
    PB15_GPIO = 1,   PB15_TXD4,   PB15_RXD4,   PB15_AF4 ,   PB15_AF5 ,   PB15_SS1  ,   PB15_AF7    ,   PB15_T3 ,   PB15_PWM3A,   PB15_AF10,
    PC0_GPIO  = 1,   PC0_TXD5 ,   PC0_AF3  ,   PC0_SDA0 ,   PC0_AF5  ,   PC0_AF6   ,   PC0_AF7     ,   PC0_T2  ,   PC0_FLT3  ,   PC0_AF10 ,
    PC1_GPIO  = 1,   PC1_AF2  ,   PC1_RXD5 ,   PC1_SDA1 ,   PC1_AF5  ,   PC1_SS0   ,   PC1_AF7     ,   PC1_AF8 ,   PC1_PWM3A ,   PC1_AF10 ,
    PC2_GPIO  = 1,   PC2_AF2  ,   PC2_AF3  ,   PC2_AF4  ,   PC2_SCL1 ,   PC2_SCK0  ,   PC2_PCA2ECI ,   PC2_T0  ,   PC2_PWM3B ,   PC2_AF10 ,
    PC3_GPIO  = 1,   PC3_TXD0 ,   PC3_RXD0 ,   PC3_SDA1 ,   PC3_SCL1 ,   PC3_MISO0 ,   PC3_PCA2A   ,   PC3_T1  ,   PC3_PWM3A ,   PC3_AF10 ,
    PC4_GPIO  = 1,   PC4_TXD0 ,   PC4_RXD0 ,   PC4_SDA1 ,   PC4_SCL1 ,   PC4_MOSI0 ,   PC4_PCA2B   ,   PC4_T2  ,   PC4_PWM2A ,   PC4_AF10 ,
    PC5_GPIO  = 1,   PC5_TXD0 ,   PC5_RXD0 ,   PC5_SDA0 ,   PC5_SCL0 ,   PC5_SCK1  ,   PC5_PCA2A   ,   PC5_T3  ,   PC5_PWM2B ,   PC5_AF10 ,
    PC6_GPIO  = 1,   PC6_TXD3 ,   PC6_RXD3 ,   PC6_SDA0 ,   PC6_SCL0 ,   PC6_MOSI1 ,   PC6_PCA3B   ,   PC6_T0  ,   PC6_PWM2A ,   PC6_AF10 ,
    PC7_GPIO  = 1,   PC7_TXD3 ,   PC7_RXD3 ,   PC7_SDA0 ,   PC7_SCL0 ,   PC7_MISO1 ,   PC7_PCA3ECI ,   PC7_T2  ,   PC7_PWM2B ,   PC7_AF10 ,
    PC8_GPIO  = 1,   PC8_TXD1 ,   PC8_RXD1 ,   PC8_AF4  ,   PC8_AF5  ,   PC8_MISO0 ,   PC8_PCA3B   ,   PC8_T1  ,   PC8_FLT2  ,   PC8_AF10 ,
    PC9_GPIO  = 1,   PC9_TXD1 ,   PC9_RXD1 ,   PC9_SDA1 ,   PC9_SCL1 ,   PC9_MOSI0 ,   PC9_PCA3A   ,   PC9_T2  ,   PC9_AF9   ,   PC9_AF10 ,
    PC10_GPIO = 1,   PC10_TXD2,   PC10_RXD2,   PC10_SDA1,   PC10_SCL1,   PC10_SCK0 ,   PC10_PCA2A  ,   PC10_T3 ,   PC10_PWM2B,   PC10_AN21,
    PC11_GPIO = 1,   PC11_TXD2,   PC11_RXD2,   PC11_SDA1,   PC11_SCL1,   PC11_SS0  ,   PC11_PCA2B  ,   PC11_AF8,   PC11_PWM2A,   PC11_AN22,
    PC12_GPIO = 1,   PC12_TXD4,   PC12_RXD4,   PC12_SDA0,   PC12_SCL0,   PC12_SS0  ,   PC12_PCA2ECI,   PC12_T0 ,   PC12_PWM2B,   PC12_AN6 ,
    PC13_GPIO = 1,   PC13_TXD4,   PC13_RXD4,   PC13_SDA0,   PC13_SCL0,   PC13_SCK0 ,   PC13_PCA1A  ,   PC13_T1 ,   PC13_FLT0 ,   PC13_AN7 ,
    PC14_GPIO = 1,   PC14_TXD5,   PC14_AF3 ,   PC14_SDA0,   PC14_SCL0,   PC14_MISO0,   PC14_PCA1B  ,   PC14_T2 ,   PC14_PWM1A,   PC14_AN8 ,
    PC15_GPIO = 1,   PC15_AF2 ,   PC15_RXD5,   PC15_SDA0,   PC15_SCL0,   PC15_MOSI0,   PC15_PCA1ECI,   PC15_T3 ,   PC15_PWM1B,   PC15_AN9 ,
    PD0_GPIO  = 1,   PD0_TXD0 ,   PD0_RXD0 ,   PD0_AF4  ,   PD0_AF5  ,   PD0_AF6   ,   PD0_PCA2B   ,   PD0_AF8 ,   PD0_PWM1A ,   PD0_AN10 ,
    PD1_GPIO  = 1,   PD1_TXD0 ,   PD1_RXD0 ,   PD1_AF4  ,   PD1_SCL1 ,   PD1_MISO1 ,   PD1_PCA2ECI ,   PD1_AF8 ,   PD1_PWM1B ,   PD1_AN11 ,
    PD2_GPIO  = 1,   PD2_TXD1 ,   PD2_RXD1 ,   PD2_SDA1 ,   PD2_AF5  ,   PD2_MOSI1 ,   PD2_PCA2A   ,   PD2_AF8 ,   PD2_FLT1  ,   PD2_AN12 ,
    PD3_GPIO  = 1,   PD3_TXD1 ,   PD3_RXD1 ,   PD3_AF4  ,   PD3_AF5  ,   PD3_SCK1  ,   PD3_PCA2B   ,   PD3_AF8 ,   PD3_FLT3  ,   PD3_AN13 ,
    PD4_GPIO  = 1,   PD4_TXD5 ,   PD4_RXD5 ,   PD4_AF4  ,   PD4_AF5  ,   PD4_SS1   ,   PD4_PCA2A   ,   PD4_AF8 ,   PD4_PWM3B ,   PD4_AN14 ,
    PD5_GPIO  = 1,   PD5_TXD5 ,   PD5_RXD5 ,   PD5_AF4  ,   PD5_AF5  ,   PD5_SS1   ,   PD5_PCA2ECI ,   PD5_T0  ,   PD5_FLT3  ,   PD5_AF10 ,
    PD6_GPIO  = 1,   PD6_TXD5 ,   PD6_RXD5 ,   PD6_AF4  ,   PD6_SCL1 ,   PD6_SCK1  ,   PD6_AF7     ,   PD6_T1  ,   PD6_AF9   ,   PD6_AF10 ,
    PD7_GPIO  = 1,   PD7_TXD2 ,   PD7_RXD2 ,   PD7_SDA1 ,   PD7_AF5  ,   PD7_MOSI1 ,   PD7_AF7     ,   PD7_T2  ,   PD7_AF9   ,   PD7_AN15 ,
    PD8_GPIO  = 1,   PD8_TXD2 ,   PD8_RXD2 ,   PD8_AF4  ,   PD8_SCL1 ,   PD8_MISO1 ,   PD8_AF7     ,   PD8_T3  ,   PD8_FLT1  ,   PD8_AN16 ,
    PD9_GPIO  = 1,   PD9_TXD2 ,   PD9_RXD2 ,   PD9_AF4  ,   PD9_AF5  ,   PD9_MISO1 ,   PD9_PCA2ECI ,   PD9_T0  ,   PD9_PWM1B ,   PD9_AN23 ,
    PD10_GPIO = 1,   PD10_TXD2,   PD10_RXD3,   PD10_AF4 ,   PD10_AF5 ,   PD10_SCK1 ,   PD10_PCA2A  ,   PD10_T2 ,   PD10_PWM2B,   PD10_AN24,
    PD11_GPIO = 1,   PD11_AF2 ,   PD11_AF3 ,   PD11_AF4 ,   PD11_AF5 ,   PD11_SS1  ,   PD11_PCA2B  ,   PD11_AF8,   PD11_PWM3B,   PD11_AN25,
    PD12_GPIO = 1,   PD12_AF2 ,   PD12_AF3 ,   PD12_AF4 ,   PD12_AF5 ,   PD12_AF6  ,   PD12_AF7    ,   PD12_AF8,   PD12_PWM2A,   PD12_AN26,
    PD13_GPIO = 1,   PD13_TXD1,   PD13_RXD1,   PD13_SDA0,   PD13_SCL0,   PD13_MOSI1,   PD13_PCA0A  ,   PD13_AF8,   PD13_PWM0A,   PD13_AF10,
    PD14_GPIO = 1,   PD14_TXD1,   PD14_RXD1,   PD14_SDA0,   PD14_SCL0,   PD14_AF6  ,   PD14_PCA0B  ,   PD14_AF8,   PD14_PWM0B,   PD14_AF10,
    
} GPIO_AF_TypeDef;


/**
  * @brief  GPIO Pin index 
  */
typedef enum
{
    GPIO_Pin_0   = ((uint16_t)0x0001),   /*!< GPIO Pin 0  */
    GPIO_Pin_1   = ((uint16_t)0x0002),   /*!< GPIO Pin 1  */
    GPIO_Pin_2   = ((uint16_t)0x0004),   /*!< GPIO Pin 2  */
    GPIO_Pin_3   = ((uint16_t)0x0008),   /*!< GPIO Pin 3  */
    GPIO_Pin_4   = ((uint16_t)0x0010),   /*!< GPIO Pin 4  */
    GPIO_Pin_5   = ((uint16_t)0x0020),   /*!< GPIO Pin 5  */
    GPIO_Pin_6   = ((uint16_t)0x0040),   /*!< GPIO Pin 6  */
    GPIO_Pin_7   = ((uint16_t)0x0080),   /*!< GPIO Pin 7  */
    GPIO_Pin_8   = ((uint16_t)0x0100),   /*!< GPIO Pin 8  */
    GPIO_Pin_9   = ((uint16_t)0x0200),   /*!< GPIO Pin 9  */
    GPIO_Pin_10  = ((uint16_t)0x0400),   /*!< GPIO Pin 10  */
    GPIO_Pin_11  = ((uint16_t)0x0800),   /*!< GPIO Pin 11  */
    GPIO_Pin_12  = ((uint16_t)0x1000),   /*!< GPIO Pin 12  */
    GPIO_Pin_13  = ((uint16_t)0x2000),   /*!< GPIO Pin 13  */
    GPIO_Pin_14  = ((uint16_t)0x4000),   /*!< GPIO Pin 14  */
    GPIO_Pin_15  = ((uint16_t)0x8000),   /*!< GPIO Pin 15  */
    GPIO_Pin_All = ((uint16_t)0xFFFF)    /*!< All pins selected */
} GPIO_Pin_Index;

  

/**
  * @brief  GPIO Bitlock status 
  */
typedef enum
{
    GPIO_BitLock_Disable   = (uint8_t)0x00,/*!< GPIO Bitlock Disable  */
    GPIO_BitLock_Enable    = (uint8_t)0x01 /*!< GPIO Bitlock Enable  */
} GPIO_Bitlock_Status_TypeDef;

/**
  * @brief  GPIO Bitlevel type
  */
typedef enum
{
    GPIO_BitLevel_CMOS    = (uint8_t)0x00,/*!< GPIO Bitlevel CMOS  */
    GPIO_BitLevel_TTL     = (uint8_t)0x01 /*!< GPIO Bitlevel TTL  */
} GPIO_BitLevel_TypeDef;


/** 
  * @brief   GPIO Init structure definition  
  */
typedef struct
{
    uint32_t GPIO_Pin;              /*!< Specifies the GPIO pins to be configured.
                                       This parameter can be any value of @ref GPIO_Pin_Index */

    GPIOMode_TypeDef GPIO_Mode;     /*!< Specifies the operating mode for the selected pins.
                                       This parameter can be a value of @ref GPIOMode_TypeDef */

    GPIOODrv_TypeDef GPIO_ODrv;   /*!< Specifies the speed for the selected pins.
                                       This parameter can be a value of @ref GPIOODrv_TypeDef */

    GPIOOType_TypeDef GPIO_OType;   /*!< Specifies the operating output type for the selected pins.
                                       This parameter can be a value of @ref GPIOOType_TypeDef */

    GPIOPuPd_TypeDef GPIO_PuPd;     /*!< Specifies the operating Pull-up/Pull down for the selected pins.
                                       This parameter can be a value of @ref GPIOPuPd_TypeDef */
} GPIO_InitTypeDef;  



/**
* @}
*/
  
  
/** @defgroup GPIO_Exported_Constants GPIO Exported Constants 
  * @{
  */

#define GPIO_MODER_MODER0_Msk          (0x1UL /*<< GPIO_MODER_MODER_Pos*/)        /*!<GPIO MODER: MODER0 Mask     */
#define GPIO_ODRVR_ODRVR_SINK_Pos      (0x1UL /*<< GPIO_ODRVR_ODRVR_Pos*/)        /*!<GPIO_CFG ODRVR: ODRVR0 Mask */
#define GPIO_OTYPER_OT_Msk             (0x1UL /*<< GPIO_OTYPER_OT_Pos*/)          /*!<GPIOA OTYPER: OT Mask       */
#define GPIO_AFRL_AFR0_Msk             (0xFUL /*<< GPIO_AFRL_AFR0_Pos*/)          /*!<GPIO_CFG AFRL: AFR0 Mask    */
#define GPIO_AFRH_AFR8_Msk             (0xFUL /*<< GPIO_AFRH_AFR8_Pos*/)          /*!<GPIO_CFG AFRH: AFR8 Mask    */
#define GPIO_PUPDR_PHDR0_Msk           (0x1UL /*<< GPIOD_PUPDR_PHDR0_Pos*/)       /*!<GPIOD PUPDR: PHDR0 Mask     */





/**
* @}
*/


/** @defgroup GPIO_Parameter_Check_Macro GPIO Parameter Check Macro 
  * @{
  */
  
/**
  * @brief   Check is GPIO mode
  */ 
#define IS_GPIO_MODE(MODE)             (((MODE) == GPIO_Mode_IN) || ((MODE) == GPIO_Mode_OUT))

/**
  * @brief   Check is GPIO output type
  */ 
#define IS_GPIO_OTYPE(OTYPE)           (((OTYPE) == GPIO_OType_PP) || ((OTYPE) == GPIO_OType_OD))

/**
  * @brief   Check is GPIO output drive
  */ 
#define IS_GPIO_ODRV(ODRV)             (((ODRV) == GPIO_ODrv_NORMAL) || ((ODRV) == GPIO_ODrv_VERYSTRONG)) 


/**
  * @brief   Check is GPIO pull type
  */ 
#define IS_GPIO_PUPD(PUPD)             (((PUPD) == GPIO_PuPd_NOPULL) || ((PUPD) == GPIO_PuPd_UP))


/**
  * @brief   Check is GPIO bit action 
  */ 
#define IS_GPIO_BIT_ACTION(BIT_ACTION) (((BIT_ACTION) == Bit_RESET) || ((BIT_ACTION) == Bit_SET))



/**
  * @brief   Check is GPIO port
  */ 
#define IS_GPIO_ALL_PERIPH(PERIPH)     (((PERIPH) == GPIOA) || ((PERIPH) == GPIOB) \
                                       || ((PERIPH) == GPIOC) || ((PERIPH) == GPIOD))
                                       
/**
  * @brief   Check is GPIO bit lock 
  */                                        
#define IS_GPIO_BIT_LOCK(LOCK)         ((LOCK == GPIO_BitLock_Disable) || (LOCK == GPIO_BitLock_Enable))

/**
  * @brief   Check is GPIO bit level
  */ 
#define IS_GPIO_BIT_LEVEL(LEVEL)       ((LEVEL == GPIO_BitLevel_CMOS) || (LEVEL == GPIO_BitLevel_TTL))



/**
  * @brief   Check is GPIO pin
  */ 
#define IS_GPIO_PIN(PIN)               ((((PIN) & (uint16_t)0x00) == 0x00) && ((PIN) != (uint16_t)0x00))

/**
  * @brief   Check is GPIO pin
  */ 
#define  IS_GET_GPIO_PIN(PIN)        (((PIN) == GPIO_Pin_0)  || ((PIN) == GPIO_Pin_1)    \
                                     || ((PIN) == GPIO_Pin_2)  || ((PIN) == GPIO_Pin_3)  \
                                     || ((PIN) == GPIO_Pin_4)  || ((PIN) == GPIO_Pin_5)  \
                                     || ((PIN) == GPIO_Pin_6)  || ((PIN) == GPIO_Pin_7)  \
                                     || ((PIN) == GPIO_Pin_8)  || ((PIN) == GPIO_Pin_9)  \
                                     || ((PIN) == GPIO_Pin_10) || ((PIN) == GPIO_Pin_11) \
                                     || ((PIN) == GPIO_Pin_12) || ((PIN) == GPIO_Pin_13) \
                                     || ((PIN) == GPIO_Pin_14) || ((PIN) == GPIO_Pin_15))

/**
  * @brief   Check is GPIO Alternate function
  */ 
#define  IS_GPIO_AF(AF)   ((AF) <= 10)



/**
* @}
*/



    /* Exported macro ------------------------------------------------------------*/
    /* Exported functions --------------------------------------------------------*/

/** @addtogroup GPIO_Exported_Functions
  * @{
  */

extern void GPIO_Reset(GPIO_TypeDef *GPIOx);

extern void GPIO_Init(GPIO_TypeDef *GPIOx, GPIO_InitTypeDef *GPIO_InitStruct);

extern void GPIO_StructInit(GPIO_InitTypeDef *GPIO_InitStruct);

extern void GPIO_PinTTLConfig(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin, uint8_t GPIO_BitLevel);

extern void GPIO_PinLock(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin, uint8_t GPIO_BitLock);

extern uint8_t GPIO_ReadInputDataBit(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin);

extern uint16_t GPIO_ReadInputData(GPIO_TypeDef *GPIOx);

extern uint8_t GPIO_ReadOutputDataBit(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin);

extern uint16_t GPIO_ReadOutputData(GPIO_TypeDef *GPIOx);

extern void GPIO_SetBits(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin);

extern void GPIO_ResetBits(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin);

extern void GPIO_WriteBit(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin, BitAction BitVal);

extern void GPIO_Write(GPIO_TypeDef *GPIOx, uint16_t PortVal);

extern void GPIO_ToggleBits(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin);

extern void GPIO_PinAFConfig(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin, GPIO_AF_TypeDef GPIO_AF);

/**
  * @}
  */

#ifdef __cplusplus
}
#endif

#endif /*__SH30F9XX_SB0_GPIO_H */

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT Sinowealth *****END OF FILE****/
