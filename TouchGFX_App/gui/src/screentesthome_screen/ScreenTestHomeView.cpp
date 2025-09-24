#include <gui/screentesthome_screen/ScreenTestHomeView.hpp>
#include <images/BitmapDatabase.hpp>
#include <gui/model/Model.hpp>
#include <gui/common/FrontendApplication.hpp>
#include <cstdlib>  // For abs() function
#include "ConfigMemory.h"
#include "nonVolStorage.h"
#include <gui/common/FrontendApplication.hpp>
#include <cstring>

// External button state variables for CAN transmission
extern uint8_t Raisetest;
extern uint8_t Lowertest;
extern uint8_t Latchtest;
extern uint8_t Traystate;

ScreenTestHomeView::ScreenTestHomeView() :
    buttonHandlerObj(10, static_cast<Button*>(&button0),
                         static_cast<Button*>(&button1),
                         static_cast<Button*>(&button2),
                         static_cast<Button*>(&button3)),
    m_buttonController(static_cast<TouchGfxInputController*>(HAL::getInstance()->getButtonController())),
    showButtonFeedback(false),
    buttonFeedbackStartTime(0),
    lastButtonPressed(0),
    latchToggleState(false),
	isButtonHeld(false)
{
}

extern char firmware_version[];
#define VERSION_STRING_LENGTH		24

void ScreenTestHomeView::setupScreen()
{
    ScreenTestHomeViewBase::setupScreen();
    Unicode::strncpy(textVersionBuffer, firmware_version, VERSION_STRING_LENGTH);

    // Initialize display with current state
    initializeDisplayState();
    // Check current alarm state when entering page
    initializeAlarmDisplay();  // This now checks current state instead of always hiding
    // Initialize LATCH display
    initializeLatchDisplay();
}

// Also add this initialization method to setupScreen():
void ScreenTestHomeView::initializeLatchDisplay()
{
    // Simple initialization based on current Latchtest value
    const char* latchText = (Latchtest == 1) ? "CLOSE" : "OPEN";

    // Update the LATCH text - replace with your actual buffer name
    Unicode::strncpy(LATCHBuffer, latchText, LATCH_SIZE - 1);
    LATCHBuffer[LATCH_SIZE - 1] = 0;
}

void ScreenTestHomeView::initializeAlarmDisplay()
{
    // Get current alarm state from model
    Model& model = static_cast<FrontendApplication*>(&application())->getModel();
    bool currentAlarmState = model.isAlarmActive();

    if(currentAlarmState)
    {
        // Alarm is active - show the popup
        Alarmpopup.setVisible(true);
        ALARM.setVisible(true);

        // Update alarm title and description
        const char* title = model.getAlarmTitle();
        const char* description = model.getAlarmDescription();

        if (title && strlen(title) > 0) {
            Unicode::strncpy(AlarmtitletextBuffer, title, ALARMTITLETEXT_SIZE - 1);
            AlarmtitletextBuffer[ALARMTITLETEXT_SIZE - 1] = 0;
        }

        if (description && strlen(description) > 0) {
            Unicode::strncpy(AlarmdesctextBuffer, description, ALARMDESCTEXT_SIZE - 1);
            AlarmdesctextBuffer[ALARMDESCTEXT_SIZE - 1] = 0;
        }

        Alarmtitletext.setVisible(true);
        Alarmdesctext.setVisible(true);
    }
    else
    {
        // No alarm - hide all alarm elements
        Alarmpopup.setVisible(false);
        Alarmtitletext.setVisible(false);
        Alarmdesctext.setVisible(false);
        ALARM.setVisible(false);
    }
}


void ScreenTestHomeView::tearDownScreen()
{
    ScreenTestHomeViewBase::tearDownScreen();
}

void ScreenTestHomeView::initializeDisplayState()
{
    Model& model = static_cast<FrontendApplication*>(&application())->getModel();

    // Force update all displays on setup (bypass change detection)
    forceUpdateTruckStatusDisplay(model);
    forceUpdateRollDisplay(model);
    forceUpdatePTODisplay(model);

    // Force immediate invalidation to ensure display updates
    this->invalidate();
}

