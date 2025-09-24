#include <gui/menulayer1_screen/MenuLayer1View.hpp>

//MenuLayer1View::MenuLayer1View()
//{
//
//}

//MenuLayer1View::MenuLayer1View() :
//	buttonHandlerObj(10, static_cast<Button*>(&button0),
//						 static_cast<Button*>(&button1),
//						 static_cast<Button*>(&button2),
//						 static_cast<Button*>(&button3)) {
//}

MenuLayer1View::MenuLayer1View() : buttonHandlerObj(10, static_cast<Button*>(&button0),
		 static_cast<Button*>(&button1),
		 static_cast<Button*>(&button2),
		 static_cast<Button*>(&button3)) ,selectedIndex(0), totalItems(0), topVisibleIndex(0)
{

}

void MenuLayer1View::setupScreen()
{
    MenuLayer1ViewBase::setupScreen();
    initializeMenuItems(); //added to existing func
    updateMenuDisplay();   //added to existing func
}

void MenuLayer1View::tearDownScreen()
{
    MenuLayer1ViewBase::tearDownScreen();
}

void MenuLayer1View::buttonHandler( uint8_t value ) {
  buttonHandlerObj.handleKey(value);

  switch (value)
  	  {
      case 0:
    	  //button 0: Exit to home screen
    	  application().gotoScreenTestHomeScreenNoTransition();
    	  break;
      case 1:
    	  //button 1
    	  handleUpButton();
    	  break;
      case 2:
    	  //button 2
    	  handleDownButton();
    	  break;
      case 3:
    	  handleEnterButton();
    	  //button 3
    	  break;
      default:
    	  break;
    }

  this->invalidate();
}


void MenuLayer1View::initializeMenuItems()
{
    totalItems = presenter->getCategoryCount();

    for (uint8_t i = 0; i < MAX_VISIBLE_ITEMS && i < totalItems; i++) {
        menuItems[i].setXY(16, 40 + (i * 40)); // Use setXY instead of setPosition
        menuItems[i].setVisible(true);
        add(menuItems[i]);
    }
}

void MenuLayer1View::updateMenuDisplay()
{
    MenuCategory* categories = presenter->getMenuCategories();

    for (uint8_t i = 0; i < MAX_VISIBLE_ITEMS; i++) {
        uint8_t itemIndex = topVisibleIndex + i;

        if (itemIndex < totalItems) {
            menuItems[i].setMenuText(categories[itemIndex].name);
        	//if (itemIndex == 0) menuItems[i].setMenuText("System");
        	//else if (itemIndex == 1) menuItems[i].setMenuText("Display");
        	//else if (itemIndex == 2) menuItems[i].setMenuText("Network");

        	menuItems[i].setHighlighted(itemIndex == selectedIndex);
            menuItems[i].setVisible(true);
        } else {
            menuItems[i].setVisible(false);
        }
    }
    invalidate();
}

void MenuLayer1View::handleUpButton()
{
    if (selectedIndex > 0) {
        selectedIndex--;

        // Scroll up if necessary
        if (selectedIndex < topVisibleIndex) {
            topVisibleIndex = selectedIndex;
        }

        updateMenuDisplay();
    }
}

void MenuLayer1View::handleDownButton()
{
    if (selectedIndex < (totalItems - 1)) {
        selectedIndex++;

        // Scroll down if necessary
        if (selectedIndex >= (topVisibleIndex + MAX_VISIBLE_ITEMS)) {
            topVisibleIndex = selectedIndex - MAX_VISIBLE_ITEMS + 1;
        }

        updateMenuDisplay();
    }
}

void MenuLayer1View::handleEnterButton()
{
    presenter->menuItemSelected(selectedIndex);
}

void MenuLayer1View::handleHomeButton()
{
    static_cast<FrontendApplication*>(Application::getInstance())->gotoHomeScreen();
}
