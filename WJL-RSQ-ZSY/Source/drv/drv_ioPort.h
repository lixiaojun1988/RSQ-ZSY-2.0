
#ifndef __DRV_IOPORT_H__
#define __DRV_IOPORT_H__

#define PORT_LED_1 P00
#define PORT_LED_2 P04
#define PORT_LED_3 P05
#define PORT_LED_4 P14
#define PORT_LED_5 P26

#define singleCommSendPort_L (P13 = 0) //单线通讯发送引脚低电平
#define singleCommSendPort_H (P13 = 1) //单线通讯发送引脚高电平
#define singleCommRevPort  P12       //单线通讯接收引脚
#endif