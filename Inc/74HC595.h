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
	pinConfig oe;
}HC595;

typedef enum{
	HC595_LATCH,
	HC595_CLK,
	HC595_DS,
	HC595_OE
}pinName;

#define latchPort devTemp->latch.port
#define latchPin devTemp->latch.pin
#define clkPort devTemp->clk.port
#define clkPin devTemp->clk.pin
#define dsPort devTemp->ds.port
#define dsPin devTemp->ds.pin
#define oePort devTemp->oe.port
#define oePin devTemp->oe.pin

#define HC595_WRITE(PIN,LOGIC) ( 	((PIN) == HC595_LATCH)? 	HAL_GPIO_WritePin(latchPort,latchPin,(LOGIC)) 	:	\
									((PIN) == HC595_CLK) 	?	HAL_GPIO_WritePin(clkPort,clkPin,(LOGIC)) 		: 	\
									((PIN) == HC595_OE) 	?	HAL_GPIO_WritePin(oePort,oePin,(LOGIC)) 		: 	\
									((PIN) == HC595_DS) 	?	HAL_GPIO_WritePin(dsPort,dsPin,(LOGIC)) 		: 0	\
								)

HAL_StatusTypeDef HC595_Send_Data(uint16_t dt);
void HC595_TestPin(pinName pin);
void HC595_Disable();
void HC595_Enable();
HAL_StatusTypeDef HC595_AssignPin(HC595* dev,GPIO_TypeDef *port,uint16_t pin, pinName pinName);
void HC595_TestOutput();
#endif /* INC_74HC595_H_ */
#endif
