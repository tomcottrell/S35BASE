#ifndef SCREENTESTHOMEVIEW_HPP
#define SCREENTESTHOMEVIEW_HPP

#include <gui_generated/screentesthome_screen/ScreenTestHomeViewBase.hpp>
#include <gui/screentesthome_screen/ScreenTestHomePresenter.hpp>

//#include "button_handler.hpp"
#include "../../../BSP/Inc/button_handler.hpp"
#include <touchgfx/hal/HAL.hpp>
#include "../../../BSP/Inc/touchgfx_input_controller.hpp"
#include "touchgfx/Color.hpp"

// Buffer size defines
#define TESTTEXT_SIZE 32
#define TEXTROLL_SIZE 16
#define PTOSTATUSTXT_SIZE 16
#define TEXTPTO_SIZE 8

// Forward declaration
class Model;

class ScreenTestHomeView : public ScreenTestHomeViewBase
{
public:
    ScreenTestHomeView();
    virtual ~ScreenTestHomeView() {}
    virtual void setupScreen();
    virtual void tearDownScreen();

    virtual void buttonHandler(uint8_t value);
    virtual void handleTickEvent();

protected:
    ButtonHandler buttonHandlerObj;
    TouchGfxInputController* m_buttonController;
    bool deviceOn; // State variable stored directly in the View
    bool showButtonFeedback;
    uint32_t buttonFeedbackStartTime;
    // Add these members for button feedback
    uint8_t lastButtonPressed;

    void updateButtonFeedback();

private:
    // UI update methods
    void updateButtonStates();
    bool latchToggleState;  // Track latch toggle state
    bool updateTruckStatusDisplay(const Model& model);  // was: void
    bool updateRollDisplay(const Model& model);         // was: void
    bool updatePTODisplay(const Model& model);          // was: void
    void initializeDisplayState();
    void forceUpdateTruckStatusDisplay(const Model& model);
    bool isButtonHeld;
    bool updateButtonFeedbackDisplay(Model& model);
    void forceUpdateRollDisplay(const Model& model);
    void forceUpdatePTODisplay(const Model& model);
    bool updateLatchDisplay();
    bool updateAlarmDisplay(Model& model);
    void initializeAlarmDisplay();
    void navigateBasedOnVehicleType();
    void initializeLatchDisplay();

    // Member variables
    uint16_t previousBitmapId;
};

#endif // SCREENTESTHOMEVIEW_HPP
