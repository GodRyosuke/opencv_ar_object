#include "Manager.hpp"

#include <iostream>

#include "Renderer.hpp"

Manager::Manager()
    :m_Renderer(new Renderer())
{
    if (!Init()) {
        printf("error: failed to initialize manager\n");
        m_IsRun = false;
        return;
    }

    m_IsRun = true;
}

Manager::~Manager()
{
    delete m_Renderer;
}

bool Manager::Init()
{
    if (!m_Renderer->Init()) {
        printf("error: failed to initialize renderer\n");
        return false;
    }

    m_Renderer->SetKeyCallback([this](GLFWwindow* window, int key, int scancode, int action, int mods)->void {
        if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
            this->m_IsRun = false;
        }
    });

    return true;
}

void Manager::Update()
{
    if (m_Renderer->IsCloseWindow()) {
        m_IsRun = false;
    }
}

void Manager::Run()
{
    while (m_IsRun)
    {
        Update();
        m_Renderer->Draw();
        glfwPollEvents();
    }
    
}