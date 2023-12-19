#pragma once

#include "Component/Component.hpp"
#include "Component/SpriteComponent.hpp"
#include "glm/glm.hpp"
#include "glad/glad.h"
#include <opencv2/opencv.hpp>

#include "Definitions.hpp"
#ifdef _REALSENSE
#include <librealsense2/rs.hpp>
#endif

#include "Shader.hpp"

class VideoComponent : public SpriteComponent {
public:
    VideoComponent(class Actor* owner);
    void Update() override;
    ~VideoComponent();
    cv::Mat GetCurrentFrame() const { return m_CurrentFrame; }
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
    cv::Mat m_CurrentFrame;
#ifdef _REALSENSE
    rs2::colorizer m_ColorMap;
    rs2::rates_printer m_Printer;
    rs2::pipeline m_Pipe;
    rs2::align m_Align;
#endif
    // glm::mat4 mSpriteViewProj;
};