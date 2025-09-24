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
    previousBitmapId = newBitmapId;

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
        statusImage.invalidate();
        previousBitmapId = newBitmapId;
        changed = true;
    }

    // Check if visibility states changed
    bool currentUnlocked = UNLOCKED.isVisible();

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



