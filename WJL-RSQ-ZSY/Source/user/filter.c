#include    "all_head.h"
/***************************************************************  
 name: Filter;
 Description: 滤波器
 Input Variables: input:滤波的输入信号 deep:滤波深度参考值3或4
 Output: 返回信号滤波后的结果
****************************************************************/
// int Filter(int input, char deep)
// {
// 	static int output;
// 	static int OutputSum = 0;
// 	OutputSum += input;   
// 	output = OutputSum>>deep;
// 	OutputSum -= output;
// 	return output;
// }

uint16_t FilterDeep(uint16_t input, uint32_t* OutputSum ,char deep)
{
	uint16_t output;
    *OutputSum += input;
    output = (uint16_t)( *OutputSum >> deep);
    *OutputSum -= output;
	// *OutputSum += input;   
	// output = *OutputSum>>deep;
	// *OutputSum -= output;
	return output;
}

// /***************************************************************  
//  name: Filter_LowPassInit;
//  Description: 一阶滤波器初始化
//  Input Variables: Period:滤波函数执行间隔 Band:滤波带宽
//  Output: N/A
// ****************************************************************/
// int LPF_COFE;
// void Filter_LowPassInit(float Period, int Band)
// {
// 	LPF_COFE = 256 - (1/(1+Band*6.28*Period))*256;  //反正切（输出频率/带宽）
// }
// /***************************************************************  
//  name: Filter_LowPass;
//  Description: 一阶低通滤波器
//  Input Variables: input:滤波的输入信号
//  Output: 返回信号滤波后的结果
// ****************************************************************/
// int Filter_LowPass(int input)
// {
// 	static int output = 0;
// 	output = (output*(256-LPF_COFE) + LPF_COFE*input)>>8;
// 	return output;
// }

