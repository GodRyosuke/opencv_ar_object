#pragma once

#include "Actor/Actor.hpp"

class TestSprite : public Actor {
public:
    TestSprite(class Manager* manager);

private:
    class Component* m_SpriteComp;
};