/**
  ******************************************************************************
  * @file    sh30f9xx_sb0_spi.c
  * @author  sinowealth
  * @version V1.0.0
  * @date    2024-5-30  
  * @brief   This file provides firmware functions to manage the SPI module 
  *  
    @verbatim
  ==============================================================================
                         How to use this driver
  ==============================================================================
   [..]
     (#) Enable SPI Clock use RCC_APB1PeriphClockOnOff() function 
  
     (#) Fill SPI Init Structure member as default use SPI_StructInit() function
     
     (#) Config SPI Pin
         (++) Enable Pin Clock calling RCC_AHBPeriphClockOnOff() function 
         
         (++) Call GPIO_Init()function for config SPI pin(input or output ,pull up or no-pull etc ...),if not use all member for SPI 
              pin config ,makesure call GPIO_StructInit() function for GPIO_Init function  paramter set default value
         
         (++) Call GPIO_PinAFConfig() for config pin as SPI function
        
     (#) Config SPI Clock ,capture edge, master or slave mode, data transmission direction etc use SPI_Init() function
     
     (#) Enable SPI Module by calling SPI_OnOff() function
     
     (#) Use SPI interrupt by following step
         
         (++) Config SPI Interrupt use SPI_INTConfig() function
         
         (++) Enable Kernel SPI's Global interrupt by calling NVIC_EnableIRQ() function
         
         (++) reference startup asm file for SPI interrupt function name and rewrite this function 	
      
     (#) When SPI interrupt happen, use SPI_GetFlagStatus() function to get SPI interrupt flag

     (#) Call SPI_ClearFlag() function for clear SPI interrupt flag
  
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
#include "sh30f9xx_sb0_spi.h"

/** @addtogroup SH30F9XX_SB0_STANDARD_LIB
  * @{
  */

/** @defgroup SPI_MODULE SPI
  * @brief SH30F9XX SB0 SPI module driver library,reference this topic macro and function for drive SPI
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/* Export function prototypes -----------------------------------------------*/

/** @defgroup SPI_Exported_Functions SPI Exported Functions
 *  @brief    This is SPI exported function code group
 *
  * @{
  */

/**
  * @brief  Deinitializes the SPIx peripheral registers to their default
  *         reset values 
  * @param  SPIx: where x can be 0, 1 to select the SPI peripheral.
  * @retval None
  */
void SPI_Reset(SPI_TypeDef *SPIx)
{
    /* Check the parameters */
    assert_param(IS_SPI_MODULE(SPIx));

    if (SPIx == SPI0)
    {
        /* Reset SPI0 module */
        RCC_APB1PeriphReset(RCC_APB1_SPI0);
    }
    else if (SPIx == SPI1)
    {
        /* Reset SPI1 module */
        RCC_APB1PeriphReset(RCC_APB1_SPI1);
    }
    else{}
}

/**
  * @brief  Fills each SPI_InitStruct member with its default value.
  * @param  SPI_InitStruct : pointer to a @ref SPI_InitTypeDef structure which will
  *         be initialized.
  * @retval None
  */
void SPI_StructInit(SPI_InitTypeDef *SPI_InitStruct)
{
    SPI_InitStruct->Clock = SPR_PCLK1_DIV_2;
    SPI_InitStruct->SSPin = PIN_SS_ENABLE;
    SPI_InitStruct->ClkIdleState = CLK_IDLE_LOW;
    SPI_InitStruct->CaptureEdge = CAP_CLK_FIRST_EDGE;
    SPI_InitStruct->MasterOrSlave = SPI_SLAVE_MODE;
    SPI_InitStruct->Direction = SPI_TRANS_MSB;
    SPI_InitStruct->SlaveTransMode = SPI_SLAVE_NORMAL;
    SPI_InitStruct->DataSize = SPI_DATASIZE_8bit;
}

/**
  * @brief  Initializes the SPIx peripheral according to the specified 
  *         parameters in the SPI_InitStruct.
  * @param  SPIx SPI device: SPI0 or SPI1
  * @param  SPI_InitStruct  pointer to a @ref SPI_InitTypeDef structure that
  *         contains the configuration information for the specified SPI peripheral.
  * @retval None
  */

