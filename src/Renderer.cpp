#include "Renderer.hpp"

#include <iostream>

#include "Util.hpp"
#include "Shader.hpp"
#include "Component/SpriteComponent.hpp"

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
    gladLoadGL();
	// On some platforms, GLEW will emit a benign error code,
	// so clear it
	glGetError();
    glfwSetKeyCallback(m_GLFWWindow, MainKeyCallback);

    return true;
}

Renderer::~Renderer()
{
    m_KeyCallbacks.clear();
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

	// Draw Sprites
	glDisable(GL_DEPTH_TEST);
	// Enable alpha blending on the color buffer
	glEnable(GL_BLEND);
	glBlendEquationSeparate(GL_FUNC_ADD, GL_FUNC_ADD);
	glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ZERO);

	// mSpriteShader->UseProgram();
	for (auto sprite : m_SpriteComps) {
		sprite->Draw();
	}

    glfwSwapBuffers(m_GLFWWindow);
}

Shader* Renderer::GetShader(const Shader::ShaderDesc& shaderDesc)
{
    auto iter = m_Shaders.find(shaderDesc.m_Name);
    if (iter == m_Shaders.end()) {
        // まだ読み込まれていないので、コンパイルする
        std::string fileNames = "";
        for (auto fileName : shaderDesc.m_FilePaths) {
            fileNames += fileName + " ";
        }
        Util::Print("Compile shader files: ", fileNames, "\n");
        Shader* shader = Shader::CompileShaderFromDesc(shaderDesc);
        if (!shader) {
            Util::Print("failed to compile shader\n");
            return nullptr;
        }
        m_Shaders.emplace(shaderDesc.m_Name, shader);
        return shader;
    }

    return m_Shaders[shaderDesc.m_Name];
}

std::vector<std::function<void(GLFWwindow*, int, int, int, int)>> Renderer::m_KeyCallbacks;
void Renderer::MainKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    for (auto callback : m_KeyCallbacks) {
        callback(window,key, scancode, action, mods);
    }
}
