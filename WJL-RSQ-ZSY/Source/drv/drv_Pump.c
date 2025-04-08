#include	"ALL_HEAD.H"
void	Drv_SetPumpPWM(uint8_t	_u8Duty)
{
	uint16_t _u16SetI;
	_u16SetI = (uint16_t)(((uint32_t)_u8Duty) * (PUMP_PWN_CYCLE) / (0xFF));
	//PCA_SetCCRValue(PCA2, PCA_CHANNEL_B, (_u16SetI));	
	PCA_SetCCRValue(PCA1, PCA_CHANNEL_A, _u16SetI);
}
