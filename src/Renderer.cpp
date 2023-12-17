#include "Renderer.hpp"

#include <iostream>

#include "Manager.hpp"
#include "Util.hpp"
#include "Shader.hpp"
#include "Mesh.hpp"
#include "Component/SpriteComponent.hpp"
#include "Component/MeshComponent.hpp"

Renderer::Renderer(Manager* manager)
    :m_Manager(manager)
    // :m_ScreenWidth(640)
    // ,m_ScreenHeight(480)
    // ,m_GLFWWindow(nullptr)
{
}

bool Renderer::Init()
{


    // m_GLFWWindow = glfwCreateWindow(m_ScreenWidth, m_ScreenHeight, "opencv_ar_object", NULL, NULL);
    // if (!m_GLFWWindow)
    // {
    //     Util::Print("error: failed to crate glfw window\n");
    //     return false;
    // }
    // glfwMakeContextCurrent(m_GLFWWindow);
    gladLoadGL();
	// On some platforms, GLEW will emit a benign error code,
	// so clear it
	glGetError();
    // glfwSetKeyCallback(m_GLFWWindow, MainKeyCallback);

    // glfwSetInputMode(m_GLFWWindow, GLFW_STICKY_KEYS, GL_TRUE);
    // Hide the mouse and enable unlimited mouvement
    // glfwSetInputMode(m_GLFWWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwPollEvents();

    return true;
}

Renderer::~Renderer()
{
    for (auto i : m_Meshes)
	{
		delete i.second;
	}
    m_Meshes.clear();
    
    for (auto i : m_Shaders)
	{
		delete i.second;
	}
	m_Shaders.clear();
    
    // m_KeyCallbacks.clear();

}

// void Renderer::Update()
// {
//     // window sizeを取得
// }

void Renderer::Draw()
{
    glClearColor(0.f, 0.5f, 0.7f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glm::vec2 winSize = m_Manager->GetScreenSize();
    glViewport(0, 0, winSize.x, winSize.y);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // for (auto sk : mSkinMeshComps) {
    //     sk->Draw();
    // }
    for (auto mc : m_MeshComps) {
        mc->Draw();
    }

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

// @param f 全てのShaderについて実施してほしい処理
void Renderer::AllShaderProcess(std::function<void(class Shader*)> f) const
{
    for (auto iter : m_Shaders) {
        f(iter.second);
    }
}
// shaderNameのshaderについて実施してほしい処理f
void Renderer::SpecificShaderProcess(std::string shaderName, std::function<void(class Shader*)> f) const
{
    auto iter = m_Shaders.find(shaderName);
    if (iter == m_Shaders.end()) {
        Util::Printf("error: invalid shader: %s is called\n", shaderName.c_str());
        assert(false);
        return;
    }
    f(iter->second);
}

// std::vector<std::function<void(GLFWwindow*, int, int, int, int)>> Renderer::m_KeyCallbacks;
// void Renderer::MainKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
// {
//     Util::Print("dlfkaj\n");
    
//     for (auto callback : m_KeyCallbacks) {
//         callback(window,key, scancode, action, mods);
//     }
// }

Mesh* Renderer::GetMesh(const std::string fileName, bool isSkeletal)
{
    Mesh* m = nullptr;
    auto iter = m_Meshes.find(fileName);
    if (iter != m_Meshes.end())
    {
        m = iter->second;
    }
    else
    {
        m = new Mesh();
        if (m->Load(fileName, isSkeletal))
        {
            m_Meshes.emplace(fileName, m);
        }
        else
        {
            delete m;
            m = nullptr;
        }
    }
    return m;
}

void Renderer::AddMeshComp(MeshComponent* mesh)
{
	if (mesh->m_IsSkeletal)
	{
		// SkinMeshComponent* sk = static_cast<SkinMeshComponent*>(mesh);
		// mSkinMeshComps.emplace_back(sk);
	}
	else
	{
		m_MeshComps.emplace_back(mesh);
	}
}