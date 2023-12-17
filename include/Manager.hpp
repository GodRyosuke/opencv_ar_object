#pragma once

#include <unordered_map>
#include <string>

#include "glm/glm.hpp"
#include "glad/glad.h"
#include "GLFW/glfw3.h"

class Manager {
public:
    Manager();
    ~Manager();
    void Run();
    void AddActor(class Actor* actor);
    const class Actor* GetActor(std::string actorName);
    void RemoveActor(std::string name);
    glm::vec2 GetScreenSize() const { return glm::vec2(m_ScreenWidth, m_ScreenHeight); }
    double GetDeltaTime() const { return m_DeltaTime; }

    class Renderer* m_Renderer;
private:
    bool Init();
    void Update();
    void Input();

    bool m_IsRun;
    std::unordered_map<std::string, class Actor*> m_Actors;
    GLFWwindow* m_GLFWWindow;
    int m_ScreenWidth;
    int m_ScreenHeight;
    double m_DeltaTime;



};