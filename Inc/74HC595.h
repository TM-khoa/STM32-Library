/*
 * 74HC595.h
 *
 *  Created on: Mar 23, 2023
 *      Author: hoanganh
 */


#ifndef INC_74HC595_H_
#define INC_74HC595_H_
#include "main.h"
#ifdef CONFIG_USE_74HC595

typedef struct {
	GPIO_TypeDef *port;
	uint16_t pin;
}pinConfig;

typedef struct HC595{
	pinConfig ds;
	pinConfig clk;
	pinConfig latch;
} HC595;

typedef enum{
	HC595_LATCH,
	HC595_CLK,
	HC595_DS
}pinName;

#define latchPort dev->latch.port
#define latchPin dev->latch.pin
#define clkPort dev->clk.port
#define clkPin dev->clk.pin
#define dsPort dev->ds.port
#define dsPin dev->ds.pin

#define HC595_WRITE(PIN,LOGIC) ( 	((PIN) == HC595_LATCH)? 	HAL_GPIO_WritePin(latchPort,latchPin,(LOGIC)) 	:	\
									((PIN) == HC595_CLK) 	?	HAL_GPIO_WritePin(clkPort,clkPin,(LOGIC)) 		: 	\
									((PIN) == HC595_DS) 	?	HAL_GPIO_WritePin(dsPort,dsPin,(LOGIC)) 		: 0	\
								)

HAL_StatusTypeDef HC595_Send_Data(HC595* dev,uint16_t dt);
HAL_StatusTypeDef HC595_TestPin(HC595* dev,pinName pin);
HAL_StatusTypeDef HC595_ConfigOnePin(HC595* dev,GPIO_TypeDef *port,uint16_t pin, pinName pinName);
#endif /* INC_74HC595_H_ */
#endif
