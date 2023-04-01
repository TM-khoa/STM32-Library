/*
 * HX711.h
 *
 *  Created on: Mar 31, 2023
 *      Author: Dien
 */

#ifndef INC_HX711_H_
#define INC_HX711_H_

#include "main.h"

typedef struct{
	GPIO_TypeDef *port;
	uint16_t pin;
}pinconfig;

typedef struct{
	pinconfig SCK;
	pinconfig Data;
	uint32_t buffer;
}HX711;

typedef enum{
	HX711_SCK,
	HX711_Data,
}pinName;

#define clkPort dev->SCK.port
#define clkPin dev->SCK.pin
#define dtPort dev->Data.port
#define dtPin dev->Data.pin
#define buffer dev->buffer

#define HX711_WRITE(PIN,LOGIC) (((PIN) == HX711_SCK)? HAL_GPIO_WritePin(clkPort,clkPin,(LOGIC)) : 0)
#define HX711_READ(PIN) (((PIN) == HX711_Data)? HAL_GPIO_ReadPin(dtPort,dtPin) 	:  0)
HAL_StatusTypeDef HX711_ConfigOnePin(HX711* dev,GPIO_TypeDef *port,uint16_t pin, pinName pinName);
void HX711_readvalue (HX711* dev,pinName pinname);
#endif /* INC_HX711_H_ */