void ScreenTestHomeView::forceUpdateTruckStatusDisplay(const Model& model)
{
    bool showLocked = false;
    bool showUnlocked = false;
    bool showMoving = false;
    uint16_t newBitmapId = 0;

    // Get current truck state first (don't check error state here - alarms handled separately)
    Model::TruckState truckState = model.getTruckState();
    newBitmapId = model.getTruckStateBitmapId();

    switch(truckState)
    {
        case Model::TRUCK_STATE_LATCHED:
            showLocked = true;
            break;
        case Model::TRUCK_STATE_LOWERED:
            showUnlocked = true;
            break;
        case Model::TRUCK_STATE_MOVING:
            showUnlocked = true;
            showMoving = model.getFlashState();
            break;
        case Model::TRUCK_STATE_RAISED:
            showUnlocked = true;
            break;
        default:
            showUnlocked = true;
            break;
    }

    // Apply state immediately
    statusImage.setBitmap(Bitmap(newBitmapId));
    previousBitmapId = newBitmapId;

    UNLOCKED.setVisible(showUnlocked);
    LOCKED.setVisible(showLocked);
    MOVING.setVisible(showMoving);
}

void ScreenTestHomeView::forceUpdateRollDisplay(const Model& model)
{
    if(model.isRollValid())
    {
        int16_t rollValue = model.getRollValue();
        Unicode::snprintf(textRollBuffer, TEXTROLL_SIZE, "%d", rollValue);

        if(rollValue > 0)
        {
            imageProgressRollPos.setValue(rollValue + 64);
            imageProgressRollNeg.setValue(64);
        }
        else if(rollValue < 0)

        {
            imageProgressRollPos.setValue(64);
            imageProgressRollNeg.setValue(abs(rollValue) + 64);
        }
        else
        {
            imageProgressRollPos.setValue(64);
            imageProgressRollNeg.setValue(64);
        }
    }
    else
    {
        Unicode::snprintf(textRollBuffer, TEXTROLL_SIZE, "----");
        imageProgressRollPos.setValue(64);
        imageProgressRollNeg.setValue(64);
    }
}

bool ScreenTestHomeView::updateRollDisplay(const Model& model)
{
    bool changed = false;
    static int16_t lastRollValue = 999;  // Invalid initial value
    static bool lastRollValid = false;

    if(model.isRollValid())
    {
        int16_t rollValue = model.getRollValue();
        if (rollValue != lastRollValue || !lastRollValid)
        {
            Unicode::snprintf(textRollBuffer, TEXTROLL_SIZE, "%d", rollValue);

            if(rollValue > 0)
            {
                imageProgressRollPos.setValue(rollValue + 64);
                imageProgressRollNeg.setValue(64);
            }
            else if(rollValue < 0)
            {
                imageProgressRollPos.setValue(64);
                imageProgressRollNeg.setValue(abs(rollValue) + 64);
            }
            else
            {
                imageProgressRollPos.setValue(64);
                imageProgressRollNeg.setValue(64);
            }

            lastRollValue = rollValue;
            lastRollValid = true;
            changed = true;
        }
    }
    else if (lastRollValid)  // Only update if state changed from valid to invalid
    {
        Unicode::snprintf(textRollBuffer, TEXTROLL_SIZE, "----");
        imageProgressRollPos.setValue(64);
        imageProgressRollNeg.setValue(64);
        lastRollValid = false;
        changed = true;
    }

    return changed;
}

