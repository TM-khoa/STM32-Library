/*
 * AMS5915.c
 *
 *  Created on: Mar 26, 2023
 *      Author: SpiritBoi
 */
#include "AMS5915.h"
#ifdef CONFIG_USE_AMS5915
/**
 * @brief Initializes the AMS5915 pressure sensor.
 *
 * This function initializes and configures the AMS5915 pressure sensor
 * with the given I2C_HandleTypeDef instance for communication.
 *
 * @param ams Pointer to an AMS5915 struct to handle pressure sensor operations.
 * @param hi2c Pointer to an I2C_HandleTypeDef instance representing the I2C line connected to the pressure sensor.
 * @return HAL_StatusTypeDef value indicating the result of the initialization operation.
 */
HAL_StatusTypeDef AMS5915_Init(AMS5915 *ams, I2C_HandleTypeDef *hi2c)
{
	if(!ams && !hi2c) return HAL_ERROR;
	ams->hi2c = hi2c;
	ams->hi2c->Devaddress = AMS5915_ADDR << 1;
	return CHECK_DEVICE_READY;
}

/**
 * @brief Read raw data from AMS5915 pressure sensor.
 * @param ams Pointer to an AMS5915 struct to handle pressure sensor operations.
 * @return HAL_StatusTypeDef
 * @note Need to use void HAL_I2C_MasterRxCpltCallback(I2C_HandleTypeDef *hi2c) in main
 */
HAL_StatusTypeDef AMS5915_ReadRaw(AMS5915 *ams)
{
	if(!ams) return HAL_ERROR;
	return READ_RAW_DATA;
}

/**
 * @brief Calculate pressure value from raw data stored in AMS5915 struct buf member.
 * @param ams Pointer to an AMS5915 struct to handle pressure sensor operations.
 * @return HAL_StatusTypeDef
 * @note Need to use void HAL_I2C_MasterRxCpltCallback(I2C_HandleTypeDef *hi2c) in main
 */
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
