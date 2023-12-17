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
    Shader::ShaderDesc desc({
        "SpriteShader",
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

    SetPosition(glm::vec3(0.f, -2.f, 0.f));
    // SetScale(0.01f);
}