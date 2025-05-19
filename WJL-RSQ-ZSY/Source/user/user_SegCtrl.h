#ifndef	USER_SEGCTRL_H
#define	USER_SEGCTRL_H

#define SLOW_SEG_EN 0

#define SLOW_SEG_TMP    5//换挡时的温差（0.1℃）
#define SLOW_SEG_CNT    50//换挡是超过温差的持续时间（0.1s）

#define SEG_SWITCH_BLF_MIN  30//换挡时，比例阀最小高度

typedef enum
{
	EN_CALSW_MANUAL,
	EN_CALSW_AUTO,
	EN_CALSW_MAX
}EN_CALSW_T;
typedef struct
{
	ST_SEG_LOAD_T	astSegLoad[SUBSEG_MAX];
	uint16_t	u16SegSwitchBlfI;
#if (SLOW_SEG_EN)
    uint16_t u16SlowSegCnt;//锟斤拷锟斤拷锟斤拷锟叫讹拷锟斤拷锟斤拷
#endif
	uint8_t	u8Set;
	uint8_t	u8Cur;
	uint8_t	u8ChgStep;
	uint8_t	u8ValveStay_100ms;
	uint8_t	u8ChgCnt;
	uint8_t	u8Switching_100ms;
	uint8_t BrustStableUpCnt;
	uint8_t BrustStableDownCnt;
}ST_SEGCTRL_T;
extern	void	SetManualSeg(uint8_t	_u8set);
extern	void	ClrSubChgCnt(void);
extern	void	SegCtrlInit(void);
extern	void	SegCtrlProcess(uint8_t _Auto);
extern	void	SwitchSeg(uint8_t _Auto);
extern	const	ST_SEGCTRL_T* GetSegCtrl(void);
extern uint16_t	GetBlfIFromSeg(uint8_t	_u8seg);
extern	void SegCtrl_Timer(void);
extern  void ClrSubChgCnt(void);
#endif
