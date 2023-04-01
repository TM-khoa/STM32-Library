/*
 * HX711.c
 *
 *  Created on: Mar 31, 2023
 *      Author: Dien
 */


#include "HX711.h"


HAL_StatusTypeDef HX711_ConfigOnePin(HX711* dev,GPIO_TypeDef *port,uint16_t pin, pinName pinName)
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

void HX711_readvalue (HX711* dev,pinName pinname){
	HX711_WRITE(HX711_SCK,0);
	buffer = 0;
	while(HX711_READ(HX711_Data) == 1);
	for (int i = 0; i<24; i++){
		HX711_WRITE(HX711_SCK,1);
		buffer = buffer<<1;
		HX711_WRITE(HX711_SCK,0);
		if (HX711_READ(HX711_Data)) buffer++;
	}
	HX711_WRITE(HX711_SCK,1);
	buffer = buffer^0x8000;
	HX711_WRITE(HX711_SCK,0);
}
