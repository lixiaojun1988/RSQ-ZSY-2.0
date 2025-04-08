/**
  ******************************************************************************
  * @file    sh30f9xx_sb0_twi.c
  * @author  sinowealth
  * @version V1.0.0
  * @date    2024-5-27  
  * @brief   This file provides firmware functions to manage the TWI module 
  *  
    @verbatim
  ==============================================================================
                         How to use this driver
  ==============================================================================
   [..]
     (#) Enable TWI Clock use RCC_APB1PeriphClockOnOff() function 

     (#) Fill TWI Init Structure member as default use TWI_StructInit() function
     
     (#) Config TWI Pin
         (++) Enable Pin Clock calling RCC_AHBPeriphClockOnOff() function 
         
         (++) Call GPIO_Init()function for config TWI pin(open-drain output ,pull up or no-pull etc ...),if not use all member for TWI 
              pin config ,makesure call GPIO_StructInit() function for GPIO_Init function  paramter set default value

         (++) Call GPIO_PinAFConfig() for config pin as TWI function

     (#) Config TWI Clock ,baud rate, adress, bus time out etc use TWI_Init() function
     
     (#) Enable TWI Module by calling TWI_OnOff() function
     
     (#) Use TWI interrupt by following step

         (++) Config TWI Interrupt use TWI_INTConfig() function
         
         (++) Enable Kernel TWI's Global interrupt by calling NVIC_EnableIRQ() function
         
         (++) reference startup asm file for TWI interrupt function name and rewrite this function 	
      
     (#) Call TWI_START() function to start TWI communication as master mode
     
     (#) When TWI interrupt happen, use TWI_GetEvent() function to get TWI status
         use TWI_GetFlagStatus() function to get TWI interrupt flag

     (#) Call TWI_ClearFlag() function for clear TWI interrupt flag
     
     (#) Call TWI_STOP() function to end current TWI communication
  
  *  @endverbatim
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
#include "sh30f9xx_sb0_twi.h"

/** @addtogroup SH30F9XX_SB0_STANDARD_LIB
  * @{
  */

/** @defgroup TWI_MODULE TWI
  * @brief SH30F9XX SB0 TWI module driver library,reference this topic macro and function for drive TWI
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
static __IO uint32_t  TWITimeout = TWIT_LONG_TIMEOUT;    
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/* Export function prototypes -----------------------------------------------*/

/** @defgroup TWI_Exported_Functions TWI Exported Functions
 *  @brief    This is TWI exported function code group
 *
  * @{
  */

/**
  * @brief  Deinitializes the TWIx peripheral registers to their default
  *         reset values 
  * @param  TWIx: where x can be 0, 1 to select the TWI peripheral.
  * @retval None
  */
void TWI_Reset(TWI_TypeDef *TWIx)
{
    /* Check the parameters */
    assert_param(IS_TWI_MODULE(TWIx));

    if (TWIx == TWI0)
    {
        /* Reset TWI0 module */
        RCC_APB1PeriphReset(RCC_APB1_TWI0);
    }
    else if (TWIx == TWI1)
    {
        /* Reset TWI1 module */
        RCC_APB1PeriphReset(RCC_APB1_TWI1);
    }
    else{}
}

/**
  * @brief  Fills each TWI_InitStruct member with its default value. 
  * @param  TWI_InitStruct: pointer to a @ref TWI_InitTypeDef structure which will 
  *         be initialized.
  * @retval None
  */
void TWI_StructInit(TWI_InitTypeDef *TWI_InitStruct)
{
    /* for address register */
    /* Initialize General Address: SH_ENABLE or SH_DISABLE */
    TWI_InitStruct->GeneralAddress = SH_DISABLE;

    /* Initialize the 7 bits address */
    TWI_InitStruct->TWIAddress = 0x00;

    /* Initialize the TWI address mask bit 0:Must match 1:Ignore */
    TWI_InitStruct->MaskAddress = 0x00;

    /* Initialize the Clock prescale: @ref TWI_CR_TypeDef */
    TWI_InitStruct->PresClock = TWI_CR_DIV64;

    /* Initialize the Bus timeout count */
    TWI_InitStruct->BusTimeOut = TWI_CNT_25000;

    /* Initialize the High level timeout enable bit */
    TWI_InitStruct->HighLevelTimeOutEnable = SH_DISABLE;

    /* Initialize the Bus timeout enable bit */
    TWI_InitStruct->BusTimeOutEnable = SH_DISABLE;

    /* Initialize the Baudrate */
    TWI_InitStruct->BaudRate = 0x00;

    /* Initialize the High level timeout count */
    TWI_InitStruct->HighLevelTimeOut = 0xFF;
}

