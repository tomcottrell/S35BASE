#include <gui/menulayer3_screen/MenuLayer3View.hpp>
#include <touchgfx/Unicode.hpp>
#include <touchgfx/hal/HAL.hpp>
#include <cstring>

MenuLayer3View::MenuLayer3View() : currentItem(nullptr), tempValue(0), tempEnumIndex(0),
    m_buttonController(nullptr), // Initialize as null
    lastHoldTime(0), holdIncrement(0), needsRedraw(false), lastButtonChangeTime(0),
    processingTick(false), lastTickTime(0), tickCount(0)
{
    // Clear all buffers to prevent garbage data
    memset(nameBuffer, 0, sizeof(nameBuffer));
    memset(numberValueBuffer, 0, sizeof(numberValueBuffer));
    memset(minValueBuffer, 0, sizeof(minValueBuffer));
    memset(maxValueBuffer, 0, sizeof(maxValueBuffer));
    memset(enumBuffer, 0, sizeof(enumBuffer));
    memset(descBuffer, 0, sizeof(descBuffer));
    memset(unitsBuffer, 0, sizeof(unitsBuffer));
}

void MenuLayer3View::setupScreen()
{
    MenuLayer3ViewBase::setupScreen();

    // Safely get button controller
    HAL* hal = HAL::getInstance();
    if (hal) {
        m_buttonController = static_cast<TouchGfxInputController*>(hal->getButtonController());
    }

    initializeCurrentItem();
    updateDisplay();
}

void MenuLayer3View::tearDownScreen()
{
    // Reset state to prevent issues on next entry
    processingTick = false;
    needsRedraw = false;
    currentItem = nullptr;
    m_buttonController = nullptr;

    MenuLayer3ViewBase::tearDownScreen();
}

void MenuLayer3View::initializeCurrentItem()
{
    currentItem = presenter ? presenter->getCurrentSubItem() : nullptr;
    if (currentItem) {
        // Safer string copy with bounds checking
        const char* sourceName = currentItem->name;
        if (sourceName) {
            size_t sourceLen = strlen(sourceName);
            size_t copyLen = (sourceLen < 49) ? sourceLen : 49;

            if (copyLen > 0) {
                touchgfx::Unicode::strncpy(nameBuffer, sourceName, copyLen);
            }
            nameBuffer[copyLen] = 0; // Ensure null termination
            itemNameText.setWildcard(nameBuffer);
        }

        // Initialize dummy variables with current values
        if (currentItem->type == NUMBER_TYPE) {
            tempValue = currentItem->currentValue;

            // Calculate hold increment safely
            int32_t range = currentItem->maxValue - currentItem->minValue;
            if (range > 100) {
                holdIncrement = range / 100; // 1% of range
            } else if (range > 10) {
                holdIncrement = range / 10;  // 10% of smaller ranges
            } else {
                holdIncrement = 1; // Minimum increment
            }

            // Ensure holdIncrement is always positive and reasonable
            if (holdIncrement <= 0) holdIncrement = 1;
            if (holdIncrement > range / 2) holdIncrement = range / 2;
            if (holdIncrement > 50) holdIncrement = 50; // Cap at 50 for safety

        } else if (currentItem->type == ENUM_TYPE) {
            tempEnumIndex = currentItem->currentEnumIndex;
            // Bounds check enum index
            if (tempEnumIndex >= currentItem->enumCount) {
                tempEnumIndex = 0;
            }
            holdIncrement = 1;
        }
    }
}

void MenuLayer3View::updateDisplay()
{
    if (!currentItem) return;

    // Update description and units (common for both NUMBER and ENUM types)
    updateDescriptionAndUnits();

    if (currentItem->type == NUMBER_TYPE) {
        updateValueDisplay();
        enumValueText.setVisible(false);
    } else {
        updateEnumDisplay();
        numberValueText.setVisible(false);
        minValueText.setVisible(false);
        maxValueText.setVisible(false);
    }
    requestRedraw();
}

