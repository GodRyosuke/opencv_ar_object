#include "Manager.hpp"

#include <iostream>
#include <algorithm>

#include "Renderer.hpp"
#include "Util.hpp"
#include "Actor/Actor.hpp"
#include "Actor/TestSprite.hpp"
#include "Actor/Capture.hpp"
#include "Actor/ARMarker.hpp"

Manager::Manager()
    :m_Renderer(new Renderer())
{
    if (!Init()) {
        Util::Print("error: failed to initialize manager\n");
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
        Util::Print("error: failed to initialize renderer\n");
        return false;
    }

    m_Renderer->SetKeyCallback([this](GLFWwindow* window, int key, int scancode, int action, int mods)->void {
        if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
            this->m_IsRun = false;
        }
    });

    // Load Actors
    Actor* a = nullptr;
    // a = new TestSprite(this);
    a = new Capture(this);
    a = new ARMarker(this);

    return true;
}

void Manager::Update()
{
    m_Renderer->Update();
    if (m_Renderer->IsCloseWindow()) {
        m_IsRun = false;
    }
    for (auto actor : m_Actors) {
        actor.second->Update();
    }
}

void Manager::Run()
{
    while (m_IsRun)
    {
        Update();
        m_Renderer->Draw();
        glfwPollEvents();
        // Util::Print("dalfjkdla", 23, " flakjdf\n");
    }
}

void Manager::AddActor(Actor* actor)
{
    m_Actors.emplace(actor->m_Name.c_str(), actor);
}

const Actor* Manager::GetActor(std::string actorName)
{
    auto iter = m_Actors.find(actorName);
    if (iter == m_Actors.end()) {
        Util::Print("error: Actor ", actorName, " has not been added to manager\n");
        return nullptr;
    }
    return iter->second;
}

void Manager::RemoveActor(std::string name)
{
    auto iter = m_Actors.find(name);
    if (iter == m_Actors.end()) {
        Util::Print("error: failed to remove actor: ", name.c_str(), '\n');
        return;
    }
    m_Actors.erase(name);
}