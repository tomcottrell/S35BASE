#ifndef SCROLLLISTCONTAINERITEM_HPP
#define SCROLLLISTCONTAINERITEM_HPP

#include <gui_generated/containers/ScrollListContainerItemBase.hpp>

class ScrollListContainerItem : public ScrollListContainerItemBase
{
public:
    ScrollListContainerItem();
    virtual ~ScrollListContainerItem() {}

    virtual void initialize();
protected:
};

#endif // SCROLLLISTCONTAINERITEM_HPP