void MenuLayer3View::updateDescriptionAndUnits()
{
    if (!currentItem) return;

    // Update description text
    if (currentItem->description) {
        const char* sourceDesc = currentItem->description;
        size_t descLen = strlen(sourceDesc);
        size_t copyLen = (descLen < 99) ? descLen : 99;

        if (copyLen > 0) {
            touchgfx::Unicode::strncpy(descBuffer, sourceDesc, copyLen);
        }
        descBuffer[copyLen] = 0;
        descText.setWildcard(descBuffer);
        descText.setVisible(true);
    } else {
        descText.setVisible(false);
    }

    // Update units text
    if (currentItem->units) {
        const char* sourceUnits = currentItem->units;
        size_t unitsLen = strlen(sourceUnits);
        size_t copyLen = (unitsLen < 19) ? unitsLen : 19;

        if (copyLen > 0) {
            touchgfx::Unicode::strncpy(unitsBuffer, sourceUnits, copyLen);
        }
        unitsBuffer[copyLen] = 0;
        unitsText.setWildcard(unitsBuffer);
        unitsText.setVisible(true);
    } else {
        unitsText.setVisible(false);
    }
}

void MenuLayer3View::updateValueDisplay()
{
    if (!currentItem || currentItem->type != NUMBER_TYPE) return;

    // Bounds check tempValue before display
    if (tempValue < currentItem->minValue) tempValue = currentItem->minValue;
    if (tempValue > currentItem->maxValue) tempValue = currentItem->maxValue;

    // Display the temporary value with bounds checking
    Unicode::snprintf(numberValueBuffer, 9, "%d", tempValue); // Leave room for null terminator
    numberValueText.setWildcard(numberValueBuffer);
    numberValueText.setVisible(true);

    // Update min/max labels
    Unicode::snprintf(minValueBuffer, 14, "Min: %d", currentItem->minValue);
    minValueText.setWildcard(minValueBuffer);
    minValueText.setVisible(true);

    Unicode::snprintf(maxValueBuffer, 14, "Max: %d", currentItem->maxValue);
    maxValueText.setWildcard(maxValueBuffer);
    maxValueText.setVisible(true);
}

void MenuLayer3View::updateEnumDisplay()
{
    if (!currentItem || currentItem->type != ENUM_TYPE) return;

    // Bounds check enum index
    if (tempEnumIndex >= currentItem->enumCount) {
        tempEnumIndex = 0;
    }

    if (currentItem->enumOptions && tempEnumIndex < currentItem->enumCount) {
        const char* enumOption = currentItem->enumOptions[tempEnumIndex];
        if (enumOption) {
            size_t optionLen = strlen(enumOption);
            size_t copyLen = (optionLen < 49) ? optionLen : 49;

            touchgfx::Unicode::strncpy(enumBuffer, enumOption, copyLen);
            enumBuffer[copyLen] = 0;
            enumValueText.setWildcard(enumBuffer);
            enumValueText.setVisible(true);
        }
    }
}

void MenuLayer3View::handleTickEvent()
{
    // Safety check: limit tick rate to prevent overwhelming the system
    uint32_t currentTime = HAL_GetTick();
    if (currentTime - lastTickTime < MIN_TICK_INTERVAL) {
        return;
    }
    lastTickTime = currentTime;

    // Count ticks for debugging/monitoring
    tickCount++;
    if (tickCount > 1000000) tickCount = 0; // Prevent overflow

    if (!currentItem || processingTick) return;

    // Prevent re-entrant calls
    processingTick = true;

    // Debounce button state changes
    if (currentTime - lastButtonChangeTime < DEBOUNCE_TIME) {
        processingTick = false;
        return;
    }

    // Check for button holds with additional safety checks
    if (m_buttonController) {
        // Replace try-catch with null pointer and bounds checking
        bool buttonCheckSuccess = true;
        bool upHeld = false;
        bool downHeld = false;

        // Safely check button states with validation
        if (m_buttonController != nullptr) {
            // Add additional validation here if needed for your button controller
            upHeld = m_buttonController->isButtonHeld(1);
            downHeld = m_buttonController->isButtonHeld(2);
        } else {
            buttonCheckSuccess = false;
        }

        if (buttonCheckSuccess) {
            if (upHeld && !downHeld) {
                if (currentTime - lastHoldTime >= HOLD_REPEAT_TIME) {
                    processUpButtonHold();
                    lastHoldTime = currentTime;
                    lastButtonChangeTime = currentTime;
                }
            }
            else if (downHeld && !upHeld) {
                if (currentTime - lastHoldTime >= HOLD_REPEAT_TIME) {
                    processDownButtonHold();
                    lastHoldTime = currentTime;
                    lastButtonChangeTime = currentTime;
                }
            }
            else {
                // Reset hold time when no single button is held
                lastHoldTime = 0;
            }
        } else {
            // If button controller is invalid, reset it safely
            m_buttonController = nullptr;
        }
    }

    // Apply any pending display updates - but limit frequency
    static uint32_t lastInvalidateTime = 0;
    if (needsRedraw && (currentTime - lastInvalidateTime >= MIN_INVALIDATE_INTERVAL)) {
        this->invalidate();
        needsRedraw = false;
        lastInvalidateTime = currentTime;
    }

    processingTick = false;
}

