#include <gui/menulayer2_screen/MenuLayer2View.hpp>

//MenuLayer2View::MenuLayer2View()
//{
//
//}

MenuLayer2View::MenuLayer2View() : selectedIndex(0), totalItems(0), topVisibleIndex(0)
{

}

void MenuLayer2View::setupScreen()
{
    MenuLayer2ViewBase::setupScreen();
    initializeMenuItems(); //Added to existing func
    updateCategoryTitle(); //Added to existing func
    updateMenuDisplay();   //Added to existing func

}

void MenuLayer2View::tearDownScreen() // maybe need to delet
{
    MenuLayer2ViewBase::tearDownScreen();
}

void MenuLayer2View::initializeMenuItems()
{
    MenuCategory* currentCategory = presenter->getCurrentCategory();
    if (currentCategory) {
        totalItems = currentCategory->subItemCount;

        for (uint8_t i = 0; i < MAX_VISIBLE_ITEMS && i < totalItems; i++) {
            menuItems[i].setXY(16, 40 + (i * 40));
            menuItems[i].setVisible(true);
            add(menuItems[i]);
        }
    }
}

void MenuLayer2View::updateCategoryTitle()
{
    MenuCategory* currentCategory = presenter->getCurrentCategory();
    if (currentCategory) {
        //CategoryTitle.setWildcard(currentCategory->name);

    	static touchgfx::Unicode::UnicodeChar categoryBuffer[100]; // Adjust size as needed
    	touchgfx::Unicode::strncpy(categoryBuffer, currentCategory->name, 99);
    	categoryBuffer[99] = 0; // Null terminate
    	CategoryTitle.setWildcard(categoryBuffer);

    	CategoryTitle.invalidate();
    }
}

void MenuLayer2View::updateMenuDisplay()
{
    MenuCategory* currentCategory = presenter->getCurrentCategory();
    if (!currentCategory) return;

    for (uint8_t i = 0; i < MAX_VISIBLE_ITEMS; i++) {
        uint8_t itemIndex = topVisibleIndex + i;

        if (itemIndex < totalItems) {
            MenuSubItem& subItem = currentCategory->subItems[itemIndex];
            menuItems[i].setMenuText(subItem.name);
            menuItems[i].setHighlighted(itemIndex == selectedIndex);
            menuItems[i].setVisible(true);
        } else {
            menuItems[i].setVisible(false);
        }
    }
    invalidate();
}

void MenuLayer2View::handleUpButton()
{
    if (selectedIndex > 0) {
        selectedIndex--;
        if (selectedIndex < topVisibleIndex) {
            topVisibleIndex = selectedIndex;
        }
        updateMenuDisplay();
    }
}

void MenuLayer2View::handleDownButton()
{
    if (selectedIndex < (totalItems - 1)) {
        selectedIndex++;
        if (selectedIndex >= (topVisibleIndex + MAX_VISIBLE_ITEMS)) {
            topVisibleIndex = selectedIndex - MAX_VISIBLE_ITEMS + 1;
        }
        updateMenuDisplay();
    }
}

void MenuLayer2View::handleEnterButton()
{
    presenter->subMenuItemSelected(selectedIndex);
}

void MenuLayer2View::handleHomeButton()
{
    //static_cast<FrontendApplication*>(Application::getInstance())->gotoMenuLayer1ScreenNoTransition();
}
