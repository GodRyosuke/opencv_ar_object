#pragma once

#include "Actor/Actor.hpp"

class Axis : public Actor {
public:
    Axis(class Manager* manager);

private:
    class MeshComponent* m_MeshComp;
};