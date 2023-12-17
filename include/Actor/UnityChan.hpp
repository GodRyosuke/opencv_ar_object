#pragma once

#include "Actor/Actor.hpp"

class UnityChan : public Actor {
public:
    UnityChan(class Manager* manager);

private:
    class MeshComponent* m_MeshComp;
};