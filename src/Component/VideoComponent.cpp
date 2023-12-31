#include "Component/VideoComponent.hpp"

#include "Actor/Actor.hpp"
#include "Manager.hpp"
#include "Renderer.hpp"
#include "VertexArray.hpp"
#include "Shader.hpp"
#include "Texture.hpp"
#include "Definitions.hpp"

VideoComponent::VideoComponent(Actor* owner)
    :SpriteComponent(owner, new Texture(GL_TEXTURE0))
    // ,m_Texture(new Texture(GL_TEXTURE0))
    // ,m_Video(cv::VideoCapture(0))
#ifdef _REALSENSE
    ,m_Align(RS2_STREAM_COLOR)
#endif
{
#ifdef _REALSENSE
    rs2::pipeline_profile profile = m_Pipe.start();
#else
    m_Video = cv::VideoCapture(0);
#endif
}

VideoComponent::~VideoComponent()
{
    // delete mVAO;
}

void VideoComponent::Update()
{
    // 最新のフレームを取得
#ifdef _REALSENSE
    rs2::frameset data 
        = m_Pipe.wait_for_frames().    // Wait for next set of frames from the camera
        apply_filter(m_Printer).     // Print each enabled stream frame rate
        apply_filter(m_ColorMap);   // Fi
    
    auto aligned_frames = m_Align.process(data);
    rs2::video_frame color_frame = aligned_frames.first(RS2_STREAM_COLOR);
    // rs2::video_frame depth_frame = aligned_frames.get_depth_frame().apply_filter(m_ColorMap);

    const int img_width = 640;
    const int img_height = 480;
    m_CurrentFrame = cv::Mat(cv::Size(img_width, img_height), CV_8UC3, (void*)color_frame.get_data(), cv::Mat::AUTO_STEP);
    cv::cvtColor(m_CurrentFrame, m_CurrentFrame, cv::COLOR_RGB2BGR);
#else
    m_Video.read(m_CurrentFrame);
#endif
    m_Texture->Update(m_CurrentFrame);   // 取得したフレームに合わせてtextureを更新
}


