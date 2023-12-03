#pragma once

#include <unordered_map>
#include <string>

class Manager {
public:
    Manager();
    ~Manager();
    void Run();
    void AddActor(class Actor* actor);
    const class Actor* GetActor(std::string actorName);
    void RemoveActor(std::string name);

    class Renderer* m_Renderer;
private:
    bool Init();
    void Update();

    bool m_IsRun;
    std::unordered_map<std::string, class Actor*> m_Actors;

};