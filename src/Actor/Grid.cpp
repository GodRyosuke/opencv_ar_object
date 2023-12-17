#include "Actor/Grid.hpp"

#define _USE_MATH_DEFINES
#include "math.h"

#include "Manager.hpp"
#include "Renderer.hpp"
#include "Definitions.hpp"
#include "Shader.hpp"
#include "Component/MeshComponent.hpp"

Line::Line(Manager* manager, std::string lineName)
    :Actor(manager, lineName)
{
    Shader::ShaderDesc desc({
        "GridShader",
        {
        std::string(SHADER_PATH) + "mesh.vert",
        std::string(SHADER_PATH) + "grid.frag"
        // std::string(SHADER_PATH) + "mesh.vert",
        // std::string(SHADER_PATH) + "unitychan.frag"
        },
        Shader::ShaderDesc::VERTEX_FRAGMENT
    });
    // mesh
    Shader* shader = m_Manager->m_Renderer->GetShader(desc);
    m_MeshComp = new MeshComponent(
        this,
        std::string(ASSET_PATH) + "grid/grid.obj",
        shader
    );
}

Grid::Grid(Manager* manager)
    :Actor(manager, "grid")
{
    const int numY = 21;
    const int numX = 21;
    // for (int y = 0; y < numY; y++) {
    //     Actor* a = new Line(manager);
    //     a->SetPosition(glm::vec3(0.0, y, 0.0));
    //     a->SetScale(10.f);
    // }
    for (int x = 0; x < numX; x++) {
        Actor* a = new Line(manager, "xline" + std::to_string(x));
        int half = numX / 2;
        a->SetPosition(glm::vec3(x, 0.0, 0.0) - glm::vec3(half, 0.0, 0.0));
        a->SetScale(20.f);
    }
    for (int y = 0; y < numY; y++) {
        Actor* a = new Line(manager, "yline" + std::to_string(y));
        int half = numY / 2;
        a->SetPosition(glm::vec3(0.0, y, 0.0) - glm::vec3(0.0, half, 0.0));
        a->SetScale(20.f);
        glm::mat4 rot = glm::mat4(1.f);
        rot = glm::rotate(glm::mat4(1.0f), (float)M_PI / 2.0f, glm::vec3(0.0f, 0.0f, 1.0f));
        a->SetRotation(rot);
    }

}

// Grid::~Grid()
// {
//     while (!m_Lines.empty())
//     {
//         delete m_Lines.back();
//     }
// }