/**
  ******************************************************************************
  * @file    system_sh30f9xx_sb0.c
  * @author  sinowealth
  * @version V1.0.0
  * @date    2024-05-21  
  * @brief   This file SystemInit function redefinition
  *         
  @verbatim
  ==============================================================================
                         How to use this driver
  ==============================================================================
 [..]
   (#) Add this system_sh30f9xx_sb0.c and system_sh30f9xx_sb0.h in project

   (#) open system_sh30f9xx_sb0.h  and uncomment  clock configuration macro for project SYSCLK_FREQ_HSE,SYSCLK_FREQ_HSI,SYSCLK_FREQ_LSI
    
   (#) user code can call SystemCoreClockUpdate function update SystemCoreClock variable for get current AHB clock 

  @endverbatim
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
#include "system_sh30f9xx_sb0.h"

/*******************************************************************************
*  Clock Definitions
*******************************************************************************/
 #ifdef SYSCLK_FREQ_HSE
 uint32_t SystemCoreClock = HSE_VALUE; /*!< System Clock Frequency (Core Clock) */
 #elif defined SYSCLK_FREQ_HSI
 
#define  SYSCLK_FREQ_HSI_48M	//��Ƶ�ڲ�rc48m 1��Ƶ
 //#define  SYSCLK_FREQ_HSI_24M  // ��Ƶ�ڲ�rc 48m 2��Ƶ
 uint32_t SystemCoreClock = HSI_VALUE; /*!< System Clock Frequency (Core Clock) */
 #elif defined SYSCLK_FREQ_LSI
 uint32_t SystemCoreClock = LSI_VALUE; /*!< System Clock Frequency (Core Clock) */
 #endif

/*******************************************************************************
*  Private function declare
*******************************************************************************/
#ifdef SYSCLK_FREQ_HSE
static void SetSysClockToHSE(void);
#elif defined SYSCLK_FREQ_HSI
static void SetSysClockToHSI(void);
#elif defined SYSCLK_FREQ_LSI
static void SetSysClockToLSI(void);
#endif

/**
  * @brief  Setup the microcontroller system flash latency and clock
  * @note   This function should be used only after reset.
  * @retval None
  */
void SystemInit(void)
{
#ifdef SYSCLK_FREQ_HSE
    SetSysClockToHSE(); //        HSE:4-12M        SYSCLK=4-12M    AHB=HCLK=4-12M    APB0=4-12M    APB1=4-12M
#elif defined SYSCLK_FREQ_HSI
    SetSysClockToHSI(); //        RC:48M           SYSCLK=48M      AHB=HCLK=48M      APB0=24M      APB1=24M
#elif defined SYSCLK_FREQ_LSI
    SetSysClockToLSI(); //        RC:128K          SYSCLK=128K     AHB=HCLK=128K     APB0=128K     APB1=128K
#endif
}

/**
  * @brief  The SystemCoreClock variable sets the system core clock as a key parameter 
  *         for clock calculation in the entire system. Every time the system clock source is switched, 
  *         this function must be called to update the value of the SystemCoreClock variable. 
  *         Otherwise, any configuration based on this variable will be incorrect.      
  * @retval None
  */
void SystemCoreClockUpdate(void)
{
    uint8_t tmp = 0;
    uint8_t desOpt = 0;

    /* Get SYSCLK source -------------------------------------------------------*/
#ifndef SINO_SYSCLK_SET_VALUE
    tmp = RCC->CR.BIT.SWS;
    desOpt = (*(volatile uint8_t *)0x0FFFC048);
    switch (tmp)
    {
    case 0x00: /* HSI used as system clock */
        if ((desOpt&0x40) == 0x40)
        {
            SystemCoreClock = HSI_VALUE;
        }
        else
        {
            SystemCoreClock = HSI_VALUE>>1;
        }
        break;
    case 0x01: /* HSE used as system clock */
        SystemCoreClock = HSE_VALUE;
        break;
    case 0x02: /* LSI used as system clock */
        SystemCoreClock = LSI_VALUE;
        break;
    }
#else
    SystemCoreClock = SINO_SYSCLK_SET_VALUE;
#endif
}

#ifdef SYSCLK_FREQ_HSE
/**
  * @brief  Selects HSE as System clock source and configure HCLK, PCLK0
  *         and PCLK1 prescalers.
  * @note   This function should be used only after reset.  4M~12M
  * @param  None
  * @retval None
  */
