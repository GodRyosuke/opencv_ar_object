#pragma once
#include <cstdint>

#include "Actor/Actor.hpp"
#include "InputEvent.hpp"

class Component
{
public:
    // Constructor
    // (the lower the update order, the earlier the component updates)
    Component(class Actor* owner, int updateOrder = 100);
    // Destructor
    virtual ~Component();
    // Update this component by delta time
    virtual void Update();
    // Process input for this component
    virtual void ProcessInput(InputEvent::Data event) {}
    // Called when world transform changes
    virtual void OnUpdateWorldTransform() { }

    std::string GetOwnerName() { return m_Owner->m_Name; }
    int GetUpdateOrder() const { return mUpdateOrder; }
protected:
    // Owning actor
    class Actor* m_Owner;
    // Update order of component
    int mUpdateOrder;
};
