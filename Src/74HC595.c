/*
 * 74HC595.c
 *
 *  Created on: Mar 23, 2023
 *      Author: hoanganh
 */

#include "74HC595.h"
#ifdef CONFIG_USE_74HC595
HAL_StatusTypeDef HC595_ConfigOnePin(HC595* dev,GPIO_TypeDef *port,uint16_t pin, pinName pinName)
{
	if(!dev) return HAL_ERROR;
	switch(pinName){
	case HC595_CLK:
		dev->clk.port = port;
		dev->clk.pin = pin;
		break;
	case HC595_DS:
		dev->ds.port = port;
		dev->ds.pin = pin;
		break;
	case HC595_LATCH:
		dev->latch.port = port;
		dev->latch.pin = pin;
		break;
	}
	return HAL_OK;
}
HAL_StatusTypeDef HC595_TestPin(HC595* dev,pinName pinName)
{
	if(!dev) return HAL_ERROR;
	HC595_WRITE(pinName,1);
	HAL_Delay(1000);
	HC595_WRITE(pinName,0);
	HAL_Delay(1000);
	return HAL_OK;
}

HAL_StatusTypeDef HC595_Send_Data(HC595* dev,uint16_t dt)
{
	if(!dev) return HAL_ERROR;
	for(int i = 0;i<8;i++)
	{
		if(dt & 0x80)
			HC595_WRITE(HC595_DS,1);
		else
			HC595_WRITE(HC595_DS,0);

		HC595_WRITE(HC595_CLK,0);
		HC595_WRITE(HC595_CLK,1);
		dt <<= 1;
	}
	HC595_WRITE(HC595_LATCH,0);
	HC595_WRITE(HC595_LATCH,1);
	return HAL_OK;
}
#endif
