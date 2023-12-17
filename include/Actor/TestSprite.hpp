#pragma once

#include "Actor/Actor.hpp"

class TestSprite : public Actor {
public:
    TestSprite(class Manager* manager);
    void UpdateActor() override;

private:
    class SpriteComponent* m_SpriteComp;
    const double m_imageRatio;
};