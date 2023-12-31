#include "Actor/TestSprite.hpp"

#include "Manager.hpp"
#include "Component/SpriteComponent.hpp"
#include "Texture.hpp"
#include "Renderer.hpp"
#include "Definitions.hpp"
#include "Util.hpp"

TestSprite::TestSprite(Manager* manager)
    :Actor(manager, "TestSprite")
    ,m_imageRatio(0.7)
{
    Texture* tex = new Texture();
    tex->Load(std::string(ASSET_PATH) + "ArMarker.png");
    // Texture* tex = new Texture(std::string(ASSET_PATH) + "socket_img.jpg", GL_TEXTURE0);
    m_SpriteComp = new SpriteComponent(this, tex);

    // textureの幅を画面のm_imageRatio倍に合わせる
    glm::vec2 texScale = glm::vec2(static_cast<float>(tex->GetWidth()) , static_cast<float>(tex->GetHeight()));
    texScale *= manager->GetScreenSize().x * m_imageRatio / tex->GetWidth();
    glm::mat4 scaleMat = glm::scale(glm::mat4(1.0f), glm::vec3(texScale.x, texScale.y, 1.f));
    SetScale(scaleMat);
}

void TestSprite::UpdateActor()
{
    // textureの幅を現在の画面サイズの幅のm_imageRatio倍に合わせる
    const Texture* tex = m_SpriteComp->GetTexture();
    glm::vec2 texScale = glm::vec2(static_cast<float>(tex->GetWidth()) , static_cast<float>(tex->GetHeight()));
    texScale *= m_Manager->GetScreenSize().x * m_imageRatio / tex->GetWidth();
    glm::mat4 scaleMat = glm::scale(glm::mat4(1.0f), glm::vec3(texScale.x, texScale.y, 1.f));
    SetScale(scaleMat);
}
