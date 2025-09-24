#ifndef PASSWORDENTRYVIEW_HPP
#define PASSWORDENTRYVIEW_HPP

#include <gui_generated/passwordentry_screen/PasswordEntryViewBase.hpp>
#include <gui/passwordentry_screen/PasswordEntryPresenter.hpp>

class PasswordEntryView : public PasswordEntryViewBase
{
public:
    PasswordEntryView();
    virtual ~PasswordEntryView() {}
    virtual void setupScreen();
    virtual void tearDownScreen();

    // Public methods for TouchGFX interactions
    void handleUpButton();
    void handleDownButton();
    void handleEnterButton();

private:
    uint8_t currentDigit;           // 0-3, which digit we're editing
    uint8_t passwordDigits[4];      // Current password being entered
    uint8_t attempts;               // Failed attempt counter

    void moveToNextDigit();
    void updateDigitDisplay();
    void updateBorderHighlight();
    void validatePassword();
    void resetPasswordEntry();
    void showError();
    void clearError();

    // Password validation
    bool checkPassword();
};

#endif // PASSWORDENTRYVIEW_HPP
