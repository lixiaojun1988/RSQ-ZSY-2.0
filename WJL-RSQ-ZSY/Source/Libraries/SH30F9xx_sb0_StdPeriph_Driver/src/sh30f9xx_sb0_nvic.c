/**
  ******************************************************************************
  * @file    sh30f9xx_sb0_nvic.c
  * @author  sinowealth
  * @version V1.0.0
  * @date    2024-5-24  
  * @brief   This file provides firmware functions to manage the NVIC module 
  *  
    @verbatim

  ==============================================================================
                          How to use this driver
  ==============================================================================
 [..]
   (#) Config One Module priority use follow step:
 
        (++) Fill NVIC_InitTypeDef type structure variable member as default use NVIC_StructInit() function
        
        (++) Config NVIC_InitTypeDef type structure variable member accroding which module use.
        
        (++) Call NVIC_Init() function for module interrupt config    
   
   (#) User may call NVIC_SetVectorTable() function for redirect interrupt vector table purpuse.
   
   (#) User may call SysTick_TimeConfig() function for config systick interrupt period and enable systick module 
       and call SysTick_GetCounter() function for get current systick counter.
  * @endverbatim
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
#include "sh30f9xx_sb0_nvic.h"

/** @addtogroup SH30F9XX_SB0_STANDARD_LIB
  * @{
  */

/** @defgroup NVIC_MODULE NVIC
  * @brief SH30F9XX SB0 NVIC module driver library,reference this topic macro and function for drive NVIC
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/* Export function prototypes -----------------------------------------------*/

/** @defgroup NVIC_Exported_Functions NVIC Exported Functions
 *  @brief    This is NVIC exported function code group
 *
  * @{
  */
  
/**
  * @brief  Fills each NVIC_InitStruct member with its default value.
  * @param  NVIC_InitStruct : pointer to a @ref NVIC_InitTypeDef structure which will
  *         be initialized.
  * @retval None
  */
void NVIC_StructInit(NVIC_InitTypeDef *NVIC_InitStruct)
{
    NVIC_InitStruct->NVIC_IRQChannel = (IRQn_Type)0;
    NVIC_InitStruct->NVIC_IRQChannelPriority = 0;
    NVIC_InitStruct->NVIC_IRQChannelCmd = SH_DISABLE;
}

/**
  * @brief  Initializes the NVIC peripheral according to the specified parameters 
  *         in the NVIC_InitStruct.
  * @param  NVIC_InitStruct: pointer to a @ref NVIC_InitTypeDef structure that contains
  *         the configuration information for the specified NVIC peripheral.
  * @retval None
  */
void NVIC_Init(NVIC_InitTypeDef *NVIC_InitStruct)
{
    uint32_t tmppriority = 0x00;
    /* Check the parameters */
    assert_param(IS_FUNCTION_STATE(NVIC_InitStruct->NVIC_IRQChannelCmd));

    if (NVIC_InitStruct->NVIC_IRQChannelCmd != SH_DISABLE)
    {
        /* Compute the Corresponding IRQ Priority --------------------------------*/
        tmppriority = NVIC->IP[NVIC_InitStruct->NVIC_IRQChannel >> 0x02];
        tmppriority &= (uint32_t)(~(((uint32_t)0xFF) << ((NVIC_InitStruct->NVIC_IRQChannel & 0x03) * 8)));
        tmppriority |= (uint32_t)((((uint32_t)NVIC_InitStruct->NVIC_IRQChannelPriority << 6) & 0xFF) \
                       << ((NVIC_InitStruct->NVIC_IRQChannel & 0x03) * 8));

        NVIC->IP[NVIC_InitStruct->NVIC_IRQChannel >> 0x02] = tmppriority;
        //NVIC_SetPriority(NVIC_InitStruct->NVIC_IRQChannel,NVIC_InitStruct->NVIC_IRQChannelPriority);
        /* Enable the Selected IRQ Channels --------------------------------------*/
        NVIC->ISER[0U] = (uint32_t)(0x01 << (NVIC_InitStruct->NVIC_IRQChannel & (uint8_t)0x1F));
    }
    else
    {
        /* Disable the Selected IRQ Channels -------------------------------------*/
        NVIC->ICER[0U] = (uint32_t)(0x01 << (NVIC_InitStruct->NVIC_IRQChannel & (uint8_t)0x1F));
    }
}
/**
  ******************************************************************************
  * @brief  Configure interrupt Priority
  * @param  IRQChannel: Specifies the IRQ channel
  *          This parameter can be an enumerator of @ref IRQn_Type
  * @param  ChannelPriority: Specifies the priority for the IRQ channel
  *          This parameter can be a value between 0 and 3 as described.
  *          The smaller the value, the higher the priority
  * @retval None

  ******************************************************************************
  */
void NVIC_Configuration(IRQn_Type IRQChannel, uint8_t ChannelPriority)
{
    NVIC_InitTypeDef NVIC_InitStructure;
    /* Configure interrupt source */
    NVIC_InitStructure.NVIC_IRQChannel = IRQChannel;
    /* priority*/
    NVIC_InitStructure.NVIC_IRQChannelPriority = ChannelPriority;
    /* enable interrupt */
    NVIC_InitStructure.NVIC_IRQChannelCmd = SH_ENABLE;
    /* initial config NVIC */
    NVIC_Init(&NVIC_InitStructure);
}

/**
  * @brief  Sets the vector table location and Offset.
  * @param  NVIC_VectTab: specifies if the vector table is in RAM or FLASH memory.
  *          This parameter can be one of the following values:
  *            @arg @b NVIC_VectTab_RAM
  *            @arg @b NVIC_VectTab_FLASH
  * @param  Offset: Vector Table base offset field. This value must be a multiple 
  *         of 0x100.
  * @retval None
  */
void NVIC_SetVectorTable(uint32_t NVIC_VectTab, uint32_t Offset)
{
    SCB->VTOR = NVIC_VectTab | (Offset & (uint32_t)0x0FFFFF00);
}

/**
  * @brief  Config SysTick timer.
  * @param  MilliSecond: specifie milliseconds to be set to SysTick,
  *         This parameter can be 0 to 1000:
  * @retval None
  */
void SysTick_TimeConfig(uint16_t MilliSecond)
{
    RCC_Clocks_TypeDef clockStatus;

    RCC_GetClocksFreq(&clockStatus);
    SysTick->LOAD = ((clockStatus.hclkFreq / 100) >> 3) * MilliSecond;
    SysTick->VAL = 0;
    SysTick->CTRL &= ~SysTick_CTRL_CLKSOURCE_Msk;
    SysTick->CTRL |= SysTick_CTRL_TICKINT_Msk | SysTick_CTRL_ENABLE_Msk;
}

/**
  * @brief  Get the SysTick Counter value.
  * @retval SysTick Counter Register value.
  */
uint32_t SysTick_GetCounter(void)
{
    /* Get the SysTick Counter value */
    return SysTick->VAL;
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
