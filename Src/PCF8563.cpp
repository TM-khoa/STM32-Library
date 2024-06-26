/*
 * PCF8563.cpp
 *
 *  Created on: May 13, 2024
 *      Author: SpiritBoi
 */

#include "PCF8563.h"

static inline uint8_t BCDtoDec(uint8_t BCD_value);
static inline uint8_t DecToBCD(uint8_t BCD_value);

#ifdef __cplusplus
void PCF8563::Begin(I2C_HandleTypeDef *hi2c) {
	while (PCF8563_CHECKREADY != HAL_OK);
	_i2c = hi2c;
	WriteAND(Control_status_1, (uint8_t) ~PCF8563_CTRL_STATUS1_TEST1);
	WriteAND(Control_status_1, (uint8_t) ~PCF8563_CTRL_STATUS1_TESTC);
	WriteAND(CLKOUT_control, (uint8_t) ~PCF8563_CLKOUT_FE);
	StartClock();
	EnableCLKOUT(1);
	SetFrequencyCLKOUT(CLKOUT_1_Hz);
}
void PCF8563::StartClock() {
	WriteAND(Control_status_1, (uint8_t) ~PCF8563_CTRL_STATUS1_STOP);
}
void PCF8563::StopClock() {
	WriteOR(Control_status_1, (uint8_t) PCF8563_CTRL_STATUS1_STOP);
}
void PCF8563::EnableCLKOUT(bool enable) {
	if (enable)
		WriteOR(CLKOUT_control, (uint8_t) PCF8563_CLKOUT_FE);
	else
		WriteAND(CLKOUT_control, (uint8_t) ~PCF8563_CLKOUT_FE);
}
void PCF8563::SetFrequencyCLKOUT(PCF8563_CLKOUT freq) {
	switch (freq) {
	case CLKOUT_32768_Hz:
		WriteAND(CLKOUT_control, ~((1 << 0) | (1 << 1)));
		break;
	case CLKOUT_1024_Hz:
		WriteAND(CLKOUT_control, ~(1 << 1));
		break;
		WriteOR(CLKOUT_control, (1 << 0));
		break;
	case CLKOUT_32_Hz:
		WriteAND(CLKOUT_control, ~(1 << 0));
		break;
		WriteOR(CLKOUT_control, (1 << 1));
		break;
	case CLKOUT_1_Hz:
		WriteOR(CLKOUT_control, (1 << 1) | (1 << 0));
		break;
	default:
		return;
	}
}
void PCF8563::WriteTimeRegisters(RTC_t time) {
	uint8_t t[8] = {0};
	t[0] = DecToBCD(time.second);
	t[1] = DecToBCD(time.minute);
	t[2] = DecToBCD(time.hour);
	t[3] = DecToBCD(time.day);
	t[4] = DecToBCD(time.weekday);
	t[5] = DecToBCD(time.month);
	t[6] = DecToBCD(time.year);
	uint8_t temp[2];
	for (uint8_t i = 0; i < PCF8563_TIME_REGISTER_RANGE; i++) {
		temp[0] = i + PCF8563_TIME_REGISTER_OFFSET;
		temp[1] = t[i];
		while (HAL_I2C_Master_Transmit(_i2c, PCF8563_address, temp, 2, HAL_MAX_DELAY) != HAL_OK);

	}
}

RTC_t PCF8563::ReadTimeRegisters() {
	uint8_t i2cReceiveData[8];
	uint8_t txData[1] = {0x02};
	HAL_I2C_Master_Transmit(_i2c, PCF8563_address, txData, 1, HAL_MAX_DELAY);
	HAL_I2C_Master_Receive(_i2c, PCF8563_address, i2cReceiveData, 7, HAL_MAX_DELAY);
	RTC_t time = {.year = (uint8_t) (BCDtoDec(i2cReceiveData[6])),
			.month = (uint8_t) (BCDtoDec(i2cReceiveData[5] & 0x3f)),
			.day = (uint8_t) (BCDtoDec(i2cReceiveData[3] & 0x3f)),
			.weekday = (uint8_t) (BCDtoDec(i2cReceiveData[4] & 0x07)),
			.hour = (uint8_t) (BCDtoDec(i2cReceiveData[2] & 0x3f)),
			.minute = (uint8_t) (BCDtoDec(i2cReceiveData[1] & 0x7f)),
			.second = (uint8_t) (BCDtoDec(i2cReceiveData[0] & 0x7f)), };
	return time;
}

void PCF8563::WriteRegister(uint8_t REG, uint8_t Value) {
	uint8_t i2cTXData[2] = {REG,
			Value};
	while (HAL_I2C_Master_Transmit(_i2c, PCF8563_address, i2cTXData, 2, HAL_MAX_DELAY) != HAL_OK);
}

uint8_t PCF8563::ReadRegister(uint8_t REG) {
	uint8_t i2cReceiveData[1];
	HAL_I2C_Master_Transmit(_i2c, PCF8563_address, &REG, 1, HAL_MAX_DELAY);
	HAL_I2C_Master_Receive(_i2c, PCF8563_address, i2cReceiveData, 1, HAL_MAX_DELAY);
	return i2cReceiveData[0];
}

void PCF8563::WriteOR(uint8_t Address, uint8_t data) {
	uint8_t a = ReadRegister(Address);
	a |= data;
	WriteRegister(Address, a);
}

void PCF8563::WriteAND(uint8_t Address, uint8_t data) {
	uint8_t a = ReadRegister(Address);
	a &= data;
	WriteRegister(Address, a);
}
#endif

