#include <gui/passwordentry_screen/PasswordEntryView.hpp>
#include <gui/common/FrontendApplication.hpp>
#include <touchgfx/Unicode.hpp>
#include <touchgfx/Color.hpp>

PasswordEntryView::PasswordEntryView() :
currentDigit(0),
attempts(0)
{
}

void PasswordEntryView::setupScreen()
{
    PasswordEntryViewBase::setupScreen();
    resetPasswordEntry();
    updateBorderHighlight();
    clearError();
}

void PasswordEntryView::tearDownScreen()
{
    PasswordEntryViewBase::tearDownScreen();
}

void PasswordEntryView::resetPasswordEntry()
{
    currentDigit = 0;
    for(int i = 0; i < 4; i++) {
        passwordDigits[i] = 0;
    }
    updateDigitDisplay();
    updateBorderHighlight();
}

void PasswordEntryView::handleUpButton()
{
    passwordDigits[currentDigit] = (passwordDigits[currentDigit] + 1) % 10;
    updateDigitDisplay();
    this->invalidate();
}

void PasswordEntryView::handleDownButton()
{
    passwordDigits[currentDigit] = (passwordDigits[currentDigit] == 0) ? 9 : passwordDigits[currentDigit] - 1;
    updateDigitDisplay();
    this->invalidate();
}

void PasswordEntryView::handleEnterButton()
{
    if (currentDigit < 3) {
        moveToNextDigit();
    } else {
        validatePassword();
    }
    this->invalidate();
}

void PasswordEntryView::moveToNextDigit()
{
    currentDigit++;
    if (currentDigit > 3) currentDigit = 3;
    updateBorderHighlight();
}

void PasswordEntryView::updateDigitDisplay()
{
    // Update the 4 text areas with current digits
    Unicode::snprintf(digit1Buffer, 2, "%d", passwordDigits[0]);
    Unicode::snprintf(digit2Buffer, 2, "%d", passwordDigits[1]);
    Unicode::snprintf(digit3Buffer, 2, "%d", passwordDigits[2]);
    Unicode::snprintf(digit4Buffer, 2, "%d", passwordDigits[3]);
}

void PasswordEntryView::updateBorderHighlight()
{
    // Reset all borders to normal
    boxWithBorder1.setBorderColor(touchgfx::Color::getColorFromRGB(128, 128, 128)); // Gray
    boxWithBorder2.setBorderColor(touchgfx::Color::getColorFromRGB(128, 128, 128));
    boxWithBorder3.setBorderColor(touchgfx::Color::getColorFromRGB(128, 128, 128));
    boxWithBorder4.setBorderColor(touchgfx::Color::getColorFromRGB(128, 128, 128));

    // Highlight current digit box
    switch(currentDigit) {
        case 0:
            boxWithBorder1.setBorderColor(touchgfx::Color::getColorFromRGB(0, 123, 255)); // Blue
            break;
        case 1:
            boxWithBorder2.setBorderColor(touchgfx::Color::getColorFromRGB(0, 123, 255));
            break;
        case 2:
            boxWithBorder3.setBorderColor(touchgfx::Color::getColorFromRGB(0, 123, 255));
            break;
        case 3:
            boxWithBorder4.setBorderColor(touchgfx::Color::getColorFromRGB(0, 123, 255));
            break;
    }
}

void PasswordEntryView::validatePassword()
{
    if (checkPassword()) {
        // Correct password - go to menu
        clearError();
        application().gotoMenuLayer1Screen();
    } else {
        // Wrong password
        attempts++;
        showError();

        if (attempts >= 3) {
            // Too many attempts - return to home
            application().gotoHomeScreen();
        } else {
            // Reset and try again
            resetPasswordEntry();
        }
    }
}

bool PasswordEntryView::checkPassword()
{
    uint16_t storedPassword = presenter->getStoredPassword();
    uint16_t enteredPassword = passwordDigits[0] * 1000 +
                              passwordDigits[1] * 100 +
                              passwordDigits[2] * 10 +
                              passwordDigits[3];

    return (enteredPassword == storedPassword);
}

void PasswordEntryView::showError()
{
    // Show error message on your prompt text
    Unicode::strncpy(promptBuffer, "INCORRECT PASSWORD", PROMPT_SIZE);
    prompt.setColor(touchgfx::Color::getColorFromRGB(255, 0, 0)); // Red
    prompt.invalidate();
}

void PasswordEntryView::clearError()
{
    Unicode::strncpy(promptBuffer, "ENTER PASSWORD", PROMPT_SIZE);
    prompt.setColor(touchgfx::Color::getColorFromRGB(255, 255, 255)); // White
    prompt.invalidate();
}
