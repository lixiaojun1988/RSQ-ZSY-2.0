/**
 * @file      sh30f9xx_sb0_interrupt.c
 * @brief     Main Interrupt Service Routines.
 *            This file provides template for all exceptions handler and
 *            peripherals interrupt service routine.
 * @author    sinowealth software group ()
 * @date      2024-06-21
 * @version   V1.0
 * @copyright Copyright (c) 1994 - 2024 SINOWEALTH Electronic Ltd.
 *
 * @attention
 * SINOWEALTH IS SUPPLYING THIS SOFTWARE FOR USE EXCLUSIVELY ON SINOWEALTH'S
 * MICROCONTROLLER PRODUCTS. IT IS PROTECTED UNDER APPLICABLE COPYRIGHT LAWS.
 * THIS SOFTWARE IS FOR GUIDANCE IN ORDER TO SAVE TIME. AS A RESULT, SINOWEALTH
 * SHALL NOT BE HELD LIABLE FOR ANY DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES
 * WITH RESPECT TO ANY CLAIMS ARISING FROM THE CONTENT OF SUCH FIRMWARE AND/OR
 * THE USE MADE BY CUSTOMERS OF THE CODING INFORMATION CONTAINED HEREIN IN
 * CONNECTION WITH THEIR PRODUCTS.
 * @par Change log:
 * <table>
 * <tr><th>Date        <th>Version  <th>Author    <th>Description
 * <tr><td>2024-06-21  <td>1.0      <td>bruce     <td>Create initial version
 * </table>
 */

/* Includes ------------------------------------------------------------------*/
#include "sh30f9xx_sb0_libcfg.h"
#include "all_head.h"
// #include "..\source_code\timer\timer.h"
/** @addtogroup Template_Project
 * @{
 */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M0+ Processor Exceptions Handlers                         */
/******************************************************************************/

/**
 * @brief   This function handles NMI exception.
 * @retval  None
 * @note    After the occurrence of CSM, although the system clock has
 *          automatically switched to HSI/8, in order to maintain system stability,
 *          the AHB bus division coefficient must be set to 8. After the failed clock
 *          is restored (HSERDY flag is set), clear the CSM flag, and the system clock
 *          will switch back to the previous abnormal clock. Then, change the AHB bus
 *          division coefficient back to the original set value.
 */
void NMI_Handler(void)
{
  static uint8_t clockDivision = 0;
  if (RCC_GetFlagStatus(RCC_CSM_HSE_IF) == SH_SET)
  {
    /* The system clock has automatically switched to HSI/8 */

    /*Backup AHB bus division coefficient*/
    clockDivision = RCC_GetAHBClockDivision();
    /*The AHB bus division coefficient must be set to 8 divisions*/
    RCC_SetAHBClockDivision(RCC_AHB_DIV8);

    /* This is to prevent the occurrence of the HSI clock being manually turned off when HSE is used as a system */
    RCC_HSIOnOff(SH_ON);
    while (RCC_GetHSEReadyFlag() == SH_RESET)
      ;
    /* Switch the clock source to HSE */
    RCC_SetSystemClockSource(RCC_SYS_SRC_HSI);

    /* HSE must be closed first and then opened again */
    RCC_HSEOnOff(SH_OFF);
    __nop();
    RCC_HSEOnOff(SH_ON);

    /* Next, we are preparing to restore the clock by querying it */
    while (RCC_GetHSEReadyFlag() == SH_RESET)
    {
      HWDT_ReloadCounter();
    }
    /* Clear the CSM interrupt hold bit */
    RCC_ClearITPendingBit(RCC_CICCLR_CSMC);
    /* Switch the clock source to HSE */
    RCC_SetSystemClockSource(RCC_SYS_SRC_HSE);
    /* Restore the AHB bus to its pre abnormal value */
    RCC_SetAHBClockDivision((RCC_AHB_DIVISION_TypeDef)clockDivision);
  }
}

/**
 * @brief  This function handles Hard Fault exception.
 * @retval None
 */
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
    HWDT->CLR = 0xAAAA;
  }
}

/**
 * @brief  This function handles SVC exception.
 * @retval None
 */
void SVC_Handler(void)
{
}

/**
 * @brief  This function handles PendSVC exception.
 * @retval None
 */
void PendSV_Handler(void)
{
}

/**
 * @brief  This function handles SysTick exception.
 * @param  None
 * @retval None
 */
void SysTick_Handler(void)
{
  taskTick();
  usartCheckReceive();
  comm_tick();
  ValveWorkCtrl();
}

/**
 * @brief  This function handles ADC exception.
 * @param  None
 * @retval None
 */
void ADC_Handler(void)
{
}

