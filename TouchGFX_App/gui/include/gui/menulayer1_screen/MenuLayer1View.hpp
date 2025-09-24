#ifndef MENULAYER1VIEW_HPP
#define MENULAYER1VIEW_HPP

#include <gui_generated/menulayer1_screen/MenuLayer1ViewBase.hpp>
#include <gui/menulayer1_screen/MenuLayer1Presenter.hpp>
#include <gui/containers/MenuItemContainer.hpp>
#include "button_handler.hpp"

class MenuLayer1View : public MenuLayer1ViewBase
{
public:
    MenuLayer1View();
    virtual ~MenuLayer1View() {}
    virtual void setupScreen();
    virtual void tearDownScreen();

    // Button handlers
    virtual void buttonHandler(uint8_t value);
    virtual void handleUpButton();
    void handleDownButton();
    void handleEnterButton();
    void handleHomeButton();

    uint8_t getSelectedIndex() const { return selectedIndex; }

protected:
    ButtonHandler buttonHandlerObj;
    static const uint8_t MAX_VISIBLE_ITEMS = 4;
    MenuItemContainer menuItems[MAX_VISIBLE_ITEMS];

    uint8_t selectedIndex;
    uint8_t totalItems;
    uint8_t topVisibleIndex;

    void updateMenuDisplay();
    void initializeMenuItems();
};

#endif // MENULAYER1VIEW_HPP
