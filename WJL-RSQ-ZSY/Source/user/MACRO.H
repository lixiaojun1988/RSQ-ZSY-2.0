
#ifndef _MACRO_H_
#define _MACRO_H_
//#include <intrins.h>
#define LEFT 0
#define RIGHT 1
#define True 1
#define False 0
#define Enable 1
#define Disable 0
#define AN0 0
#define AN1 1
#define AN2 2
#define AN3 3
#define AN4 4
#define AN5 5
#define AN6 6
#define AN7 7
#define AN8 8


#define bool uint8_t
#define HIBYTE(v1) ((Byte)((v1) >> 8))
#define LOBYTE(v1) ((Byte)((v1)&0xff))
typedef void (*FUCTION)(void);
#define NOP _nop_()
// #define uint16  unsigned int
// #define ulong   unsigned long int
// #define uchar   unsigned char
// #define uint    unsigned char

typedef unsigned char BYTE1;
typedef unsigned short WORD;

#define GETMIN(a,b) 			((a)<(b)?(a):(b))
#define GETMAX(a,b) 			((a)>(b)?(a):(b))
#define RANGLMT(x, min, max) 	(GETMIN(GETMAX(x, min), max))
#define B0 0x01
#define B1 0x02
#define B2 0x04
#define B3 0x08
#define B4 0x10
#define B5 0x20
#define B6 0x40
#define B7 0x80

#define AB0 0xFE
#define AB1 0xFD
#define AB2 0xFB
#define AB3 0xF7
#define AB4 0xEF
#define AB5 0xDF
#define AB6 0xBF
#define AB7 0x7F

#define BIT0 0x01
#define BIT1 0x02
#define BIT2 0x04
#define BIT3 0x08
#define BIT4 0x10
#define BIT5 0x20
#define BIT6 0x40
#define BIT7 0x80
#define BIT8 0x0100
#define BIT9 0x0200
#define BIT10 0x0400
#define BIT11 0x0800
#define BIT12 0x1000
#define BIT13 0x2000
#define BIT14 0x4000
#define BIT15 0x8000
#define BIT16 0x010000
#define BIT17 0x020000
#define BIT18 0x040000
#define BIT19 0x080000
#define BIT20 0x100000
#define BIT21 0x200000
#define BIT22 0x400000
#define BIT23 0x800000
#define BIT24 0x01000000
#define BIT25 0x02000000
#define BIT26 0x04000000
#define BIT27 0x08000000
#define BIT28 0x10000000
#define BIT29 0x20000000
#define BIT30 0x40000000
#define BIT31 0x80000000

#define NOP10() _nop_(), _nop_(), _nop_(), _nop_(), _nop_(), _nop_(), _nop_(), _nop_(), _nop_(), _nop_(), _nop_(), _nop_(), _nop_(), _nop_(), _nop_(), _nop_()
#define NOP4() NOP10(), NOP10(), NOP10(), NOP10()
#define NOP2() NOP10(), NOP10()

#define LongToBin(n)         \
	(                        \
		((n >> 21) & 0x80) | \
		((n >> 18) & 0x40) | \
		((n >> 15) & 0x20) | \
		((n >> 12) & 0x10) | \
		((n >> 9) & 0x08) |  \
		((n >> 6) & 0x04) |  \
		((n >> 3) & 0x02) |  \
		((n)&0x01))
#define Bin(n) LongToBin(0x##n##l) // write binary charactor set,exsample : Bin(11111111) = 0xff
#define BIN(b7, b6, b5, b4, b3, b2, b1, b0) (b0 + b1 * 2 + b2 * 4 + b3 * 8 + b4 * 16 + b5 * 32 + b6 * 64 + b7 * 128)
#define	INC_B(a)	((a) < (0xff)?(a++):a)
#define	INC_W(a)	((a) < (0xffff)?(a++):a)
#define	INC_L(a)	((a) < (0xffffffff)?(a++):a)
#define	DEC(a)	((a) > (0)?(a--):a)
#define	ARR_NUM(a)						(sizeof(a)/sizeof(a[0]))
#endif
