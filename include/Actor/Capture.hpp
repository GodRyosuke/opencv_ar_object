#pragma once

#include "Actor/Actor.hpp"

class Capture : public Actor {
public:
    Capture(class Manager* manager);
    void UpdateActor() override;

private:
    class VideoComponent* m_VideoComp;
};