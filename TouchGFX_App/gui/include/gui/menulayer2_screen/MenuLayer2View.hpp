#ifndef MENULAYER2VIEW_HPP
#define MENULAYER2VIEW_HPP

#include <gui_generated/menulayer2_screen/MenuLayer2ViewBase.hpp>
#include <gui/menulayer2_screen/MenuLayer2Presenter.hpp>
#include <gui/containers/MenuItemContainer.hpp>

class MenuLayer2View : public MenuLayer2ViewBase
{
public:
    MenuLayer2View();
    virtual ~MenuLayer2View() {}
    virtual void setupScreen();
    virtual void tearDownScreen();

    void handleUpButton();
    void handleDownButton();
    void handleEnterButton();
    void handleHomeButton();

    uint8_t getSelectedIndex() const { return selectedIndex; }
    void updateCategoryTitle();

protected:
    static const uint8_t MAX_VISIBLE_ITEMS = 4;
    MenuItemContainer menuItems[MAX_VISIBLE_ITEMS];

    uint8_t selectedIndex;
    uint8_t totalItems;
    uint8_t topVisibleIndex;

    void updateMenuDisplay();
    void initializeMenuItems();
};

#endif // MENULAYER2VIEW_HPP
