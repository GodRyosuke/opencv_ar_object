#include "Renderer.hpp"

#include <iostream>


// namespace {
// void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
// {
//     if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
//         glfwSetWindowShouldClose(window, GLFW_TRUE);
//     }
// }
    
// void glfwsample()
// {
//     if (!glfwInit())
//     {
//         // Initialization failed
//         printf("failed to initialize glfw\n");
//         return;
//     }
//     glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
//     glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
//     const int screenWidth = 640;
//     const int screenHeight = 480;
//     GLFWwindow* window = glfwCreateWindow(screenWidth, screenHeight, "wander", NULL, NULL);
//     if (!window)
//     {
//         printf("error: failed to crate glfw window\n");
//         return;
//         // Window or OpenGL context creation failed
//     }
//     glfwMakeContextCurrent(window);
//     gladLoadGL();
// 	// On some platforms, GLEW will emit a benign error code,
// 	// so clear it
// 	glGetError();

//     glfwSetKeyCallback(window, key_callback);

//     GLuint vertex_buffer = 0;
//     glGenBuffers(1, &vertex_buffer);
//     while (!glfwWindowShouldClose(window))
//     {
//         glClearColor(0, 0.5, 0.7, 1.0f);
//         glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//         glViewport(0, 0, screenWidth, screenHeight);
//         glEnable(GL_DEPTH_TEST);
//         glEnable(GL_BLEND);
//         glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        
//         glfwSwapBuffers(window);
//         glfwPollEvents();
//     }

//     glfwDestroyWindow(window);
//     glfwTerminate();

// }
// }

Renderer::Renderer()
    :m_ScreenWidth(640)
    ,m_ScreenHeight(480)
    ,m_GLFWWindow(nullptr)
{
}

bool Renderer::Init()
{
    if (!glfwInit())
    {
        // Initialization failed
        printf("error: failed to initialize glfw\n");
        return false;
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);

    m_GLFWWindow = glfwCreateWindow(m_ScreenWidth, m_ScreenHeight, "opencv_ar_object", NULL, NULL);
    if (!m_GLFWWindow)
    {
        printf("error: failed to crate glfw window\n");
        return false;
    }
    glfwMakeContextCurrent(m_GLFWWindow);
    gladLoadGL();
	// On some platforms, GLEW will emit a benign error code,
	// so clear it
	glGetError();
    glfwSetKeyCallback(m_GLFWWindow, MainKeyCallback);

    return true;
}

Renderer::~Renderer()
{
    glfwDestroyWindow(m_GLFWWindow);
    glfwTerminate();
}

void Renderer::Update()
{
    // window sizeを取得
    glfwGetWindowSize(m_GLFWWindow, &m_ScreenWidth, &m_ScreenHeight);
}

void Renderer::Draw()
{
    glClearColor(0.f, 0.5f, 0.7f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glViewport(0, 0, m_ScreenWidth, m_ScreenHeight);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glfwSwapBuffers(m_GLFWWindow);
}

std::vector<std::function<void(GLFWwindow*, int, int, int, int)>> Renderer::m_KeyCallbacks;
void Renderer::MainKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    for (auto callback : m_KeyCallbacks) {
        callback(window,key, scancode, action, mods);
    }
}

