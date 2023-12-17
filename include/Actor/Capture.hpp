#pragma once

#include "Actor/Actor.hpp"

#include <opencv2/opencv.hpp>

class Capture : public Actor {
public:
    Capture(class Manager* manager);
    void UpdateActor() override;
    cv::Mat GetCurrentFrame() const;

private:
    class VideoComponent* m_VideoComp;
};