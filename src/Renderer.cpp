#include "Renderer.hpp"

#include <iostream>

#include "Manager.hpp"
#include "Util.hpp"
#include "Shader.hpp"
#include "Mesh.hpp"
#include "Animation.hpp"
#include "Component/SpriteComponent.hpp"
#include "Component/MeshComponent.hpp"
#include "Component/SkinMeshComponent.hpp"

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

    for (auto i : m_Animations)
	{
		delete i.second;
	}
	m_Animations.clear();
    
    for (auto i : m_Shaders)
	{
		delete i.second;
	}
	m_Shaders.clear();

    m_Drawers.clear();
    // while (!m_Drawers.empty())
    // {
    //     delete m_Drawers.back();
    // }

    // m_MeshComps.clear();
    // m_SkinMeshComps.clear();
    
    
    // m_KeyCallbacks.clear();

}

// void Renderer::Update()
// {
//     // window sizeを取得
// }

void Renderer::Draw()
{
    glClearColor(0.86f, 0.86f, 0.86f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glm::vec2 winSize = m_Manager->GetScreenSize();
    glViewport(0, 0, winSize.x, winSize.y);
    
    for (auto drawer : m_Drawers) {
        drawer->Draw();
    }
    
    
    
    // glEnable(GL_DEPTH_TEST);
    // glEnable(GL_BLEND);
    // glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // for (auto sk : m_SkinMeshComps) {
    //     sk->Draw();
    // }
    // for (auto mc : m_MeshComps) {
    //     mc->Draw();
    // }

	// // Draw Sprites
	// glDisable(GL_DEPTH_TEST);
	// // Enable alpha blending on the color buffer
	// glEnable(GL_BLEND);
	// glBlendEquationSeparate(GL_FUNC_ADD, GL_FUNC_ADD);
	// glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ZERO);

	// // mSpriteShader->UseProgram();
	// for (auto sprite : m_SpriteComps) {
	// 	sprite->Draw();
	// }

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

const Animation* Renderer::GetAnimation(std::string fileName)
{
    Animation* anim = nullptr;

    auto iter = m_Animations.find(fileName);
    if (iter != m_Animations.end())
    {
        // すでに読み込まれていたら
        anim = iter->second;
    }
    else
    {
        // 同名のMesh fileがあればそれをAnimationファイルとして使用
		auto mIter = m_Meshes.find(fileName);
		if (mIter != m_Meshes.end()) {	// Mesh fileをAnimationとして併用
			anim = new Animation(mIter->second->GetaiScene());
			return anim;
		} 
		
        anim = new Animation();
        if (anim->Load(fileName))
        {
            m_Animations.emplace(fileName, anim);
        }
        else
        {
            delete anim;
            anim = nullptr;
        }
    }

    return anim;
}

void Renderer::AddSpriteComp(SpriteComponent* sprite, int order)
{
    // m_SpriteComps.push_back(sprite);
    
    m_Drawers.push_back(new SpriteDrawer(sprite, order));
    std::sort(m_Drawers.begin(), m_Drawers.end(), [](Drawer* a, Drawer* b)->bool {
        return a->m_RenderingOrder < b->m_RenderingOrder;
    });
}

void Renderer::AddMeshComp(MeshComponent* mesh, int order)
{
	if (mesh->m_IsSkeletal)
	{
		SkinMeshComponent* sk = static_cast<SkinMeshComponent*>(mesh);
        m_Drawers.push_back(new SkinMeshDrawer(sk, order));
        
		// m_SkinMeshComps.emplace_back(sk);
	}
	else
	{
        m_Drawers.push_back(new MeshDrawer(mesh, order));
		// m_MeshComps.emplace_back(mesh);
	}
    std::sort(m_Drawers.begin(), m_Drawers.end(), [](Drawer* a, Drawer* b)->bool {
        return a->m_RenderingOrder < b->m_RenderingOrder;
    });
}


Drawer::Drawer(int order)
    :m_RenderingOrder(order)
{}

SpriteDrawer::SpriteDrawer(SpriteComponent* sprite, int order)
    :Drawer(order)
    ,m_SpriteComp(sprite)
{}

void SpriteDrawer::Draw()
{
    glDisable(GL_DEPTH_TEST);
	// Enable alpha blending on the color buffer
	glEnable(GL_BLEND);
	glBlendEquationSeparate(GL_FUNC_ADD, GL_FUNC_ADD);
	glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ZERO);

    m_SpriteComp->Draw();
}

MeshDrawer::MeshDrawer(MeshComponent* mesh, int order)
    :Drawer(order)
    ,m_MeshComp(mesh)
{}

void MeshDrawer::Draw()
{
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    m_MeshComp->Draw();
}

SkinMeshDrawer::SkinMeshDrawer(SkinMeshComponent* skinMesh, int order)
    :Drawer(order)
    ,m_SkinMeshComp(skinMesh)
{}

void SkinMeshDrawer::Draw()
{
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    m_SkinMeshComp->Draw();
}