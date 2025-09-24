////////////////////////////////////////////////////////////////////////////////////////////////////
// fileName: LCD_heater.c
//
// Description: LCD heater PWM control
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
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stm32l4xx_hal.h>
#include <main.h>
#include <LCD_heater.h>
#include <TickTimers.h>
#include <analogs.h>


////////////////////////////////////////////////////////////////////////////////////////////////////
//Defines
////////////////////////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////////////////////////
// Typedefs
////////////////////////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////////////////////////
// Private variables
////////////////////////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////////////////////////
// Private interface
////////////////////////////////////////////////////////////////////////////////////////////////////
extern TIM_HandleTypeDef htim3;


////////////////////////////////////////////////////////////////////////////////////////////////////
// Public interface
////////////////////////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////
//
// lcd_heater_control()
//
// Controls the LCD heater.  Maximum heater PWM value is de-rated at voltages above 12V:
// Vsupply <= 12V, PWM max 100%
// Vsupply <= 24V, PWM max 50%
// Vsupply <= 36V, PWM max 20%
// Vsupply > 36V, PWM max 10%
//
// Heater output proportional to LCD temperature
//
//    Parameters:
//			none
//
//    Returns:
// 			none
//
////////////////////////////////////////////////////////////////////////
void lcd_heater_control()
{
	float max_pwm_value_for_Vin;
	float pwm_proportional_value;
	static float lcd_heater_pwm_duty;

	if(lcd_heater_timer == 0)
	{
		//Update heater control once per second
		lcd_heater_timer = LCD_CONTROL_PERIOD_MS;

		//Determine maximum possible heater PWM duty depending on supply voltage
		if(SupplyVoltage <= 12)
		{
			//Supply <= 12V: PWM 100% duty available
			max_pwm_value_for_Vin = 100;
		}
		else if(SupplyVoltage <= 24)
		{
			//Supply 12 - 24V: De-rate max PWM duty from 100% to 50%
			max_pwm_value_for_Vin = 24 - SupplyVoltage;
			max_pwm_value_for_Vin /= 12;
			max_pwm_value_for_Vin *= 50;
			max_pwm_value_for_Vin += 50;
		}
		else if(SupplyVoltage <= 36)
		{
			//Supply 24 - 36V: De-rate max PWM duty from 50% to 20%
			max_pwm_value_for_Vin = 36 - SupplyVoltage;
			max_pwm_value_for_Vin /= 12;
			max_pwm_value_for_Vin *= 30;
			max_pwm_value_for_Vin += 20;
		}
		else
		{
			//Supply greater than 36V: use fixed 10% duty
			max_pwm_value_for_Vin = 10;
		}

		//Determine proportional value for heater depending on LCD temperature
		if(LCD_temperature < LCD_HEATER_SET_POINT)
		{
			//LCD temp less than set point: Determine proportional PWM value
			pwm_proportional_value = LCD_HEATER_SET_POINT - LCD_temperature;		//Get temperature 'error'

			if(pwm_proportional_value > LCD_HEATER_PROPORTIONAL_BAND)
				pwm_proportional_value = LCD_HEATER_PROPORTIONAL_BAND;

			pwm_proportional_value /= LCD_HEATER_PROPORTIONAL_BAND;		// calculate 0-1 multiplier
		}
		else
		{
			//LCD temp > set point for heater: heater off
			pwm_proportional_value = 0;
		}

		//Determine PWM value
		lcd_heater_pwm_duty = pwm_proportional_value * max_pwm_value_for_Vin;

		//Convert to 1mS 'ticks' for tick-timer control
		lcd_heater_pwm_duty /= 100;
		lcd_heater_pwm_duty *= LCD_CONTROL_PERIOD_MS;
	}

	//Control heater output
	if(lcd_heater_PWM_timer == 0)
		lcd_heater_PWM_timer = LCD_CONTROL_PERIOD_MS;

	if(((uint16_t)lcd_heater_pwm_duty) > lcd_heater_PWM_timer)
	{
		//Heater ON
		HAL_GPIO_WritePin(LCD_HEATER_GPIO_Port, LCD_HEATER_Pin, 1);
	}
	else
	{
		//Heater off
		HAL_GPIO_WritePin(LCD_HEATER_GPIO_Port, LCD_HEATER_Pin, 0);
	}
}
