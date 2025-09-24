#ifndef SCREENLCDTESTPRESENTER_HPP
#define SCREENLCDTESTPRESENTER_HPP

#include <gui/model/ModelListener.hpp>
#include <mvp/Presenter.hpp>

using namespace touchgfx;

class ScreenLCDtestView;
class Model;


class ScreenLCDtestPresenter : public touchgfx::Presenter, public ModelListener
{
public:
    ScreenLCDtestPresenter(ScreenLCDtestView& v);

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

    virtual ~ScreenLCDtestPresenter() {}
    void bind(Model* m) { model = m; }

private:
    ScreenLCDtestPresenter();

    ScreenLCDtestView& view;
    Model* model;
};

#endif // SCREENLCDTESTPRESENTER_HPP
