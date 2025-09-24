#ifndef MENULAYER3VIEW_HPP
#define MENULAYER3VIEW_HPP

#include <gui_generated/menulayer3_screen/MenuLayer3ViewBase.hpp>
#include <gui/menulayer3_screen/MenuLayer3Presenter.hpp>
#include <touchgfx/hal/HAL.hpp>
#include "../../../BSP/Inc/touchgfx_input_controller.hpp"

class MenuLayer3View : public MenuLayer3ViewBase
{
public:
    MenuLayer3View();
    virtual ~MenuLayer3View() {}
    virtual void setupScreen();
    virtual void tearDownScreen();
    virtual void handleTickEvent();

    // Button handlers
    virtual void handleUpButton();
    virtual void handleDownButton();
    virtual void handleEnterButton();
    virtual void handleHomeButton();

protected:

private:
    // Timing constants for safety
    static const uint32_t DEBOUNCE_TIME = 50;              // 50ms debounce for holds
    static const uint32_t HOLD_REPEAT_TIME = 100;          // 100ms repeat rate for holds
    static const uint32_t BUTTON_PRESS_DEBOUNCE = 25;      // 25ms debounce for single presses
    static const uint32_t ENTER_BUTTON_DEBOUNCE = 200;     // 200ms debounce for enter/home
    static const uint32_t MIN_TICK_INTERVAL = 10;          // Minimum 10ms between ticks
    static const uint32_t MIN_INVALIDATE_INTERVAL = 50;    // Minimum 50ms between invalidates

    // Menu item handling
    void initializeCurrentItem();
    void updateDisplay();
    void updateValueDisplay();
    void updateDescriptionAndUnits();
    void updateEnumDisplay();
    void requestRedraw();

    // Button hold processing
    void processUpButtonHold();
    void processDownButtonHold();

    // Member variables
    MenuSubItem* currentItem;

    // Temporary values (not saved until Enter pressed)
    int32_t tempValue;
    uint8_t tempEnumIndex;

    // Button hold functionality
    TouchGfxInputController* m_buttonController;
    uint32_t lastHoldTime;
    int32_t holdIncrement;

    // Anti-freeze protection
    bool needsRedraw;
    uint32_t lastButtonChangeTime;
    bool processingTick;
    uint32_t lastTickTime;           // Track tick timing
    uint32_t tickCount;              // Monitor tick frequency

    // Text buffers for display
    touchgfx::Unicode::UnicodeChar nameBuffer[50];
    touchgfx::Unicode::UnicodeChar numberValueBuffer[10];
    touchgfx::Unicode::UnicodeChar minValueBuffer[15];
    touchgfx::Unicode::UnicodeChar maxValueBuffer[15];
    touchgfx::Unicode::UnicodeChar enumBuffer[50];
    touchgfx::Unicode::UnicodeChar descBuffer[100];    // For description
    touchgfx::Unicode::UnicodeChar unitsBuffer[20];    // For units
};

#endif // MENULAYER3VIEW_HPP