void SPI_Init(SPI_TypeDef *SPIx, SPI_InitTypeDef *SPI_InitStruct)
{
    assert_param(IS_SPI_MODULE(SPIx));
    assert_param(IS_SPI_SPR_Type(SPI_InitStruct->Clock));
    assert_param(IS_SPI_MSTR_Type(SPI_InitStruct->MasterOrSlave));
    assert_param(IS_SPI_SPSFF_Type(SPI_InitStruct->SlaveTransMode));

    /* Selects the data transfer baudrate for the specified SPI.*/
    SPIx->CR.BIT.SPR = SPI_InitStruct->Clock;            
    /* Enables or disables the NSS Pin for the selected SPI.*/
    SPIx->CR.BIT.SSDIS = SPI_InitStruct->SSPin;          
    SPIx->CR.BIT.CPOL = SPI_InitStruct->ClkIdleState;    
    SPIx->CR.BIT.CPHA = SPI_InitStruct->CaptureEdge;     
    /* Selects the SPI mode, SPI_MASTER or SPI_SLAVE*/
    SPIx->CR.BIT.MSTR = SPI_InitStruct->MasterOrSlave;   
    /* Selects the data transfer direction  for the specified SPI.*/
    SPIx->CR.BIT.DIR = SPI_InitStruct->Direction;        
 
    SPIx->CR.BIT.SPSFF = SPI_InitStruct->SlaveTransMode; 
    /* Configures the data size for the selected SPI*/
    SPIx->CR.BIT.SPDATL = SPI_InitStruct->DataSize;      
}

/**
  * @brief  Enables or disables the specified SPI peripheral.
  * @param  SPIx: where x can be 0, 1 to select the SPI peripheral.
  * @param  OnOffState: new state of the SPIx peripheral. 
  *         This parameter can be one of the following values:
  *            @arg @b SH_ON
  *            @arg @b SH_OFF
  * @retval None
  */
void SPI_OnOff(SPI_TypeDef *SPIx, CmdState OnOffState)
{
    assert_param(IS_SPI_MODULE(SPIx));
    assert_param(IS_CMD_STATE(OnOffState));

    SPIx->CR.BIT.SPIEN = OnOffState;
}

/**
  * @brief  Checks whether the specified SPI interrupt has occurred or not.
  * @param  SPIx: where x can be 0, 1 in SPI mode 
  * @param  SPIFunc: specifies the SPI interrupt source to check. 
  *         This parameter can be one of the following values:
  *     @arg @b SPI_FUNC_SEND
  *     @arg @b SPI_FUNC_RECEIVE
  * @retval FlagStatus The new state interrupt (SH_SET or SH_RESET).
  */
FlagStatus SPI_GetINTStatus(SPI_TypeDef *SPIx, SPI_FUNC_TypeDef SPIFunc)
{
    FlagStatus status = SH_RESET;

    assert_param(IS_SPI_MODULE(SPIx));
    assert_param(IS_SPI_FUNC_Type(SPIFunc));

    if (SPIFunc == SPI_FUNC_SEND)
    {
        if(SPIx->FR.BIT.SPTI && SPIx->CR.BIT.SPTIE)
        {
            status = SH_SET;
        }
        else
        {
            status = SH_RESET;
        }
    }
    else if (SPIFunc == SPI_FUNC_RECEIVE)
    {
        if(SPIx->FR.BIT.SPRI && SPIx->CR.BIT.SPRIE)
        {
            status = SH_SET;
        }
        else
        {
            status = SH_RESET;
        }
    }

    return status;
}

/**
  * @brief  Enable or disable the specified SPI's interrupts.
  * @param  SPIx: where x can be 0 or 1 to select the SPI peripheral.
  * @param  SPIFunc: specifies the SPI interrupt source to check. 
  *         This parameter can be one of the following values:
  *         @arg @b SPI_FUNC_SEND
  *         @arg @b SPI_FUNC_RECEIVE
  * @param  NewState: New state of SPI interrupt.
  *         This parameter can be one of the following values:
  *         @arg @b SH_ENABLE
  *         @arg @b SH_DISABLE
  * @retval None
  */
