#ifndef	DRV_DCFAN_H
#define	DRV_DCFAN_H

#define	SET_DCFAN_OFF	0
#define	FANU_MAX	(0x31f)
#define	FANU_MIN	(0)

extern	void	Drv_SetDCFanPWM(uint16_t	_u16SetI);
extern	uint16_t	GetDCFanSet(void);
#endif
