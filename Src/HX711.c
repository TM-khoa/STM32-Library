/*
 * HX711.c
 *
 *  Created on: Mar 31, 2023
 *      Author: Dien
 */


/**
 * Một số lỗi khi sử dụng thư viện:
 * Chưa khởi tạo biến đối tượng HX711, trả về HAL_ERROR
 * Tắt sai LINE ngắt
 * Chưa bật LINE ngắt
 *
 */

#include "HX711.h"

#ifdef CONFIG_USE_HX711

HX711* devTemp;

HAL_StatusTypeDef HX711_AssignPin(HX711* dev,GPIO_TypeDef *port,uint16_t pin, pinName pinName)
{
	if(!dev) return HAL_ERROR;
	switch(pinName){
	case HX711_SCK:
		dev->SCK.port = port;
		dev->SCK.pin = pin;
		break;
	case HX711_Data:
		dev->Data.port = port;
		dev->Data.pin = pin;
		break;
	}

	return HAL_OK;
}

HAL_StatusTypeDef HX711_AssignEXTILine(HX711* dev,IRQn_Type IRQ)
{
	if(!dev) return HAL_OK;
	dev->EXTI_LINE = IRQ;
	return HAL_OK;
}

void HX711_SetTarget(HX711* dev)
{
	while(!dev->EXTI_LINE);
	devTemp = dev;
}

uint32_t HX711_ReadValue()
{
	while(!devTemp->EXTI_LINE);
	HX711_DISABLE_DOUT_INTERRUPT;
	HX711_SCK_WRITE(0);
	buffer = 0;
	for (int i = 0; i<24; i++){
		HX711_SCK_WRITE(1);
		buffer = buffer << 1;
		HX711_SCK_WRITE(0);
		if (HX711_DATA_READ) buffer++;
	}
	HX711_SCK_WRITE(1);
	buffer = buffer^0x8000;
	HX711_SCK_WRITE(0);
	HX711_ENABLE_DOUT_INTERRUPT;
	return buffer;
}
uint32_t HX711_ReadAverage()
{
	static uint32_t Val=0;
	static uint8_t count=0;
	if(count < HX711_AVERAGE_SAMPLE){
		count = 0;
		Val += HX711_ReadValue();
	} else return Val/HX711_AVERAGE_SAMPLE;
}
#endif
