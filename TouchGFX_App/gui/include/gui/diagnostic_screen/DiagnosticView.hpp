#ifndef DIAGNOSTICVIEW_HPP
#define DIAGNOSTICVIEW_HPP

#include <gui_generated/diagnostic_screen/DiagnosticViewBase.hpp>
#include <gui/diagnostic_screen/DiagnosticPresenter.hpp>

class DiagnosticView : public DiagnosticViewBase
{
public:
    DiagnosticView();
    virtual ~DiagnosticView() {}
    virtual void setupScreen();
    virtual void tearDownScreen();
    virtual void handleTickEvent();

protected:

private:
    // Display update methods
    bool updateDiagnosticDisplays();
    void initializeDisplayState();
    void forceUpdateDiagnosticDisplays();  // Add this method
};

#endif // DIAGNOSTICVIEW_HPP
