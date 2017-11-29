#ifndef _DELAY_H_
#define _DELAY_H_
#include "main.h"
#include "stm32f1xx_hal.h"

void DWT_Init(void);
uint32_t DWT_Get(void);
uint8_t DWT_Compare(int32_t tp);
void Delay_us(uint32_t micros);

#endif
