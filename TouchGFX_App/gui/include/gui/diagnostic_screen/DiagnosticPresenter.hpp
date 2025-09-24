#ifndef DIAGNOSTICPRESENTER_HPP
#define DIAGNOSTICPRESENTER_HPP

#include <gui/model/ModelListener.hpp>
#include <mvp/Presenter.hpp>

using namespace touchgfx;

class DiagnosticView;
class Model;

class DiagnosticPresenter : public touchgfx::Presenter, public ModelListener
{
public:
    DiagnosticPresenter(DiagnosticView& v);

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

    virtual ~DiagnosticPresenter() {}
    void bind(Model* m) { model = m; }

private:
    DiagnosticPresenter();

    DiagnosticView& view;
    Model* model;
};

#endif // DIAGNOSTICPRESENTER_HPP
