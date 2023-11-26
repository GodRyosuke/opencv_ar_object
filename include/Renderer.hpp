#pragma once

#include <functional>
#include <vector>

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

private:
    static void MainKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);

    int m_ScreenWidth;
    int m_ScreenHeight;
    GLFWwindow* m_GLFWWindow;
    static std::vector<std::function<void(GLFWwindow*, int, int, int, int)>> m_KeyCallbacks;
};