#ifndef _SOFTWARE_I2C_H_
#define _SOFTWARE_I2C_H_

extern void SoftI2C_start(void);
extern void SoftI2C_stop(void);

extern void SoftI2C_write_byte(uint8_t value);
extern uint8_t SoftI2C_read_byte(uint8_t ack);


#endif
