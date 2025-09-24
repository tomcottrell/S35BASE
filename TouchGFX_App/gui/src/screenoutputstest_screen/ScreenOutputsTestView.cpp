#include <gui/screenoutputstest_screen/ScreenOutputsTestView.hpp>

#include <stdbool.h>

extern bool Output1_control;
extern bool Output2_control;

ScreenOutputsTestView::ScreenOutputsTestView() :
	buttonHandlerObj(10, static_cast<Button*>(&button0),
						 static_cast<Button*>(&button1),
						 static_cast<Button*>(&button2),
						 static_cast<Button*>(&button3)) {
}

void ScreenOutputsTestView::setupScreen()
{
    ScreenOutputsTestViewBase::setupScreen();
}

void ScreenOutputsTestView::tearDownScreen()
{
    ScreenOutputsTestViewBase::tearDownScreen();
}


/*
 * Virtual Action Handlers
 */
void ScreenOutputsTestView::buttonHandler( uint8_t value ) {
  buttonHandlerObj.handleKey(value);

  switch (value)
  	  {
      case 0:
    	  //button 0: Exit
    	  application().gotoScreenTestHomeScreenNoTransition();
    	  break;
      case 1:
    	  //button 1: Toggle output 1 on/off
    	  Output1_control = (1 - Output1_control);
    	  break;
      case 2:
    	  //button 2: Toggle output 2 on/off
    	  Output2_control = (1 - Output2_control);
    	  break;
      case 3:
    	  //button 3:
    	  break;
      default:
    	  break;
    }

  this->invalidate();

}

void ScreenOutputsTestView::handleTickEvent()
{
	//Output 1 status
	Unicode::snprintf(textOutput1Buffer,TEXTOUTPUT1_SIZE, "%d", Output1_control);

	//Output 2 status
	Unicode::snprintf(textOutput2Buffer,TEXTOUTPUT2_SIZE, "%d", Output2_control);

	this->invalidate();
}
