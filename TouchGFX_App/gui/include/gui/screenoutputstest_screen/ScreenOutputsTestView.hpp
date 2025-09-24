#ifndef SCREENOUTPUTSTESTVIEW_HPP
#define SCREENOUTPUTSTESTVIEW_HPP

#include <gui_generated/screenoutputstest_screen/ScreenOutputsTestViewBase.hpp>
#include <gui/screenoutputstest_screen/ScreenOutputsTestPresenter.hpp>
#include "button_handler.hpp"

class ScreenOutputsTestView : public ScreenOutputsTestViewBase
{
public:
    ScreenOutputsTestView();
    virtual ~ScreenOutputsTestView() {}
    virtual void setupScreen();
    virtual void tearDownScreen();


    virtual void buttonHandler(uint8_t value);
    virtual void handleTickEvent();
protected:
    ButtonHandler buttonHandlerObj;
};

#endif // SCREENOUTPUTSTESTVIEW_HPP