void ScreenTestHomeView::forceUpdatePTODisplay(const Model& model)
{
    if(model.isPTOValid())
    {
        if(model.isPTOActive())
        {
            PTOStatusON.setVisible(true);
            PTOStatusOFF.setVisible(false);
            Unicode::strncpy(PTOSTATUSTXTBuffer, "PTO ON", PTOSTATUSTXT_SIZE);
        }
        else
        {
            PTOStatusON.setVisible(false);
            PTOStatusOFF.setVisible(true);
            Unicode::strncpy(PTOSTATUSTXTBuffer, "PTO OFF", PTOSTATUSTXT_SIZE);
        }
    }
    else
    {
        PTOStatusON.setVisible(false);
        PTOStatusOFF.setVisible(true);
        Unicode::strncpy(PTOSTATUSTXTBuffer, "PTO ---", PTOSTATUSTXT_SIZE);
    }
}

/*
 * Virtual Action Handlers
 */
void ScreenTestHomeView::buttonHandler(uint8_t value)
{
    buttonHandlerObj.handleKey(value);

    switch (value)
    {
        case 0:
            // Button 0: Navigation
            break;

        case 1: // RAISE button TAPPED (show instruction)
        case 2: // LOWER button TAPPED (show instruction)
            lastButtonPressed = value;
            showButtonFeedback = true;
            buttonFeedbackStartTime = HAL_GetTick();
            isButtonHeld = false; // Flag to indicate this is a tap
            break;

        case 3: // LATCH button - toggle behavior
            static bool latchActive = false;
            latchActive = !latchActive;

            Latchtest = latchActive ? 1 : 0;
            Traystate = latchActive ? 3 : 0;

            lastButtonPressed = 3;
            showButtonFeedback = true;
            buttonFeedbackStartTime = HAL_GetTick();
            isButtonHeld = false;
            break;
    }

    this->invalidate();
}

// Update handleTickEvent to include latch display update:
void ScreenTestHomeView::handleTickEvent()
{
    updateButtonStates();

    Model& model = static_cast<FrontendApplication*>(&application())->getModel();
    bool displayChanged = false;

    displayChanged |= updateTruckStatusDisplay(model);
    displayChanged |= updateRollDisplay(model);
    displayChanged |= updatePTODisplay(model);
    displayChanged |= updateButtonFeedbackDisplay(model);
    displayChanged |= updateAlarmDisplay(model);
    displayChanged |= updateLatchDisplay();  // ADD THIS LINE

    if (displayChanged) {
        this->invalidate();
    }
}

void ScreenTestHomeView::updateButtonStates()
{
    // Reset button flags
    Raisetest = 0;
    Lowertest = 0;

    bool wasHeld = isButtonHeld;
    isButtonHeld = false;

    // Check for button hold states
    if(m_buttonController && m_buttonController->isButtonHeld(1))
    {
        Raisetest = 1;
        Traystate = 1;
        isButtonHeld = true;

        // Trigger hold feedback if just started holding or button changed
        if(!wasHeld || lastButtonPressed != 1)
        {
            lastButtonPressed = 1;
            showButtonFeedback = true;
            buttonFeedbackStartTime = HAL_GetTick();
        }
    }
    else if(m_buttonController && m_buttonController->isButtonHeld(2))
    {
        Lowertest = 1;
        Traystate = 2;
        isButtonHeld = true;

        if(!wasHeld || lastButtonPressed != 2)
        {
            lastButtonPressed = 2;
            showButtonFeedback = true;
            buttonFeedbackStartTime = HAL_GetTick();
        }
    }
    else if(m_buttonController && m_buttonController->isButtonHeld(3))
    {
        Latchtest = 1;
        Traystate = 3;
        // Latch doesn't use hold behavior
    }
    else
    {
        // No buttons held
        if(Latchtest == 0) // Only clear tray state if latch is not active
        {
            Traystate = 0;
        }
    }
}

