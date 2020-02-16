/**
   @copyright (C) 2017 Melexis N.V.
   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at
       http://www.apache.org/licenses/LICENSE-2.0
   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.
*/

#include "MLX90640_I2C_Driver.h"
#include "stm32f7xx_hal.h"

I2C_HandleTypeDef hi2c2;

//Read a number of words from startAddress. Store into Data array.
//Returns 0 if successful, -1 if error
int MLX90640_I2CRead(uint8_t _deviceAddress, unsigned int startAddress, unsigned int nWordsRead, uint16_t *data)
{
	unsigned int j;
	for(j=0; j<nWordsRead; ++j)
	{
		uint8_t buf[2];
		HAL_I2C_Mem_Read(&hi2c2, _deviceAddress, startAddress, 2, buf, 2, 100);
		startAddress += 1;
		data[j] = (buf[0] << 8 | buf[1]);
	}
	return (0); //Success
}

//Write two bytes to a two byte address
int MLX90640_I2CWrite(uint8_t _deviceAddress, unsigned int writeAddress, uint16_t data)
{
	uint8_t DATA[2] = {data & 0xFF, data >> 8};
	HAL_I2C_Mem_Write(&hi2c2, _deviceAddress, writeAddress, 2, DATA, 2, 100);

	uint16_t dataCheck;
	MLX90640_I2CRead(_deviceAddress, writeAddress, 1, &dataCheck);
	if (dataCheck != data)
	{
		return -2;
	}
	return (0); //Success
}
