#include <gui/model/Model.hpp>
#include <gui/model/ModelListener.hpp>
#include <touchgfx/Unicode.hpp>
#include "ConfigMemory.h"
#include "nonVolStorage.h"
#include <images/BitmapDatabase.hpp>
#include <cstdlib>
// comment added

// Add external CAN variables
extern uint16_t j1939Roll;
extern uint16_t j1939PTOStatus;
extern uint16_t j1939Truck_Latched;
extern uint16_t j1939Truck_Lowered;
extern uint16_t j1939Truck_Moving;
extern uint16_t j1939Truck_Raised;
extern uint16_t j1939PTO_Disabled;
extern uint16_t j1939EStop_Active;
extern uint16_t j1939Angle_Exceeded;
extern uint16_t j1939Truck_Press;
extern uint16_t j1939Trailer_Latched;
extern uint16_t j1939Trailer_Lowered;
extern uint16_t j1939Trailer_Moving;
extern uint16_t j1939Trailer_Raised;
extern uint16_t j1939Trailer_Press;
extern uint16_t j1939Overpressure;  // Fixed missing semicolon and added this
extern uint8_t LCD_brightness;
extern bool Output1_control;
extern volatile uint8_t can_data_updated;

// HAL function for timing
extern "C" uint32_t HAL_GetTick(void);

// Define enum options
static const char* TypeModeOptions[] = {"Truck", "Truck and Trailer"};

// Update the MenuSubItem structures to include description and units
static MenuSubItem SystemSubItems[] = {
{"Vehicle Type", ENUM_TYPE, 0, 0, 0, TypeModeOptions, 2, 0, "Select vehicle configuration", ""},
{"Brightness", NUMBER_TYPE, 75, 10, 100, nullptr, 0, 0, "Adjust display brightness", "%"}
};

static MenuSubItem SensorSubItems[] = {
{"Truck PTO Pressure MAX", NUMBER_TYPE, 400, 0, 1000, nullptr, 0, 0, "High Range of the Truck\n Hydraulic Pressure Sensor", "bar"},
{"Truck PTO Pressure MIN", NUMBER_TYPE, 0, 0, 100, nullptr, 0, 0, "Low Range of the Truck\n Hydraulic Pressure Sensor", "bar"},
{"Trailer PTO Pressure MAX", NUMBER_TYPE, 400, 0, 1000, nullptr, 0, 0, "High Range of the Trailer\n Hydraulic Pressure Sensor", "bar"},
{"Trailer PTO Pressure MIN", NUMBER_TYPE, 0, 0, 100, nullptr, 0, 0, "Low Range of the Trailer\n Hydraulic Pressure Sensor", "bar"}
};

// UPDATED: Removed Emergency Set Points (items 4 and 8)
static MenuSubItem PressureSubItems[] = {
{"Angle Alarm Set Point", NUMBER_TYPE, 15, 0, 64, nullptr, 0, 0, "Angle threshold for alarm activation\n raise function will be disabled \n if reached", "deg"},
{"Truck Lowered Set Point", NUMBER_TYPE, 50, 0, 400, nullptr, 0, 0, "PTO pressure threshold\n when truck is lowered", "bar"},
{"Truck Raised Set Point", NUMBER_TYPE, 280, 0, 400, nullptr, 0, 0, "PTO pressure threshold\n when truck is raised", "bar"},
{"Truck Overload Set Point", NUMBER_TYPE, 380, 0, 400, nullptr, 0, 0, "PTO pressure threshold for \n truck warning raise function will \n be disabled if reached", "bar"},
// REMOVED: {"Truck Emergency Set Point", NUMBER_TYPE, 395, 0, 400, nullptr, 0, 0, "PTO shutdown pressure threshold for truck", "bar"},
{"Trailer Lowered Set Point", NUMBER_TYPE, 50, 0, 400, nullptr, 0, 0, "PTO pressure threshold\n when trailer is lowered", "bar"},
{"Trailer Raised Set Point", NUMBER_TYPE, 280, 0, 400, nullptr, 0, 0, "PTO pressure threshold\n when trailer is raised", "bar"},
{"Trailer Overload Set Point", NUMBER_TYPE, 380, 0, 400, nullptr, 0, 0, "PTO pressure threshold for \n truck warning raise function will \n be disabled if reached", "bar"},
// REMOVED: {"Trailer Emergency Set Point", NUMBER_TYPE, 395, 0, 400, nullptr, 0, 0, "PTO shutdown pressure threshold for trailer", "bar"},
};

