/*
 * UART_Utility.c
 *
 *  Created on: Apr 17, 2023
 *      Author: SpiritBoi
 */


#include "UART_Utility.h"
#ifdef CONFIG_USE_UART_UTILITY

UART_Utility_t _util;

void UART_Util_BeginToGetMessage(UART_HandleTypeDef *huart,uint8_t *MesgBuffer,char *CharEndOfMessage){
	_util.huart = huart;
	charEndOfMessage=CharEndOfMessage;
	buf = MesgBuffer;
	HAL_UART_Receive_IT(huart, bufTemp, 1);
}

void UART_Util_GetMessage_IT_Callback(UART_HandleTypeDef *huart){
	static uint8_t count=0;
	if(huart->Instance == UART->Instance)
	{
		HAL_UART_Receive_IT(UART, bufTemp, 1);
		count++;
		strcat((char*)buf,(char*)bufTemp);
		if(!strcmp((char*)bufTemp,charEndOfMessage)){
			UART_UTIL_SETFLAG(UART_UTIL_FLAG_MESSAGE_GET_COMPLETE);
		}
	}
}

bool UART_Util_CheckGetMessageComplete(bool ClearAfterCheck){
	if(ClearAfterCheck){
		uint8_t a = UART_UTIL_CHECKFLAG(UART_UTIL_FLAG_MESSAGE_GET_COMPLETE);
		if(a)UART_UTIL_CLEARFLAG(UART_UTIL_FLAG_MESSAGE_GET_COMPLETE);
		return a;
	}
	return UART_UTIL_CHECKFLAG(UART_UTIL_FLAG_MESSAGE_GET_COMPLETE);
}



#endif



