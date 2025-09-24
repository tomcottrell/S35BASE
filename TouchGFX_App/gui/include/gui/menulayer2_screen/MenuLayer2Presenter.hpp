#ifndef MENULAYER2PRESENTER_HPP
#define MENULAYER2PRESENTER_HPP

#include <gui/model/ModelListener.hpp>
#include <mvp/Presenter.hpp>

using namespace touchgfx;

class MenuLayer2View;
class Model;

class MenuLayer2Presenter : public touchgfx::Presenter, public ModelListener
{
public:
    MenuLayer2Presenter(MenuLayer2View& v);

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

    virtual ~MenuLayer2Presenter() {}

    void bind(Model* m) { model = m; }

    // Add these methods
    void subMenuItemSelected(uint8_t index);
    MenuCategory* getCurrentCategory();

private:
    MenuLayer2Presenter();

    MenuLayer2View& view;
    Model* model;
};

#endif // MENULAYER2PRESENTER_HPP
