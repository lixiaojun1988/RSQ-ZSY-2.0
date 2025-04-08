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
#define     UARTx_TXD_PORT      GPIOC
#define     UARTx_TXD_PIN       GPIO_Pin_8
#define     UARTx_TXD_AF        PC9_TXD1

// RXD
#define     UARTx_RXD_PORT      GPIOC
#define     UARTx_RXD_PIN       GPIO_Pin_9
#define     UARTx_RXD_AF        PC9_RXD1

// Uart定义
#define     UARTx_BAUDRATE      (4800)              // 波特率
#define     UARTx               UART1               // Uart Base
#define     UARTx_IRQn          UART1_IRQn          // IRQ
#define     UARTx_Handler       UART1_Handler       // 中断处理程序
#define     UARTx_TDR           UART1->TDR          // 发送数据寄存器
#define     UARTx_RDR           UART1->RDR          // 接收数据寄存器
#define     UARTx_CLOCK         RCC_APB1_UART1      // 时钟
#define     UARTx_GPIO_AF       GPIO_AF_UART        // 复用


/**
 * @brief
 *
 */
void    bsp_uart1_init(void)
{
    GPIO_InitTypeDef    GPIO_InitStructure;
    UART_InitTypeDef    UART_InitStructure;

    /* Enable the GPIO Clock */
   RCC_AHBPeriphClockOnOff(RCC_AHB_GPIO, SH_ON);    

    /* Enable the Uartx Clock */
    RCC_APB1PeriphClockOnOff(UARTx_CLOCK, SH_ON);
		NVIC_DisableIRQ(UARTx_IRQn);//
		RCC_APB1PeriphReset(UARTx_CLOCK);//
    /* 串口TXD引脚设置为输出高 */
    GPIO_InitStructure.GPIO_Pin   = UARTx_TXD_PIN;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_ODrv  = GPIO_ODrv_NORMAL;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;
    GPIO_SetBits(UARTx_TXD_PORT, GPIO_InitStructure.GPIO_Pin);
    GPIO_Init(UARTx_TXD_PORT, &GPIO_InitStructure);
    GPIO_PinAFConfig(UARTx_TXD_PORT, GPIO_InitStructure.GPIO_Pin, UARTx_TXD_AF);

    /* RXD：设置为输入上拉 */
    GPIO_InitStructure.GPIO_Pin  = UARTx_RXD_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
    GPIO_InitStructure.GPIO_ODrv = GPIO_ODrv_NORMAL;
		GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_SetBits(UARTx_RXD_PORT, GPIO_InitStructure.GPIO_Pin);
    GPIO_Init(UARTx_RXD_PORT, &GPIO_InitStructure);
    GPIO_PinAFConfig(UARTx_RXD_PORT, GPIO_InitStructure.GPIO_Pin, UARTx_RXD_AF);

    /* 串口设置 */
    UART_InitStructure.UART_Mode     = UART_Mode_1;                     // 模式1
    UART_InitStructure.UART_BaudRate = UARTx_BAUDRATE;                  // 波特率：
    UART_InitStructure.UART_StopBits = UART_StopBits_1;                 // 停止位：1位
    UART_InitStructure.UART_Parity   = UART_SoftWare_Custom;            // 软件自定义(无校验)
    UART_InitStructure.UART_Enable   = UART_Enable_Tx | UART_Enable_Rx; // 允许收发
		//UART_InitStructure.UART_Enable   =  UART_Enable_Rx; // 允许收发
    UART_Init(UARTx, &UART_InitStructure);

    UART_ClearFlag(UARTx, UART_FLAG_ALL);                                        // 清除中断标志位
    UART_INTConfig(UARTx, UART_INT_RX, SH_ENABLE);        // 允许中断：发送/接收
		UART_INTConfig(UARTx, UART_INT_TC , SH_ENABLE);        // 允许中断：发送/接收
    NVIC_EnableIRQ(UARTx_IRQn);
///  直接操作寄存器位控制 参考
//		UARTx->CR.BIT.TEN=0; //发送使能关闭
//		UARTx->CR.BIT.TEN=1; //发送使能打开	
//		UARTx->CR.BIT.REN=0; //接收使能关闭
//		UARTx->CR.BIT.REN=1; //接收使能打开
}


