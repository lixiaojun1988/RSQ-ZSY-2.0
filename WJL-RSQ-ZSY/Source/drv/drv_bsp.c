
#include "all_head.h"
unsigned int xdata PWMREG[14] _at_ 0x1034; // PWM占空比调节寄存器
#define SetGlobalInterruptEnableState(x) \
	{                                    \
		EA = x;                          \
	}

void systickInit(void)
{
	TMCON = 0X07; //------111 ;Timer0、Tiemr1和Tiemr2选择时钟Fsys
	// T0设置，现象：P02输出频率为1K的方波(主频：32M)
	TMOD |= 0x01;				// 0000 0001;Timer0设置工作方式1
	TL0 = (65536 - 3200) % 256; // 时钟为Fsys，则溢出时间=16000*（1/Fsys）;
	TH0 = (65536 - 3200) / 256;
	TR0 = 0;
	ET0 = 1;					 // 定时器0允许
	TR0 = 1;					 // 打开定时器0
								 //-----------------------
	TMOD |= 0x10;				 // Timer1设置工作方式1
	TL1 = (65536 - 32000) % 256; // 时钟为Fsys，则溢出时间=16000*（1/Fsys）;
	TH1 = (65536 - 32000) / 256;
	TR1 = 0;
	ET1 = 1; // 定时器1允许
	TR1 = 1; // 打开定时器1
    IP|=0X02;//T0中断优先级高
	WDTCON = (WDTCON & 0XF8) | 0x02; // 125ms看门狗复位
}

void IO_Init(void)
{
	P0 = 0X00;
	P1 = 0X00;
	P2 = 0X00; 

	P0CON = 0Xff; // p04 ir1,P02-P03输入带上拉
	P1CON = 0Xf9;//P11 IR输入 P12RXD
	P2CON = 0Xfe; // P20 IR输入

	P0PH = 0X00;
	P1PH = 0X00;
	P2PH = 0X00;
  led_hard_dispAllClose();
}
void PWM_Init(void)
{

	PWMCON0 = 0x00;	 // 分频系数设置为4分频，独立模式，边沿对齐
	PWMCFG = 0x00;	 // 不反向
	PWMCON1 |= 0x80; // PWM07输出
	PWMPDL = 0x49;	 // 周期配置低8位
	PWMPDH = 0x03;	 // 周期配置高8位
	PWMREG[13] = 0;
	PWMCON0 |= 0x80; // 使能PWM0


  TXINX = 0x04;     //TimerX寄存器组指向TIM4
  THX = 0X00;     //计数器高8位清零
  TLX =  0x00;    //计数器低8位清零
  TXMOD |= 0x20;      //使能PWM41波形输出
 // TXMOD |= 0x80;      //PWM41输出反向
	RCAPXH = 0x03; // 配置周期寄存器高8位
	RCAPXL = 0x49; // 配置周期寄存器低8位
  PWMREG[5] = 0;      //配置PWM41占空比
  TXCON |= 0x04;        //使能PWM4

}
void bspInit(void)
{
	IO_Init();
	systickInit();
	key_Init();
	PWM_Init();
	SetGlobalInterruptEnableState(1);
}

// 100US定时器

void Timer0Int(void) interrupt 1 // 100us
{
	TL0 = (65536 - 3200) % 256; // 手动重装载值低8位
	TH0 = (65536 - 3200) / 256; // 手动重装载值高8位
	
    GestureDrvHander();
    singleCommHandle();

}

void Timer1Int(void) interrupt 3 // 1ms
{
	TL1 = (65536 - 32000) % 256; // 手动重装载值低8位
	TH1 = (65536 - 32000) / 256; // 手动重装载值高8位
	// 
	taskTick();
	LED_ScanDrive();
}