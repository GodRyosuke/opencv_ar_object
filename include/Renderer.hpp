#pragma once

#include <functional>
#include <vector>
#include <unordered_map>
#include <string>

#include "glm/glm.hpp"
#include "glad/glad.h"
#include "GLFW/glfw3.h"

class Renderer {
public:
    Renderer();
    ~Renderer();
    bool Init();
    void Update();
    void Draw();
    bool IsCloseWindow() { return glfwWindowShouldClose(m_GLFWWindow); }
    void SetKeyCallback(std::function<void(GLFWwindow*, int, int, int, int)> callback)
    { m_KeyCallbacks.push_back(callback); }

    void AddSpriteComp(class SpriteComponent* sprite) { m_SpriteComps.push_back(sprite); }
    glm::vec2 GetScreenSize() { return glm::vec2(m_ScreenWidth, m_ScreenHeight); }
    void AddShader(std::string name, class Shader* shader);
    class Shader* GetShader(std::string name);

private:
    static void MainKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);

    int m_ScreenWidth;
    int m_ScreenHeight;
    GLFWwindow* m_GLFWWindow;
    static std::vector<std::function<void(GLFWwindow*, int, int, int, int)>> m_KeyCallbacks;
    std::vector<class SpriteComponent*> m_SpriteComps;
    std::unordered_map<std::string, class Shader*> m_Shaders;
};