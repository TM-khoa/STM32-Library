/*
 * PCF8563.c
 *
 *  Created on: Apr 23, 2023
 *      Author: SpiritBoi
 *      Source: https://github.com/Bill2462/PCF8563-Arduino-Library/tree/master/src
 */

#include "PCF8563.h"
#ifdef CONFIG_USE_PCF8563

static inline void PCF8563_Write_OR(uint8_t Address, uint8_t data);
static inline void PCF8563_Write_AND(uint8_t Address, uint8_t data);
static inline uint8_t BCD_Decode(uint8_t BCD_value);
static inline uint8_t BCD_Encode(uint8_t Value);

PCF8563_Handle *_pcf8563;
void PCF8563_Init(PCF8563_Handle *rtc,I2C_HandleTypeDef *hi2c){
	if(!rtc) return;
	_pcf8563 = rtc;
	if(hi2c) _pcf8563->hi2c = hi2c;
	else {if(!_pcf8563->hi2c) return;}
	_pcf8563->hi2c->Devaddress = PCF8563_address << 1;
	while(PCF8563_CHECKREADY!=HAL_OK);
	PCF8563_StopClock();
	PCF8563_Write_AND(Control_status_1, (uint8_t)~PCF8563_CTRL_STATUS1_TEST1);
	PCF8563_Write_AND(Control_status_1, (uint8_t)~PCF8563_CTRL_STATUS1_TESTC);
	PCF8563_Write_AND(CLKOUT_control, (uint8_t)~PCF8563_CLKOUT_FE);
}

void PCF8563_StartClock()
{

	PCF8563_Write_AND(Control_status_1,(uint8_t)~PCF8563_CTRL_STATUS1_STOP);
}

void PCF8563_StopClock()
{
	PCF8563_Write_OR(Control_status_1,(uint8_t)PCF8563_CTRL_STATUS1_STOP);
}

void PCF8563_CLKOUT_Enable(bool Enable)
{
	if(Enable) PCF8563_Write_OR(CLKOUT_control,(uint8_t)PCF8563_CLKOUT_FE);
	PCF8563_Write_AND(CLKOUT_control,(uint8_t)~PCF8563_CLKOUT_FE);
}

void PCF8563_CLKOUT_SetFreq(PCF8563_CLKOUT freq)
{
	switch(freq){
	case CLKOUT_32768_Hz:
		PCF8563_Write_AND(CLKOUT_control,~((1<<0)|(1<<1))); break;
	case CLKOUT_1024_Hz:
		PCF8563_Write_AND(CLKOUT_control,~(1<<1));break;
		PCF8563_Write_OR(CLKOUT_control,(1<<0)); break;
	case CLKOUT_32_Hz:
		PCF8563_Write_AND(CLKOUT_control,~(1<<0));break;
		PCF8563_Write_OR(CLKOUT_control,(1<<1)); break;
	case CLKOUT_1_Hz:
		PCF8563_Write_OR(CLKOUT_control,(1<<1)|(1<<0)); break;
	default: return;
	}
}

void PCF8563_Write(uint8_t REG,uint8_t Value)
{
	uint8_t i2cTXData[2]={REG,Value};
	while(HAL_I2C_Master_Transmit(PCF8563_I2C, PCF8563_I2C->Devaddress, i2cTXData, 2,HAL_MAX_DELAY)!=HAL_OK);
}

uint8_t PCF8563_Read(uint8_t REG)
{
	uint8_t i2cReceiveData[1];
	HAL_I2C_Master_Transmit(PCF8563_I2C, PCF8563_I2C->Devaddress, &REG, 1,HAL_MAX_DELAY);
	HAL_I2C_Master_Receive(PCF8563_I2C, PCF8563_I2C->Devaddress, i2cReceiveData, 1,HAL_MAX_DELAY);
	return i2cReceiveData[0];
}


void PCF8563_WriteTimeRegisters(PCF8563_Time *time)
{
	uint8_t t[8]={0};
	if(time){
		t[0] = BCD_Encode(time->second);
		t[1] = BCD_Encode(time->minute);
		t[2] = BCD_Encode(time->hour);
		t[3] = BCD_Encode(time->day);
		t[4] = BCD_Encode(time->weekday);
		t[5] = BCD_Encode(time->month);
		t[6] = BCD_Encode(time->year);
	} else {
		t[0] = BCD_Encode(pcfSecond);
		t[1] = BCD_Encode(pcfMinute);
		t[2] = BCD_Encode(pcfHour);
		t[3] = BCD_Encode(pcfDay);
		t[4] = BCD_Encode(pcfWeekday);
		t[5] = BCD_Encode(pcfMonth);
		t[6] = BCD_Encode(pcfYear);
	}
	if(		t[0] < 60
		&&  t[1] < 60
		&&  t[2] < 24
		&&  t[3] > 1 && t[3] < 32
		&&  t[4] < 6
		&&  t[5] > 1 && t[5] < 12
		&&  t[6] < 99){
	}
		uint8_t temp[2];
		for(uint8_t i = 0;i < PCF8563_TIME_REGISTER_RANGE;i++){
			temp[0]=i+PCF8563_TIME_REGISTER_OFFSET;
			temp[1]=t[i];
			while(HAL_I2C_Master_Transmit(PCF8563_I2C, PCF8563_I2C->Devaddress, temp, 2,HAL_MAX_DELAY)!=HAL_OK);
		}
}
uint8_t PCF8563_ReadTimeRegisters()
{
	uint8_t i2cReceiveData[8];
	uint8_t txData[1]={0x02};
	HAL_I2C_Master_Transmit(PCF8563_I2C, PCF8563_I2C->Devaddress, txData, 1,HAL_MAX_DELAY);
	HAL_I2C_Master_Receive(PCF8563_I2C, PCF8563_I2C->Devaddress, i2cReceiveData, 7,HAL_MAX_DELAY);
	pcfSecond = (BCD_Decode(i2cReceiveData[0]&0x7f));
	pcfMinute = (BCD_Decode(i2cReceiveData[1]&0x7f));
	pcfHour = (BCD_Decode(i2cReceiveData[2]&0x3f));
	pcfDay = (BCD_Decode(i2cReceiveData[3]&0x3f));
	pcfWeekday =(BCD_Decode(i2cReceiveData[4]&0x07));
	pcfMonth = (BCD_Decode(i2cReceiveData[5]&0x3f));
	pcfYear = (BCD_Decode(i2cReceiveData[6]&0x3f));
	__NOP();
	return 0;
}

static inline void PCF8563_Write_OR(uint8_t Address, uint8_t data)
{
	uint8_t a = PCF8563_Read(Address);
	a |= data;
	PCF8563_Write(Address, a);
}

static inline void PCF8563_Write_AND(uint8_t Address, uint8_t data)
{
	uint8_t a = PCF8563_Read(Address);
	a &= data;
	PCF8563_Write(Address, a);
}

static inline uint8_t BCD_Decode(uint8_t BCD_value)
{
  uint8_t output;
  output = ((BCD_value&0xF0)>>4)*10;
  output += (BCD_value & 0x0F);
  return output;
}

static inline uint8_t BCD_Encode(uint8_t Value)
{
  uint8_t output;
  output = ((Value/10) << 4);
  output |= (Value%10);
  return output;
}
#endif

