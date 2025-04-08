/**
 * *****************************************************************************
 * @file     bsp_uart.c
 * @brief
 * @author   sinowealth
 * @version  1.0
 * @date     2024-08-07
 *
 * *****************************************************************************
 */

/* Includes ------------------------------------------------------------------*/

#include "sh30f9010.h"


// TXD
#define     UARTx_TXD_PORT      GPIOB
#define     UARTx_TXD_PIN       GPIO_Pin_9
#define     UARTx_TXD_AF        PB9_TXD5

// RXD
#define     UARTx_RXD_PORT      GPIOB
#define     UARTx_RXD_PIN       GPIO_Pin_12
#define     UARTx_RXD_AF        PB12_RXD5

// Uart定义
#define     UARTx_BAUDRATE      (9600)              // 波特率
#define     UARTx               UART5               // Uart Base
#define     UARTx_IRQn          UART5_IRQn          // IRQ
#define     UARTx_Handler       UART5_Handler       // 中断处理程序
#define     UARTx_TDR           UART5->TDR          // 发送数据寄存器
#define     UARTx_RDR           UART5->RDR          // 接收数据寄存器
#define     UARTx_CLOCK         RCC_APB1_UART5      // 时钟
#define     UARTx_GPIO_AF       GPIO_AF_UART        // 复用

/**
 * @brief
 *
 */
