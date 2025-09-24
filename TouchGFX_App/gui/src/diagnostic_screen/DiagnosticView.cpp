#include <gui/diagnostic_screen/DiagnosticView.hpp>
#include "ConfigMemory.h" // For S35_config access
#include <cstring> // For strcmp

extern uint16_t j1939Angle_Exceeded;
extern uint16_t j1939Truck_Press;
extern uint16_t j1939PTOStatus;
extern uint16_t j1939EStopStatus;
extern uint16_t j1939Roll;
extern uint16_t j1939TruckProximity;
extern uint16_t j1939TrailerProximity;
extern uint16_t j1939Trailer_Press;

DiagnosticView::DiagnosticView()
{
}

void DiagnosticView::setupScreen()
{
    DiagnosticViewBase::setupScreen();

    // Initialize display with current values
    initializeDisplayState();
}

void DiagnosticView::tearDownScreen()
{
    DiagnosticViewBase::tearDownScreen();
}

void DiagnosticView::initializeDisplayState()
{
    // Force update all diagnostic displays with current values
    forceUpdateDiagnosticDisplays();
}

void DiagnosticView::handleTickEvent()
{
    bool displayChanged = updateDiagnosticDisplays();

    if (displayChanged) {
        this->invalidate(); // Only invalidate if something changed
    }
}

void DiagnosticView::forceUpdateDiagnosticDisplays()
{
    // Get current vehicle type from config
    uint8_t currentVehicleType = S35_config[TOUCHGFX_VEHICLETYPE];

    // Roll (Inclinometer) - handle edge case for 65535
    if (j1939Roll == 0xFFFF || j1939Roll == 65535)
    {
        Unicode::snprintf(InclotxtBuffer, INCLOTXT_SIZE, "----");
    }
    else
    {
        Unicode::snprintf(InclotxtBuffer, INCLOTXT_SIZE, "%d Deg", (j1939Roll - 64));
    }

    // Truck Hydraulic Pressure
    if (j1939Truck_Press == 0xFFFF || j1939Truck_Press == 65535)
    {
        Unicode::snprintf(HydPresstxtBuffer, HYDPRESSTXT_SIZE, "0 bar");
    }
    else
    {
        Unicode::snprintf(HydPresstxtBuffer, HYDPRESSTXT_SIZE, "%d bar", j1939Truck_Press);
    }

    // Trailer Hydraulic Pressure (show NaN if Truck-only mode)
    if (currentVehicleType == 0) // Truck only
    {
        Unicode::snprintf(HydPresstxt_trailerBuffer, HYDPRESSTXT_TRAILER_SIZE, "NaN");
    }
    else if (j1939Trailer_Press == 0xFFFF || j1939Trailer_Press == 65535)
    {
        Unicode::snprintf(HydPresstxt_trailerBuffer, HYDPRESSTXT_TRAILER_SIZE, "0 bar");
    }
    else
    {
        Unicode::snprintf(HydPresstxt_trailerBuffer, HYDPRESSTXT_TRAILER_SIZE, "%d bar", j1939Trailer_Press);
    }

    // Truck Latch/Proximity - show LATCHED/UNLATCHED
    if (j1939TruckProximity == 0xFFFF || j1939TruckProximity == 65535)
    {
        Unicode::strncpy(LatchtxtBuffer, "OPEN", LATCHTXT_SIZE);
    }
    else if (j1939TruckProximity > 0)
    {
        Unicode::strncpy(LatchtxtBuffer, "LATCHED", LATCHTXT_SIZE);
    }
    else
    {
        Unicode::strncpy(LatchtxtBuffer, "OPEN", LATCHTXT_SIZE);
    }

    // Trailer Latch/Proximity (show NaN if Truck-only mode)
    if (currentVehicleType == 0) // Truck only
    {
        Unicode::strncpy(Latchtxt_trailerBuffer, "NaN", LATCHTXT_TRAILER_SIZE);
    }
    else if (j1939TrailerProximity == 0xFFFF || j1939TrailerProximity == 65535)
    {
        Unicode::strncpy(Latchtxt_trailerBuffer, "OPEN", LATCHTXT_TRAILER_SIZE);
    }
    else if (j1939TrailerProximity > 0)
    {
        Unicode::strncpy(Latchtxt_trailerBuffer, "LATCHED", LATCHTXT_TRAILER_SIZE);
    }
    else
    {
        Unicode::strncpy(Latchtxt_trailerBuffer, "OPEN", LATCHTXT_TRAILER_SIZE);
    }

    // E-Stop Status - show ACTIVE/INACTIVE
    if (j1939EStopStatus == 0xFFFF || j1939EStopStatus == 65535)
    {
        Unicode::strncpy(EStoptxtBuffer, "INACTIVE", ESTOPTXT_SIZE);
    }
    else if (j1939EStopStatus > 0)
    {
        Unicode::strncpy(EStoptxtBuffer, "ACTIVE", ESTOPTXT_SIZE);
    }
    else
    {
        Unicode::strncpy(EStoptxtBuffer, "INACTIVE", ESTOPTXT_SIZE);
    }
}

