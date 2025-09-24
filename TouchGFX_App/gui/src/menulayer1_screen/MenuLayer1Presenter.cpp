#include <gui/menulayer1_screen/MenuLayer1View.hpp>
#include <gui/menulayer1_screen/MenuLayer1Presenter.hpp>

MenuLayer1Presenter::MenuLayer1Presenter(MenuLayer1View& v)
    : view(v)
{

}

void MenuLayer1Presenter::activate()
{

}

void MenuLayer1Presenter::deactivate()
{

}

void MenuLayer1Presenter::menuItemSelected(uint8_t index)
{
    model->setCurrentLayer1Selection(index);
    static_cast<FrontendApplication*>(Application::getInstance())->gotoMenuLayer2ScreenNoTransition();
}

uint8_t MenuLayer1Presenter::getCategoryCount()
{
    return model->getCategoryCount();
}

MenuCategory* MenuLayer1Presenter::getMenuCategories()
{
    return model->getMenuCategories();
}
