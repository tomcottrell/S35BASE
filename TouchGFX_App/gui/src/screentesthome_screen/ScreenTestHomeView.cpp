#include <gui/screentesthome_screen/ScreenTestHomeView.hpp>
#include <gui/model/Model.hpp>
#include <gui/common/FrontendApplication.hpp>
#include "ConfigMemory.h"
#include <cstring>

// External firmware version string
extern char firmware_version[];
#define VERSION_STRING_LENGTH 24

ScreenTestHomeView::ScreenTestHomeView() :
    buttonHandlerObj(10, static_cast<Button*>(&button0),
                         static_cast<Button*>(&button1),
                         static_cast<Button*>(&button2),
                         static_cast<Button*>(&button3)),
    m_buttonController(static_cast<TouchGfxInputController*>(HAL::getInstance()->getButtonController())),
    showButtonFeedback(false),
    buttonFeedbackStartTime(0),
    lastButtonPressed(0)
{
}

void ScreenTestHomeView::setupScreen()
{
    ScreenTestHomeViewBase::setupScreen();

    // Display firmware version
    Unicode::strncpy(textVersionBuffer, firmware_version, VERSION_STRING_LENGTH);
    textVersion.invalidate();

    // Initialize display state
    initializeDisplayState();
}

void ScreenTestHomeView::tearDownScreen()
{
    ScreenTestHomeViewBase::tearDownScreen();
}

void ScreenTestHomeView::initializeDisplayState()
{
    // Initialize basic system status displays
    updateSystemStatus();

    // Clear any button feedback
    showButtonFeedback = false;
    hideButtonFeedback();
}

void ScreenTestHomeView::updateSystemStatus()
{
    // Show basic system operational status
    // This can be expanded later for IX3212 PDM status

    // Example: Show CAN communication status
    Model& model = static_cast<FrontendApplication*>(&application())->getModel();

    // Update system time display if you have one
    // updateTimeDisplay();

    // Update basic status indicators
    // This is where you'll add IX3212 status later
}

void ScreenTestHomeView::handleTickEvent()
{
    Model& model = static_cast<FrontendApplication*>(&application())->getModel();
    bool displayChanged = false;

    // Update button feedback display
    displayChanged |= updateButtonFeedbackDisplay();

    // Update system status periodically
    static uint32_t lastStatusUpdate = 0;
    if (HAL_GetTick() - lastStatusUpdate > 1000) { // Update every second
        updateSystemStatus();
        lastStatusUpdate = HAL_GetTick();
        displayChanged = true;
    }

    if (displayChanged) {
        this->invalidate();
    }
}

bool ScreenTestHomeView::updateButtonFeedbackDisplay()
{
    bool changed = false;

    if (showButtonFeedback) {
        uint32_t currentTime = HAL_GetTick();

        // Show feedback for 1 second
        if (currentTime - buttonFeedbackStartTime > 1000) {
            showButtonFeedback = false;
            hideButtonFeedback();
            changed = true;
        } else {
            // Update feedback display based on which button was pressed
            showButtonFeedback_impl();
        }
    }

    return changed;
}

void ScreenTestHomeView::showButtonFeedback_impl()
{
    // Simple button feedback - can be customized for your application
    switch (lastButtonPressed) {
        case 1:
            // Button 1 feedback - you can customize this
            // Example: Show "Function 1 Activated" message
            break;
        case 2:
            // Button 2 feedback
            // Example: Show "Function 2 Activated" message
            break;
        case 3:
            // Button 3 feedback
            // Example: Show "Function 3 Activated" message
            break;
    }
}

void ScreenTestHomeView::hideButtonFeedback()
{
    // Hide any button feedback displays
    // Clear feedback text or hide feedback containers
}

/*
 * Button Handler - Simplified for baseline
 */
void ScreenTestHomeView::buttonHandler(uint8_t value)
{
    buttonHandlerObj.handleKey(value);

    switch (value)
    {
        case 0:
            // Button 0: Navigate to menu system
            application().gotoMenuLayer1ScreenNoTransition();
            break;

        case 1:
            // Button 1: Custom function 1
            handleButton1();
            break;

        case 2:
            // Button 2: Custom function 2
            handleButton2();
            break;

        case 3:
            // Button 3: Custom function 3
            handleButton3();
            break;
    }

    this->invalidate();
}

void ScreenTestHomeView::handleButton1()
{
    // Custom function for button 1
    // This is where you'll add your IX3212 control logic

    // Example: Toggle IX3212 output 1
    // IX3212_ToggleOutput(1);

    // Show feedback
    lastButtonPressed = 1;
    showButtonFeedback = true;
    buttonFeedbackStartTime = HAL_GetTick();
}

void ScreenTestHomeView::handleButton2()
{
    // Custom function for button 2
    // This is where you'll add your IX3212 control logic

    // Example: Cycle through IX3212 modes
    // IX3212_CycleModes();

    // Show feedback
    lastButtonPressed = 2;
    showButtonFeedback = true;
    buttonFeedbackStartTime = HAL_GetTick();
}

void ScreenTestHomeView::handleButton3()
{
    // Custom function for button 3
    // This is where you'll add your IX3212 control logic

    // Example: Emergency stop all IX3212 outputs
    // IX3212_EmergencyStop();

    // Show feedback
    lastButtonPressed = 3;
    showButtonFeedback = true;
    buttonFeedbackStartTime = HAL_GetTick();
}

// Optional: Add IX3212-specific display updates (for future use)
void ScreenTestHomeView::updateIX3212Status()
{
    // This function will be implemented when you add IX3212 integration
    // Example:
    // - Show PDM communication status
    // - Display output states
    // - Show input values
    // - Indicate any faults
}

void ScreenTestHomeView::updateTimeDisplay()
{
    // If you have a time display on your screen, update it here
    // This is optional and depends on your GUI design
}

/*
 * Helper Functions
 */
void ScreenTestHomeView::clearAllFeedback()
{
    showButtonFeedback = false;
    hideButtonFeedback();
    this->invalidate();
}