bool ScreenTestHomeView::updateTruckStatusDisplay(const Model& model)
{
    bool changed = false;
    bool showLocked = false;
    bool showUnlocked = false;
    bool showMoving = false;
    uint16_t newBitmapId = 0;

    // Remove error condition handling - alarms handled separately now
    Model::TruckState truckState = model.getTruckState();
    newBitmapId = model.getTruckStateBitmapId();

    switch(truckState)
    {
        case Model::TRUCK_STATE_LATCHED:
            showLocked = true;
            showUnlocked = false;
            showMoving = false;
            break;

        case Model::TRUCK_STATE_LOWERED:
        case Model::TRUCK_STATE_MOVING:
        case Model::TRUCK_STATE_RAISED:
        case Model::TRUCK_STATE_UNKNOWN:
        default:
            showLocked = false;
            showUnlocked = true;
            showMoving = (truckState == Model::TRUCK_STATE_MOVING) ? model.getFlashState() : false;
            break;
    }

    // Update bitmap if changed
    if(newBitmapId != previousBitmapId)
    {
        statusImage.setBitmap(Bitmap(newBitmapId));
        statusImage.invalidate();
        previousBitmapId = newBitmapId;
        changed = true;
    }

    // Check if visibility states changed
    bool currentUnlocked = UNLOCKED.isVisible();
    bool currentLocked = LOCKED.isVisible();
    bool currentMoving = MOVING.isVisible();

    if (currentUnlocked != showUnlocked ||
        currentLocked != showLocked ||
        currentMoving != showMoving) {
        changed = true;
    }

    // Update UI elements
    UNLOCKED.setVisible(showUnlocked);
    LOCKED.setVisible(showLocked);
    MOVING.setVisible(showMoving);
    // Remove ALARM.setVisible() from here - handled in updateAlarmDisplay

    return changed;
}

bool ScreenTestHomeView::updatePTODisplay(const Model& model)
{
    bool changed = false;
    static bool lastPTOActive = false;
    static bool lastPTOValid = false;
    static bool firstRun = true;

    if(model.isPTOValid())
    {
        bool currentPTOActive = model.isPTOActive();

        // Check if PTO state changed or if this is the first run
        if (currentPTOActive != lastPTOActive || !lastPTOValid || firstRun)
        {
            if(currentPTOActive)
            {
                PTOStatusON.setVisible(true);
                PTOStatusOFF.setVisible(false);
                Unicode::strncpy(PTOSTATUSTXTBuffer, "PTO ON", PTOSTATUSTXT_SIZE);
            }
            else
            {
                PTOStatusON.setVisible(false);
                PTOStatusOFF.setVisible(true);
                Unicode::strncpy(PTOSTATUSTXTBuffer, "PTO OFF", PTOSTATUSTXT_SIZE);
            }

            lastPTOActive = currentPTOActive;
            lastPTOValid = true;
            changed = true;
        }
    }
    else // PTO data invalid
    {
        // Only update if state changed from valid to invalid
        if (lastPTOValid || firstRun)
        {
            PTOStatusON.setVisible(false);
            PTOStatusOFF.setVisible(true);
            Unicode::strncpy(PTOSTATUSTXTBuffer, "PTO ---", PTOSTATUSTXT_SIZE);

            lastPTOValid = false;
            changed = true;
        }
    }

    firstRun = false;
    return changed;
}

void ScreenTestHomeView::navigateBasedOnVehicleType()
{
    // Check vehicle type from configuration
    // 0 = "Truck", 1 = "Truck/Trailer" (based on TypeModeOptions array in Model.cpp)
    if (S35_config[TOUCHGFX_VEHICLETYPE] == 1) {
        // Truck/Trailer mode - go to ScreenTrailer
        application().gotoScreenTrailerScreenNoTransition();
    } else {
        // Truck only mode - go to Diagnostic Page
        application().gotoDiagnosticScreenNoTransition();
    }
}