static void SetSysClockToHSE(void)
{
    __IO uint32_t StartUpCounter = 0, HSEStatus = 0;

    /*Config Flash latency and external oscillator pin and debug pin function*/
    FLASH->ACR.V32 = 0x5AA50000; // Set flash latency to 0 wait state

    RCC->RCCLOCK = 0x33CC; //Unlock RCC Config Register
    //It is conducive to crystal oscillator starting 
    RCC->CFGR.BIT.HSESEL = 0x03;
    RCC->HSERFB = 0X03;//500KHz feedback
    RCC->HSEROEN = 1; 

    RCC->AHBENR.BIT.SYSCFGEN = 1;  //Enable SYSCFG Module
    SYSCFG->SAFR.V32 = 0x5AA50029; //Unlock SYSCFG SAFR register and set XTAL1 and XTAL2 pin for oscillator function
    /* SYSCLK, HCLK, PCLK2 and PCLK1 configuration ---------------------------*/
    /* Enable HSE */
    RCC->CR.V32 |= ((uint32_t)RCC_CR_HSEON);

    /* Wait till HSE is ready and if Time out is reached exit */
    do
    {
        HSEStatus = RCC->CR.V32 & RCC_CR_HSERDY;
        StartUpCounter++;
        HWDT->CLR = 0xAAAA; //Feed Dog
    } while ((HSEStatus == 0) && (StartUpCounter != HIGH_SPEED_CLOCK_STARTUP_TIMEOUT));

    if ((RCC->CR.V32 & RCC_CR_HSERDY) != 0) //HSE Startup Check
    {
        HSEStatus = (uint32_t)0x01; // HSE Startup succeed
    }
    else
    {
        HSEStatus = (uint32_t)0x00; // HSE Startup failed
    }

    if (HSEStatus) //HSE Startup succeed,Sysclock switch to HSE
    {
        RCC->CR.BIT.SW = SYSTEM_CLK_SRC_HSE; //Switch Sysclock to HSE
        do
        {
        } while (RCC->CR.BIT.SWS != SYSTEM_CLK_SRC_HSE); //Current SysClock is HSE

        /*********************Config HCLK PCLK0 PCLK1*************************/
        RCC->CFGR.BIT.HPRE = 0;  //Config AHB SYSCLK Prescaler to 1
        RCC->CFGR.BIT.PPRE0 = 0; //Config APB0 PCLK0 Prescaler to 1
        RCC->CFGR.BIT.PPRE1 = 0; //Config APB1 PCLK1 Prescaler to 1
    }
    else // HSE Startup failed, User can add here some code to deal with this error,eg switch to HSI
    {
    }
    RCC->RCCLOCK = 0; //LOCK RCC
}
#elif defined SYSCLK_FREQ_HSI
/**
  * @brief  Sets System clock frequency to HSI and configure HCLK, PCLK0
  *         and PCLK1 prescalers.
  * @note   This function should be used only after reset.   24M
  * @retval None
  */
static void SetSysClockToHSI(void)
{
    /*Config Flash latency and external oscillator pin and debug pin function*/
    FLASH->ACR.V32 = 0x5AA50002; // Set flash latency to 2 wait state

    RCC->RCCLOCK = 0x33CC;         //Unlock RCC Config Register

    RCC->AHBENR.BIT.SYSCFGEN = 1;  //Enable SYSCFG Module
    SYSCFG->SAFR.V32 = 0x5AA50028; //Unlock SYSCFG SAFR register and set XTAL1 and XTAL2 pin to GPIO function

    RCC->CR.BIT.HSION = 1; //Enable HSI
    while (!RCC->CR.BIT.HSIRDY)
    {
    }; //Wait HSI Ready

    RCC->CR.BIT.SW = SYSTEM_CLK_SRC_HSI; //Switch Sysclk to HSI
    while (RCC->CR.BIT.SWS != SYSTEM_CLK_SRC_HSI); //Wait Sysclock switch to HSI

    /*********************Config HCLK PCLK0 PCLK1*************************/
	#ifdef SYSCLK_FREQ_HSI_48M
		#warning "���ڲ�RC48M  1��Ƶ 48M"
		/// SYSCLK=48M      AHB=HCLK=48M      APB0=24M      APB1=24M
    RCC->CFGR.BIT.HPRE  = 0;  //Config AHB SYSCLK Prescaler to 1
    RCC->CFGR.BIT.PPRE0 = 1; //Config APB0 PCLK0 Prescaler to 2
    RCC->CFGR.BIT.PPRE1 = 1; //Config APB1 PCLK1 Prescaler to 2
	#endif
		
	#ifdef SYSCLK_FREQ_HSI_24M
			#warning "���ڲ�RC48M  2��Ƶ 24M"  // ע��ʱ����APB0/1��Ƶ��Ҫ����24M
		/// SYSCLK=48M      AHB=HCLK/2=24M      APB0=24M      APB1=24M
	  RCC->CFGR.BIT.HPRE  = 1;  //Config AHB SYSCLK Prescaler to 2
    RCC->CFGR.BIT.PPRE0 = 0; //Config APB0 PCLK0 Prescaler to 1
    RCC->CFGR.BIT.PPRE1 = 0; //Config APB1 PCLK1 Prescaler to 1
	#endif		
    RCC->RCCLOCK = 0; //RCC LOCK
}

