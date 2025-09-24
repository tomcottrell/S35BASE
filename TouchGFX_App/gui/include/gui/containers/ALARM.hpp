#ifndef ALARM_HPP
#define ALARM_HPP

#include <gui_generated/containers/ALARMBase.hpp>

class ALARM : public ALARMBase
{
public:
    ALARM();
    virtual ~ALARM() {}

    virtual void initialize();
protected:
};

#endif // ALARM_HPP