//测试 DEMO——————————————————————————————————————————
// 收到什么数据就发送返回什么数据 DEMO
#define     UART_TRAN_BUF_LEN    16         // 发送缓冲区长度
#define     UART_RECE_BUF_LEN    16         // 接收缓冲区长度
typedef struct
{
    uint8_t     tranProcess;                // 发送进程
    uint8_t     tranIdx;                    // 发送第几个字节（已发送字节数）
    uint8_t     tranLen;                    // 这一帧要发送多少个字节
    uint8_t     tranBuf[UART_TRAN_BUF_LEN]; // 发送缓冲区

    uint8_t     receIdx;                    // 接收第几个字节（已接收字节数）
    uint8_t     receBuf[UART_RECE_BUF_LEN]; // 接收缓冲区
}ST_UartT;

ST_UartT     gSUART1Test;

#define     sUart   gSUART1Test


/**
  * @brief  Uart Test : 收到什么数据就发送返回什么数据
  * @param  None
  * @retval None 主循环调用
  */
void    UART1_Test(void)
{
    static  uint8_t     testIdx;                // 测试计数器

    uint8_t     receIdx=sUart.receIdx;

    if( sUart.tranProcess == 0 )                // 非发送中才允许去更新发送内容
    {
        if( testIdx != receIdx )
        {
            sUart.tranLen = 0;
            do
            {
                sUart.tranBuf[sUart.tranLen] = sUart.receBuf[testIdx];
                if( ++testIdx >= UART_RECE_BUF_LEN )
                {
                    testIdx = 0;
                }
                if( ++sUart.tranLen >= UART_TRAN_BUF_LEN )
                {
                    break;
                }
            }while(testIdx != receIdx);
            sUart.tranIdx       = 1;        // 已发送字节
            sUart.tranProcess   = 1;        // 发送中

            // 启动发送
						UART1->FR.BIT.TCC = 1;      // 清标志位
            UART1->TDR.BIT.TDR  = sUart.tranBuf[0];

        }
    }
}

// 中断处理函数，串口中断调用
void UART1_Handler(void)
{
    // received
    if( UART1->FR.BIT.RI) 
    {
			// 清接收标志
// RI只能通过对RDR读操作来清零  每次一定要读取缓存，取走数据
      //  recv_data = UART1->RDR.BIT.RDR;

        sUart.receBuf[sUart.receIdx] = UART1->RDR.BIT.RDR;
        if( ++sUart.receIdx >= UART_RECE_BUF_LEN )
        {
            sUart.receIdx = 0;
        }
    }

    // Transmit
    if (UART1->FR.BIT.TC)
    {
        UART1->FR.BIT.TCC = 1;      // 清标志位

        if( sUart.tranIdx < sUart.tranLen )
        {
            UART1->TDR.BIT.TDR = sUart.tranBuf[sUart.tranIdx];
            sUart.tranIdx ++;
        }
        else
        {
            sUart.tranProcess = 0;
        }
    }
		
		
					//		// 判断是否发生接收溢出标志, // 或主判断
		if(UART1->FR.BIT.RXOV)  
		{						
			//1： 要读取buf一下RDR	，清RDR缓存				
			//2:  要清接收溢出标志位
			UART_ReceiveData(UART1);	// 读取一次接收缓存				
			UART1->FR.BIT.RXOVC =1;	 //清接收溢出标志位
		}
}
//测试 DEMO——————————————————————————————————————————
/*----------------------------------------- E.N.D -----------------------------------------*/

/*----------------------------------------- E.N.D -----------------------------------------*/

/*----------------------------------------- E.N.D -----------------------------------------*/
