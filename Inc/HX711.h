/*
 * HX711.h
 *
 *  Created on: Mar 31, 2023
 *      Author: Dien
 */
#ifndef INC_HX711_H_
#define INC_HX711_H_

#include "main.h"
#ifdef CONFIG_USE_HX711
typedef struct{
	GPIO_TypeDef *port;
	uint16_t pin;
}pinconfig;

typedef struct{
	pinconfig SCK;
	pinconfig Data;
	uint32_t buffer;
	uint32_t EXTI_LINE;
}HX711;

typedef enum{
	HX711_SCK,
	HX711_Data,
}pinName;

#define clkPort devTemp->SCK.port
#define clkPin devTemp->SCK.pin
#define dtPort devTemp->Data.port
#define dtPin devTemp->Data.pin
#define buffer devTemp->buffer
#define HX711_EXTI_line devTemp->EXTI_LINE

#define HX711_ENABLE_DOUT_INTERRUPT (HAL_NVIC_EnableIRQ(HX711_EXTI_line))
#define HX711_DISABLE_DOUT_INTERRUPT (HAL_NVIC_DisableIRQ(HX711_EXTI_line))
#define HX711_SCK_WRITE(LOGIC) (HAL_GPIO_WritePin(clkPort,clkPin,(LOGIC)))
#define HX711_DATA_READ (HAL_GPIO_ReadPin(dtPort,dtPin))

#define HX711_CALIB_OFFSET
#define HX711_AVERAGE_SAMPLE 10

HAL_StatusTypeDef HX711_AssignPin(HX711* dev,GPIO_TypeDef *port,uint16_t pin, pinName pinName);
HAL_StatusTypeDef HX711_AssignEXTILine(HX711* dev,IRQn_Type IRQ);
uint32_t HX711_ReadValue();
uint32_t HX711_ReadAverage();
void HX711_SetTarget(HX711* dev);
#endif /* INC_HX711_H_ */
#endif