static MenuSubItem TimerSubItems[] = {
{"Angle Bubble Timer", NUMBER_TYPE, 5, 0, 100, nullptr, 0, 0, "Time over the alarm set point before\n the angle alarm is activated", "sec"},
{"PTO Pressure Bubble Timer", NUMBER_TYPE, 5, 0, 100, nullptr, 0, 0, "Time over the alarm set point before the\n PTO hydraulic pressure alarm is activated", "sec"},
};

Model::Model() : modelListener(0), currentLayer1Selection(0), currentLayer2Selection(0), settingsLoaded(false),
                 lastButtonPressed(0), buttonFeedbackActive(false), buttonFeedbackTimer(0)
{
    initializeMenuData();
}

void Model::initializeMenuData()
{
    menuCategories[0] = {"Display Settings", SystemSubItems, 2};
    menuCategories[1] = {"Sensor Calibration", SensorSubItems, 4};
    menuCategories[2] = {"Warnings and Alarms", PressureSubItems, 7};  // UPDATED: Changed from 9 to 7 items
    menuCategories[3] = {"Timers", TimerSubItems, 2};
}


MenuSubItem* Model::getCurrentSubItem()
{
    if (currentLayer1Selection < CATEGORY_COUNT) {
        MenuCategory& category = menuCategories[currentLayer1Selection];
        if (currentLayer2Selection < category.subItemCount) {
            return &category.subItems[currentLayer2Selection];
        }
    }
    return nullptr;
}

void Model::saveCurrentValue(int32_t value)
{
    MenuSubItem* item = getCurrentSubItem();
    if (item && item->type == NUMBER_TYPE) {
        item->currentValue = value;
        syncMenuItemsToConfig();
        saveSettingsToFile();
    }
}

void Model::saveCurrentEnumSelection(uint8_t enumIndex)
{
    MenuSubItem* item = getCurrentSubItem();
    if (item && item->type == ENUM_TYPE) {
        item->currentEnumIndex = enumIndex;
        syncMenuItemsToConfig();
        saveSettingsToFile();
    }
}

void Model::tick()
{
    if (!can_data_updated) return;
    can_data_updated = 0;

    if (!settingsLoaded) {
        loadSettingsFromStorage();
        settingsLoaded = true;
    }

    //if(buttonFeedbackActive)
    //{
    //    if(HAL_GetTick() - buttonFeedbackTimer > 3000)
    //    {
    //        buttonFeedbackActive = false;
    //        stateChanged = true;
     //   }
    //
}

// UPDATED: Removed PTO_DISABLED, added OVERPRESSURE alarm


// UPDATED: Removed PTO_DISABLED check, added OVERPRESSURE check

void Model::updateFlashingStates()
{
    uint32_t currentTime = HAL_GetTick();

    if(currentTime - lastFlashTime >= 1000)
    {
        flashState = !flashState;
        lastFlashTime = currentTime;

        if (currentTruckState == TRUCK_STATE_MOVING)
        {
            stateChanged = true;
        }
    }
}


bool Model::getFlashState() const
{
    return flashState;
}

// UPDATED: Removed emergency setpoint mappings
void Model::syncMenuItemsToConfig()
{
    // System settings
    S35_config[TOUCHGFX_VEHICLETYPE] = SystemSubItems[0].currentEnumIndex;
    S35_config[TOUCHGFX_BRIGHTNESS] = SystemSubItems[1].currentValue;
    LCD_brightness = SystemSubItems[1].currentValue;

    // Sensor settings
    S35_config[TOUCHGFX_TRUCKMAX] = SensorSubItems[0].currentValue;
    S35_config[TOUCHGFX_TRUCKMIN] = SensorSubItems[1].currentValue;
    S35_config[TOUCHGFX_TRAILERMAX] = SensorSubItems[2].currentValue;
    S35_config[TOUCHGFX_TRAILERMIN] = SensorSubItems[3].currentValue;

    // Pressure settings (UPDATED: Removed emergency setpoints)
    S35_config[TOUCHGFX_ANGLE] = PressureSubItems[0].currentValue;
    S35_config[TOUCHGFX_TRUCK_LOWERED] = PressureSubItems[1].currentValue;
    S35_config[TOUCHGFX_TRUCK_RAISED] = PressureSubItems[2].currentValue;
    S35_config[TOUCHGFX_TRUCK_OVERLOAD] = PressureSubItems[3].currentValue;
    // REMOVED: S35_config[TOUCHGFX_TRUCK_EMERGENCY] = PressureSubItems[4].currentValue;
    S35_config[TOUCHGFX_TRAILER_LOWERED] = PressureSubItems[4].currentValue;  // Index adjusted
    S35_config[TOUCHGFX_TRAILER_RAISED] = PressureSubItems[5].currentValue;   // Index adjusted
    S35_config[TOUCHGFX_TRAILER_OVERLOAD] = PressureSubItems[6].currentValue; // Index adjusted
    // REMOVED: S35_config[TOUCHGFX_TRAILER_EMERGENCY] = PressureSubItems[8].currentValue;

    // Timer settings
    S35_config[TOUCHGFX_INCLO_BUBBLE] = TimerSubItems[0].currentValue;
    S35_config[TOUCHGFX_PRESSURE_BUBBLE] = TimerSubItems[1].currentValue;
}

