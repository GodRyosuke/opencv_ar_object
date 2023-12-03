#pragma once

#include "Actor/Actor.hpp"

class Capture : public Actor {
public:
    Capture(class Manager* manager);
    void UpdateActor() override;

private:
    class SpriteComponent* m_SpriteComp;
    const double m_imageRatio;
};