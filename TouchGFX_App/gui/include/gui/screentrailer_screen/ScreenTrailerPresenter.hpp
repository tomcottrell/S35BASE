#ifndef SCREENTRAILERPRESENTER_HPP
#define SCREENTRAILERPRESENTER_HPP

#include <gui/model/ModelListener.hpp>
#include <mvp/Presenter.hpp>

using namespace touchgfx;

class ScreenTrailerView;
class Model;

class ScreenTrailerPresenter : public touchgfx::Presenter, public ModelListener
{
public:
    ScreenTrailerPresenter(ScreenTrailerView& v);

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

    virtual ~ScreenTrailerPresenter() {}

    void bind(Model* m) { model = m; }

private:
    ScreenTrailerPresenter();

    ScreenTrailerView& view;
    Model* model;
};

#endif // SCREENTRAILERPRESENTER_HPP
