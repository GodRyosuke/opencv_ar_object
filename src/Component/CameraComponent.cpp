#include "Component/CameraComponent.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtx/vector_angle.hpp"
#include "glm/gtx/rotate_vector.hpp"

#include "Actor/Actor.hpp"
#include "Manager.hpp"
#include "Renderer.hpp"
#include "Shader.hpp"

CameraComponent::CameraComponent(Actor* actor)
    :Component(actor)
    ,mNearZ(0.1f)
    ,mFarZ(100.f)
    ,mFOVDeg(45.f)
    ,mUP(0.f, 0.f, 1.f)
    ,mCamOrientation(glm::vec3(0.5f, 0.f, 0.f))
    ,mRecomputeView(true)
{
    // actor->GetGame()->GetRenderer()->SetCameraToCSM(this);
}

// Camera::Camera(glm::vec3 cameraPos, glm::vec3 orientation, float nearZ, float farZ, float fovDeg)
//     :Orientation(orientation)
//     ,mFOV(fovDeg * M_PI / 180.f)
//     ,mNear_z(nearZ)
//     ,mFar_z(farZ)
//     ,Up(glm::vec3(0.f, 0.f, 1.f))
//     ,View(glm::lookAt(cameraPos, cameraPos + orientation, Up))
//     ,Proj(glm::perspective(glm::radians(fovDeg), 1024.f/ 768.f, nearZ, farZ))
//     ,Center(cameraPos + orientation)
// {
// }

// void CameraComponent::SetCameraOrientation(const glm::vec3 orientation)
// {
//     // glm::mat4 view = mCamera->View;
//     // glm::mat4 proj = mCamera->Proj;
//     mCamOrientation = orientation;
//     glm::vec3 cameraPos = mOwner->GetPosition();
//     mView = glm::lookAt(cameraPos, cameraPos + orientation, mUP);
    
//     // delete mCamera;
//     // mCamera = new Camera(mOwner->GetPosition(), orientation);
// }

void CameraComponent::Update()
{
    // View, Projection更新
    
    // mCamera.View = glm::lookAt(
	// 	mOwner->GetPosition(),
	// 	mOwner->GetPosition() + mCamera.Orientation,
	// 	mCamera.Up);
    // mCamera.Proj = 
    //     glm::perspective(glm::radians(45.0f), static_cast<float>(screenWidth) / static_cast<float>(screenHeight), 0.1f, 100.0f);
    if (mRecomputeView) {
        glm::vec3 cameraPos = mOwner->GetPosition();
        mView = glm::lookAt(cameraPos, cameraPos + mCamOrientation, mUP);
        mRecomputeView = false;
    }
    
    const float screenWidth = mOwner->GetGame()->GetRenderer()->GetScreenWidth();
    const float screenHeight = mOwner->GetGame()->GetRenderer()->GetScreenHeight();
    // glm::vec3 orientation = mCamera->Orientation;
    // delete mCamera;
    // mCamera = new Camera(mOwner->GetPosition(), orientation, static_cast<float>(screenWidth), static_cast<float>(screenHeight));

    mProj = glm::perspective(glm::radians(mFOVDeg), screenWidth/ screenHeight, mNearZ, mFarZ);

    // GPUに送信
    auto shaders = mOwner->GetGame()->GetRenderer()->GetShaders();
    for (auto shader : shaders) {
        shader.second->UseProgram();
        shader.second->SetMatrixUniform("CameraProj", mProj);
        shader.second->SetVectorUniform("gEyeWorldPos", mOwner->GetPosition());
        shader.second->SetMatrixUniform("CameraView", mView);

    }
    
    // std::vector<Shader*> shaders;
    // shaders.push_back(mOwner->GetGame()->GetRenderer()->GetShader("TestMeshShader"));
    // shaders.push_back(mOwner->GetGame()->GetRenderer()->GetShader("TestSkinShader"));
    // shaders.push_back(mOwner->GetGame()->GetRenderer()->GetShader("PlaneShader"));
    // shaders.push_back(mOwner->GetGame()->GetRenderer()->GetShader("ChestShader"));
    // for (auto shader : shaders) {
    //     shader->UseProgram();
    //     shader->SetMatrixUniform("CameraProj", mCamera->Proj);
    // }
    // shaders.push_back(mOwner->GetGame()->GetRenderer()->GetShader("OrthoMeshShadowMapShader"));
    // shaders.push_back(mOwner->GetGame()->GetRenderer()->GetShader("OrthoSkinShadowMapShader"));
    // for (auto shader : shaders) {
    //     shader->UseProgram();
    //     shader->SetVectorUniform("gEyeWorldPos", mOwner->GetPosition());
    //     shader->SetMatrixUniform("CameraView", mCamera->View);
    // }
}

glm::mat4 CameraComponent::GetView()
{
    glm::vec3 cameraPos = mOwner->GetPosition();
    glm::mat4 view = glm::lookAt(cameraPos, cameraPos + mCamOrientation, mUP);
    return view;
}