void SPI_INTConfig(SPI_TypeDef *SPIx, SPI_FUNC_TypeDef SPIFunc, FunctionalState NewState)
{
    /* Check the parameters */
    assert_param(IS_SPI_MODULE(SPIx));
    assert_param(IS_SPI_FUNC_Type(SPIFunc));
    assert_param(IS_FUNCTION_STATE(NewState));

    if (NewState != SH_DISABLE)
    {
        if (SPIFunc == SPI_FUNC_SEND)
        {
            SPIx->CR.BIT.SPTIE = 1;
        }
        else if (SPIFunc == SPI_FUNC_RECEIVE)
        {
            SPIx->CR.BIT.SPRIE = 1;
        }
    }
    else
    {
        if (SPIFunc == SPI_FUNC_SEND)
        {
            SPIx->CR.BIT.SPTIE = 0;
        }
        else if (SPIFunc == SPI_FUNC_RECEIVE)
        {
            SPIx->CR.BIT.SPRIE = 0;
        }
    }
}

/**
  * @brief  Get SPI transmit flag
  * @param  SPIx: SPI device: SPI0 or SPI1
  * @param  Flag: SPI flag mask
  *         This parameter can be one of following values:  
  *     @arg @b SPI_FLAG_RECEIVE_INT
  *     @arg @b SPI_FLAG_SEND_INT
  *     @arg @b SPI_FLAG_MODE_ERROR
  *     @arg @b SPI_FLAG_RECIEVE_OVER
  *     @arg @b SPI_FLAG_SEND_CONFLICT
  * @retval  FlagStatus RCC reset flag
  *     @arg @b SH_SET
  *     @arg @b SH_RESET
  */
FlagStatus SPI_GetFlagStatus(SPI_TypeDef *SPIx, SPI_FLAG_TypeDef Flag)
{
    assert_param(IS_SPI_MODULE(SPIx));
    assert_param(IS_SPI_FLAG_Type(Flag));

    return ((SPIx->FR.V32 & Flag) ? SH_SET : SH_RESET);
}

/**
  * @brief  clear SPI transmit flags
  * @param  SPIx: SPI device: SPI0 or SPI1
  * @param  Flags: SPI flags clear bit mask
  *         This parameter can be any combination of following values:  
  *     @arg @b SPI_FLAG_MODE_ERROR
  *     @arg @b SPI_FLAG_RECIEVE_OVER
  *     @arg @b SPI_FLAG_SEND_CONFLICT
  * @retval None
  */
void SPI_ClearFlag(SPI_TypeDef *SPIx, SPI_FLAG_TypeDef Flags)
{
    assert_param(IS_SPI_MODULE(SPIx));
    assert_param(IS_SPI_FLAG_Type(Flags));

    SPIx->FR.V32 = (Flags << 16);
}

/**
  * @brief  SPI send data
  * @param  SPIx SPI device: SPI0 or SPI1
  * @param  Data send data
  * @retval None
  */
void SPI_SendData(SPI_TypeDef *SPIx, uint32_t Data)
{
    assert_param(IS_SPI_MODULE(SPIx));

    SPIx->TDR = Data;
}

/**
  * @brief  SPI receive data
  * @param  SPIx SPI device: SPI0 or SPI1
  * @retval received data
  */
uint32_t SPI_ReceiveData(SPI_TypeDef *SPIx)
{
    assert_param(IS_SPI_MODULE(SPIx));

    return SPIx->RDR;
}

/**
  * @brief  Enables or disables the SPI FIFO function.
  * @param  SPIx: where x can be 0, 1 to select the SPI peripheral.
  * @param  OnOffState: new state of the FIFOx peripheral. 
  *         This parameter can be one of the following values:
  *            @arg @b SH_ON
  *            @arg @b SH_OFF
  * @retval None
  */
void SPI_FIFO_OnOff(SPI_TypeDef *SPIx, CmdState OnOffState)
{
    assert_param(IS_SPI_MODULE(SPIx));
    assert_param(IS_CMD_STATE(OnOffState));

    SPIx->CR.BIT.FIFOEN = OnOffState;
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
