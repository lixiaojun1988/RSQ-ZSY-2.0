/**
 * *****************************************************************************
 * @file     bsp_pwm.c
 * @brief
 * @author   sinowealth
 * @version  1.0
 * @date     2024-08-07
 *
 * *****************************************************************************
 */

/* Includes ------------------------------------------------------------------*/
#include "sh30f9010.h"



/**
 * @brief
 *
 */
void    bsp_pwm3_init(void)
{
    GPIO_InitTypeDef    GPIO_InitStructure;
    PWM_InitTypeDef     PWM_InitStructure;

    /*-------------------------------------------------------------------
    ;               PWM  pc3
    -------------------------------------------------------------------*/
    /* Enable the GPIO Clock *///FJ-PWM
    RCC_AHBPeriphClockOnOff(RCC_AHB_GPIO, SH_ON);    

    /* IO 锟斤拷锟斤拷 */
    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_3;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_ODrv  = GPIO_ODrv_NORMAL;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;
    GPIO_ResetBits(GPIOC, GPIO_InitStructure.GPIO_Pin); // 锟斤拷锟斤拷锟�
    GPIO_Init(GPIOC, &GPIO_InitStructure);

    /* PWM */
    RCC_APB0PeriphClockOnOff(RCC_APB0_PWM3, SH_ON);     /* Enable the PWMx Clock */
		PWM_OnOff(PWM3, SH_OFF);  
    /* PWMx锟斤拷锟斤拷 */
    PWM_InitStructure.PWM_CLKSource = TCK_PCLK0_DIV1;       // 锟斤拷频锟斤拷
    PWM_InitStructure.PWM_Period    = 24000/2;             // 锟斤拷锟节ｏ拷24mhz/4000=4khz
    PWM_InitStructure.PWM_Duty      = 12000/2;             // 占锟秸比ｏ拷
    PWM_InitStructure.PWM_DeadBand  = 0;                // 锟斤拷锟斤拷时锟斤拷
    PWM_Init(PWM3, &PWM_InitStructure);
    PWM_OutputMode(PWM3, PWM_CHANNEL_A, SH_HIGH);
		//PWM_OutputMode(PWM2, PWM_CHANNEL_B, SH_HIGH);
    GPIO_PinAFConfig(GPIOC, GPIO_InitStructure.GPIO_Pin, PC3_PWM3A);   // 锟斤拷锟斤拷为PWMx
    PWM_SetDuty(PWM3,  0);///duty
    PWM_OnOff(PWM3, SH_ON);                             // 锟斤拷锟斤拷PWMx

//		PWM_SetCycle(PWM2, 2400);///period
//		
}

/**
 * @brief
 *
 */
void    bsp_pwm_init(void)
{
    bsp_pwm3_init();
}

/*----------------------------------------- E.N.D -----------------------------------------*/
