#pragma once

#include <functional>
#include <vector>
#include <unordered_map>
#include <string>

#include "glm/glm.hpp"
#include "glad/glad.h"
#include "GLFW/glfw3.h"

#include "Shader.hpp"

class Renderer {
public:
    Renderer(class Manager* manager);
    ~Renderer();
    bool Init();
    // bool LoadShaders();
    // void Update();
    void Draw();
    // bool IsCloseWindow() { return glfwWindowShouldClose(m_GLFWWindow); }
    // void SetKeyCallback(std::function<void(GLFWwindow*, int, int, int, int)> callback)
    // { m_KeyCallbacks.push_back(callback); }

    void AddSpriteComp(class SpriteComponent* sprite) { m_SpriteComps.push_back(sprite); }
    void AddMeshComp(class MeshComponent* mesh);
    class Mesh* GetMesh(const std::string fileName, bool isSkeletal = false);

    // glm::vec2 GetScreenSize() const { return glm::vec2(m_ScreenWidth, m_ScreenHeight); }
    // void AddShader(std::string name, class Shader* shader);

    class Shader* GetShader(const Shader::ShaderDesc& shaderDesc);
    // @param f 全てのShaderについて実施してほしい処理
    void AllShaderProcess(std::function<void(class Shader*)> f) const;
    // shaderNameのshaderについて実施してほしい処理f
    void SpecificShaderProcess(std::string shaderName, std::function<void(class Shader*)> f) const;

private:
    // static void MainKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);


    // int m_ScreenWidth;
    // int m_ScreenHeight;
    // GLFWwindow* m_GLFWWindow;
    // static std::vector<std::function<void(GLFWwindow*, int, int, int, int)>> m_KeyCallbacks;

    class Manager* m_Manager;
    std::vector<class SpriteComponent*> m_SpriteComps;
    std::vector<class MeshComponent*> m_MeshComps;
    std::unordered_map<std::string, class Mesh*> m_Meshes;

    std::unordered_map<std::string, class Shader*> m_Shaders;
    // std::unordered_map<std::string, ShaderDesc> m_ShaderDescs; 
    // std::unordered_map<std::string, class Shader*> m_Shaders;
};