/**     
  * @brief  Initializes the TWIx peripheral according to the specified 
  *         parameters in the InitCfg.
  * @param  TWIx: where x can be 0, 1 to select the TWI peripheral.
  * @param  InitCfg: pointer to a @ref TWI_InitTypeDef structure that
  *         contains the configuration information for the specified TWI peripheral.
  * @retval None
  */

void TWI_Init(TWI_TypeDef *TWIx, const TWI_InitTypeDef *InitCfg)
{
    assert_param(IS_TWI_MODULE(TWIx));
    assert_param((InitCfg->BaudRate & 0x80) == 0);

    TWIx->ADDR.BIT.GC = InitCfg->GeneralAddress;
    TWIx->CR.BIT.CR   = InitCfg->PresClock;
    TWIx->CR.BIT.CNT  = InitCfg->BusTimeOut;

    TWIx->CR.BIT.EFREE = InitCfg->HighLevelTimeOutEnable;
    TWIx->CR.BIT.ETOT  = InitCfg->BusTimeOutEnable;

    TWIx->ADDR.BIT.ADDR   = InitCfg->TWIAddress;
    TWIx->ADDR.BIT.TWIAMR = InitCfg->MaskAddress;
    TWIx->BRT = InitCfg->BaudRate;
    TWIx->HOC = InitCfg->HighLevelTimeOut;
}

/**
  * @brief  Enables or disables the specified TWI peripheral.
  * @param  TWIx: where x can be 0, 1 to select the TWI peripheral.
  * @param  OnOffState: state of the TWIx peripheral. 
  *         This parameter can be one of the following values:
  *            @arg @b SH_ON
  *            @arg @b SH_OFF
  * @retval None
  */
void TWI_OnOff(TWI_TypeDef *TWIx, CmdState OnOffState)
{
    assert_param(IS_TWI_MODULE(TWIx));
    assert_param(IS_CMD_STATE(OnOffState));

    TWIx->CR.BIT.TWIEN = OnOffState;
}

/**
  * @brief  TWI General address enable.
  * @param  TWIx: where x can be 0, 1 to select the TWI peripheral.
  * @param  TWIState: new mode of the TWIx peripheral. 
  *         This parameter can be one of the following values:
  *            @arg @b SH_ENABLE
  *            @arg @b SH_DISABLE
  * @retval None
  */
void TWI_AddrModeConfig(TWI_TypeDef *TWIx, FunctionalState TWIState)
{

    /* Check the parameters */
    assert_param(IS_TWI_MODULE(TWIx));
    assert_param(IS_FUNCTION_STATE(TWIState));

    TWIx->ADDR.BIT.GC = TWIState;
}

/**
  * @brief  Enables or disables the specified TWI interrupts.
  * @param  TWIx: where x can be 0, 1 to select the TWI peripheral.
  * @param  NewState: state of the specified TWI interrupt.
  *         This parameter can be one of the following values:
  *            @arg @b SH_ENABLE
  *            @arg @b SH_DISABLE
  * @retval None
  */
void TWI_INTConfig(TWI_TypeDef *TWIx, FunctionalState NewState)
{
    assert_param(IS_TWI_MODULE(TWIx));
    assert_param(IS_FUNCTION_STATE(NewState));

    TWIx->CR.BIT.TWINTE = NewState;
}

/**
  * @brief  Checks whether the specified TWI interrupt has occurred or not.
  * @param  TWIx: where x can be 0, 1 to select the TWI peripheral.
  * @retval FlagStatus The new state interrupt (SH_SET or RESET).
  */
FlagStatus TWI_GetINTStatus(TWI_TypeDef *TWIx)
{
    uint32_t tmpV;
    assert_param(IS_TWI_MODULE(TWIx));
    tmpV = TWIx->FR.BIT.TWINT;
    return ((tmpV) ? SH_SET : SH_RESET);
}

/**
  * @brief  Clears the TWIx interrupt pending bit.
  * @param  TWIx: where x can be 0, 1 to select the TWI peripheral.
  * @retval None
  */
void TWI_ClearINTStatus(TWI_TypeDef *TWIx)
{
    assert_param(IS_TWI_MODULE(TWIx));
    TWIx->FR.BIT.TWINTC = 1;
}

