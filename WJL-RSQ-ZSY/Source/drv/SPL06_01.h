#ifndef _SPL06_01_H_
#define _SPL06_01_H_


#define HW_ADR 0x76
#define CONTINUOUS_PRESSURE     1
#define CONTINUOUS_TEMPERATURE  2
#define CONTINUOUS_P_AND_T      3
#define PRESSURE_SENSOR     0
#define TEMPERATURE_SENSOR  1


struct spl0601_calib_param_t {	
    int16_t c0;
    int16_t c1;
    int32_t c00;
    int32_t c10;
    int16_t c01;
    int16_t c11;
    int16_t c20;
    int16_t c21;
    int16_t c30;       
};

struct spl0601_t {	
    struct spl0601_calib_param_t calib_param;/**<calibration data*/	
    uint8_t chip_id; /**<chip id*/	
    int32_t i32rawPressure;
    int32_t i32rawTemperature;
    int32_t i32kP;    
    int32_t i32kT;
};

extern void spl0601_init(void);
extern void spl0601_rateset(uint8_t iSensor, uint8_t u8OverSmpl, uint8_t u8SmplRate);
extern void spl0601_start_temperature(void);
extern void spl0601_start_pressure(void);
extern void spl0601_start_continuous(uint8_t mode);
extern void spl0601_get_raw_temp(void);
extern void spl0601_get_raw_pressure(void);
extern float spl0601_get_temperature(void);
extern float spl0601_get_pressure(void);

#endif

