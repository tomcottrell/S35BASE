#ifndef PASSWORDENTRYPRESENTER_HPP
#define PASSWORDENTRYPRESENTER_HPP

#include <gui/model/ModelListener.hpp>
#include <mvp/Presenter.hpp>
#include <cstdint>  // For uint16_t

using namespace touchgfx;

class PasswordEntryView;
class Model;  // Add this forward declaration

class PasswordEntryPresenter : public touchgfx::Presenter, public ModelListener
{
public:
    PasswordEntryPresenter(PasswordEntryView& v);

    virtual void activate();
    virtual void deactivate();
    virtual ~PasswordEntryPresenter() {}

    // Change bind signature to match MenuLayer1Presenter
    void bind(Model* m) { model = m; }

    // Add method to get stored password
    uint16_t getStoredPassword();

private:
    PasswordEntryPresenter();

    PasswordEntryView& view;
    Model* model;  // Add model member
};

#endif // PASSWORDENTRYPRESENTER_HPP
