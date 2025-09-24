////////////////////////////////////////////////////////////////////////////////////////////////////
// fileName: Analogs.c
//
// Description: Analogue input measurements
//
// Author: Enovation controls
//
//         http://www.enovationcontrols.com
//
// copyright Copyright 2024 by Enovation Controls.
//            This file and/or parts of it are subject to
//            software license terms.
//
//            Without written approval of Enovation Controls this
//            software may not be copied, redistributed or used
//            in any other way.
//
////////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////////////
// Includes
////////////////////////////////////////////////////////////////////////////////////////////////////
#include <stm32l4xx_hal.h>
#include <stdbool.h>
#include <stdint.h>
#include <analogs.h>
#include <main.h>


////////////////////////////////////////////////////////////////////////////////////////////////////
//Defines
////////////////////////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////////////////////////
// Typedefs
////////////////////////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////////////////////////
// Private variables
////////////////////////////////////////////////////////////////////////////////////////////////////
uint8_t analog_config[2];
float SupplyVoltage;
float LCD_temperature;
float analogInput1value;
float analogInput2value;

////////////////////////////////////////////////////////////////////////////////////////////////////
// Private interface
////////////////////////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////////////////////////
// Public interface
////////////////////////////////////////////////////////////////////////////////////////////////////



///////////////////////////////////
// Setup analogue input hardware //
///////////////////////////////////
void setup_input_hardware()
{
	//Analog input #1
	switch(analog_config[0])
	{
		case ANALOG_CONFIG_RESISTIVE:
			HAL_GPIO_WritePin(ENABLE_RESISTIVE_AIN1_GPIO_Port, ENABLE_RESISTIVE_AIN1_Pin, 1);		//Pull-up resistor ON
			HAL_GPIO_WritePin(ENABLE_CURRENT_AIN1_GPIO_Port, ENABLE_CURRENT_AIN1_Pin, 0);			//Pull-down resistor OFF
			break;
		case ANALOG_CONFIG_VOLTAGE:
			HAL_GPIO_WritePin(ENABLE_RESISTIVE_AIN1_GPIO_Port, ENABLE_RESISTIVE_AIN1_Pin, 0);		//Pull-up resistor OFF
			HAL_GPIO_WritePin(ENABLE_CURRENT_AIN1_GPIO_Port, ENABLE_CURRENT_AIN1_Pin, 0);			//Pull-down resistor OFF
			break;
		case ANALOG_CONFIG_4_to_20mA:
			HAL_GPIO_WritePin(ENABLE_RESISTIVE_AIN1_GPIO_Port, ENABLE_RESISTIVE_AIN1_Pin, 0);		//Pull-up resistor OFF
			HAL_GPIO_WritePin(ENABLE_CURRENT_AIN1_GPIO_Port, ENABLE_CURRENT_AIN1_Pin, 1);			//Pull-down resistor ON
			break;
		default:
			//Set to 0-5V range by default
			HAL_GPIO_WritePin(ENABLE_RESISTIVE_AIN1_GPIO_Port, ENABLE_RESISTIVE_AIN1_Pin, 0);		//Pull-up resistor OFF
			HAL_GPIO_WritePin(ENABLE_CURRENT_AIN1_GPIO_Port, ENABLE_CURRENT_AIN1_Pin, 0);			//Pull-down resistor OFF
			break;
	}

	//Analog input #2
	switch(analog_config[1])
	{
		case ANALOG_CONFIG_RESISTIVE:
			HAL_GPIO_WritePin(ENABLE_RESISTIVE_AIN2_GPIO_Port, ENABLE_RESISTIVE_AIN2_Pin, 1);		//Pull-up resistor ON
			HAL_GPIO_WritePin(ENABLE_CURRENT_AIN2_GPIO_Port, ENABLE_CURRENT_AIN2_Pin, 0);			//Pull-down resistor OFF
			break;
		case ANALOG_CONFIG_VOLTAGE:
			HAL_GPIO_WritePin(ENABLE_RESISTIVE_AIN2_GPIO_Port, ENABLE_RESISTIVE_AIN2_Pin, 0);		//Pull-up resistor OFF
			HAL_GPIO_WritePin(ENABLE_CURRENT_AIN2_GPIO_Port, ENABLE_CURRENT_AIN2_Pin, 0);			//Pull-down resistor OFF
			break;
		case ANALOG_CONFIG_4_to_20mA:
			HAL_GPIO_WritePin(ENABLE_RESISTIVE_AIN2_GPIO_Port, ENABLE_RESISTIVE_AIN2_Pin, 0);		//Pull-up resistor OFF
			HAL_GPIO_WritePin(ENABLE_CURRENT_AIN2_GPIO_Port, ENABLE_CURRENT_AIN2_Pin, 1);			//Pull-down resistor ON
			break;
		default:
			//Set to 0-32V range by default
			HAL_GPIO_WritePin(ENABLE_RESISTIVE_AIN2_GPIO_Port, ENABLE_RESISTIVE_AIN2_Pin, 0);		//Pull-up resistor OFF
			HAL_GPIO_WritePin(ENABLE_CURRENT_AIN2_GPIO_Port, ENABLE_CURRENT_AIN2_Pin, 0);			//Pull-down resistor OFF
			break;
	}
}



