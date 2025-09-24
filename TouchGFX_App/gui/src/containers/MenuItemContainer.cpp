#include <gui/containers/MenuItemContainer.hpp>
#include <touchgfx/Color.hpp>

MenuItemContainer::MenuItemContainer()
{
	// Initialize the buffer
	textBuffer[0] = 0;
}

void MenuItemContainer::initialize()
{
    MenuItemContainerBase::initialize();
}

void MenuItemContainer::setMenuText(const char* text)
{
    //menuItemText.setWildcard(text);

	touchgfx::Unicode::strncpy(textBuffer, text, 99);
	textBuffer[99] = 0; // Null terminate
	menuItemText.setWildcard(textBuffer);
	//static touchgfx::Unicode::UnicodeChar textBuffer[100]; // Adjust size as needed
	//touchgfx::Unicode::strncpy(textBuffer, text, 99);
	//textBuffer[99] = 0; // Null terminate
	//menuItemText.setWildcard(textBuffer);

    menuItemText.invalidate();
}

void MenuItemContainer::setHighlighted(bool highlighted)
{
    if (highlighted) {
        // Set blue background with black border
        menuItemBackground.setColor(touchgfx::Color::getColorFromRGB(40, 105, 184)); // Blue background
        menuItemBackground.setBorderColor(touchgfx::Color::getColorFromRGB(0, 0, 0)); // Black border
        menuItemText.setColor(touchgfx::Color::getColorFromRGB(255, 255, 255)); // White text

    } else {
        // Normal state
        menuItemBackground.setColor(touchgfx::Color::getColorFromRGB(180,195,214)); // Light gray
        menuItemBackground.setBorderColor(touchgfx::Color::getColorFromRGB(180,195,214)); // Hide border by matching background
        menuItemText.setColor(touchgfx::Color::getColorFromRGB(0, 0, 0)); // Black text
    }

    // Invalidate elements
    menuItemBackground.invalidate();
    menuItemText.invalidate();
}
