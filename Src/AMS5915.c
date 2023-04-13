/*
 * AMS5915.c
 *
 *  Created on: Mar 26, 2023
 *      Author: SpiritBoi
 */
#include "AMS5915.h"
#ifdef CONFIG_USE_AMS5915
/**
 *
 * @param ams pointer point to device object handle
 * @param hi2c hardware target i2c
 * @return HAL_OK if device is found in I2C line
 */
HAL_StatusTypeDef AMS5915_Init(AMS5915 *ams, I2C_HandleTypeDef *hi2c)
{
	if(!ams && !hi2c) return HAL_ERROR;
	ams->hi2c = hi2c;
	ams->hi2c->Devaddress = AMS5915_ADDR << 1;
	return HAL_I2C_IsDeviceReady(hi2c, ams->hi2c->Devaddress, 3, 500);
}

/**
 *
 * @param ams pointer point to device object handle
 * @return HAL_OK; if *ams is NULL, return HAL_ERROR
 * Need to use void HAL_I2C_MasterRxCpltCallback(I2C_HandleTypeDef *hi2c) in main
 */
HAL_StatusTypeDef AMS5915_ReadRAW(AMS5915 *ams)
{
	if(!ams) return HAL_ERROR;
	return HAL_I2C_Master_Receive_IT(ams->hi2c, ams->hi2c->Devaddress, ams->buf, 4);
}

float AMS5915_CalPressure(AMS5915 *ams)
{
	static uint16_t sensp ;
	static float p, digout_p;
	sensp = (digout_pmax - digout_pmin) / (pmax - pmin);
	digout_p = ((ams->buf[0] & 0x3f) << 8) | (ams->buf[1]);
	p = (digout_p - digout_pmin) / sensp + pmin;
	return p;
}

#endif /* defined(CONFIG_USE_AMS5915) */
