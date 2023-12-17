#include "Actor/Capture.hpp"

#include "Manager.hpp"
#include "Component/VideoComponent.hpp"
#include "Texture.hpp"
#include "Renderer.hpp"
#include "Definitions.hpp"
#include "Util.hpp"
#include <opencv2/opencv.hpp>

Capture::Capture(Manager* manager)
    :Actor(manager, "Capture")
{
    m_VideoComp = new VideoComponent(this);
    m_VideoComp->m_DisableDrawing = true;   // 動画の描画はしない
    
    double screenWidth = manager->GetScreenSize().x;
    double screenHeight = manager->GetScreenSize().y;
    glm::vec2 texScale = glm::vec2(static_cast<float>(screenWidth) , static_cast<float>(screenHeight));
    glm::mat4 scaleMat = glm::scale(glm::mat4(1.0f), glm::vec3(texScale.x, texScale.y, 1.f));
    SetScale(scaleMat);
}

void Capture::UpdateActor()
{
    // textureの幅を現在の画面サイズの幅のm_imageRatio倍に合わせる
    const Texture* tex = m_VideoComp->GetTexture();
    double screenRatio = m_Manager->GetScreenSize().x / m_Manager->GetScreenSize().y;
    double videoRatio = static_cast<double>(tex->GetWidth()) / static_cast<double>(tex->GetHeight());
    double videoWidth = 0.0;
    double videoHeight = 0.0;
    if (screenRatio < videoRatio) {
        videoWidth = m_Manager->GetScreenSize().x;
        videoHeight = videoWidth / videoRatio;
    } else {
        videoHeight = m_Manager->GetScreenSize().y;
        videoWidth = videoHeight * videoRatio;
    }
    
    glm::vec2 texScale = glm::vec2(static_cast<float>(videoWidth) , static_cast<float>(videoHeight));
    // texScale *= m_Manager->GetScreenSize().x * m_imageRatio / tex->GetWidth();
    glm::mat4 scaleMat = glm::scale(glm::mat4(1.0f), glm::vec3(texScale.x, texScale.y, 1.f));
    SetScale(scaleMat);
}

cv::Mat Capture::GetCurrentFrame() const {
    return m_VideoComp->GetCurrentFrame();
}
