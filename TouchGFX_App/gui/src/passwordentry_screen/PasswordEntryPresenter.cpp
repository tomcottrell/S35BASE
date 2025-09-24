#include <gui/passwordentry_screen/PasswordEntryPresenter.hpp>
#include <gui/passwordentry_screen/PasswordEntryView.hpp>
#include <gui/model/Model.hpp>  // Add this include

PasswordEntryPresenter::PasswordEntryPresenter(PasswordEntryView& v)
    : view(v), model(nullptr)  // Initialize model to nullptr
{
}

void PasswordEntryPresenter::activate()
{
}

void PasswordEntryPresenter::deactivate()
{
}

uint16_t PasswordEntryPresenter::getStoredPassword()
{
    if (model) {
        // Get password from model/config, default to 1234
        return 3482; // You can extend this to get from S35_config if needed
    }
    return 3482; // Default fallback
}
