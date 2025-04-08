#ifndef	MDL_BLF_H
#define	MDL_BLF_H

#define BLF_MIN 0
#define BLF_MAX 6000
#define	SET_BLF_OFF	0
extern	void	Blf_Iteration_10ms(void);
extern	uint16_t	GetBLFRealOut(void);
extern	void	MDL_SetBlfOut(uint16_t _u16SetBlfI);
extern  uint16_t    GetBlfCurOut(void);//输出实时占空比
#endif

