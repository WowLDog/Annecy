#include "main.h"
#include "stm32f1xx_hal.h"
/*****************************************
*https://kbiva.wordpress.com/2013/03/25/microsecond-delay-function-for-stm32/
******************************************/
extern uint32_t SystemCoreClock;

void DWT_Init(void)
{
	if(!(CoreDebug->DEMCR & CoreDebug_DEMCR_TRCENA_Msk))
	{
		CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;
		DWT->CYCCNT = 0;
		DWT->CTRL |= DWT_CTRL_CYCCNTENA_Msk;
	}
}

uint32_t DWT_Get(void)
{
	return DWT->CYCCNT;
}

uint8_t DWT_Compare(int32_t tp)
{
	return (((int32_t)DWT_Get() - tp) < 0 );
}


void Delay_us(uint32_t micros)
{
	int32_t tp = DWT_Get() + micros * (SystemCoreClock/1000000);
	
	while(DWT_Compare(tp));
}
