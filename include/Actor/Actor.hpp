#pragma once

#include <vector>
#include <string>

#include "glm/glm.hpp"
#include "glm/gtc/quaternion.hpp"

#include "InputEvent.hpp"

class Actor {
public:
    Actor(class Manager* manager, std::string name);
    virtual ~Actor();

    void Update();
    // Actorが保有しているComponentを更新
    void UpdateComponents();
    // Actor固有の更新
    virtual void UpdateActor();

    void ProcessInput(GLFWwindow* window);
    // Acttor固有の入力
    virtual void ActorInput(GLFWwindow* window);

    const glm::vec3& GetPosition() const { return mPosition; }
    void SetBaseForward(glm::vec3 base) { mBaseForward = base; }
    void SetPosition(const glm::vec3& pos) { mPosition = pos; mRecomputeWorldTransform = true; }
    // float GetScale() const { return mScale; }
    void SetScale(float scale) { mScale = glm::scale(glm::mat4(1.0f), glm::vec3(scale, scale, scale));  mRecomputeWorldTransform = true; }
    void SetScale(glm::mat4 scale) { mScale = scale; mRecomputeWorldTransform = true; }
    const glm::quat& GetRotation() const { return mRotation; }
    void SetRotation(const glm::mat4& rotation) { mRotation = rotation;  mRecomputeWorldTransform = true; }
    void SetRotation(const glm::quat& rotation) { mRotation = rotation;  mRecomputeWorldTransform = true; }

    void ComputeWorldTransform();
    const glm::mat4 GetWorldTransform() const { return mWorldTransform; };

    const Manager* GetManager() { return m_Manager; }
    glm::vec3 GetForward() const;
    glm::vec3 GetRight() const;


    void RotateToNewForward(const glm::vec3& forward);
    // const World* GetWorld() const { return mWorld; }

    // Add/remove components
    void AddComponent(class Component* component);
    void RemoveComponent(class Component* component);

    const std::string m_Name;

protected:
    class Manager* m_Manager;

private:
    glm::vec3 mPosition;
    glm::quat mRotation;
    glm::mat4 mScale;
    // float mScale;
    glm::mat4 mWorldTransform;
    bool mRecomputeWorldTransform;
    glm::vec3 mBaseForward;
    std::vector<class Component*> mComponents;
    // class Game* mGame
};
