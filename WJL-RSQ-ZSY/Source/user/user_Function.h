#ifndef	USER_FUNCTION_H
#define	USER_FUNCTION_H
typedef enum
{
	EN_CO_SAFE,//��ȫ
	EN_CO_EARLY_WARM,//Ԥ��
	EN_CO_WARM,//����
	EN_CO_ERR,//�������
	EN_CO_UNCOMM = 7,//ʧ��
	EN_CO_MAX
}EN_CO_T;
typedef enum
{
	EN_DBG_CO_LV1,
	EN_DBG_CO_LV2,
	EN_DBG_CO_NO,
	EN_DBG_CO_MAX
}EN_DBG_CO_T;

#define EN_COCH4_ERR	1//˫���������쳣
#define EN_COCH4_NOMAL	2//˫������������

typedef struct
{
	uint8_t	u8CH4Warm;
	uint8_t	u8CH4Safe;
	uint8_t	u8CoWarm;
	uint8_t	u8CoSafe;
}ST_GAS_PRO_T;//˫����������ֵ

typedef struct
{
	uint8_t	bCH4Warm	:1;
	uint8_t	bCoWarm		:1;
	uint8_t	bProAir     :1;
    uint8_t  bProWindPress :1;
	uint8_t	Res			:4;
}ST_FUNCTION_SIGN_T;

typedef struct 
{
    //�ۼ�
    uint32_t u32GasUseSum;
    uint32_t u32WaterUseSum;
	
	//ˮ�����ۼ�
	uint32_t u32WaterExpend;
    //�����ۼ�
    uint16_t u16GasUsed;
    uint16_t u16WaterUsed;

    //˫������CO״̬
    uint8_t	u8CoSat;
	ST_FUNCTION_SIGN_T stSign;
}ST_FUNCTION_OUTPUTS_T;

//��������
typedef enum
{
	EN_REC_GAS,
	EN_REC_WATER,
	EN_REC_MAX
}EN_REC_T;

extern void WaterGasInit(void);
extern uint8_t RecordLiterFunction(uint32_t* _LaterRec,EN_REC_T _enRecordType);
extern const ST_FUNCTION_OUTPUTS_T* GetFunOutputs(void);
extern void FunFunction(void);
extern void FunctionInit(void);
#endif

