#ifndef MENULAYER1PRESENTER_HPP
#define MENULAYER1PRESENTER_HPP

#include <gui/model/ModelListener.hpp>
#include <mvp/Presenter.hpp>

using namespace touchgfx;

class MenuLayer1View;
class Model;

class MenuLayer1Presenter : public touchgfx::Presenter, public ModelListener
{
public:
    MenuLayer1Presenter(MenuLayer1View& v);

    /**
     * The activate function is called automatically when this screen is "switched in"
     * (ie. made active). Initialization logic can be placed here.
     */
    virtual void activate();

    /**
     * The deactivate function is called automatically when this screen is "switched out"
     * (ie. made inactive). Teardown functionality can be placed here.
     */
    virtual void deactivate();

    virtual ~MenuLayer1Presenter() {}

    void bind(Model* m) { model = m; }

    // Add these methods
    void menuItemSelected(uint8_t index);
    uint8_t getCategoryCount();
    MenuCategory* getMenuCategories();

private:
    MenuLayer1Presenter();

    MenuLayer1View& view;
    Model* model;
};

#endif // MENULAYER1PRESENTER_HPP
