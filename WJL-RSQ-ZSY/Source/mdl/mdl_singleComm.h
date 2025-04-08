
#ifndef	_MDL_SINGLECOMM_H_
#define	_MDL_SINGLECOMM_H_
extern void singleCommHandle(void);
extern void singleCommInit(void);
typedef struct
{
    unsigned char allowSend : 1;        //允许发送
    unsigned char validRev : 1;         //接收有效
    unsigned char sendIsBusy : 1;       //正在发送
    unsigned char sendBuff[4];          //发送数据BUFF
    unsigned char revBuff[4];           //接收数据BUFF
    unsigned char revLen;               //接收数据长度
    unsigned char SendLen;              //发送数据长度
} sinleCommData;//单线通讯结构体

/*
typedef struct
{
	  unsigned char disConnectFlag:1; //连接失败
	  unsigned char ConnectSucess :1; //连接成功
    unsigned long int cnt;          //失连计数
} singleCommError;        //通讯故障
*/
extern sinleCommData *sinleCommDataGet(void);
//extern singleCommError *sinleCommErrorGet(void);
#endif