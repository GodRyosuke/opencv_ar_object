#include "Actor/UnityChan.hpp"

#include "Manager.hpp"
#include "Renderer.hpp"
#include "Definitions.hpp"
#include "Shader.hpp"
#include "Component/MeshComponent.hpp"

UnityChan::UnityChan(Manager* manager)
    :Actor(manager, "UnityChan")
{
    // shader
    // Shader::ShaderDesc desc({
    //     "GridShader",
    //     {
    //     std::string(SHADER_PATH) + "grid.vert",
    //     std::string(SHADER_PATH) + "grid.frag"
    //     // std::string(SHADER_PATH) + "mesh.vert",
    //     // std::string(SHADER_PATH) + "unitychan.frag"
    //     },
    //     Shader::ShaderDesc::VERTEX_FRAGMENT
    // });
    
    Shader::ShaderDesc desc({
        "UnityChanShader",
        {
        std::string(SHADER_PATH) + "mesh.vert",
        std::string(SHADER_PATH) + "unitychan.frag"
        },
        Shader::ShaderDesc::VERTEX_FRAGMENT
    });
    // mesh
    m_MeshComp = new MeshComponent(
        this,
        std::string(ASSET_PATH) + "unitychan/unitychan.fbx",
        m_Manager->m_Renderer->GetShader(desc)
    );

    {
        glm::mat4 rot = glm::mat4(1.f);
        rot = glm::rotate(glm::mat4(1.0f), (float)M_PI / 2.0f, glm::vec3(1.0f, 0.0f, 0.0f));
        rot *= glm::rotate(glm::mat4(1.0f), -(float)M_PI / 2.0f, glm::vec3(0.0f, 1.0f, 0.0f));
        SetRotation(rot);
    }
    SetScale(0.01f);
    SetPosition(glm::vec3(0.f, -2.f, 0.f));
    // SetScale(0.01f);
}