/**
 * @brief  This function handles TIM2 exception.
 * @param  None
 * @retval None
 */
void TIM2_FIFO1_Handler(void) // 100uS
{
  TIM2->TIMINTF.V32 = TIM_TIMINTF_TFC_Msk;
  CheckPulseTm();

}

/**
 * @brief  This function handles TIM3 exception.
 * @param  None
 * @retval None
 */
// void TIM3_Handler(void)
//{
//     TIM3->TIMINTF.V32 = TIM_TIMINTF_TFC_Msk;    // ????

//}

///**
//  * @brief  This function handles UART1 interrupt request.
//  * @param  None
//  * @retval None
//  */
// void UART0_Handler(void)
//{
////    uint8_t recv_data;

////    // received
////    if (UART0->FR.BIT.RI)
////    {
////        // RI?????RDR??????
////        recv_data = UART0->RDR.BIT.RDR;

////    }

////    // Transmit
////    if (UART0->FR.BIT.TC)
////    {
////        UART0->FR.BIT.TCC = 1;      // ????

////        // UART0->TDR.BIT.TDR = 0xAA; // test ??AA
////    }
//		UART0_ISR();//
//
//
//		// ????????????, // ????
//		if(UART0->FR.BIT.RXOV)
//		{
//			//1: ???buf??RDR	,?RDR??
//			//2:  ?????????
//			UART_ReceiveData(UART0);	// ????????
//			UART0->FR.BIT.RXOVC =1;	 //????????
//		}
//
//}

/**
 * @brief  This function handles UART1 interrupt request.
 * @param  None
 * @retval None
 */
// void UART1_Handler(void)
//{
//     uint8_t recv_data;

////    // received
////    if (UART1->FR.BIT.RI)
////    {
////        // RI?????RDR??????
////        recv_data = UART1->RDR.BIT.RDR;
////    }

////    // Transmit
////    if (UART1->FR.BIT.TC)
////    {
////        UART1->FR.BIT.TCC = 1;      // ????
////     //   UART1->TDR.BIT.TDR = 0xAA; // test ??AA
////    }
//
//		UART1_ISR();
//		// ????????????, // ????
//		if(UART1->FR.BIT.RXOV)
//		{
//			//1: ???buf??RDR	,?RDR??
//			//2:  ?????????
//			UART_ReceiveData(UART1);	// ????????
//			UART1->FR.BIT.RXOVC =1;	 //????????
//		}
//
//}

///**
//  * @brief  This function handles UART1 interrupt request.
//  * @param  None
//  * @retval None
//  */
// void UART2_Handler(void)
//{
//    uint8_t recv_data;

//    // received
//    if (UART2->FR.BIT.RI)
//    {
//        // RI?????RDR??????
//        recv_data = UART2->RDR.BIT.RDR;

//    }

//    // Transmit
//    if (UART2->FR.BIT.TC)
//    {
//        UART2->FR.BIT.TCC = 1;      // ????

//      //   UART2->TDR.BIT.TDR = 0xAA; // test ??AA
//    }
//
//
//		// ????????????, // ????
//		if(UART2->FR.BIT.RXOV)
//		{
//			//1: ???buf??RDR	,?RDR??
//			//2:  ?????????
//			UART_ReceiveData(UART2);	// ????????
//			UART2->FR.BIT.RXOVC =1;	 //????????
//		}
//
//}

///**
//  * @brief  This function handles UART1 interrupt request.
//  * @param  None
//  * @retval None
//  */
// void UART3_Handler(void)
//{
////    uint8_t recv_data;

////    // received
////    if (UART3->FR.BIT.RI)
////    {
////        // RI?????RDR??????
////        recv_data = UART3->RDR.BIT.RDR;

////    }

////    // Transmit
////    if (UART3->FR.BIT.TC)
////    {
////        UART3->FR.BIT.TCC = 1;      // ????

////        //UART3->TDR.BIT.TDR = 0xAA; // test ??AA
////    }
//	UART3_ISR();

//
//
//		// ????????????, // ????
//		if(UART3->FR.BIT.RXOV)
//		{
//			//1: ???buf??RDR	,?RDR??
//			//2:  ?????????
//			UART_ReceiveData(UART3);	// ????????
//			UART3->FR.BIT.RXOVC =1;	 //????????
//		}
//
//}
/**
 * @brief  This function handles PCA1 exception.
 * @param  None
 * @retval None
 */
void PCA1_Handler(void)
{
  PCA1->SR.V32 |= ((uint32_t)0x93 << 16); // ????

  //   bsp_pca_handler();
}

/************************ (C) COPYRIGHT Sinowealth *****END OF FILE****/