bool DiagnosticView::updateDiagnosticDisplays()
{
    bool changed = false;

    // Static variables to track previous values
    static uint16_t lastRoll = 0xFFFF;
    static uint16_t lastTruckPress = 0xFFFF;
    static uint16_t lastTrailerPress = 0xFFFF;
    static uint16_t lastTruckProximity = 0xFFFF;
    static uint16_t lastTrailerProximity = 0xFFFF;
    static uint16_t lastEStopStatus = 0xFFFF;
    static uint8_t lastVehicleType = 0xFF;
    static bool firstRun = true;

    // Get current vehicle type from config
    uint8_t currentVehicleType = S35_config[TOUCHGFX_VEHICLETYPE];

    // Check if vehicle type changed (affects trailer display)
    if (currentVehicleType != lastVehicleType || firstRun)
    {
        lastVehicleType = currentVehicleType;
        changed = true;
    }

    // Roll (Inclinometer) - only update if changed
    if (j1939Roll != lastRoll || firstRun)
    {
        if (j1939Roll == 0xFFFF || j1939Roll == 65535)
        {
            Unicode::snprintf(InclotxtBuffer, INCLOTXT_SIZE, "----");
        }
        else
        {
            Unicode::snprintf(InclotxtBuffer, INCLOTXT_SIZE, "%d Deg", (j1939Roll - 64));
        }
        lastRoll = j1939Roll;
        changed = true;
    }

    // Truck Hydraulic Pressure - only update if changed
    if (j1939Truck_Press != lastTruckPress || firstRun)
    {
        if (j1939Truck_Press == 0xFFFF || j1939Truck_Press == 65535)
        {
            Unicode::snprintf(HydPresstxtBuffer, HYDPRESSTXT_SIZE, "0 bar");
        }
        else
        {
            Unicode::snprintf(HydPresstxtBuffer, HYDPRESSTXT_SIZE, "%d bar", j1939Truck_Press);
        }
        lastTruckPress = j1939Truck_Press;
        changed = true;
    }

    // Trailer Hydraulic Pressure - only update if changed or vehicle type changed
    if (j1939Trailer_Press != lastTrailerPress || currentVehicleType != lastVehicleType || firstRun)
    {
        if (currentVehicleType == 0) // Truck only
        {
            Unicode::snprintf(HydPresstxt_trailerBuffer, HYDPRESSTXT_TRAILER_SIZE, "NaN");
        }
        else if (j1939Trailer_Press == 0xFFFF || j1939Trailer_Press == 65535)
        {
            Unicode::snprintf(HydPresstxt_trailerBuffer, HYDPRESSTXT_TRAILER_SIZE, "0 bar");
        }
        else
        {
            Unicode::snprintf(HydPresstxt_trailerBuffer, HYDPRESSTXT_TRAILER_SIZE, "%d bar", j1939Trailer_Press);
        }
        lastTrailerPress = j1939Trailer_Press;
        changed = true;
    }

    // Truck Latch/Proximity - only update if changed
    if (j1939TruckProximity != lastTruckProximity || firstRun)
    {
        if (j1939TruckProximity == 0xFFFF || j1939TruckProximity == 65535)
        {
            Unicode::strncpy(LatchtxtBuffer, "OPEN", LATCHTXT_SIZE);
        }
        else if (j1939TruckProximity > 0)
        {
            Unicode::strncpy(LatchtxtBuffer, "LATCHED", LATCHTXT_SIZE);
        }
        else
        {
            Unicode::strncpy(LatchtxtBuffer, "OPEN", LATCHTXT_SIZE);
        }
        lastTruckProximity = j1939TruckProximity;
        changed = true;
    }

    // Trailer Latch/Proximity - only update if changed or vehicle type changed
    if (j1939TrailerProximity != lastTrailerProximity || currentVehicleType != lastVehicleType || firstRun)
    {
        if (currentVehicleType == 0) // Truck only
        {
            Unicode::strncpy(Latchtxt_trailerBuffer, "NaN", LATCHTXT_TRAILER_SIZE);
        }
        else if (j1939TrailerProximity == 0xFFFF || j1939TrailerProximity == 65535)
        {
            Unicode::strncpy(Latchtxt_trailerBuffer, "OPEN", LATCHTXT_TRAILER_SIZE);
        }
        else if (j1939TrailerProximity > 0)
        {
            Unicode::strncpy(Latchtxt_trailerBuffer, "LATCHED", LATCHTXT_TRAILER_SIZE);
        }
        else
        {
            Unicode::strncpy(Latchtxt_trailerBuffer, "OPEN", LATCHTXT_TRAILER_SIZE);
        }
        lastTrailerProximity = j1939TrailerProximity;
        changed = true;
    }

    // E-Stop Status - only update if changed
    if (j1939EStopStatus != lastEStopStatus || firstRun)
    {
        if (j1939EStopStatus == 0xFFFF || j1939EStopStatus == 65535)
        {
            Unicode::strncpy(EStoptxtBuffer, "INACTIVE", ESTOPTXT_SIZE);
        }
        else if (j1939EStopStatus > 0)
        {
            Unicode::strncpy(EStoptxtBuffer, "ACTIVE", ESTOPTXT_SIZE);
        }
        else
        {
            Unicode::strncpy(EStoptxtBuffer, "INACTIVE", ESTOPTXT_SIZE);
        }
        lastEStopStatus = j1939EStopStatus;
        changed = true;
    }

    firstRun = false;
    return changed;
}
