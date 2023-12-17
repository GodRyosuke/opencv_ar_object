#include "Manager.hpp"

#include <iostream>
#include <algorithm>

#include "Renderer.hpp"
#include "Util.hpp"
#include "InputEvent.hpp"
#include "Actor/Actor.hpp"
#include "Actor/TestSprite.hpp"
#include "Actor/Capture.hpp"
#include "Actor/ARMarker.hpp"
#include "Actor/UnityChan.hpp"
#include "Actor/DebugActor.hpp"
#include "Actor/Grid.hpp"
#include "Actor/Axis.hpp"

Manager::Manager()
    // :m_Renderer(new Renderer())
    :m_GLFWWindow(nullptr)
    ,m_ScreenWidth(640)
    ,m_ScreenHeight(480)
{
    m_Renderer = new Renderer(this);
    if (!Init()) {
        Util::Print("error: failed to initialize manager\n");
        m_IsRun = false;
        return;
    }

    m_IsRun = true;
}

Manager::~Manager()
{
    for (auto i : m_Actors)
	{
		delete i.second;
	}
	m_Actors.clear();
    
    delete m_Renderer;
    glfwDestroyWindow(m_GLFWWindow);
    glfwTerminate();
}

bool Manager::Init()
{
    if (!glfwInit())
    {
        // Initialization failed
        Util::Print("error: failed to initialize glfw\n");
        return false;
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
    
    m_GLFWWindow = glfwCreateWindow(m_ScreenWidth, m_ScreenHeight, "opencv_ar_object", NULL, NULL);
    if (!m_GLFWWindow)
    {
        Util::Print("error: failed to crate glfw window\n");
        return false;
    }
    glfwMakeContextCurrent(m_GLFWWindow);
    
    if (!m_Renderer->Init()) {
        Util::Print("error: failed to initialize renderer\n");
        return false;
    }

    // m_Renderer->SetKeyCallback([this](GLFWwindow* window, int key, int scancode, int action, int mods)->void {
    //     if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
    //         this->m_IsRun = false;
    //     }
    // });

    // Load Actors
    Actor* a = nullptr;
    a = new Grid(this);
    a = new Axis(this);
    a = new UnityChan(this);
    a = new DebugActor(this);


    // m_Renderer->SetKeyCallback([this](GLFWwindow* window, int key, int scancode, int action, int mods)->void {
    //     InputEvent::Data event({window, key, scancode, action, mods});
    //     for (auto actor : this->m_Actors) {
    //         actor.second->ProcessInput(event);
    //     }
    //     // if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
    //     //     this->m_IsRun = false;
    //     // }
    // });

    // a = new TestSprite(this);
    // a = new Capture(this);
    // a = new ARMarker(this);

    return true;
}

void Manager::Update()
{
    glfwGetWindowSize(m_GLFWWindow, &m_ScreenWidth, &m_ScreenHeight);

    // m_Renderer->Update();
    if (glfwWindowShouldClose(m_GLFWWindow)) {
        m_IsRun = false;
    }
    for (auto actor : m_Actors) {
        actor.second->Update();
    }
}

void Manager::Input()
{
    glfwPollEvents();
    for (auto actor : this->m_Actors) {
        actor.second->ProcessInput(m_GLFWWindow);
    }
    if (glfwGetKey(m_GLFWWindow, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        m_IsRun = false;
    }
}

void Manager::Run()
{
    double currentTime = 0.0;
    double lastTime = 0.0;
    const double fps = 30.0;
    while (m_IsRun)
    {
        currentTime = glfwGetTime();
		m_DeltaTime = currentTime - lastTime;

		if (m_DeltaTime >= 1.0 / fps)
		{
			// Creates new title
			// std::string FPS = std::to_string((1.0 / timeDiff) * counter);
			// std::string ms = std::to_string((timeDiff / counter) * 1000);
			// std::string newTitle = "YoutubeOpenGL - " + FPS + "FPS / " + ms + "ms";
			// glfwSetWindowTitle(window, newTitle.c_str());

			// Resets times and counter
			lastTime = currentTime;
			// counter = 0;

			// Use this if you have disabled VSync
			//camera.Inputs(window);
		}
        
        Update();
        m_Renderer->Draw();
        Input();

        glfwSwapBuffers(m_GLFWWindow);
    }
}

void Manager::AddActor(Actor* actor)
{
    auto iter = m_Actors.find(actor->m_Name);
    if (iter != m_Actors.end()) {
        Util::Printf("error: actor %s is already added to manager\n", actor->m_Name.c_str());
        return;
    }
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