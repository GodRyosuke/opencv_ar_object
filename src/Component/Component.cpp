#include "Component/Component.hpp"
#include "Actor/Actor.hpp"

Component::Component(Actor* owner, int updateOrder)
    :m_Owner(owner)
    , mUpdateOrder(updateOrder)
{
    // Add to actor's vector of components
    m_Owner->AddComponent(this);
}

Component::~Component()
{
    m_Owner->RemoveComponent(this);
}

void Component::Update()
{
}
