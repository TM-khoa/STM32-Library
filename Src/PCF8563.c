/*
 * PCF8563.c
 *
 *  Created on: Apr 23, 2023
 *      Author: SpiritBoi
 *      Source: https://github.com/Bill2462/PCF8563-Arduino-Library/tree/master/src
 */

#include "PCF8563.h"
#ifdef CONFIG_USE_PCF8563

PCF8563_Handle *_rtc;
void PCF8563_Init(PCF8563_Handle *rtc,I2C_HandleTypeDef *hi2c){
	if(!rtc) return;
	_rtc = rtc;
	if(hi2c) _rtc->hi2c = hi2c;
	else {if(!_rtc->hi2c) return;}
	_rtc->hi2c->Devaddress = PCF8563_address << 1;
	while(PCF8563_CHECKREADY!=HAL_OK);
}

void PCF8563_Write(uint8_t REG,uint8_t Value)
{
	uint8_t i2cTXData[2]={REG,Value};
	while(HAL_I2C_Master_Transmit(rtc8563I2C, rtc8563I2C->Devaddress, i2cTXData, 2,HAL_MAX_DELAY)!=HAL_OK);

}

uint8_t PCF8563_Read(uint8_t REG)
{
	uint8_t i2cReceiveData[1];
	HAL_I2C_Master_Transmit(rtc8563I2C, rtc8563I2C->Devaddress, &REG, 1,HAL_MAX_DELAY);
	HAL_I2C_Master_Receive(rtc8563I2C, rtc8563I2C->Devaddress, i2cReceiveData, 1,HAL_MAX_DELAY);
	return i2cReceiveData[0];
}


uint8_t PCF8563_BCD_Decode(uint8_t BCD_value)
{
  uint8_t output;
  output = ((BCD_value&0xF0)>>4)*10;
  output += (BCD_value & 0x0F);
  return output;
}

uint8_t PCF8563_BCD_Encode(uint8_t Value)
{
  uint8_t output;
  output = ((Value/10) << 4);
  output |= (Value%10);
  return output;
}

//Time PCF8563_GetTime()
//{
//  Time output;
//
//  //read data registers contents
//  const uint8_t YEAR    = read(Years);
//  const uint8_t MONTH   = read(Century_months);
//  const uint8_t DAY     = read(Days);
//  const uint8_t WEEKDAY = read(Weekdays);
//  const uint8_t HOUR    = read(Hours);
//  const uint8_t MINUTE  = read(Minutes);
//  const uint8_t SECONDS = read(VL_seconds);
//
//  //convert readed data to numbers using bcd_to_number function).
//  output.year    = bcd_to_number((YEAR&0b11110000)>>4,YEAR&0b00001111);
//  output.month   = bcd_to_number((MONTH&0b00010000)>>4,MONTH&0b00001111);
//  output.day     = bcd_to_number((DAY&0b00110000)>>4,DAY&0b00001111);
//  output.weekday = bcd_to_number(0,WEEKDAY&0b00000111);
//  output.hour    = bcd_to_number((HOUR&0b00110000)>>4,HOUR&0b00001111);
//  output.minute  = bcd_to_number((MINUTE&0b01110000)>>4,MINUTE&0b00001111);
//  output.second  = bcd_to_number((SECONDS&0b01110000)>>4,SECONDS&0b00001111);
//
//  return output;
//}

#endif

