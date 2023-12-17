#pragma once

#include "Component/Component.hpp"
#include "glm/glm.hpp"
#include <vector>

// class Camera {
// public:
//     Camera(glm::vec3 cameraPos, glm::vec3 orientation, 
//         float nearZ = 0.1f, float farZ = 100.f, float fovDeg = 45.f);
//     const glm::vec3 Orientation;
//     const glm::vec3 Center;
//     const glm::vec3 Up;
//     const glm::mat4 View;
//     const glm::mat4 Proj;
//     const float mNear_z;
//     const float mFar_z;
//     const float mFOV;   // radians of  camera projection
// };

class CameraComponent : Component {
public:
    CameraComponent(class Actor* actor);
    void Update() override;
    void OnUpdateWorldTransform() { mRecomputeView = true; }

    // const glm::vec3 GetCameraOrientation() const { return mCamera->Orientation; }
    // const glm::vec3 GetCameraUp() const { return mCamera->Up; }
    void SetCameraOrientation(const glm::vec3 orientation) { mCamOrientation = orientation; mRecomputeView = true; }
    // const Camera* GetCamera() const { return mCamera; }

    glm::vec3 GetOrientation() const { return mCamOrientation; }
    glm::vec3 GetUP() const { return mUP; }
    std::vector<float> GetNearFarZ() const { return std::vector<float>{ mNearZ, mFarZ }; }
    glm::mat4 GetProj() const { return mProj; }
    glm::mat4 GetView();
    //  const { return mView; }
    float GetFOVDeg() const { return mFOVDeg; }

private:
    bool mRecomputeView;
    // const Camera* mCamera;
    glm::mat4 mProj;
    glm::mat4 mView;
    float mFOVDeg;
    glm::vec3 mCamOrientation;
    float mNearZ;
    float mFarZ;
    glm::vec3 mUP;
};