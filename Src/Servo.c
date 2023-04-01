/*
 * Servo.c
 *
 *  Created on: Mar 31, 2023
 *      Author: SpiritBoi
 */
#include "Servo.h"
#ifdef CONFIG_USE_SERVO

Servo sv;

void Servo_Config(TIM_HandleTypeDef *htim,uint32_t TIM_CHANNEL)
{
	sv.htim = htim;
	sv.TIM_CHANNEL = TIM_CHANNEL;
}

HAL_StatusTypeDef Servo_SetAngle(uint8_t Angle)
{
	if(!SERVO_ANGLE_VALID(Angle)) return HAL_ERROR;
	SERVO_MOVE(Angle);
	return HAL_OK;
}

#endif
