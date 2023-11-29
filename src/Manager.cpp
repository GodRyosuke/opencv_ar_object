#include "Manager.hpp"

#include <iostream>
#include <algorithm>

#include "Renderer.hpp"
#include "Actor/Actor.hpp"
#include "Util.hpp"
#include <cassert>

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
    m_Renderer->Update();
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

void Manager::AddActor(Actor* actor)
{
    m_Actors.emplace(actor->m_Name.c_str(), actor);
}

void Manager::RemoveActor(std::string name)
{
    auto iter = m_Actors.find(name);
    if (iter == m_Actors.end()) {
        printf("error: failed to remove actor: %s\n", name.c_str());
        return;
    }
    m_Actors.erase(name);
}