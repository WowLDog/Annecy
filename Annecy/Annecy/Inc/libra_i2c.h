#ifndef _LIBRA_I2C_H_
#define _LIBRA_I2C_H_
#include "stm32f1xx_hal.h"
#include "stdint.h"

void i2cSetSDAState(uint8_t state);
void i2cStart(void);
void i2cStop(void);
void i2cSendAck(void);
void i2cSendNAck(void);
void i2cWriteOneByte(uint8_t data);

void hdcWriting(uint8_t address, uint8_t pointer ,
								uint8_t MSB    , uint8_t LSB      );
uint16_t hdcReading(uint8_t address, uint8_t pointer);
uint8_t i2cWaitAck(void);
uint8_t i2cReadOneByte(void);
#endif
