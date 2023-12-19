#pragma once

#include "Actor/Actor.hpp"

class Axis : public Actor {
public:
    Axis(class Manager* manager, std::string name);

private:
    class MeshComponent* m_MeshComp;
};