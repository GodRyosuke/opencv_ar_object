#pragma once

#include "Actor/Actor.hpp"

class Grid : public Actor {
public:
    Grid(class Manager* manager);

private:
    class MeshComponent* m_MeshComp;
};