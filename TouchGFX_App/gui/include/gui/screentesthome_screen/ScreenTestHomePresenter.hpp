#ifndef SCREENTESTHOMEPRESENTER_HPP
#define SCREENTESTHOMEPRESENTER_HPP

#include <gui/model/ModelListener.hpp>
#include <mvp/Presenter.hpp>

using namespace touchgfx;

class ScreenTestHomeView;
class Model;

class ScreenTestHomePresenter : public touchgfx::Presenter, public ModelListener
{
public:
    ScreenTestHomePresenter(ScreenTestHomeView& v);

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

    virtual ~ScreenTestHomePresenter() {}

    void bind(Model* m) { model = m; }

private:
    ScreenTestHomePresenter();

    ScreenTestHomeView& view;
    Model* model;
};

#endif // SCREENTESTHOMEPRESENTER_HPP
