#include "Actor/DebugActor.hpp"

#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtx/rotate_vector.hpp"
#include "glm/gtx/vector_angle.hpp"

#include "Manager.hpp"
#include "Renderer.hpp"
#include "Definitions.hpp"
#include "Shader.hpp"
// #include "Component/MeshComponent.hpp"

DebugActor::DebugActor(Manager* manager)
    :Actor(manager, "DebugActor")
    ,m_CameraOrientation(glm::vec3(0.f, -0.5f, 0.f))
    ,m_CameraUp(glm::vec3(0.f, 0.f, 1.f))
    ,m_IsFirstClick(true)
{
    SetPosition(glm::vec3(-1.0f, 2.5f, 1.0f));

    // shader
    // Shader::ShaderDesc desc({
    //     "SpriteShader",
    //     {
    //     std::string(SHADER_PATH) + "mesh.vert",
    //     std::string(SHADER_PATH) + "unitychan.frag"
    //     },
    //     Shader::ShaderDesc::VERTEX_FRAGMENT
    // });
    // // mesh
    // m_MeshComp = new MeshComponent(
    //     this,
    //     std::string(ASSET_PATH) + "unitychan/unitychan.fbx",
    //     m_Manager->m_Renderer->GetShader(desc)
    // );
}

void DebugActor::ActorInput(GLFWwindow* window)
{
    glm::vec3 cameraPos = GetPosition();
    const float keyMoveSpeed = 0.1f;
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        // 前へ進む
        cameraPos += keyMoveSpeed * m_CameraOrientation;
    }
    // if (event.key == GLFW_KEY_W && event.action == GLFW_PRESS) {
    //     // 前へ進む
    //     cameraPos += keyMoveSpeed * m_CameraOrientation;
    // }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        // 後ろへ下がる
        cameraPos -= keyMoveSpeed * m_CameraOrientation;
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        // 左
        cameraPos -= keyMoveSpeed * glm::normalize(glm::cross(m_CameraOrientation, m_CameraUp));
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        // 右
        cameraPos += keyMoveSpeed * glm::normalize(glm::cross(m_CameraOrientation, m_CameraUp));
    }
    SetPosition(cameraPos);

    // カメラの向きを更新
    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
    // if (event.key == GLFW_MOUSE_BUTTON_LEFT && event.action == GLFW_PRESS) {
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

        // 初回クリック時はカーソルを中央に移動
        glm::vec2 screenSize = GetManager()->GetScreenSize();
        if (m_IsFirstClick) {
            glfwSetCursorPos(window, screenSize.x / 2.f, screenSize.y / 2.f);
            m_IsFirstClick = false;
        }
        double mouseX;
        double mouseY;
        glfwGetCursorPos(window, &mouseX, &mouseY);
        UpdateCameraOrientation(glm::vec2(mouseX, mouseY));
        // カーソル位置を画面中央に戻す
        glfwSetCursorPos(window, screenSize.x / 2.f, screenSize.y / 2.f);
    // } else if (event.key == GLFW_MOUSE_BUTTON_LEFT && event.action == GLFW_RELEASE) {
    } else if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_RELEASE) {
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        m_IsFirstClick = true;
    }

    const double fov = 45; // degree
    const double nearP = 0.1f;
    const double farP = 200;
    glm::mat4 camView = glm::lookAt(GetPosition(), GetPosition() + m_CameraOrientation, m_CameraUp);
    glm::vec2 screenSize = GetManager()->GetScreenSize();
    glm::mat4 camProj = glm::perspective<float>(glm::radians(fov), screenSize.x / screenSize.y, nearP, farP);
    GetManager()->m_Renderer->AllShaderProcess([this, camView, camProj](Shader* shader) {
        shader->UseProgram();
        shader->SetMatrixUniform("CameraView", camView);
        // shader->SetMatrixUniform("CameraProj", camProj);
    });
}

void DebugActor::UpdateCameraOrientation(glm::vec2 mousePos)
{
    glm::vec2 screenSize = GetManager()->GetScreenSize();
    const double moveSensitivity = 100.0;
    double deltaY = mousePos.y - (screenSize.y/2);
    double deltaX = mousePos.x - (screenSize.x/2);
    double rotX = 0;
    double rotY = 0;
    // 画面のサイズが奇数であるときの対策
    if (std::abs(deltaX) >= 1.0) {
        rotY = moveSensitivity * (deltaX) / screenSize.x;
    }
    if (std::abs(deltaY) >= 1.0) {
        rotX = moveSensitivity * (deltaY) / screenSize.y;
    }

    // 垂直方向の更新
    glm::vec3 newOrientation = glm::rotate<float>(m_CameraOrientation, glm::radians(-rotX), glm::normalize(glm::cross(m_CameraOrientation, m_CameraUp)));
    // 垂直に向いていなければ、向きを更新
    if (std::abs(glm::angle(newOrientation, m_CameraUp) - glm::radians(90.f)) <= glm::radians(85.f)) {
        m_CameraOrientation = newOrientation;
    }

    // 左右方向の更新
    m_CameraOrientation = glm::rotate<float>(m_CameraOrientation, glm::radians(-rotY), m_CameraUp);
}

void DebugActor::UpdateActor()
{
    // const double fov = 45; // degree
    // const double nearP = 0.1f;
    // const double farP = 200;
    // glm::mat4 camView = glm::lookAt(GetPosition(), GetPosition() + m_CameraOrientation, m_CameraUp);
    // glm::vec2 screenSize = GetManager()->GetScreenSize();
    // glm::mat4 camProj = glm::perspective<float>(glm::radians(fov), screenSize.x / screenSize.y, nearP, farP);
    // GetManager()->m_Renderer->AllShaderProcess([this, camView, camProj](Shader* shader) {
    //     shader->SetMatrixUniform("CameraView", camView);
    //     shader->SetMatrixUniform("CameraProj", camProj);
    // });
}