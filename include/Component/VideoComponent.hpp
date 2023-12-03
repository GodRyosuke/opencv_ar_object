#pragma once

#include "Component/Component.hpp"
#include "Component/SpriteComponent.hpp"
#include "glm/glm.hpp"
#include "glad/glad.h"
#include <opencv2/opencv.hpp>

#include "Shader.hpp"

class VideoComponent : public SpriteComponent {
public:
    VideoComponent(class Actor* owner);
    void Update() override;
    ~VideoComponent();
    // void SetTexture(class Texture* tex) { mTexture = tex; }
    // void Draw();
    // const Texture* GetTexture() { return m_Texture; }

    
private:
    // class Texture* m_Texture;
    // float* mSpriteVertices;
    // class VertexArray* mVAO;
    // GLenum mTextureUnit;
    // const Shader::ShaderDesc m_ShaderDesc;
    cv::VideoCapture m_Video;

    // glm::mat4 mSpriteViewProj;
};