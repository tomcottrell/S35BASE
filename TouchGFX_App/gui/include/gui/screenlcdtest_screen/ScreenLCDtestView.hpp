#ifndef SCREENLCDTESTVIEW_HPP
#define SCREENLCDTESTVIEW_HPP

#include <gui_generated/screenlcdtest_screen/ScreenLCDtestViewBase.hpp>
#include <gui/screenlcdtest_screen/ScreenLCDtestPresenter.hpp>
//#include "button_handler.hpp"

#include "../../../BSP/Inc/button_handler.hpp"
#include <touchgfx/hal/HAL.hpp>
#include "../../../BSP/Inc/touchgfx_input_controller.hpp"
#include "touchgfx/Color.hpp"


class ScreenLCDtestView : public ScreenLCDtestViewBase
{
public:
    ScreenLCDtestView();
    virtual ~ScreenLCDtestView() {}
    virtual void setupScreen();
    virtual void tearDownScreen();

    virtual void buttonHandler(uint8_t value);
    virtual void handleTickEvent();
protected:
    ButtonHandler buttonHandlerObj;
    TouchGfxInputController* m_buttonController;
};

#endif // SCREENLCDTESTVIEW_HPP
