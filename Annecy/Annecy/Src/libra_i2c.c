#include "main.h"
#include "stm32f1xx_hal.h"
#include "stdint.h"

#define i2cSetSDA(state)  HAL_GPIO_WritePin(H_SDA_GPIO_Port, H_SDA_Pin, state)
#define i2cSetSCL(state)	HAL_GPIO_WritePin(H_SCL_GPIO_Port, H_SCL_Pin, state)
#define i2cReadSDA()      HAL_GPIO_ReadPin(H_SDA_GPIO_Port,H_SDA_Pin)
/**
  * @brief  This function change i2c_SDA state
  * @param  1 - in
						0 -out
  * @retval None
  */
void i2cSetSDAState(uint8_t state)
{
	H_SDA_GPIO_Port->CRL &= 0xF0FFFFFF;
	if(state) 
		H_SDA_GPIO_Port->CRL |= 0x08000000;
	else
		H_SDA_GPIO_Port->CRL |= 0x03000000;
}

void i2cStart()
{
	i2cSetSDAState(0);
	i2cSetSDA(GPIO_PIN_SET);
	i2cSetSCL(GPIO_PIN_SET);
	i2cSetSDA(GPIO_PIN_RESET);
	Delay_us(5);
	i2cSetSCL(GPIO_PIN_RESET);
}

void i2cStop()
{
	i2cSetSDAState(0);
	i2cSetSDA(GPIO_PIN_RESET);
	i2cSetSCL(GPIO_PIN_RESET);
	Delay_us(5);
	i2cSetSDA(GPIO_PIN_SET);
	i2cSetSCL(GPIO_PIN_SET);
	__nop();	
}

uint8_t i2cWaitAck(void)
{
	uint8_t tmp = 0;
	i2cSetSDAState(1);
	i2cSetSDA(GPIO_PIN_SET);
	i2cSetSCL(GPIO_PIN_SET);
	while(i2cReadSDA())
	{
		tmp++;
		if(tmp > 250)
		{
			i2cStop();
			return 1;
		}
	}
	i2cSetSCL(GPIO_PIN_RESET);
	return 0;
}

void i2cSendAck(void)
{
	i2cSetSCL(GPIO_PIN_RESET);
	i2cSetSDAState(0);
	__nop();
	i2cSetSCL(GPIO_PIN_SET);
	Delay_us(5);
	i2cSetSCL(GPIO_PIN_RESET);
}

void i2cSendNAck(void)
{
	i2cSetSCL(GPIO_PIN_RESET);
	i2cSetSDAState(0);
	i2cSetSDA(GPIO_PIN_SET);
	__nop();
	i2cSetSCL(GPIO_PIN_SET);
	Delay_us(5);
	i2cSetSCL(GPIO_PIN_RESET);
}

void i2cWriteOneByte(uint8_t data)
{
	uint8_t i;
	i2cSetSDAState(0);
	i2cSetSCL(GPIO_PIN_RESET);
	for(i=0 ;i<8 ;i++ )
	{
		i2cSetSDA((data&0x80)>>7);
		data = data << 1;
		__nop();
		i2cSetSCL(GPIO_PIN_SET);

		Delay_us(5);
		i2cSetSCL(GPIO_PIN_RESET);
		Delay_us(2);
	}
//	if(i2cWaitAck()) 
//		i2cSendAck();
//	else 
//		i2cSendNAck();
}
	
uint8_t i2cReadOneByte()
{
	uint8_t i = 0 ;
	uint8_t tmp = 0 ;
	i2cSetSDAState(1);
	for(i=0 ;i<8 ;i++ )
	{
		i2cSetSCL(GPIO_PIN_RESET);
		Delay_us(5);
		i2cSetSCL(GPIO_PIN_SET);

		tmp <<= 1;
		if(i2cReadSDA()) tmp ++;
		Delay_us(5);		
	}
	if(i2cWaitAck()) 
		i2cSendAck();
	else 
		i2cSendNAck();
	return tmp;
}

void hdcWriting(uint8_t address, uint8_t pointer ,
								uint8_t MSB    , uint8_t LSB      )
{
	i2cStart();
	i2cWriteOneByte(address);
	i2cWriteOneByte(pointer); 
	i2cWriteOneByte(MSB);
	i2cWriteOneByte(LSB);
	i2cStop();	
}

uint16_t hdcReading(uint8_t address, uint8_t pointer)
{
	uint8_t MSB = 0;
	uint8_t LSB = 0;
	uint16_t retval = 0;
	address = address & 0xFE;
	i2cStart();
	i2cWriteOneByte(address);
	i2cWriteOneByte(pointer);
	address = address | 0x01;
	i2cStart();
	i2cWriteOneByte(address);
	MSB = i2cReadOneByte();
	LSB = i2cReadOneByte();
	i2cStop();
	retval = (uint16_t)MSB<<8 || (uint16_t)LSB;
	return retval;
}