#elif defined SYSCLK_FREQ_LSI
/**
  * @brief  Sets System clock frequency to LSI and configure HCLK, PCLK0
  *         and PCLK1 prescalers.
  * @note   This function should be used only after reset.
  * @retval None
  */
static void SetSysClockToLSI(void)
{
    __IO uint32_t StartUpCounter = 0, LSIStatus = 0;

    /*Config Flash latency and external oscillator pin and debug pin function*/
    FLASH->ACR.V32 = 0x5AA50000; // Set flash latency to 0 wait state

    RCC->RCCLOCK = 0x33CC; //Unlock RCC Config Register

    RCC->AHBENR.BIT.SYSCFGEN = 1; //Enable SYSCFG Module Clock

    SYSCFG->SAFR.V32 = 0x5AA50028; //Unlock SYSCFG SAFR register and set XTALX1 and XTALX2 pin for GPIO function

    /* SYSCLK, HCLK, PCLK2 and PCLK1 configuration ---------------------------*/
    /* Enable LSE */
    RCC->CR.V32 |= ((uint32_t)RCC_CR_LSION);

    /* Wait till LSI is ready and if Time out is reached exit */
    do
    {
        LSIStatus = RCC->CR.V32 & RCC_CR_LSIRDY;
        StartUpCounter++;
        HWDT->CLR = 0xAAAA; //Feed Dog
    } while ((LSIStatus == 0) && (StartUpCounter != LOW_SPEED_CLOCK_STARTUP_TIMEOUT));

    if ((RCC->CR.V32 & RCC_CR_LSIRDY) != 0) //LSI Startup Check
    {
        LSIStatus = (uint32_t)0x01; // LSI Startup succeed
    }
    else
    {
        LSIStatus = (uint32_t)0x00; // LSI Startup failed
    }

    if (LSIStatus) //LSI Startup succeed,Sysclock switch to LSI
    {
        RCC->CR.BIT.SW = SYSTEM_CLK_SRC_LSI; //Switch Sysclock to LSI
        do
        {
        } while (RCC->CR.BIT.SWS != SYSTEM_CLK_SRC_LSI); //Current SysClock is LSI

        /*********************Config HCLK PCLK0 PCLK1*************************/
        RCC->CFGR.BIT.HPRE = 0;  //Config AHB SYSCLK Prescaler to 1,no effect while clock source is LSI
        RCC->CFGR.BIT.PPRE0 = 0; //Config APB0 PCLK0 Prescaler to 1,no effect while clock source is LSI
        RCC->CFGR.BIT.PPRE1 = 0; //Config APB1 PCLK1 Prescaler to 1,no effect while clock source is LSI
    }
    else // HSE Startup failed, User can add here some code to deal with this error,eg switch to HSI
    {
    }
    RCC->RCCLOCK = 0;                                                                                           //LOCK RCC
    HWDT->CR.V32 |= ((0x5AA5 << HWDT_CR_LOCK_Pos)| (0x0 << HWDT_CR_HWDTRLR_Pos));   //Set HWDT max reload period ,avoid hwdt reset while jump to main
}

#endif
/************************ (C) COPYRIGHT Sinowealth *****END OF FILE****/
