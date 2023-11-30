#pragma once

#include "Component/Component.hpp"
#include "glm/glm.hpp"
#include "glad/glad.h"


class SpriteComponent : public Component {
public:
    SpriteComponent(class Actor* owner);
    ~SpriteComponent();
    void SetTexture(class Texture* tex) { mTexture = tex; }
    void Draw();

private:
    float* mSpriteVertices;
    class VertexArray* mVAO;
    class Texture* mTexture;
    GLenum mTextureUnit;
    // glm::mat4 mSpriteViewProj;
};