////////////////////////////////////////////////////////////////////////////////////////////////////
// fileName: xxxxxxxxx.h
//
// Description: Configuration memeory map
//
// Author: Enovation controls
//
//         http://www.enovationcontrols.com
//
// copyright Copyright 2024 by Enovation Controls.
//           This file and/or parts of it are subject to
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
#include <stdint.h>
#include <ConfigMemory.h>


////////////////////////////////////////////////////////////////////////////////////////////////////
// S35 CONFIGURATION MEMORY MAP
////////////////////////////////////////////////////////////////////////////////////////////////////
//Add new entries here and in ConfigMemory.h and update CONFIG_LENGTH constant
uint16_t S35_config[CONFIG_LENGTH];

const uint16_t LCD_BRIGHTNESS = 0;
const uint16_t FOUR_UP_SCREEN1_GAUGE1_SPN = 1;
const uint16_t FOUR_UP_SCREEN1_GAUGE2_SPN = 2;
const uint16_t FOUR_UP_SCREEN1_GAUGE3_SPN = 3;
const uint16_t FOUR_UP_SCREEN1_GAUGE4_SPN = 4;
const uint16_t FOUR_UP_SCREEN2_GAUGE1_SPN = 5;
const uint16_t FOUR_UP_SCREEN2_GAUGE2_SPN = 6;
const uint16_t FOUR_UP_SCREEN2_GAUGE3_SPN = 7;
const uint16_t FOUR_UP_SCREEN2_GAUGE4_SPN = 8;
const uint16_t TSC1_SCREEN_IDLE_SPEED = 9;
const uint16_t TSC1_SCREEN_PRESET1_SPEED = 10;
const uint16_t TSC1_SCREEN_PRESET2_SPEED = 11;
const uint16_t REGEN_SCREEN_GAUGE1_SPN = 12;
const uint16_t REGEN_SCREEN_GAUGE2_SPN = 13;
const uint16_t REGEN_SCREEN_GAUGE3_SPN = 14;
const uint16_t REGEN_SCREEN_GAUGE4_SPN = 15;
const uint16_t TOUCHGFX_VEHICLETYPE = 16;
const uint16_t TOUCHGFX_BRIGHTNESS = 17;
const uint16_t TOUCHGFX_TRUCKMAX = 18;
const uint16_t TOUCHGFX_TRUCKMIN = 19;
const uint16_t TOUCHGFX_TRAILERMAX = 20;
const uint16_t TOUCHGFX_TRAILERMIN = 21;
const uint16_t TOUCHGFX_TRUCK_LOWERED = 22;
const uint16_t TOUCHGFX_TRUCK_RAISED = 23;
const uint16_t TOUCHGFX_TRUCK_OVERLOAD = 24;
const uint16_t TOUCHGFX_TRUCK_EMERGENCY = 25;
const uint16_t TOUCHGFX_TRAILER_LOWERED = 26;
const uint16_t TOUCHGFX_TRAILER_RAISED = 27;
const uint16_t TOUCHGFX_TRAILER_OVERLOAD = 28;
const uint16_t TOUCHGFX_TRAILER_EMERGENCY = 29;
const uint16_t TOUCHGFX_ANGLE = 30;
const uint16_t TOUCHGFX_INCLO_BUBBLE = 31;
const uint16_t TOUCHGFX_PRESSURE_BUBBLE = 32;

////////////////////////////////////////////////////////////////////////
//
// SetDefaultConfig()
//
// Updates all config variables with their default values
//
//    Parameters:
//       none
//
////////////////////////////////////////////////////////////////////////
void SetDefaultConfig()
{
	//DISPLAY MENU
	S35_config[LCD_BRIGHTNESS] = 80;				//LCD backlight brightness (%)

	//CUSTOMIZE INTERFACE MENU
	//four up screen 1 gauge setup
	S35_config[FOUR_UP_SCREEN1_GAUGE1_SPN] = 100;	//SPN 100 engine Oil pressure
	S35_config[FOUR_UP_SCREEN1_GAUGE2_SPN] = 110;	//SPN 110 Coolant temperature
	S35_config[FOUR_UP_SCREEN1_GAUGE3_SPN] = 92;	//SPN 92 engine load
	S35_config[FOUR_UP_SCREEN1_GAUGE4_SPN] = 3719;	//SPN 3917 DPF soot level
	//four up screen 2 gauge setup
	S35_config[FOUR_UP_SCREEN2_GAUGE1_SPN] = 98;	//SPN 98 engine Oil level
	S35_config[FOUR_UP_SCREEN2_GAUGE2_SPN] = 167;	//SPN 167 charging system voltage
	S35_config[FOUR_UP_SCREEN2_GAUGE3_SPN] = 173;	//SPN 173 engine exhaust temperature
	S35_config[FOUR_UP_SCREEN2_GAUGE4_SPN] = 247;	//SPN 247 engine hours

	//TO BE ADDED TO MENUS
	//Speed control screen
	S35_config[TSC1_SCREEN_IDLE_SPEED] = 800;
	S35_config[TSC1_SCREEN_PRESET1_SPEED] = 1200;
	S35_config[TSC1_SCREEN_PRESET2_SPEED] = 2000;
	//Regen screen gauge setup
	S35_config[REGEN_SCREEN_GAUGE1_SPN] = 3719;		//SPN 3917 DPF soot level
	S35_config[REGEN_SCREEN_GAUGE2_SPN] = 110;		//SPN 110 Coolant temperature
	S35_config[REGEN_SCREEN_GAUGE3_SPN] = 92;		//SPN 92 engine load
	S35_config[REGEN_SCREEN_GAUGE4_SPN] = 190;		//SPN 190 engine speed

    // ADD THESE NEW TOUCHGFX DEFAULTS:
    S35_config[TOUCHGFX_VEHICLETYPE] = 0;           // Default to Truck (index 0)
    S35_config[TOUCHGFX_BRIGHTNESS] = 75;        // Default to Stereo (index 0)
    S35_config[TOUCHGFX_TRUCKMAX] = 400;          // Default timeout 30 seconds
    S35_config[TOUCHGFX_TRUCKMIN] = 0;       // Default brightness 75%
    S35_config[TOUCHGFX_TRAILERMAX] = 400;      // Default to Normal (index 0)
    S35_config[TOUCHGFX_TRAILERMIN] = 0;         // Default contrast 60%
    S35_config[TOUCHGFX_TRUCK_LOWERED] = 50;
    S35_config[TOUCHGFX_TRUCK_RAISED] = 280;
    S35_config[TOUCHGFX_TRUCK_OVERLOAD] = 380;
    S35_config[TOUCHGFX_TRUCK_EMERGENCY] = 395;
    S35_config[TOUCHGFX_TRAILER_LOWERED] = 50;
    S35_config[TOUCHGFX_TRAILER_RAISED] = 280;
    S35_config[TOUCHGFX_TRAILER_OVERLOAD] = 380;
    S35_config[TOUCHGFX_TRAILER_EMERGENCY] = 395;
    S35_config[TOUCHGFX_ANGLE] = 15;
    S35_config[TOUCHGFX_INCLO_BUBBLE] = 5;
    S35_config[TOUCHGFX_PRESSURE_BUBBLE] = 5;
}

////////////////////////////////////////////////////////////////////////
//
// UpdateAllConfigFeatures()
//
// Update all features affected by updated configuration data
//
// Updates all config variables with their default values
//
//    Parameters:
//       none
//
////////////////////////////////////////////////////////////////////////
void UpdateAllConfigFeatures()
{


}
