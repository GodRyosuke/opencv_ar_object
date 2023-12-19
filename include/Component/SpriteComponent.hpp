#pragma once

#include "Component/Component.hpp"
#include "glm/glm.hpp"
#include "glad/glad.h"

#include "Shader.hpp"

class SpriteComponent : public Component {
public:
    SpriteComponent(class Actor* owner, class Texture* tex, int rendOrder = 100);
    ~SpriteComponent();
    // void SetTexture(class Texture* tex) { mTexture = tex; }
    void Draw();

    const class Texture* GetTexture() { return m_Texture; };
    bool m_DisableDrawing;

protected:
    float* mSpriteVertices;
    class VertexArray* mVAO;
    // GLenum mTextureUnit;
    const Shader::ShaderDesc m_ShaderDesc;
    class Texture* m_Texture;

    // glm::mat4 mSpriteViewProj;
};