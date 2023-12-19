#pragma once

#include "Actor/Actor.hpp"

class Line : public Actor {
public:
    Line(class Manager* manager, std::string lineName);

private:
    class MeshComponent* m_MeshComp;
};

class Grid : public Actor {
public:
    Grid(class Manager* manager);
    // ~Grid();

// private:
//     std::vector<Actor*> m_Lines;
};