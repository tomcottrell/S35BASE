#include <gui/screenlcdtest_screen/ScreenLCDtestView.hpp>

ScreenLCDtestView::ScreenLCDtestView() :
		buttonHandlerObj(10, static_cast<Button*>(&button0),
		 	 	 	 	 	 static_cast<Button*>(&button1),
							 static_cast<Button*>(&button2),
							 static_cast<Button*>(&button3)),
		m_buttonController(static_cast<TouchGfxInputController*>(HAL::getInstance()->getButtonController() ))
{
}

extern uint8_t LCD_brightness;

void ScreenLCDtestView::setupScreen()
{
    ScreenLCDtestViewBase::setupScreen();
}

void ScreenLCDtestView::tearDownScreen()
{
    ScreenLCDtestViewBase::tearDownScreen();
}

/*
 * Virtual Action Handlers
 */
void ScreenLCDtestView::buttonHandler( uint8_t value ) {
  buttonHandlerObj.handleKey(value);

  switch (value)
      {
    	  case 0:
    		  //Button 0: Goto home screen
    		  if(LCD_brightness == 0)
    			  LCD_brightness = 1;
    		  application().gotoScreenTestHomeScreenNoTransition();
    		  break;
    	  case 1:
    		  //Button 1: Increase LCD brightness (single press)
    		  if(LCD_brightness < 100)
    			  LCD_brightness += 1;
    		  break;
    	  case 2:
    		  //Button 1: Decrease LCD brightness (single press)
    		  if(LCD_brightness > 0)
    			  LCD_brightness -= 1;
    		  break;
    	  case 3:
    		  break;
      }

  this->invalidate();
}

void ScreenLCDtestView::handleTickEvent()
{
	//LCD brightness inc/dec (button held)
	if(m_buttonController->isButtonHeld(1))
	{
		//Increase brightness
		if(LCD_brightness < 100)
			++LCD_brightness;
	}
	else if(m_buttonController->isButtonHeld(2))
	{
		//Decrease brightness
		if(LCD_brightness > 0)
			--LCD_brightness;
	}

	Unicode::snprintf(textValueBrightnessBuffer,TEXTVALUEBRIGHTNESS_SIZE, "%d", LCD_brightness);

	this->invalidate();
}
