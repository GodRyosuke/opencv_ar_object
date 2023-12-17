#include "Actor/Axis.hpp"

#define _USE_MATH_DEFINES
#include "math.h"

#include "Manager.hpp"
#include "Renderer.hpp"
#include "Definitions.hpp"
#include "Shader.hpp"
#include "Component/MeshComponent.hpp"

Axis::Axis(Manager* manager)
    :Actor(manager, "axis")
{
    Shader::ShaderDesc desc({
        "AxisShader",
        {
        std::string(SHADER_PATH) + "mesh.vert",
        std::string(SHADER_PATH) + "axis.frag"
        // std::string(SHADER_PATH) + "mesh.vert",
        // std::string(SHADER_PATH) + "unitychan.frag"
        },
        Shader::ShaderDesc::VERTEX_FRAGMENT
    });
    // mesh
    Shader* shader = m_Manager->m_Renderer->GetShader(desc);
    m_MeshComp = new MeshComponent(
        this,
        std::string(ASSET_PATH) + "axis/axis3.fbx",
        shader
    );

    SetScale(2.f);
    
}