#ifndef FRONTENDAPPLICATION_HPP
#define FRONTENDAPPLICATION_HPP

#include <gui_generated/common/FrontendApplicationBase.hpp>

class FrontendHeap;

using namespace touchgfx;

class FrontendApplication : public FrontendApplicationBase
{
public:
    FrontendApplication(Model& m, FrontendHeap& heap);
    virtual ~FrontendApplication() { }

    // Add getModel() method to access the model from views
    Model& getModel() { return model; }

    // Add these navigation methods
    void gotoHomeScreen() { gotoScreenTestHomeScreenNoTransition(); } // Replace 'MainScreen' with your actual home screen name
    void gotoMenuLayer1Screen() { gotoMenuLayer1ScreenNoTransition(); }
    void gotoMenuLayer2Screen() { gotoMenuLayer2ScreenNoTransition(); }
    void gotoMenuLayer3Screen() { gotoMenuLayer3ScreenNoTransition(); }
    //void gotoTrailer() { gotoScreenTrailerScreenNoTransition(); }

    virtual void handleTickEvent()
    {
        model.tick();
        FrontendApplicationBase::handleTickEvent();
    }

private:
};

#endif // FRONTENDAPPLICATION_HPP
