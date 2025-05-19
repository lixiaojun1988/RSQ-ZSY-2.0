/*************************************************************************
 * Copyright:    wjl Kitchenware.
 * File:         万家乐燃气热水器主控自适应方案
 * Layer:
 * Author:       xjli
 * Version:      V2
 * Brief:		     SH32F9010P  44PIN
 * date：       2025-4-30
 *************************************************************************/

#define __RAM_DEFINE__

#include "all_head.h"

int main(void)
{
	EA_Disable();
	bsp_iwdt_init(); //
	bsp_lvr_init();	 // LVR
	bsp_gpio_init();
	bsp_adc_init();
	bsp_exit_init();

	//    bsp_nvic_init();
	bsp_pca_init(); // Pb1 PC3  4k
	bsp_pwm_init(); // PC4_AF10 4k
	bsp_systick_init();
	//	bsp_uart1_init();
	bsp_tim_init();	  // 1ms
	bsp_uart2_init(); // 4800

	//	bsp_uart3_init();
	EA_Enable(); //
	softInit();
	while (1)
	{
		CLR_WDT();
		task();
	}
}

#ifdef DEFAULT_ASSERT_ENABLE
/**
 * @brief  Reports the name of the source file and the source line number
 *   where the assert_param error has occurred.
 * @param  file: pointer to the source file name
 * @param  line: assert_param error line source number
 * @retval None
 */
void assert_failed(uint8_t *file, uint32_t line)
{
	/* User can add his own implementation to report the file name and line number,
	   ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

	/* Infinite loop */
	while (1)
	{
	}
}
#endif

/*----------------------------------------- E.N.D -----------------------------------------*/
