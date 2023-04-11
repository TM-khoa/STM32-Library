/*
 * 74HC595.c
 *
 *  Created on: Mar 23, 2023
 *      Author: hoanganh
 */

#include "74HC595.h"
#ifdef CONFIG_USE_74HC595
HC595 *devTemp = NULL;
HAL_StatusTypeDef HC595_AssignPin(HC595* dev,GPIO_TypeDef *port,uint16_t pin, pinName pinName)
{
	if(!dev) return HAL_ERROR;
	switch(pinName){
	case HC595_CLK:
		dev->clk.port = port;
		dev->clk.pin = pin;
		break;
	case HC595_DS:
		dev->ds.port = port;
		dev->ds.pin = pin;
		break;
	case HC595_LATCH:
		dev->latch.port = port;
		dev->latch.pin = pin;
		break;
	case HC595_OE:
		dev->oe.port = port;
		dev->oe.pin = pin;
		break;
	}
	devTemp = dev;
	return HAL_OK;
}
HAL_StatusTypeDef HC595_TestPin(pinName pinName)
{
	while(!devTemp);
	HC595_WRITE(pinName,1);
	HAL_Delay(2000);
	HC595_WRITE(pinName,0);
	HAL_Delay(2000);
}

void HC595_Enable()
{
	HC595_WRITE(HC595_OE,0);
}

void HC595_Disable()
{
	HC595_WRITE(HC595_OE,1);
}
// Bit đầu tiên sẽ là Q cuối cùng của 74HC595(tính cả nối tiếp)
HAL_StatusTypeDef HC595_Send_Data(uint8_t dt[],uint8_t n)
{
	if(!devTemp) return HAL_ERROR;
	for(int x = 0;x<n;x++)
	{
		for(int i = 0;i<8;i++)
		{
			if(dt[x] & 0x80)
				HC595_WRITE(HC595_DS,1);
			else
				HC595_WRITE(HC595_DS,0);

			HC595_WRITE(HC595_CLK,0);
			HC595_WRITE(HC595_CLK,1);
			dt[x] <<= 1;
		}
		HC595_WRITE(HC595_LATCH,0);
		HC595_WRITE(HC595_LATCH,1);
	}
	return HAL_OK;
}
// (Qn output)
// n là ouput Qn cần test ( từ Q0-> Qn max)
// hc_max là số hc595 hiện có
HAL_StatusTypeDef HC595_Test_OutputPin(uint8_t n,uint8_t hc_max)
{
	if(!devTemp) return HAL_ERROR;
	// Ý tưởng : Khi muốn gửi đến 1 bit bất kỳ bắt buộc phải gửi từ Qn max trước
	// => Tính toán và gửi các dummy byte sau đó sẽ gửi bit cần gửi
	// Ví dụ cho code bên dưới : n = 13, hc_max = 5 ( có 5 HC595 => Qn max = 39, nhưng chỉ muốn test Qn với n = 13)
	// Tìm vị trí HC595 hiện tại cần test hc_x = 1 ( tính từ 0)
	// Tạo một temp_arr chứa các dummy byte và byte chứa bit cần test => nó sẽ chứa 4 phần tử (gửi 3 dummy byte và 1 byte chứa bit cần test)
	// Tìm vị trí của bit cần test trong byte chứa bit cần test n = 13 tương ứng với bit thứ 3 tính từ LSB pos_in_hc = (1+1)*8-1-13 = 2 ( không bằng 3 bởi vì sử dụng để thực hiện phép dịch trái 0x01)
	// Sau đó tạo các dummy byte bằng for như ví dụ trên thì sẽ có 3 vòng for tương đương với 3 dummy byte
	// Sau đó là byte chứa bit cần test, thực hiện dịch để tìm vị trí chính xác và dùng hàm HC595_Send_Data gửi dữ liệu

	// Kết quả: Tạo một xung từ bit cần test

	uint8_t hc_x = n/8;
	uint8_t temp_arr[hc_max - hc_x];
	uint8_t pos_in_hc = ((hc_x)+1)*8-1-n;
	for(uint8_t i = 0; i < (hc_max - hc_x-1);i++)
	{
		if(i != (hc_max - hc_x-1))
			temp_arr[i] = 0; // Gửi các dummy byte
	}
	temp_arr[hc_max - hc_x-1] =(1<<pos_in_hc);
	HC595_Send_Data(temp_arr,hc_max);
	HAL_Delay(1000);
	temp_arr[hc_max - hc_x-1] =(0<<pos_in_hc);
	HC595_Send_Data(temp_arr,hc_max);
	HAL_Delay(1000);
	return HAL_OK;
}

void HC595_TestOutput()
{
	HC595_Send_Data(0x55);
	HAL_Delay(2000);
	HC595_Send_Data(0x55<<1);
	HAL_Delay(2000);
}
#endif

