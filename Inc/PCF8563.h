/*
 * PCF8563.h
 *
 *  Created on: Apr 23, 2023
 *      Author: SpiritBoi
 *      Source: https://github.com/Bill2462/PCF8563-Arduino-Library/tree/master/src
 */

#ifndef INC_PCF8563_H_
#define INC_PCF8563_H_
#include "main.h"
#ifdef CONFIG_USE_PCF8563
#include "stdbool.h"
typedef struct PCF8563_Time
{
  uint8_t year;
  uint8_t month;
  uint8_t day;
  uint8_t weekday;
  uint8_t hour;
  uint8_t minute;
  uint8_t second;
}PCF8563_Time;

typedef struct PCF8563_Handle{
	I2C_HandleTypeDef *hi2c;
	PCF8563_Time t;
}PCF8563_Handle;

typedef enum
{
  CLKOUT_32768_Hz,
  CLKOUT_1024_Hz,
  CLKOUT_32_Hz,
  CLKOUT_1_Hz,
}PCF8563_CLKOUT;

typedef enum
{
    PCF8563_address  = 0x51,
    Control_status_1 = 0x00,
    Control_status_2 = 0x01,
    VL_seconds       = 0x02,
    Minutes          = 0x03,
    Hours            = 0x04,
    Days             = 0x05,
    Weekdays         = 0x06,
    Century_months   = 0x07,
    Years            = 0x08,
    Minute_alarm     = 0x09,
    Hour_alarm       = 0x0A,
    Day_alarm        = 0x0B,
    Weekday_alarm    = 0x0C,
    CLKOUT_control   = 0x0D,
    Timer_control    = 0x0E,
    Timer            = 0x0F,
}registers;

#define PCF8563_CTRL_STATUS1_TEST1 (1<<7)
#define PCF8563_CTRL_STATUS1_STOP (1<<5)
#define PCF8563_CTRL_STATUS1_TESTC (1<<3)
#define PCF8563_CTRL_STATUS2_TI_TP (1<<4)
#define PCF8563_CTRL_STATUS2_AF (1<<3)
#define PCF8563_CTRL_STATUS2_TF (1<<2)
#define PCF8563_CTRL_STATUS2_AIE (1<<1)
#define PCF8563_CTRL_STATUS2_TIE (1<<0)
#define PCF8563_CLKOUT_FE (1<<7)

#define PCF8563_CHECKREADY HAL_I2C_IsDeviceReady(hi2c, (PCF8563_address << 1), 3, HAL_MAX_DELAY)
#define PCF8563_READ()
#define PCF8563_WRITE_BIT(REG,BIT_MASK)
#define PCF8563_TIME_REGISTER_OFFSET 2
#define PCF8563_TIME_REGISTER_RANGE 7

#define PCF8563_I2C _pcf8563->hi2c
#define pcfSecond _pcf8563->t.second
#define pcfMinute _pcf8563->t.minute
#define pcfHour _pcf8563->t.hour
#define pcfDay _pcf8563->t.day
#define pcfMonth _pcf8563->t.month
#define pcfWeekday _pcf8563->t.weekday
#define pcfYear _pcf8563->t.year

uint8_t PCF8563_Read(uint8_t REG);
void PCF8563_Write(uint8_t REG,uint8_t Value);
uint8_t PCF8563_ReadTimeRegisters();
void PCF8563_WriteTimeRegisters(PCF8563_Time *t);
void PCF8563_CLKOUT_SetFreq(PCF8563_CLKOUT freq);
void PCF8563_CLKOUT_Enable(bool Enable);
void PCF8563_StartClock();
void PCF8563_StopClock();
//general control
void PCF8563_Init();
void PCF8563_StopClock();
void PCF8563_StartClock();
//time settings functions
void PCF8563_SetYear(uint8_t year);
void PCF8563_SetMonth(uint8_t month);
void PCF8563_SetDay(uint8_t day);
void PCF8563_SetHour(uint8_t hour);
void PCF8563_SetMin(uint8_t min);
void PCF8563_SetSecond(uint8_t second);
//clkout output
void PCF8563_EnableClkOutput();
void PCF8563_DisableClkOutput();
//void PCF8563_SetClkOutputFrequency(PCF8563_CLKOUT frequency);
#endif
#endif /* INC_PCF8563_H_ */
