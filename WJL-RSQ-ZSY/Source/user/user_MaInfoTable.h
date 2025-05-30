#ifndef	USER_MAINFOTABLE_H
#define	USER_MAINFOTABLE_H
#include	"user_MachineInfo.h"


static	const	ST_FA_INFO_T	astFaInfo_UpFan[FA_MAX_UPPERFAN] =
{
	{100,EN_GAS_12T,EN_CODN_DIS	,EN_STR_NOR},//0
	{100,EN_GAS_12T,EN_CODN_EN	,EN_STR_NOR},//1
	{100,EN_GAS_20Y,EN_CODN_DIS	,EN_STR_NOR},//2
	{100,EN_GAS_20Y,EN_CODN_EN	,EN_STR_NOR},//3
	{130,EN_GAS_12T,EN_CODN_DIS	,EN_STR_NOR},//4
	{130,EN_GAS_12T,EN_CODN_EN	,EN_STR_NOR},//5
	{130,EN_GAS_20Y,EN_CODN_DIS	,EN_STR_NOR},//6
	{130,EN_GAS_20Y,EN_CODN_EN	,EN_STR_NOR},//7
	{120,EN_GAS_12T,EN_CODN_DIS	,EN_STR_NOR},//8
	{120,EN_GAS_12T,EN_CODN_EN	,EN_STR_NOR},//9
	{120,EN_GAS_20Y,EN_CODN_DIS	,EN_STR_NOR},//10
	{120,EN_GAS_20Y,EN_CODN_EN	,EN_STR_NOR},//11
	{160,EN_GAS_12T,EN_CODN_DIS	,EN_STR_NOR},//12
	{160,EN_GAS_12T,EN_CODN_EN	,EN_STR_NOR},//13
	{160,EN_GAS_20Y,EN_CODN_DIS	,EN_STR_NOR},//14
	{160,EN_GAS_20Y,EN_CODN_EN	,EN_STR_NOR},//15
	{ 80,EN_GAS_12T,EN_CODN_DIS	,EN_STR_NOR},//16
	{ 80,EN_GAS_20Y,EN_CODN_DIS	,EN_STR_NOR},//17
	{140,EN_GAS_12T,EN_CODN_EN	,EN_STR_NOR},//18
	{140,EN_GAS_12T,EN_CODN_DIS	,EN_STR_NOR},//19
	{140,EN_GAS_20Y,EN_CODN_DIS	,EN_STR_NOR},//20
	{180,EN_GAS_12T,EN_CODN_DIS	,EN_STR_NOR},//21
	{180,EN_GAS_20Y,EN_CODN_DIS	,EN_STR_NOR},//22
	{200,EN_GAS_12T,EN_CODN_DIS	,EN_STR_NOR},//23
	{200,EN_GAS_20Y,EN_CODN_DIS	,EN_STR_NOR},//24
};
static	const	ST_FA_INFO_T	astFaInfo_LowFan[FA_MAX_LOWERFAN] =
{
	{110,EN_GAS_12T,EN_CODN_DIS	,EN_STR_NOR},//0
	{110,EN_GAS_20Y,EN_CODN_DIS	,EN_STR_NOR},//1
	{130,EN_GAS_12T,EN_CODN_DIS	,EN_STR_NOR},//2
	{160,EN_GAS_12T,EN_CODN_DIS	,EN_STR_NOR},//3
	{240,EN_GAS_12T,EN_CODN_DIS	,EN_STR_NOR},//4
	{130,EN_GAS_20Y,EN_CODN_DIS	,EN_STR_NOR},//5
	{160,EN_GAS_20Y,EN_CODN_DIS	,EN_STR_NOR},//6
	{240,EN_GAS_20Y,EN_CODN_DIS	,EN_STR_NOR},//7
	{160,EN_GAS_12T,EN_CODN_EN	,EN_STR_NOR},//8
	{130,EN_GAS_12T,EN_CODN_EN	,EN_STR_NOR},//9
	{240,EN_GAS_12T,EN_CODN_EN	,EN_STR_NOR},//10
	{130,EN_GAS_12T,EN_CODN_DIS	,EN_STR_STL},//11
	{160,EN_GAS_20Y,EN_CODN_DIS	,EN_STR_STL},//12
	{180,EN_GAS_12T,EN_CODN_DIS	,EN_STR_NOR},//13
	{180,EN_GAS_20Y,EN_CODN_DIS	,EN_STR_NOR},//14
	{140,EN_GAS_12T,EN_CODN_DIS	,EN_STR_NOR},//15
	{140,EN_GAS_20Y,EN_CODN_DIS	,EN_STR_NOR},//16
	{200,EN_GAS_12T,EN_CODN_DIS	,EN_STR_NOR},//17
	{200,EN_GAS_20Y,EN_CODN_DIS	,EN_STR_NOR},//18
	{260,EN_GAS_12T,EN_CODN_DIS	,EN_STR_NOR},//19
	{260,EN_GAS_20Y,EN_CODN_DIS	,EN_STR_NOR},//20
	{300,EN_GAS_12T,EN_CODN_DIS	,EN_STR_NOR},//21
	{300,EN_GAS_20Y,EN_CODN_DIS	,EN_STR_NOR},//22
	{120,EN_GAS_12T,EN_CODN_DIS	,EN_STR_NOR},//23
	{120,EN_GAS_20Y,EN_CODN_DIS	,EN_STR_NOR},//24
	{200,EN_GAS_12T,EN_CODN_DIS	,EN_STR_STL},//25
	{200,EN_GAS_20Y,EN_CODN_DIS	,EN_STR_STL},//26
	{280,EN_GAS_12T,EN_CODN_DIS	,EN_STR_NOR},//27
	{280,EN_GAS_20Y,EN_CODN_DIS	,EN_STR_NOR},//28
	{160,EN_GAS_20Y,EN_CODN_DIS	,EN_STR_STL},//29
};
static	const	ST_FA_INFO_T*	const au8FaInfoList[2] =
{
	astFaInfo_UpFan,
	astFaInfo_LowFan
};
static	const	ST_MA_INFO_T	astMaInfo_Z8L[] =
{
	{&(astFaInfo_UpFan[4])	,EN_FAN_YUXIANG		,EN_MODEL_Z8L},	//13L
	{&(astFaInfo_UpFan[4])	,EN_FAN_XIECHUANG	,EN_MODEL_Z8L},	//13L
	{&(astFaInfo_UpFan[12])	,EN_FAN_YUXIANG		,EN_MODEL_Z8L},	//16L
	{&(astFaInfo_UpFan[12])	,EN_FAN_XIECHUANG	,EN_MODEL_Z8L},	//16L
	{&(astFaInfo_UpFan[21])	,EN_FAN_YUXIANG		,EN_MODEL_Z8L},	//18L
	{&(astFaInfo_UpFan[21])	,EN_FAN_XIECHUANG	,EN_MODEL_Z8L},	//18L
};


#endif
