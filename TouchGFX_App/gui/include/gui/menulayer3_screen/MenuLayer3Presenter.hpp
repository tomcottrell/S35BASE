#ifndef MENULAYER3PRESENTER_HPP
#define MENULAYER3PRESENTER_HPP

#include <gui/model/ModelListener.hpp>
#include <mvp/Presenter.hpp>

using namespace touchgfx;

class MenuLayer3View;
class Model;

class MenuLayer3Presenter : public touchgfx::Presenter, public ModelListener
{
public:
    MenuLayer3Presenter(MenuLayer3View& v);

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

    virtual ~MenuLayer3Presenter() {}

    void bind(Model* m) { model = m; }

    // Add these methods
    MenuSubItem* getCurrentSubItem();
    void saveNumberValue(int32_t value);
    void saveEnumSelection(uint8_t enumIndex);

private:
    MenuLayer3Presenter();

    MenuLayer3View& view;
    Model* model;
};

#endif // MENULAYER3PRESENTER_HPP