void Model::loadSettingsFromStorage()
{
    LoadConfigFile();
    loadSettingsFromConfig();
}

void Model::saveSettingsToFile()
{
    SaveConfigFile();
}

// UPDATED: Removed emergency setpoint loading
void Model::loadSettingsFromConfig()
{
    // System settings - with bounds checking
    SystemSubItems[0].currentEnumIndex = constrain(S35_config[TOUCHGFX_VEHICLETYPE], 0, 1);
    SystemSubItems[1].currentValue = constrain(S35_config[TOUCHGFX_BRIGHTNESS], 10, 100);
    LCD_brightness = constrain(S35_config[TOUCHGFX_BRIGHTNESS], 10, 100);

    // Sensor settings - with bounds checking
    SensorSubItems[0].currentValue = constrain(S35_config[TOUCHGFX_TRUCKMAX], 0, 1000);
    SensorSubItems[1].currentValue = constrain(S35_config[TOUCHGFX_TRUCKMIN], 0, 100);
    SensorSubItems[2].currentValue = constrain(S35_config[TOUCHGFX_TRAILERMAX], 0, 1000);
    SensorSubItems[3].currentValue = constrain(S35_config[TOUCHGFX_TRAILERMIN], 0, 100);

    // Pressure settings - with bounds checking (UPDATED: Removed emergency setpoints)
    PressureSubItems[0].currentValue = constrain(S35_config[TOUCHGFX_ANGLE], 0, 64);
    PressureSubItems[1].currentValue = constrain(S35_config[TOUCHGFX_TRUCK_LOWERED], 0, 400);
    PressureSubItems[2].currentValue = constrain(S35_config[TOUCHGFX_TRUCK_RAISED], 0, 400);
    PressureSubItems[3].currentValue = constrain(S35_config[TOUCHGFX_TRUCK_OVERLOAD], 0, 400);
    // REMOVED: PressureSubItems[4].currentValue = constrain(S35_config[TOUCHGFX_TRUCK_EMERGENCY], 0, 400);
    PressureSubItems[4].currentValue = constrain(S35_config[TOUCHGFX_TRAILER_LOWERED], 0, 400);  // Index adjusted
    PressureSubItems[5].currentValue = constrain(S35_config[TOUCHGFX_TRAILER_RAISED], 0, 400);   // Index adjusted
    PressureSubItems[6].currentValue = constrain(S35_config[TOUCHGFX_TRAILER_OVERLOAD], 0, 400); // Index adjusted
    // REMOVED: PressureSubItems[8].currentValue = constrain(S35_config[TOUCHGFX_TRAILER_EMERGENCY], 0, 400);

    // Timer settings - with bounds checking
    TimerSubItems[0].currentValue = constrain(S35_config[TOUCHGFX_INCLO_BUBBLE], 0, 100);
    TimerSubItems[1].currentValue = constrain(S35_config[TOUCHGFX_PRESSURE_BUBBLE], 0, 100);
}

void Model::bind(void* presenter)
{
}

int32_t Model::constrain(int32_t value, int32_t minVal, int32_t maxVal)
{
    if (value < minVal) return minVal;
    if (value > maxVal) return maxVal;
    return value;
}


// Add this method to Model.cpp after the existing getButtonFeedbackMessage method

// NEW: Method to get latch status text for OPEN/CLOSE display

