#include "Actor/Grid.hpp"

#include "Manager.hpp"
#include "Renderer.hpp"
#include "Definitions.hpp"
#include "Shader.hpp"
#include "Component/MeshComponent.hpp"

Grid::Grid(Manager* manager)
    :Actor(manager, "grid")
{
    Shader::ShaderDesc desc({
        "GridShader",
        {
        std::string(SHADER_PATH) + "grid.vert",
        std::string(SHADER_PATH) + "grid.frag"
        // std::string(SHADER_PATH) + "mesh.vert",
        // std::string(SHADER_PATH) + "unitychan.frag"
        },
        Shader::ShaderDesc::VERTEX_FRAGMENT
    });
    // mesh
    m_MeshComp = new MeshComponent(
        this,
        std::string(ASSET_PATH) + "grid/grid2.obj",
        m_Manager->m_Renderer->GetShader(desc)
    );

    SetScale(10.f);
}