/**
  * @brief  Get TWI transmit flag
  * @param  TWIx: where x can be 0, 1 to select the TWI peripheral.
  * @param  Flag: TWI flag mask
  *         This parameter can be one of following values:
  *           @arg @b TWI_FLAG_INT
  *           @arg @b TWI_FLAG_TFREE_TIMEOUT
  *           @arg @b TWI_FLAG_TOUT_TIMEOUT 
  * @retval  FlagStatus  flag status
  *           @arg @b SH_SET
  *           @arg @b SH_RESET
  */
FlagStatus TWI_GetFlagStatus(TWI_TypeDef *TWIx, TWI_FLAG_TypeDef Flag)
{
    assert_param(IS_TWI_MODULE(TWIx));
    assert_param(IS_TWI_FLAG_Type(Flag));

    return ((TWIx->FR.V32 & Flag) ? SH_SET : SH_RESET);
}

/**
  * @brief  Clear TWI transmit flags
  * @param  TWIx: where x can be 0, 1 to select the TWI peripheral.
  * @param  Flags: TWI flags clear bit mask
  *         This parameter can be any combination of following values:
  *           @arg @b TWI_FLAG_INT
  *           @arg @b TWI_FLAG_TFREE_TIMEOUT
  *           @arg @b TWI_FLAG_TOUT_TIMEOUT
  * @retval None
  */
void TWI_ClearFlag(TWI_TypeDef *TWIx, TWI_FLAG_TypeDef Flags)
{
    assert_param(IS_TWI_MODULE(TWIx));
    assert_param(IS_TWI_FLAG_Types(Flags));

    TWIx->FR.V32 = (Flags << 16);
}

/**
  * @brief  TWI send data
  * @param  TWIx: where x can be 0, 1 to select the TWI peripheral.
  * @param  Data: Send data
  * @retval None
  */
void TWI_SendData(TWI_TypeDef *TWIx, uint8_t Data)
{
    assert_param(IS_TWI_MODULE(TWIx));

    TWIx->DR = Data;
}

/**
  * @brief  TWI receive data
  * @param  TWIx: TWI device TWI0 TWI1
  * @retval received data
  */
uint8_t TWI_ReceiveData(TWI_TypeDef *TWIx)
{
    assert_param(IS_TWI_MODULE(TWIx));

    return TWIx->DR;
}

/**
  * @brief  get TWI transmit event
  * @param  TWIx: where x can be 0, 1 to select the TWI peripheral.
  * @retval TWI_EVENT_Type TWI transmit event flag
  */
TWI_EVENT_TypeDef TWI_GetEvent(TWI_TypeDef *TWIx)
{
    assert_param(IS_TWI_MODULE(TWIx));

    return (TWI_EVENT_TypeDef)(TWIx->STAT.V32);
}

/**
  * @brief  Generates TWI communication START condition.
  * @param  TWIx: where x can be 0, 1 to select the TWI peripheral.
  * @param  NewState: new state of the I2C START condition generation.
  *         This parameter can be one of the following values:
  *            @arg @b SH_ENABLE
  *            @arg @b SH_DISABLE
  * @retval None.
  */
void TWI_START(TWI_TypeDef *TWIx, FunctionalState NewState)
{
    assert_param(IS_TWI_MODULE(TWIx));
    assert_param(IS_FUNCTION_STATE(NewState));

    TWIx->CR.BIT.STA = NewState;
}

/**
  * @brief  Generates TWI communication STOP condition.
  * @param  TWIx: where x can be 0, 1 to select the TWI peripheral.
  * @param  NewState: new state of the I2C STOP condition generation.
  *         This parameter can be one of the following values:
  *            @arg @b SH_ENABLE
  *            @arg @b SH_DISABLE
  * @retval None.
  */
void TWI_STOP(TWI_TypeDef *TWIx, FunctionalState NewState)
{
    assert_param(IS_TWI_MODULE(TWIx));
    assert_param(IS_FUNCTION_STATE(NewState));

    TWIx->CR.BIT.STO = NewState;
}

/**
  * @brief  Enables or disables the specified TWI Response feature.
  * @param  TWIx: where x can be 0, 1 to select the TWI peripheral.
  * @param  NewAck: state of the TWI Acknowledgement.
  *         This parameter can be one of the following values:
  *            @arg @b TWI_AA_ACK
  *            @arg @b TWI_AA_NACK
  * @retval None.
  */
void TWI_ACK_Config(TWI_TypeDef *TWIx, TWI_AA_TypeDef NewAck)
{
    /* Check the parameters */
    assert_param(IS_TWI_MODULE(TWIx));
    assert_param(IS_TWI_AA_Type(NewAck));

    TWIx->CR.BIT.AA = NewAck;
}

