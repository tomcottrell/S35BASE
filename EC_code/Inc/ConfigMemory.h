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

#ifndef CONFIG_MEMORY_H
#define CONFIG_MEMORY_H

////////////////////////////////////////////////////////////////////////////////////////////////////
// Includes
////////////////////////////////////////////////////////////////////////////////////////////////////
#include <stdint.h>

////////////////////////////////////////////////////////////////////////////////////////////////////
// S35 CONFIGURATION MEMORY MAP
////////////////////////////////////////////////////////////////////////////////////////////////////
//Add new entries here and in ConfigMemory.c and update CONFIG_LENGTH constant
extern uint16_t S35_config[];

#define CONFIG_LENGTH 33

extern const uint16_t LCD_BRIGHTNESS;
extern const uint16_t FOUR_UP_SCREEN1_GAUGE1_SPN;
extern const uint16_t FOUR_UP_SCREEN1_GAUGE2_SPN;
extern const uint16_t FOUR_UP_SCREEN1_GAUGE3_SPN;
extern const uint16_t FOUR_UP_SCREEN1_GAUGE4_SPN;
extern const uint16_t FOUR_UP_SCREEN2_GAUGE1_SPN;
extern const uint16_t FOUR_UP_SCREEN2_GAUGE2_SPN;
extern const uint16_t FOUR_UP_SCREEN2_GAUGE3_SPN;
extern const uint16_t FOUR_UP_SCREEN2_GAUGE4_SPN;
extern const uint16_t TSC1_SCREEN_IDLE_SPEED;
extern const uint16_t TSC1_SCREEN_PRESET1_SPEED;
extern const uint16_t TSC1_SCREEN_PRESET2_SPEED;
extern const uint16_t REGEN_SCREEN_GAUGE1_SPN;
extern const uint16_t REGEN_SCREEN_GAUGE2_SPN;
extern const uint16_t REGEN_SCREEN_GAUGE3_SPN;
extern const uint16_t REGEN_SCREEN_GAUGE4_SPN;
extern const uint16_t TOUCHGFX_VEHICLETYPE;
extern const uint16_t TOUCHGFX_BRIGHTNESS;
extern const uint16_t TOUCHGFX_TRUCKMAX;
extern const uint16_t TOUCHGFX_TRUCKMIN;
extern const uint16_t TOUCHGFX_TRAILERMAX;
extern const uint16_t TOUCHGFX_TRAILERMIN;
extern const uint16_t TOUCHGFX_TRUCK_LOWERED;
extern const uint16_t TOUCHGFX_TRUCK_RAISED;
extern const uint16_t TOUCHGFX_TRUCK_OVERLOAD;
extern const uint16_t TOUCHGFX_TRUCK_EMERGENCY;
extern const uint16_t TOUCHGFX_TRAILER_LOWERED;
extern const uint16_t TOUCHGFX_TRAILER_RAISED;
extern const uint16_t TOUCHGFX_TRAILER_OVERLOAD;
extern const uint16_t TOUCHGFX_TRAILER_EMERGENCY;
extern const uint16_t TOUCHGFX_ANGLE;
extern const uint16_t TOUCHGFX_INCLO_BUBBLE;
extern const uint16_t TOUCHGFX_PRESSURE_BUBBLE;

////////////////////////////////////////////////////////////////////////////////////////////////////
// Public interface
////////////////////////////////////////////////////////////////////////////////////////////////////
void SetDefaultConfig();
void UpdateAllConfigFeatures();

#endif
