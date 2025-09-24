#ifndef MODEL_HPP
#define MODEL_HPP

#include <stdint.h>

class ModelListener;

// Menu system type definitions
enum MenuItemType {
    NUMBER_TYPE,
    ENUM_TYPE
};

struct MenuSubItem {
    const char* name;
    MenuItemType type;
    int32_t currentValue;
    int32_t minValue;
    int32_t maxValue;
    const char** enumOptions;
    uint8_t enumCount;
    uint8_t currentEnumIndex;
    const char* description;  // Add this
    const char* units;        // Add this

};

struct MenuCategory {
    const char* name;
    MenuSubItem* subItems;
    uint8_t subItemCount;
};

class Model
{
public:
    Model();

    void bindModel(ModelListener* listener)
    {
        modelListener = listener;
    }

    void tick();

    void bind(void* presenter);

    // Menu system methods - used by MenuLayer presenters
    MenuSubItem* getCurrentSubItem();
    void saveCurrentValue(int32_t value);
    void saveCurrentEnumSelection(uint8_t enumIndex);

    void setCurrentLayer1Selection(uint8_t selection) { currentLayer1Selection = selection; }
    void setCurrentLayer2Selection(uint8_t selection) { currentLayer2Selection = selection; }
    uint8_t getCurrentLayer1Selection() const { return currentLayer1Selection; }
    uint8_t getCurrentLayer2Selection() const { return currentLayer2Selection; }

    // Menu system getters - used by MenuLayer1 and MenuLayer2 presenters
    MenuCategory* getMenuCategories() { return menuCategories; }
    uint8_t getCategoryCount() const { return CATEGORY_COUNT; }

    // New system state enums
    enum TruckState {
        TRUCK_STATE_UNKNOWN = 0,
        TRUCK_STATE_LATCHED,
        TRUCK_STATE_LOWERED,
        TRUCK_STATE_MOVING,
        TRUCK_STATE_RAISED
    };

    // Trailer state enumeration (add to existing enums)
    enum TrailerState {
        TRAILER_STATE_UNKNOWN = 0,
        TRAILER_STATE_LOWERED,
        TRAILER_STATE_MOVING,
        TRAILER_STATE_RAISED,
        TRAILER_STATE_LATCHED
    };

    enum ErrorState {
        ERROR_NONE = 0,
        ERROR_ESTOP_ACTIVE = 1,
        ERROR_ANGLE_EXCEEDED = 2,
        ERROR_OVERPRESSURE = 3,    // NEW: Added overpressure error
        ERROR_MOVING = 4
    };

    // Truck state getter methods - used by ScreenTestHomeView
    TruckState getTruckState() const;
    ErrorState getErrorState() const;
    bool getFlashState() const;

    // Trailer getter methods (add to existing getters)
    TrailerState getTrailerState() const;
    uint16_t getTrailerPressure() const;
    const char* getTrailerStateMessage() const;
    uint16_t getTrailerStateBitmapId() const;

    int16_t getRollValue() const;
    bool isRollValid() const;

    bool isPTOActive() const;
    bool isPTOValid() const;

    uint16_t getTruckPressure() const;

    const char* getErrorMessage() const;
    //const char* getTruckStateMessage() const;
    const char* getButtonFeedbackMessage(uint8_t buttonPressed, bool isHeld = false) const;

    // NEW: Trailer-specific button feedback method
    const char* getTrailerButtonFeedbackMessage(uint8_t buttonPressed, bool isHeld = false) const;

    uint8_t getCurrentButtonState() const;
    uint16_t getTruckStateBitmapId() const;
    uint16_t getTruckOverloadSetpoint() const;  // Add this line

    //void setLatchButtonPressed(bool pressed);
    //bool getLatchButtonPressed() const;
    const char* getLatchStatusText() const;

    bool isAlarmActive() const;
    const char* getAlarmTitle() const;
    const char* getAlarmDescription() const;

protected:
    ModelListener* modelListener;

private:
    // Menu system variables
    static const uint8_t CATEGORY_COUNT = 4;
    MenuCategory menuCategories[CATEGORY_COUNT];
    uint8_t currentLayer1Selection;
    uint8_t currentLayer2Selection;
    bool settingsLoaded;

    // Truck system state variables
    TruckState currentTruckState;
    ErrorState currentErrorState;

    TrailerState currentTrailerState;

    bool flashState;
    uint32_t lastFlashTime;

    int16_t rollValue;
    bool rollValid;

    bool ptoActive;
    bool ptoValid;

    bool stateChanged;

    // Private methods
    void initializeMenuData();
    void initializeSystemState();
    void syncMenuItemsToConfig();
    void loadSettingsFromStorage();
    void saveSettingsToFile();
    void loadSettingsFromConfig();
    int32_t constrain(int32_t value, int32_t minVal, int32_t maxVal);
    bool alarmActive;
    //bool latchButtonPressed;
    void updateAlarmState();

    // System processing methods
    void processTruckStatus();
    void processTrailerStatus();
    void processErrorConditions();
    void updateFlashingStates();
    void processRollData();
    void processPTOStatus();
    void notifyStateChanged();

    // Add these new state tracking members
    uint8_t lastButtonPressed;
    bool buttonFeedbackActive;
    uint32_t buttonFeedbackTimer;
};

#endif // MODEL_HPP
