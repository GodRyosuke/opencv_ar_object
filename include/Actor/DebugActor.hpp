#pragma once

#include "Actor/Actor.hpp"

class DebugActor : public Actor {
public:
    DebugActor(class Manager* manager);
    void ActorInput(InputEvent::Data event) override;
    void UpdateActor() override;

private:
    void UpdateCameraOrientation(glm::vec2 mousePos);

    glm::vec3 m_CameraOrientation;
    const glm::vec3 m_CameraUp;
    bool m_IsFirstClick;
    // double m_KeyMoveSpeed;
    // class MeshComponent* m_MeshComp;
};