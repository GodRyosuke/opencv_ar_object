#pragma once

#include "Actor/Actor.hpp"
#include <opencv2/aruco/charuco.hpp>

class ARMarker : public Actor {
public:
    ARMarker(class Manager* manager);
    void UpdateActor() override;

private:
    // class VideoComponent* m_VideoComp;
    class SpriteComponent* m_SpriteComp;
    cv::Ptr<cv::aruco::Dictionary> m_dictionary; 
    class Texture* m_DetectedImgTex;

};