#pragma once

#include "Component/Component.hpp"
#include "glm/glm.hpp"
#include "glad/glad.h"

#include "Shader.hpp"

class SpriteComponent : public Component {
public:
    SpriteComponent(class Actor* owner, class Texture* tex);
    ~SpriteComponent();
    // void SetTexture(class Texture* tex) { mTexture = tex; }
    void Draw();

    const class Texture* m_Texture;
    
private:
    float* mSpriteVertices;
    class VertexArray* mVAO;
    // GLenum mTextureUnit;
    const Shader::ShaderDesc m_ShaderDesc;

    // glm::mat4 mSpriteViewProj;
};