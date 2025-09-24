#ifndef SCREENTRAILERVIEW_HPP
#define SCREENTRAILERVIEW_HPP

#include <gui_generated/screentrailer_screen/ScreenTrailerViewBase.hpp>
#include <gui/screentrailer_screen/ScreenTrailerPresenter.hpp>

//#include "button_handler.hpp"
#include "../../../BSP/Inc/button_handler.hpp"
#include <touchgfx/hal/HAL.hpp>
#include "../../../BSP/Inc/touchgfx_input_controller.hpp"
#include "touchgfx/Color.hpp"

// Buffer size defines
//#define TESTTEXT_SIZE 32
//#define TEXTROLL_SIZE 16
//#define PTOSTATUSTXT_SIZE 16
//#define TEXTPTO_SIZE 8
//#define LATCH_SIZE 10

class Model; // Forward declaration

class ScreenTrailerView : public ScreenTrailerViewBase
{
public:
    ScreenTrailerView();
    virtual ~ScreenTrailerView() {}
    virtual void setupScreen();
    virtual void tearDownScreen();
    virtual void handleTickEvent();
    virtual void buttonHandler(uint8_t value);

protected:
    ButtonHandler buttonHandlerObj;
    TouchGfxInputController* m_buttonController;

    // Button feedback state
    bool showButtonFeedback;
    uint32_t buttonFeedbackStartTime;
    uint8_t lastButtonPressed;
    bool isButtonHeld;

    // Display state tracking
    uint16_t previousBitmapId;

private:
    // UI update methods
    void updateButtonStates();
    bool updateTrailerStatusDisplay(const Model& model);
    bool updateRollDisplay(const Model& model);
    bool updatePTODisplay(const Model& model);
    bool updateButtonFeedbackDisplay(Model& model);
    bool updateAlarmDisplay(Model& model);
    bool updateLatchDisplay();
    bool trailerLatchToggleState;

    // Initialization methods
    void initializeDisplayState();
    void forceUpdateTrailerStatusDisplay(const Model& model);
    void forceUpdateRollDisplay(const Model& model);
    void forceUpdatePTODisplay(const Model& model);
    void initializeAlarmDisplay();
    void initializeLatchDisplay();
};

#endif // SCREENTRAILERVIEW_HPP
