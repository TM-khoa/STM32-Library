/*
 * AMS5915.h
 *
 *  Created on: Mar 26, 2023
 *      Author: SpiritBoi
 */

#include "main.h"
#ifdef CONFIG_USE_AMS5915
#ifndef _AMS5915_H_
#define _AMS5915_H_

#define AMS5915_ADDR 0x28
#define AMS5915_MIN_PRES_MBAR -50.0f
#define AMS5915_MAX_PRES_MBAR 50.0f
#define SIZE_BUF 4
#define AMS5915_pmax   50
#define AMS5915_pmin  0
#define AMS5915_digout_pmin 1638
#define AMS5915_digout_pmax 14745



#define CHECK_DEVICE_READY (HAL_I2C_IsDeviceReady(hi2c, ams->hi2c->Devaddress, 3, 500))
#define READ_RAW_DATA (HAL_I2C_Master_Receive_IT(ams->hi2c, ams->hi2c->Devaddress, ams->buf, 4))

typedef struct AMS5915{
	uint8_t buf[SIZE_BUF];
	I2C_HandleTypeDef *hi2c;
}AMS5915;

typedef enum{
	AMS5915_OK,
	AMS5915_ERROR,
	AMS5915_INVALID_ARG,
}AMS5915_Status_t;

AMS5915_Status_t AMS5915_Init(AMS5915 *ams, I2C_HandleTypeDef *hi2c);
AMS5915_Status_t AMS5915_ReadRaw(AMS5915 *ams);
float AMS5915_CalPressure(AMS5915 *ams);

#endif
#endif /*  defined(CONFIG_USE_AMS5915) */
