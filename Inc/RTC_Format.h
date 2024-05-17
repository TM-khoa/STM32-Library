/*
 * RTC_Format.h
 *
 *  Created on: May 12, 2023
 *      Author: SpiritBoi
 */

#ifndef INC_RTC_FORMAT_H_
#define INC_RTC_FORMAT_H_

#include "main.h"
#include "string.h"
#include "stdio.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct RTC_t {
		uint8_t year;
		uint8_t month;
		uint8_t day;
		uint8_t weekday;
		uint8_t hour;
		uint8_t minute;
		uint8_t second;
} RTC_t;

RTC_t RTC_GetTimeFromString(char *s);
void RTC_PackTimeToString(RTC_t t, char *s);
uint8_t RTC_ConvertTickElapsedToMinute(uint16_t input_RTC_TickCount);
uint8_t RTC_ConvertTickElapsedToSecond(uint16_t input_RTC_TickCount);
uint8_t RTC_ConvertTickElapsedToHour(uint16_t input_RTC_TickCount);

#ifdef __cplusplus
}
#endif
#endif /* INC_RTC_FORMAT_H_ */
