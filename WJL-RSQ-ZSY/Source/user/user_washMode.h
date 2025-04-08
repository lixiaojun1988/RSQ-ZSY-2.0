#ifndef __USER_WASHMODE_H__
#define __USER_WASHMODE_H__

#define	BATH_WATER_DEF			10//婵炴潙顕崙鐘参熼垾宕囩濮掓稒枪椤撹顫濋幘顔兼闁挎稑鐗嗗畷鐔告媴瀹ュ繒绐�10L闁挎冻鎷�
#define	COMFORT_TEMP_DEF		41//闁煎憡甯￠埀顒€鍊归懡鈩冾渶濡鍚囨繛鎾櫅鐎规娊鏁嶉崼婵嗙濞达絽绋勭槐锟�1闁斥晛鍠涚槐锟�
#define	WASH_SPE_TMP_LIMIT		48//闁绘顫夐悾鈺佄熼垾宕囩濞戞挸顑囧▓鎴濄€掗埡浣割唺闂傚嫭鍔曢崺锟�
#define	WASH_NOMAL_TMP_LIMIT	60//闁哄拋鍣ｉ埀顒佺娣囶垱鎯旈敃渚囧晭缂傚喚鍠曠粭鍌炴⒔閿燂拷


typedef enum
{
	EN_ENV_TMP_0,
	EN_ENV_TMP_1,
	EN_ENV_TMP_2,
	EN_ENV_TMP_3,
	EN_ENV_TMP_MAX,
}EN_ENV_TMP_T;
typedef enum
{
    EN_WASH_COMFORT1, // 闁跨喐鏋婚幏鐑芥晸閺傘倖瀚�
    EN_WASH_ECO1,     // 闁跨喐鏋婚幏鐑芥晸閺傘倖瀚�
    EN_WASH_BATH,     // 濞存挳鏁撻弬銈嗗
    EN_WASH_VEG,      // 闁跨喐鏋婚幏鐑芥晸閺傘倖瀚�
    EN_WASH_COMFORT,  // 闁跨喐鏋婚幏鐑芥晸閺傘倖瀚瑰ù锟�
    EN_WASH_ENV,      // 闁跨喐鏋婚幏鐑芥晸闁句即娼婚幏锟�
    EN_WASH_CHILD,    // 闁跨喐鏋婚幏椋庮仮濞达拷
    EN_WASH_OLD,      // 闁跨喐鏋婚幏鐑芥晸閺傘倖瀚瑰ù锟�
    EN_WASH_COOK,     // 鐏忓繘鏁撻弬銈嗗闁跨喐鏋婚幏锟�
    EN_WASH_CLOTHES,  // 濞叉鏁撻弬銈嗗濡€崇础
    EN_WASH_ECO,      // ECO
    EN_WASH_SUR,      // SUR
    EN_WASH_TOP,      // 闁跨喐鏋婚幏鐑芥晸閺傘倖瀚归柨鐔告灮閹烽攱鑽�
    EN_WASH_HAND,     // 闁跨喕顢滅粵瑙勫闁跨喐鏋婚幏閿嬭崱
    EN_WASH_KITCHEN,  // 闁跨喐鏋婚幏鐑芥晸閺傘倖瀚瑰Ο鈥崇础
    EN_WASH_BOWL,     // 闁跨喐鏋婚幏鐑芥晸閿燂拷
    EN_WASH_HIGH,     // 闁跨喐鏋婚幏鐑芥晸閺傘倖瀚瑰锟�
    EN_WASH_PET,      // 闁跨喖銈虹粵瑙勫濞诧拷
    EN_WASH_MAX,
} EN_WASHMODE_T;

typedef struct
{
    uint8_t bBathFin : 1;
    uint8_t bEnvErr : 1;
    uint8_t Res : 6;
} ST_WASHMODE_SIGN_T;

typedef struct
{
    uint16_t u16BathRemainLite;
    uint16_t u16BathLite;  // 婵炴潙顕崙鐘参熼垾宕囩鐎规瓕灏欓弫銈咁潩閹绢喖娅�
    uint16_t u16BathTick;
    uint8_t u8WaterSet;
    uint8_t u8EnvSta; // 闁绘粠鍨伴。銊ャ€掗埡浣割唺闁告帇鍊栭弻锟�
    ST_WASHMODE_SIGN_T stWashSign;
} ST_WASHMODE_OUTPUTS_T;

extern ST_WASHMODE_OUTPUTS_T *GetWashModeData(void);
extern void WashModeInit(void);
extern void WashModeFunction(void); // 10MS
#if (CommReSetBathLite)
extern void ReSetBathLite(void);
#endif
#endif
