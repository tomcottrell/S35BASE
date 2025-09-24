#include <gui/menulayer2_screen/MenuLayer2View.hpp>
#include <gui/menulayer2_screen/MenuLayer2Presenter.hpp>

MenuLayer2Presenter::MenuLayer2Presenter(MenuLayer2View& v)
    : view(v)
{

}

void MenuLayer2Presenter::activate()
{

}

void MenuLayer2Presenter::deactivate()
{

}

void MenuLayer2Presenter::subMenuItemSelected(uint8_t index)
{
    model->setCurrentLayer2Selection(index);
    static_cast<FrontendApplication*>(Application::getInstance())->gotoMenuLayer3ScreenNoTransition();
}

MenuCategory* MenuLayer2Presenter::getCurrentCategory()
{
    MenuCategory* categories = model->getMenuCategories();
    uint8_t currentSelection = model->getCurrentLayer1Selection();

    if (currentSelection < model->getCategoryCount()) {
        return &categories[currentSelection];
    }
    return nullptr;
}
