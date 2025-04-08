#ifndef __COMPILE_H__
#define __COMPILE_H__
#define	DIS_TMP_ERR		0//忽略探头故障
#define	DIS_VALVE_ERR	0//忽略阀体故障
#define	DIS_IGNITION	0//不进行点火
#define	FAKE_WATER		0//开机状态后给模拟水流量
#define	FAKE_FLAME		0//模拟火焰
#define	FAKE_FSPD		0//模拟风机正常工作
#define	ADD_DBG_INFO	1//开启调试模式（会增加额外强制代码）

//模式温度
#define Env_InTemp		1//环境温度判定使用进水温度

//恒温性能
#define CtrlTemp_Deep    0

//自修复功能
#define Adpt_DH		1
#define Repair_E1		1
#define Repair_E8		1

//特殊处理
#define Adpt_LessGas_MaxSeg		1//延迟加气，是否打开分段阀

#define Qicket_SegMin		1//降气的时候以最小负荷工作一段时间

#define FanDutySlow			0//风机占空比速度调整

#define CommReSetBathLite	1//当浴缸模式完成后，接收到浴缸通信指令会重置浴缸模式升数

#define SpecMessage			1//特殊信息下发（第8针数据）
#define SpecMessageTime		4//下发频率
#endif