/**
  * @brief  Send 7bit address and read/write flag
  * @param  TWIx: where x can be 0, 1 to select the TWI peripheral.
  * @param  Address: device address
  * @param  TWI_Direction: transmit direction read or write
  *         This parameter can be one of the following values:
  *            @arg @b TWI_DIR_TRANSMITTER
  *            @arg @b TWI_DIR_RECEIVER
  * @retval None.
  */
void TWI_Send7bitAddress(TWI_TypeDef *TWIx, uint8_t Address, TWI_DIR_TypeDef TWI_Direction)
{
    assert_param(IS_TWI_MODULE(TWIx));
    assert_param(IS_TWI_DIR_Type(TWI_Direction));

    if (TWI_Direction == TWI_DIR_TRANSMITTER)
        TWIx->DR = (Address << 1);
    else
        TWIx->DR = ((Address << 1) | 1);
}

/**
  * @brief  Configures the specified TWI own address.
  * @param  TWIx: where x can be 0, 1 to select the TWI peripheral.
  * @param  Address:    specifies the own address.
  * @param  AddressMsk: specifies the own address mask bits.
  * @param  NewState:   state of the generate address.
  *         This parameter can be one of the following values:
  *            @arg @b SH_ENABLE
  *            @arg @b SH_DISABLE
  * @retval None.
  */
void TWI_OwnAddressConfig(TWI_TypeDef *TWIx, uint8_t Address, uint8_t AddressMsk, FunctionalState NewState)
{
    uint8_t addr_gc;
    
    assert_param(IS_TWI_MODULE(TWIx));
    assert_param(Address < 0x80);
    assert_param(AddressMsk < 0x80);
    assert_param(IS_FUNCTION_STATE(NewState));
    
    if(NewState == SH_ENABLE)
    {
        addr_gc = 1;
    }
    else
    {
        addr_gc = 0;
    }

    TWIx->ADDR.V32 = ((Address << TWI_ADDR_ADDR_Pos) \
                     | (AddressMsk << TWI_ADDR_TWIAMR_Pos) \
                     | (addr_gc << TWI_ADDR_GC_Pos));
}

/**
  * @brief  Send TWI commands
  * @param  TWIx: where x can be 0, 1 to select the TWI peripheral.
  * @retval TWI_EVENT_Type TWI event flags.
  */
TWI_EVENT_TypeDef TWI_SendWaitDone(TWI_TypeDef *TWIx)
{
    assert_param(IS_TWI_MODULE(TWIx));
    TWITimeout = TWIT_FLAG_TIMEOUT;
    /* clear Interrupt Flag to start transmit */
    TWIx->FR.V32 = (TWI_FR_TOUTC_Msk | TWI_FR_TFREEC_Msk | TWI_FR_TWINTC_Msk);

    while(TWI_GetFlagStatus(TWIx, TWI_FLAG_INT) != SH_SET)
    {
        if ((TWITimeout--) == 0)
            break;  
    }        

    /* Clear TWIx->CR.STA / TWIx->CR.STO */
    TWIx->CR.BIT.STA = SH_RESET;
    TWIx->CR.BIT.STO = SH_RESET;
    return (TWI_EVENT_TypeDef)TWIx->STAT.V32;
}

/**
  * @brief  Send TWI commands
  * @param  TWIx: where x can be 0, 1 to select the TWI peripheral.
  * @param  CmdType:    TWI Commands as below
  *           @arg @b TWI_CMD_START
  *           @arg @b TWI_CMD_ACK 
  *           @arg @b TWI_CMD_NACK
  *           @arg @b TWI_CMD_STOPSTART
  * @retval TWI_EVENT_TypeDef TWI event flags.
  */
TWI_EVENT_TypeDef TWI_SendCommand(TWI_TypeDef *TWIx, TWI_COMMAND_TypeDef CmdType)
{
    assert_param(IS_TWI_MODULE(TWIx));
    assert_param(IS_TWI_COMMAND_Type(CmdType));

    switch (CmdType)
    {
    case TWI_CMD_START:
        TWIx->CR.BIT.STA = SH_SET;
        TWIx->CR.BIT.STO = SH_RESET;
        break;
    case TWI_CMD_ACK:
        TWIx->CR.BIT.AA = TWI_AA_ACK;
        break;
    case TWI_CMD_NACK:
        TWIx->CR.BIT.AA = TWI_AA_NACK;
        break;
    case TWI_CMD_STOPSTART:
        TWIx->CR.BIT.STO = SH_SET;
        TWIx->CR.BIT.STA = SH_SET;
        break;
    default:
        break;
    }

    return TWI_SendWaitDone(TWIx);
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
