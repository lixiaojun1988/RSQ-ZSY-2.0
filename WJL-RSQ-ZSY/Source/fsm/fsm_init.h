
#ifndef _FSM_INIT_H_
#define _FSM_INIT_H_

typedef enum
{
	EN_ERR_E0 = 0x00,
	EN_ERR_E1,
	EN_ERR_E2,
	EN_ERR_E3,
	EN_ERR_E4,
	EN_ERR_E5,
	EN_ERR_E6,
	EN_ERR_E7,
	EN_ERR_E8,
	EN_ERR_E9,
	EN_ERR_En = 0x10,
	EN_ERR_F3,
	EN_ERR_F4,
	EN_ERR_F5,
	EN_ERR_F6,
	EN_ERR_F7,
	EN_ERR_F8,
	EN_ERR_F9,
	EN_ERR_EP,
	EN_ERR_EL,
	EN_ERR_SP = 0x20,
	EN_ERR_CLR_N = 0xA1,//锟斤拷锟斤拷没锟斤拷锟斤拷锟�
	EN_ERR_CLR_P = 0xA2,//锟斤拷锟剿拷霉锟斤拷锟�
	EN_ERR_NO = 0xff
}EN_ERRORCODE_T;
extern void FsmInitEnterHandler(void);
extern void FsmInitDoingHandler(void);
#endif 







