#include "Actor/Actor.hpp"

#define _USE_MATH_DEFINES
#include "math.h"
#include <algorithm>

#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glm/gtx/rotate_vector.hpp"
#include "glm/gtx/vector_angle.hpp"
#include "glm/gtx/quaternion.hpp"

#include "Component/Component.hpp"
#include "Manager.hpp"

Actor::Actor(Manager* manager, std::string name)
    : mPosition(glm::vec3(0.f))
    , mRotation(glm::mat4(1.f))
    , mScale(glm::mat4(1.0f))
    , m_Manager(manager)
    ,m_Name(name)
    // , mGame(world->GetGame())
    , mRecomputeWorldTransform(false)
    , mBaseForward(glm::vec3(1.f, 0.f, 0.f))
{
    manager->AddActor(this);
}

Actor::~Actor()
{
    // m_Manager->RemoveActor(m_Name);
    // Need to delete components
    // Because ~Component calls RemoveComponent, need a different style loop
    while (!mComponents.empty())
    {
        delete mComponents.back();
    }
}

void Actor::Update()
{
    ComputeWorldTransform();

    UpdateComponents();
    UpdateActor();

    ComputeWorldTransform();
}

void Actor::UpdateComponents()
{
    for (auto comp : mComponents)
    {
        comp->Update();
    }
}

void Actor::UpdateActor()
{
}

void Actor::ProcessInput(GLFWwindow* window)
{
    for (auto comp : mComponents)
    {
        comp->ProcessInput(window);
    }

    ActorInput(window);
}

void Actor::ActorInput(GLFWwindow* window)
{

}

glm::vec3 Actor::GetForward() const
{
    glm::vec3 trans = mRotation * glm::vec4(mBaseForward, 1.f);
    return trans;
}

glm::vec3 Actor::GetRight() const
{
    glm::vec3 trans = mRotation * glm::vec4(0.f, 1.f, 0.f, 1.f);
    return trans;
}


void Actor::RotateToNewForward(const glm::vec3& forward)
{
    // Figure out difference between original (unit x) and new
    float dot = glm::dot(glm::vec3(1.f, 0.f, 0.f), forward);
    float angle = acos(dot);
    // Facing down X
    if (dot > 0.9999f)
    {
        SetRotation(glm::mat4(1.f));
    }
    // Facing down -X
    else if (dot < -0.9999f)
    {
        SetRotation(glm::rotate(glm::mat4(1.f), (float)M_PI, glm::vec3(0.f, 0.f, 1.f)));
    }
    else
    {
        // Rotate about axis from cross product
        glm::vec3 axis = glm::cross(glm::vec3(1.f, 0.f, 0.f), forward);
        glm::normalize(axis);
        SetRotation(glm::rotate(glm::mat4(1.f), angle, axis));
    }
}

void Actor::ComputeWorldTransform()
{
    if (mRecomputeWorldTransform)
    {
        mRecomputeWorldTransform = false;
        // Scale, then rotate, then translate
        // glm::mat4 ScaleMat = glm::scale(glm::mat4(1.0f), glm::vec3(mScale, mScale, mScale));
        glm::mat4 TranslateMat = glm::translate(glm::mat4(1.0f), mPosition);
        glm::mat4 rotationMat = glm::toMat4(mRotation);
        mWorldTransform = TranslateMat * rotationMat * mScale;

        // Inform components world transform updated
        for (auto comp : mComponents)
        {
            comp->OnUpdateWorldTransform();
        }
    }
}

void Actor::AddComponent(Component* component)
{
    int myOrder = component->GetUpdateOrder();
    auto iter = mComponents.begin();
    for (;
        iter != mComponents.end();
        ++iter)
    {
        if (myOrder < (*iter)->GetUpdateOrder())
        {
            break;
        }
    }

    mComponents.insert(iter, component);
}

void Actor::RemoveComponent(Component* component)
{
    auto iter = std::find(mComponents.begin(), mComponents.end(), component);
    if (iter != mComponents.end())
    {
        mComponents.erase(iter);
    }
}
