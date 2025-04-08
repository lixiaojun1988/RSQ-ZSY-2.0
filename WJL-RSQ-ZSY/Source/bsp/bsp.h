/**
 * *****************************************************************************
 * @file     bsp.h
 * @brief
 * @author   sinowealth
 * @version  1.0
 * @date     2024-08-07
 *
 * *****************************************************************************
 */
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __BSP_H__
#define __BSP_H__

#define EA_Disable()			__disable_irq()	//锟斤拷锟斤拷锟叫讹拷
#define	EA_Enable()				__enable_irq()	//锟斤拷锟斤拷锟叫讹拷
#define CLR_WDT()    HWDT->CLR = 0xAAAA
#ifdef __cplusplus
 extern "C" {
#endif

void    bsp_adc_init(void);
uint16_t    bps_adc_get_value(uint8_t chn);
void    bsp_exit_init(void);
void    bsp_gpio_init(void);
void    bsp_iwdt_init(void);
void    bsp_lcd_init(void);
void    bsp_led_init(void);
void    bsp_lvr_init(void);
void    bsp_nvic_init(void);
void    bsp_pca_init(void);
void    bsp_pca_handler(void);
void    bsp_pwm_init(void);
void    bsp_spi_inquire_master_init(void);
void    bsp_systick_init(void);
void    bsp_tim_init(void);
void    bsp_uart0_init(void);
void    bsp_uart1_init(void);
void    bsp_uart2_init(void);
void    bsp_uart3_init(void);
void    bsp_uart5_init(void);



void UART0_ISR(void);
void UART0_Test(void);

void UART1_ISR(void);
void UART1_Test(void);

void UART2_ISR(void);
void UART2_Test(void);

void UART3_ISR(void);
void UART3_Test(void);

void E2_test(void);
#ifdef      __RAM_DEFINE__
    #define     EXT
#else
    #define     EXT     extern
#endif  /* __RAM_DEFINE__ */

#ifdef __cplusplus
}
#endif

#endif /* __BSP_H__ */