#ifndef __cplusplus
PCF8563_Handle *_pcf8563;
void PCF8563_Init(PCF8563_Handle *rtc, I2C_HandleTypeDef *hi2c) {
	if (!rtc) return;
	_pcf8563 = rtc;
	if (hi2c)
		_pcf8563->hi2c = hi2c;
	else {
		if (!_pcf8563->hi2c) return;
	}
	while (PCF8563_CHECKREADY != HAL_OK);
	PCF8563_Write_AND(Control_status_1, (uint8_t) ~PCF8563_CTRL_STATUS1_TEST1);
	PCF8563_Write_AND(Control_status_1, (uint8_t) ~PCF8563_CTRL_STATUS1_TESTC);
	PCF8563_Write_AND(CLKOUT_control, (uint8_t) ~PCF8563_CLKOUT_FE);
}

void PCF8563_StartClock() {

	PCF8563_Write_AND(Control_status_1, (uint8_t) ~PCF8563_CTRL_STATUS1_STOP);
}

void PCF8563_StopClock() {
	PCF8563_Write_OR(Control_status_1, (uint8_t) PCF8563_CTRL_STATUS1_STOP);
}

void PCF8563_CLKOUT_Enable(bool Enable) {
	if (Enable)
		PCF8563_Write_OR(CLKOUT_control, (uint8_t) PCF8563_CLKOUT_FE);
	else
		PCF8563_Write_AND(CLKOUT_control, (uint8_t) ~PCF8563_CLKOUT_FE);
}

void PCF8563_CLKOUT_SetFreq(PCF8563_CLKOUT freq) {
	switch (freq) {
	case CLKOUT_32768_Hz:
		PCF8563_Write_AND(CLKOUT_control, ~((1 << 0) | (1 << 1)));
		break;
	case CLKOUT_1024_Hz:
		PCF8563_Write_AND(CLKOUT_control, ~(1 << 1));
		break;
		PCF8563_Write_OR(CLKOUT_control, (1 << 0));
		break;
	case CLKOUT_32_Hz:
		PCF8563_Write_AND(CLKOUT_control, ~(1 << 0));
		break;
		PCF8563_Write_OR(CLKOUT_control, (1 << 1));
		break;
	case CLKOUT_1_Hz:
		PCF8563_Write_OR(CLKOUT_control, (1 << 1) | (1 << 0));
		break;
	default:
		return;
	}
}

static void PCF8563_Write(uint8_t REG, uint8_t Value) {
	uint8_t i2cTXData[2] = {REG,
			Value};
	while (HAL_I2C_Master_Transmit(PCF8563_I2C, PCF8563_address, i2cTXData, 2, HAL_MAX_DELAY) != HAL_OK);
}

uint8_t PCF8563_Read(uint8_t REG) {
	uint8_t i2cReceiveData[1];
	HAL_I2C_Master_Transmit(PCF8563_I2C, PCF8563_address, &REG, 1, HAL_MAX_DELAY);
	HAL_I2C_Master_Receive(PCF8563_I2C, PCF8563_address, i2cReceiveData, 1, HAL_MAX_DELAY);
	return i2cReceiveData[0];
}

void PCF8563_WriteTimeRegisters(RTC_t time) {
	uint8_t t[8] = {0};
	t[0] = DecToBCD(time.second);
	t[1] = DecToBCD(time.minute);
	t[2] = DecToBCD(time.hour);
	t[3] = DecToBCD(time.day);
	t[4] = DecToBCD(time.weekday);
	t[5] = DecToBCD(time.month);
	t[6] = DecToBCD(time.year);
	uint8_t temp[2];
	for (uint8_t i = 0; i < PCF8563_TIME_REGISTER_RANGE; i++) {
		temp[0] = i + PCF8563_TIME_REGISTER_OFFSET;
		temp[1] = t[i];
		while (HAL_I2C_Master_Transmit(PCF8563_I2C, PCF8563_address, temp, 2, HAL_MAX_DELAY) != HAL_OK);
	}
}
RTC_t PCF8563_ReadTimeRegisters() {
	uint8_t i2cReceiveData[8];
	uint8_t txData[1] = {0x02};
	HAL_I2C_Master_Transmit(PCF8563_I2C, PCF8563_address, txData, 1, HAL_MAX_DELAY);
	HAL_I2C_Master_Receive(PCF8563_I2C, PCF8563_address, i2cReceiveData, 7, HAL_MAX_DELAY);
	RTC_t time = {.hour = (BCDtoDec(i2cReceiveData[2] & 0x3f)),
			.minute = (BCDtoDec(i2cReceiveData[1] & 0x7f)),
			.second = (BCDtoDec(i2cReceiveData[0] & 0x7f)),
			.day = (BCDtoDec(i2cReceiveData[3] & 0x3f)),
			.weekday = (BCDtoDec(i2cReceiveData[4] & 0x07)),
			.month = (BCDtoDec(i2cReceiveData[5] & 0x3f)),
			.year = (BCDtoDec(i2cReceiveData[6])), };
	return time;
}

void PCF8563_Write_OR(uint8_t Address, uint8_t data) {
	uint8_t a = PCF8563_Read(Address);
	a |= data;
	PCF8563_Write(Address, a);
}

void PCF8563_Write_AND(uint8_t Address, uint8_t data) {
	uint8_t a = PCF8563_Read(Address);
	a &= data;
	PCF8563_Write(Address, a);
}
#endif

static inline uint8_t BCDtoDec(uint8_t BCD_value) {
	uint8_t output;
	output = ((BCD_value & 0xF0) >> 4) * 10;
	output += (BCD_value & 0x0F);
	return output;

}

static inline uint8_t DecToBCD(uint8_t Value) {
	uint8_t output;
	output = ((Value / 10) << 4);
	output |= (Value % 10);
	return output;
}

