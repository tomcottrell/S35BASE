#ifndef MENUITEMCONTAINER_HPP
#define MENUITEMCONTAINER_HPP

#include <gui_generated/containers/MenuItemContainerBase.hpp>
#include <touchgfx/Unicode.hpp>

class MenuItemContainer : public MenuItemContainerBase
{
public:
    MenuItemContainer();
    virtual ~MenuItemContainer() {}

    virtual void initialize();
    void setMenuText(const char* text);
    void setHighlighted(bool highlighted);

protected:
    touchgfx::Unicode::UnicodeChar textBuffer[100]; // Individual buffer per instance
};

#endif // MENUITEMCONTAINER_HPP