void MenuLayer3View::processUpButtonHold()
{
    if (!currentItem) return;

    if (currentItem->type == NUMBER_TYPE) {
        int32_t newValue = tempValue + holdIncrement;
        if (newValue <= currentItem->maxValue && newValue >= currentItem->minValue) {
            tempValue = newValue;
            updateValueDisplay();
            requestRedraw();
        }
    } else if (currentItem->type == ENUM_TYPE && currentItem->enumCount > 0) {
        tempEnumIndex = (tempEnumIndex + 1) % currentItem->enumCount;
        updateEnumDisplay();
        requestRedraw();
    }
}

void MenuLayer3View::processDownButtonHold()
{
    if (!currentItem) return;

    if (currentItem->type == NUMBER_TYPE) {
        int32_t newValue = tempValue - holdIncrement;
        if (newValue >= currentItem->minValue && newValue <= currentItem->maxValue) {
            tempValue = newValue;
            updateValueDisplay();
            requestRedraw();
        }
    } else if (currentItem->type == ENUM_TYPE && currentItem->enumCount > 0) {
        if (tempEnumIndex == 0) {
            tempEnumIndex = currentItem->enumCount - 1;
        } else {
            tempEnumIndex--;
        }
        updateEnumDisplay();
        requestRedraw();
    }
}

void MenuLayer3View::handleUpButton()
{
    if (!currentItem) return;

    uint32_t currentTime = HAL_GetTick();

    // Prevent rapid button presses
    if (currentTime - lastButtonChangeTime < BUTTON_PRESS_DEBOUNCE) {
        return;
    }

    lastButtonChangeTime = currentTime;

    if (currentItem->type == NUMBER_TYPE) {
        if (tempValue < currentItem->maxValue) {
            tempValue++;
            updateValueDisplay();
            requestRedraw();
        }
    } else if (currentItem->type == ENUM_TYPE && currentItem->enumCount > 0) {
        tempEnumIndex = (tempEnumIndex + 1) % currentItem->enumCount;
        updateEnumDisplay();
        requestRedraw();
    }
}

void MenuLayer3View::handleDownButton()
{
    if (!currentItem) return;

    uint32_t currentTime = HAL_GetTick();

    // Prevent rapid button presses
    if (currentTime - lastButtonChangeTime < BUTTON_PRESS_DEBOUNCE) {
        return;
    }

    lastButtonChangeTime = currentTime;

    if (currentItem->type == NUMBER_TYPE) {
        if (tempValue > currentItem->minValue) {
            tempValue--;
            updateValueDisplay();
            requestRedraw();
        }
    } else if (currentItem->type == ENUM_TYPE && currentItem->enumCount > 0) {
        if (tempEnumIndex == 0) {
            tempEnumIndex = currentItem->enumCount - 1;
        } else {
            tempEnumIndex--;
        }
        updateEnumDisplay();
        requestRedraw();
    }
}

void MenuLayer3View::handleEnterButton()
{
    if (!currentItem || !presenter) return;

    // Prevent multiple rapid enter presses
    uint32_t currentTime = HAL_GetTick();
    static uint32_t lastEnterTime = 0;
    if (currentTime - lastEnterTime < ENTER_BUTTON_DEBOUNCE) {
        return;
    }
    lastEnterTime = currentTime;

    // Save the temporary values to the real location
    if (currentItem->type == NUMBER_TYPE) {
        presenter->saveNumberValue(tempValue);
    } else {
        presenter->saveEnumSelection(tempEnumIndex);
    }

    // Return to Layer 2
    Application* app = Application::getInstance();
    if (app) {
        static_cast<FrontendApplication*>(app)->gotoMenuLayer2Screen();
    }
}

void MenuLayer3View::handleHomeButton()
{
    // Prevent multiple rapid home presses
    uint32_t currentTime = HAL_GetTick();
    static uint32_t lastHomeTime = 0;
    if (currentTime - lastHomeTime < ENTER_BUTTON_DEBOUNCE) {
        return;
    }
    lastHomeTime = currentTime;

    // Exit without saving (discard temporary values)
    Application* app = Application::getInstance();
    if (app) {
        static_cast<FrontendApplication*>(app)->gotoMenuLayer1ScreenNoTransition();
    }
}

void MenuLayer3View::requestRedraw()
{
    needsRedraw = true;
}