bool ScreenTestHomeView::updateButtonFeedbackDisplay(Model& model)
{
    bool changed = false;
    static bool lastShowTestText = false;
    static const char* lastDisplayMessage = "";

    // Handle button feedback timeout (3 seconds)
    if(showButtonFeedback)
    {
        if(HAL_GetTick() - buttonFeedbackStartTime > 3000)
        {
            showButtonFeedback = false;
            lastButtonPressed = 0;
            isButtonHeld = false;
            changed = true;
        }
    }

    // Determine what message to show
    bool showTestText = false;
    const char* displayMessage = "";

    if(showButtonFeedback && lastButtonPressed > 0)
    {
        // Show button feedback message
        displayMessage = model.getButtonFeedbackMessage(lastButtonPressed, isButtonHeld);
        showTestText = (displayMessage[0] != '\0');
    }
    else
    {
        // No button feedback active - show nothing
        displayMessage = "";
        showTestText = false;
    }

    // Only update display if something actually changed
    if(showTestText != lastShowTestText ||
       (showTestText && strcmp(displayMessage, lastDisplayMessage) != 0))
    {
        testtext.setVisible(showTestText);

        if(showTestText && displayMessage[0] != '\0')
        {
            Unicode::strncpy(testtextBuffer, displayMessage, TESTTEXT_SIZE);
        }
        else
        {
            // Clear text buffer only when transitioning from visible to hidden
            if(lastShowTestText && !showTestText)
            {
                testtextBuffer[0] = '\0';
            }
        }

        lastShowTestText = showTestText;
        lastDisplayMessage = displayMessage;
        changed = true;
    }

    return changed;
}

bool ScreenTestHomeView::updateAlarmDisplay(Model& model)
{
    bool changed = false;
    static bool lastAlarmState = false;
    static Model::ErrorState lastErrorState = Model::ERROR_NONE;

    bool currentAlarmState = model.isAlarmActive();
    Model::ErrorState currentErrorState = model.getErrorState();

    // Check if alarm state changed
    if(currentAlarmState != lastAlarmState || currentErrorState != lastErrorState)
    {
        // Update alarm popup visibility
        Alarmpopup.setVisible(currentAlarmState);
        ALARM.setVisible(currentAlarmState);  // ALARM image follows popup state

        if(currentAlarmState)
        {
            // Update alarm title and description using TouchGFX Unicode functions
            const char* title = model.getAlarmTitle();
            const char* description = model.getAlarmDescription();

            // Copy title using Unicode::strncpy
            if (title && strlen(title) > 0) {
                Unicode::strncpy(AlarmtitletextBuffer, title, ALARMTITLETEXT_SIZE - 1);
                AlarmtitletextBuffer[ALARMTITLETEXT_SIZE - 1] = 0; // Ensure null termination
            }

            // Copy description using Unicode::strncpy
            if (description && strlen(description) > 0) {
                Unicode::strncpy(AlarmdesctextBuffer, description, ALARMDESCTEXT_SIZE - 1);
                AlarmdesctextBuffer[ALARMDESCTEXT_SIZE - 1] = 0; // Ensure null termination
            }

            Alarmtitletext.setVisible(true);
            Alarmdesctext.setVisible(true);
        }
        else
        {
            // Hide alarm text elements when no alarm
            Alarmtitletext.setVisible(false);
            Alarmdesctext.setVisible(false);
        }

        lastAlarmState = currentAlarmState;
        lastErrorState = currentErrorState;
        changed = true;
    }

    return changed;
}

bool ScreenTestHomeView::updateLatchDisplay()
{
    bool changed = false;
    static uint8_t lastLatchtest = 0;
    static bool firstRun = true;

    // Check if Latchtest state changed OR first run
    if(Latchtest != lastLatchtest || firstRun)
    {
        // Simple logic: if Latchtest is 1, show "CLOSE", if 0 show "OPEN"
        const char* latchText = (Latchtest == 1) ? "CLOSE" : "OPEN";

        // Update the LATCH text area - check your .hpp file for exact buffer name
        // Common buffer names: LATCHBuffer, textLatchBuffer, latchTextBuffer
        Unicode::strncpy(LATCHBuffer, latchText, LATCH_SIZE - 1);
        LATCHBuffer[LATCH_SIZE - 1] = 0;

        lastLatchtest = Latchtest;
        firstRun = false;
        changed = true;
    }

    return changed;
}



