#include "Component/MeshComponent.hpp"

#include "Definitions.hpp"
#include "Actor/Actor.hpp"
#include "Shader.hpp"
#include "Texture.hpp"
#include "Mesh.hpp"
#include "Manager.hpp"
#include "VertexArray.hpp"
#include "Util.hpp"
#include "Renderer.hpp"


MeshComponent::MeshComponent(class Actor* owner, std::string mesh_path, Shader* shader, bool isSkeletal)
    :Component(owner)
    // ,m_Mesh(nullptr)
    ,m_IsSkeletal(isSkeletal)
    // ,mUseNormalMap(true)
    ,m_Shader(shader)
    // ,mShadowShader(nullptr)
    // ,mIsCastShadow(isCastShadow)
{
    m_Owner->GetManager()->m_Renderer->AddMeshComp(this);
    m_Mesh = m_Owner->GetManager()->m_Renderer->GetMesh(mesh_path, isSkeletal);
}

void MeshComponent::Draw()
{
    assert(m_Shader);
    // m_Shader = m_Owner->GetManager()->m_Renderer->GetShader({
    //     "UnityChanShader",
    //     {
    //     std::string(SHADER_PATH) + "mesh.vert",
    //     std::string(SHADER_PATH) + "unitychan.frag"
    //     },
    //     Shader::ShaderDesc::VERTEX_FRAGMENT
    // });
    m_Shader->UseProgram();
    if (!m_Mesh) {
        return;
    }
    m_Shader->SetMatrixUniform("ModelTransform", m_Owner->GetWorldTransform());

    VertexArray* vao = m_Mesh->GetVertexArray();
    vao->SetActive();
    for (int subMeshIdx = 0; subMeshIdx < m_Mesh->GetSubMeshNum(); subMeshIdx++) {
        unsigned int numIndices;
        unsigned int baseVertex;
        unsigned int baseIndex;
        // Mesh::Material material;
        const BindTexturePolicy* policy = m_Mesh->GetMeshEntry(subMeshIdx, numIndices, baseVertex, baseIndex);

        // m_Mesh->GetMeshEntry(subMeshIdx, numIndices, baseVertex, baseIndex, material);
        // m_Mesh->GetMeshEntry(subMeshIdx, numIndices, baseVertex, baseIndex, policy);
        
        // Bone Matrix設定
        SetBoneMatrices(m_Shader);

        // Material設定
        // m_Shader->SetVectorUniform("matAmbientColor", material.AmbientColor);
        //m_Shader->SetVectorUniform("uDirLight.mDirection", glm::vec3(0, -0.707, -0.707));
        // m_Shader->SetVectorUniform("matDiffuseColor", material.DiffuseColor);
        //m_Shader->SetVectorUniform("uDirLight.mSpecColor", m_Materials[MaterialIndex].SpecColor);
        // m_Shader->SetFloatUniform("matSpecPower", material.SpecPower);
        // m_Shader->SetVectorUniform("matSpecColor", material.SpecColor);
        // m_Shader->SetSamplerUniform("useNormalMap", mUseNormalMap);
        //m_Shader->SetFloatUniform("gMatSpecularIntensity", 1.f);
        //m_Shader->SetFloatUniform("gMatSpecularIntensity", 1.0f);

        policy->BindAllTexture(m_Shader);

        // if (material.DiffuseTexture) {
        //     m_Shader->SetSamplerUniform("gDiffuseTex", 0);
        //     material.DiffuseTexture->BindTexture(GL_TEXTURE0);
        // }
        // if (material.NormalTexture) {
        //     m_Shader->SetSamplerUniform("useNormal", 1);
        //     m_Shader->SetSamplerUniform("gNormalMap", 1);
        //     material.NormalTexture->BindTexture(GL_TEXTURE1);
        // }
        // if (material.MetallicTexture) {
        //     m_Shader->SetSamplerUniform("useMetallic", 1);
        //     m_Shader->SetSamplerUniform("metallicMap", 2);
        //     material.MetallicTexture->BindTexture(GL_TEXTURE2);
        // }
        // if (material.RoughnessTexture) {
        //     m_Shader->SetSamplerUniform("roughnessMap", 3);
        //     material.RoughnessTexture->BindTexture(GL_TEXTURE3);
        // }
        // if (material.AOTexture) {
        //     m_Shader->SetSamplerUniform("gAOMap", 5);
        //     material.AOTexture->BindTexture(GL_TEXTURE5);
        // }
        // m_Shader->SetSamplerUniform("gShadowMap", 10);


        // Mesh描画
        glDrawElementsBaseVertex(GL_TRIANGLES,
            numIndices,
            GL_UNSIGNED_INT,
            (void*)(sizeof(unsigned int) * baseIndex),
            baseVertex);
    }
}

// void MeshComponent::DrawShadow()
// {
//     // assert(mShadowShader);
//     // mShadowShader->UseProgram();
//     if (!m_Mesh) {
//         return;
//     }
//     mShadowShader->SetMatrixUniform("ModelTransform", mOwner->GetWorldTransform());

//     VertexArray* vao = m_Mesh->GetVertexArray();
//     vao->SetActive();
//     for (int subMeshIdx = 0; subMeshIdx < m_Mesh->GetSubMeshNum(); subMeshIdx++) {
//         unsigned int numIndices;
//         unsigned int baseVertex;
//         unsigned int baseIndex;
//         Mesh::Material material;
//         m_Mesh->GetMeshEntry(subMeshIdx, numIndices, baseVertex, baseIndex, material);

//         SetBoneMatrices(mShadowShader);

//         // Mesh描画
//         glDrawElementsBaseVertex(GL_TRIANGLES,
//             numIndices,
//             GL_UNSIGNED_INT,
//             (void*)(sizeof(unsigned int) * baseIndex),
//             baseVertex);
//     }
// }

// btConvexHullShape* MeshComponent::GetConvexShape()
// {
//     std::vector<glm::vec3> positions = m_Mesh->GetPositions();
//     btScalar* verticies = new btScalar[positions.size() * 3];
//     for (int i = 0; i < positions.size(); i++) {
//         verticies[i * 3 + 0] = positions[i].x;
//         verticies[i * 3 + 1] = positions[i].y;
//         verticies[i * 3 + 2] = positions[i].z;
//     }

//     return new btConvexHullShape(verticies, positions.size(), sizeof(btScalar) * 3);
// }
