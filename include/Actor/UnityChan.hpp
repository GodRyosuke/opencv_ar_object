#pragma once

#include "Actor/Actor.hpp"

class UnityChan : public Actor {
public:
    UnityChan(class Manager* manager);

private:
    class SkinMeshComponent* m_SkinMeshComp;
};