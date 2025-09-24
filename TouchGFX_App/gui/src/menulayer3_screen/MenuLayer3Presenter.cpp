#include <gui/menulayer3_screen/MenuLayer3View.hpp>
#include <gui/menulayer3_screen/MenuLayer3Presenter.hpp>

MenuLayer3Presenter::MenuLayer3Presenter(MenuLayer3View& v)
    : view(v)
{

}

void MenuLayer3Presenter::activate()
{

}

void MenuLayer3Presenter::deactivate()
{

}

MenuSubItem* MenuLayer3Presenter::getCurrentSubItem()
{
    return model->getCurrentSubItem();
}

void MenuLayer3Presenter::saveNumberValue(int32_t value)
{
    model->saveCurrentValue(value);
}

void MenuLayer3Presenter::saveEnumSelection(uint8_t enumIndex)
{
    model->saveCurrentEnumSelection(enumIndex);
}
