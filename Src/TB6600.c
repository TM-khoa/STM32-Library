/*
 * TB6600.c
 *
 *  Created on: Mar 28, 2023
 *      Author: SpiritBoi
 */

#include "TB6600.h"
#ifdef CONFIG_USE_TB6600
HAL_StatusTypeDef TB6600_AssignPin(TB6600* dev,GPIO_TypeDef *port,uint16_t pin, pinName pinName)
{
	if(!dev) return HAL_ERROR;

	switch(pinName){
	case TB6600_PULSE:
		/*
		 * Write dev->pulse.port = port; instead of pulsePort to copy to other libray with ease
		 */
		dev->pulse.port = port;
		dev->pulse.pin = pin;
		break;
	case TB6600_DIR:
		dev->dir.port = port;
		dev->dir.pin = pin;
		break;
	case TB6600_EN:
		dev->en.port = port;
		dev->en.pin = pin;
		break;
	}
	return HAL_OK;
}

HAL_StatusTypeDef TB6600_SetActive(TB6600* dev, StepState en)
{
	if(!dev) return HAL_ERROR;
	TB6600_WRITE(TB6600_EN,1);
	if(en == ACTIVE)
		TB6600_WRITE(TB6600_EN,0);
	return HAL_OK;
}
HAL_StatusTypeDef TB6600_SetDir(TB6600* dev, StepState dir)
{
	if(!dev) return HAL_ERROR;
	if(dir == DIR_CW) TB6600_WRITE(TB6600_DIR,0);
	else TB6600_WRITE(TB6600_DIR,1);
	return HAL_OK;
}

HAL_StatusTypeDef TB6600_StepAuto(TB6600* dev, GPIO_PinState state)
{
	if(!dev) return HAL_ERROR;
	TB6600_WRITE(TB6600_PULSE,state);
	return HAL_OK;
}
#endif
