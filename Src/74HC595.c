/*
 * 74HC595.c
 *
 *  Created on: Mar 23, 2023
 *      Author: hoanganh
 */

#include "74HC595.h"
#ifdef CONFIG_USE_74HC595
HC595 *devTemp = NULL;
HAL_StatusTypeDef HC595_AssignPin(HC595* dev,GPIO_TypeDef *port,uint16_t pin, pinName pinName)
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
	case HC595_OE:
		dev->oe.port = port;
		dev->oe.pin = pin;
		break;
	}
	devTemp = dev;
	return HAL_OK;
}
void HC595_TestPin(pinName pinName)
{
	while(!devTemp);
	HC595_WRITE(pinName,1);
	HAL_Delay(2000);
	HC595_WRITE(pinName,0);
	HAL_Delay(2000);
}

void HC595_Enable()
{
	HC595_WRITE(HC595_OE,0);
}

void HC595_Disable()
{
	HC595_WRITE(HC595_OE,1);
}

HAL_StatusTypeDef HC595_Send_Data(uint16_t dt)
{
	if(!devTemp) return HAL_ERROR;
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

void HC595_TestOutput()
{
	HC595_Send_Data(0x55);
	HAL_Delay(2000);
	HC595_Send_Data(0x55<<1);
	HAL_Delay(2000);
}
#endif
