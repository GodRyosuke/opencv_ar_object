#include "Mesh.hpp"

#include <iostream>
#include <fstream>

#include "json.hpp"

#include "Texture.hpp"
#include "Util.hpp"
#include "VertexArray.hpp"
#include "Shader.hpp"
#include "Skeleton.hpp"
// #include "json.hpp"
#include "Renderer.hpp"

namespace nl = nlohmann;

namespace {
Texture* LoadTextureFromJson(nl::json data, std::string texRootPath, std::string itemName)
{
    auto iter = data.find(itemName);
    if (iter == data.end()) {
        return nullptr;
    }

    std::string texFileName = *iter;
    std::string texFilePath = texRootPath + texFileName;
    Texture* tex = new Texture();
    if (!tex->Load(texFilePath)) {
        return nullptr;
    }
    return tex;
    // return new Texture(texFilePath);
}
}

Mesh::Mesh()
{}

Mesh::~Mesh()
{
    delete m_VAO;
}

bool Mesh::Load(std::string filePath, bool isSkeletal)
{
    if (!isSkeletal) {
        m_Skeleton = nullptr;
    }
    else {
        // ボーンの読み出し
        m_Skeleton = new Skeleton();
    }
    m_MeshName = filePath;

    // Meshを読んでいく
    m_pScene = m_Importer.ReadFile(filePath.c_str(), ASSIMP_LOAD_FLAGS);
    if (!m_pScene) {
        Util::Print("error: failed to parse ", filePath.c_str(), ": ", m_Importer.GetErrorString());
        // printf("Error parsing '%s': '%s'\n", filePath.c_str(), m_Importer.GetErrorString());
        return false;
    }

    if (isSkeletal) {
        glm::mat4 globalInvTrans = Util::ToGlmMat4(m_pScene->mRootNode->mTransformation);
        globalInvTrans = glm::inverse(globalInvTrans);
        m_Skeleton->SetGlobalInvTrans(globalInvTrans);
    }

    //GetGlobalInvTrans();

    m_SubMeshes.resize(m_pScene->mNumMeshes);
    // m_Materials.resize(m_pScene->mNumMaterials);
    unsigned int numVertices = 0;
    unsigned int numIndices = 0;
    // m_NumVertices = 0;
    // m_NumIndices = 0;
    
    // Sub Meshの読み込み
    for (unsigned int i = 0; i < m_SubMeshes.size(); i++) {
        m_SubMeshes[i] = new BasicMeshEntry({
            m_pScene->mMeshes[i]->mNumFaces * 3,    // num indices
            numVertices,                            // base vertex
            numIndices,                             // base index
            m_pScene->mMeshes[i]->mMaterialIndex    // material index
        });
        // m_SubMeshes[i].MaterialIndex = m_pScene->mMeshes[i]->mMaterialIndex; // MeshとMaterialの紐づけ
        // m_SubMeshes[i].NumIndices = m_pScene->mMeshes[i]->mNumFaces * 3;
        // m_SubMeshes[i].BaseVertex = mNumVertices;
        // m_SubMeshes[i].BaseIndex = mNumIndices;

        numVertices += m_pScene->mMeshes[i]->mNumVertices;
        numIndices += m_SubMeshes[i]->NumIndices;
    }
    m_NumVertices = numVertices;    // 全頂点数
    m_NumIndices = numIndices;      // 全頂点インデックス数

    m_Positions.reserve(m_NumVertices);
    m_Normals.reserve(m_NumVertices);
    m_TexCoords.reserve(m_NumVertices);
    m_Indices.reserve(m_NumIndices);
    if (isSkeletal) {
        m_Skeleton->ReserveBoneSpace(m_NumVertices);
    }

    // 頂点情報などの読み込み
    for (int meshIdx = 0; meshIdx < m_SubMeshes.size(); meshIdx++) {
        const aiMesh* paiMesh = m_pScene->mMeshes[meshIdx];
        Util::Printf("mesh name: %s, mesh idx: %d\n", paiMesh->mName.C_Str(), meshIdx);
        LoadMesh(paiMesh, meshIdx);
        
        if (isSkeletal) {
            bool result = m_Skeleton->Load(paiMesh, meshIdx, m_SubMeshes[meshIdx]->BaseVertex);
            if (!result) {
                Util::Print("error: failed to load skeleton\n");
                return false;
            }
        }
    }

    // MaterialとTexture読み込み
    Util::Printf("Num materials: %d\n", m_pScene->mNumMaterials);
    // Material.jsonの読み込み
    nl::json materialJsonMap;
    std::string objFileRoot = "";
    {
        std::vector<std::string>dir_names = Util::Split(filePath, "/");
        for (int i = 0; i < dir_names.size() - 1; i++) {
            objFileRoot += dir_names[i] + '/';
        }
        std::string jsonFilePath = objFileRoot + "Material.json";
        std::ifstream ifs(jsonFilePath.c_str());
        if (ifs.good())
        {
            ifs >> materialJsonMap;
        }
        else {
            Util::Printf("warn: failed to load material.josn\n");
            // return false;
        }
        ifs.close();
    }

    // materialの読み込み
    m_Policies.resize(m_pScene->mNumMaterials);
    for (int materialIdx = 0; materialIdx < m_pScene->mNumMaterials; materialIdx++) {
        const aiMaterial* pMaterial = m_pScene->mMaterials[materialIdx];

        std::string materialName = pMaterial->GetName().data;
        // Diffuse Textureを読み込む
        Material* material = new Material();
        material->DiffuseTexture = LoadTextureFromJson(materialJsonMap[materialName], objFileRoot + "Textures/", "Diffuse");
        material->NormalTexture = LoadTextureFromJson(materialJsonMap[materialName], objFileRoot + "Textures/", "Normal");
        material->MetallicTexture = LoadTextureFromJson(materialJsonMap[materialName], objFileRoot + "Textures/", "Metal");
        material->RoughnessTexture = LoadTextureFromJson(materialJsonMap[materialName], objFileRoot + "Textures/", "Roughness");
        material->AOTexture = LoadTextureFromJson(materialJsonMap[materialName], objFileRoot + "Textures/", "AO");

        BindTexturePolicy* policy = new BindTexturePolicy(material);

        // materials.jsonからテクスチャのファイルパスを読み込む
        // std::string diffuseTextureFilePath = "";
        // std::string normalTextureFilePath = "";
        // std::string metallicTextureFilePath = "";
        // std::string roughnessTextureFilePath = "";
        // std::string aoTextureFilePath = "";
        // diffuseTextureFilePath = materialJsonMap[materialName]["Diffuse"];
        // normalTextureFilePath = materialJsonMap[materialName]["Normal"];
        // metallicTextureFilePath = materialJsonMap[materialName]["Metal"];
        // roughnessTextureFilePath = materialJsonMap[materialName]["Roughness"];
        // aoTextureFilePath = materialJsonMap[materialName]["AO"];

        BindDiffuseTextureRule* bindDiffuseTextureRule = new BindDiffuseTextureRule(policy);

        aiColor3D DiffuseColor(0.0f, 0.0f, 0.0f);
        if (pMaterial->Get(AI_MATKEY_COLOR_DIFFUSE, DiffuseColor) == AI_SUCCESS) {
            Util::Printf("Loaded diffuse color [%f %f %f]\n", DiffuseColor.r, DiffuseColor.g, DiffuseColor.b);
            material->DiffuseColor.r = DiffuseColor.r;
            material->DiffuseColor.g = DiffuseColor.g;
            material->DiffuseColor.b = DiffuseColor.b;
            // bindDiffuseTextureRule->SetMaterialDiffuseColor(glm::vec3(DiffuseColor.r, DiffuseColor.g, DiffuseColor.b));
        }

        aiColor3D SpecularColor(0.0f, 0.0f, 0.0f);
        if (pMaterial->Get(AI_MATKEY_COLOR_SPECULAR, SpecularColor) == AI_SUCCESS) {
            Util::Printf("Loaded specular color [%f %f %f]\n", SpecularColor.r, SpecularColor.g, SpecularColor.b);
            material->SpecColor.r = SpecularColor.r;
            material->SpecColor.g = SpecularColor.g;
            material->SpecColor.b = SpecularColor.b;
        }

        ai_real Shiness(0.f);
        if (pMaterial->Get(AI_MATKEY_SHININESS, Shiness) == AI_SUCCESS) {
            material->SpecPower = Shiness;
        }

        policy->SetBindRule(bindDiffuseTextureRule);
        // policy->SetBindRule(new BindNormalTextureRule(LoadTextureFromPath(normalTextureFilePath)));
        // policy->SetBindRule(new BindMetallicTextureRule(LoadTextureFromPath(metallicTextureFilePath)));
        // policy->SetBindRule(new BindRoughnessTextureRule(LoadTextureFromPath(roughnessTextureFilePath)));
        // policy->SetBindRule(new BindAOTextureRule(LoadTextureFromPath(aoTextureFilePath)));
        m_Policies[materialIdx] = policy;
    }

    if (!isSkeletal) {
        m_VAO = new VertexArray(m_Positions, m_Normals, m_TexCoords, m_Tangents, m_Indices);
    }
    else {
        m_VAO = new VertexArray(m_Positions, m_Normals, m_TexCoords, m_Tangents, m_Indices, m_Skeleton);
    }

    return true;
}

void Mesh::LoadMesh(const aiMesh* pMesh, unsigned int meshIdx)
{
    const aiVector3D Zero3D(0.0f, 0.0f, 0.0f);

    // Vertex, Normal, UV取得
    for (unsigned int i = 0; i < pMesh->mNumVertices; i++) {

        const aiVector3D& pPos = pMesh->mVertices[i];
        m_Positions.push_back(glm::vec3(pPos.x, pPos.y, pPos.z));

        if (pMesh->mNormals) {
            const aiVector3D& pNormal = pMesh->mNormals[i];
            m_Normals.push_back(glm::vec3(pNormal.x, pNormal.y, pNormal.z));
        }
        else {
            aiVector3D Normal(0.0f, 1.0f, 0.0f);
            m_Normals.push_back(glm::vec3(Normal.x, Normal.y, Normal.z));
        }

        const aiVector3D& pTexCoord = pMesh->HasTextureCoords(0) ? pMesh->mTextureCoords[0][i] : Zero3D;
        m_TexCoords.push_back(glm::vec2(pTexCoord.x, pTexCoord.y));
    }

    // Index 情報取得
    for (unsigned int i = 0; i < pMesh->mNumFaces; i++) {
        const aiFace& Face = pMesh->mFaces[i];
        //Util::Printf("num indices %d\n", Face.mNumIndices);
        assert(Face.mNumIndices == 3);
        m_Indices.push_back(Face.mIndices[0]);
        m_Indices.push_back(Face.mIndices[1]);
        m_Indices.push_back(Face.mIndices[2]);
    }

    // Tangent 情報取得
    m_Tangents.resize(m_Positions.size());
    for (int i = 0; i < m_Positions.size(); i++) {
        m_Tangents[i] = glm::vec3(0.f); // 初期化
    }
    for (int i = 0; i < m_Indices.size(); i+= 3) {
        glm::vec3 v0 = m_Positions[m_Indices[i]];
        glm::vec3 v1 = m_Positions[m_Indices[i+1]];
        glm::vec3 v2 = m_Positions[m_Indices[i+2]];
        glm::vec3& tan0 = m_Tangents[m_Indices[i]];
        glm::vec3& tan1 = m_Tangents[m_Indices[i + 1]];
        glm::vec3& tan2 = m_Tangents[m_Indices[i + 2]];
        glm::vec2 uv0 = m_TexCoords[m_Indices[i]];
        glm::vec2 uv1 = m_TexCoords[m_Indices[i + 1]];
        glm::vec2 uv2 = m_TexCoords[m_Indices[i + 2]];

        glm::vec3 Edge1 = v1 - v0;
        glm::vec3 Edge2 = v2 - v0;

        float DeltaU1 = uv1.x - uv0.x;
        float DeltaV1 = uv1.y - uv0.y;
        float DeltaU2 = uv2.x - uv0.x;
        float DeltaV2 = uv2.y - uv0.y;

        float f = 1.0f / (DeltaU1 * DeltaV2 - DeltaU2 * DeltaV1);

        glm::vec3 Tangent, Bitangent;

        Tangent.x = f * (DeltaV2 * Edge1.x - DeltaV1 * Edge2.x);
        Tangent.y = f * (DeltaV2 * Edge1.y - DeltaV1 * Edge2.y);
        Tangent.z = f * (DeltaV2 * Edge1.z - DeltaV1 * Edge2.z);

        Bitangent.x = f * (-DeltaU2 * Edge1.x + DeltaU1 * Edge2.x);
        Bitangent.y = f * (-DeltaU2 * Edge1.y + DeltaU1 * Edge2.y);
        Bitangent.z = f * (-DeltaU2 * Edge1.z + DeltaU1 * Edge2.z);

        tan0 += Tangent;
        tan1 += Tangent;
        tan2 += Tangent;
    }

    for (int i = 0; i < m_Tangents.size(); i++) {
        m_Tangents[i] = glm::normalize(m_Tangents[i]);
    }
}

const class BindTexturePolicy* Mesh::GetMeshEntry(const int subMeshIdx, unsigned int& numIndices,
    unsigned int& baseVertex, unsigned int& baseIndex) const
{
    numIndices = m_SubMeshes[subMeshIdx]->NumIndices;
    baseVertex = m_SubMeshes[subMeshIdx]->BaseVertex;
    baseIndex = m_SubMeshes[subMeshIdx]->BaseIndex;
    unsigned int materialIdx = m_SubMeshes[subMeshIdx]->MaterialIndex;
    return m_Policies[materialIdx];
    // mat = m_Materials[materialIdx];
}


BindTexturePolicy::BindTexturePolicy(Mesh::Material* material)
    :m_Material(material)
{}

BindTexturePolicy::~BindTexturePolicy()
{
    while (!mRules.empty())
	{
		delete mRules.back();	
	}
}

void BindTexturePolicy::BindAllTexture(Shader* shader) const
{
    for (auto rule : mRules) {
        rule->Bind(shader);
    }
}

BindTextureRule::BindTextureRule(const BindTexturePolicy* policy)
    :m_OwnerPolicy(policy)
{}

// BindDiffuseTextureRule::BindDiffuseTextureRule(Texture* tex)
//     :BindTextureRule(tex)
// {}

// BindTextureRule::BindTextureRule()
//     :mTexture(nullptr)
// {}

BindDiffuseTextureRule::BindDiffuseTextureRule(const BindTexturePolicy* policy)
    :BindTextureRule(policy)
{
}

// BindDiffuseTextureRule::BindDiffuseTextureRule(const Texture* tex)
//     :BindTextureRule(tex)
// {}

// BindDiffuseTextureRule::BindDiffuseTextureRule(std::string filePath, Renderer* renderer)
//     :mMaterialColor(glm::vec3(0.f))
// {
//     if (filePath.length()) {
//         SetTexture(renderer->GetTexture(filePath));
//         return;
//     }
//     SetTexture(nullptr);
// }

void BindDiffuseTextureRule::Bind(Shader* shader) const
{
    shader->UseProgram();
    // Albedo Textureがあればそれを用いる
    Texture* diffuseTex = m_OwnerPolicy->GetMaterial()->DiffuseTexture;
    if (diffuseTex) {
        shader->SetSamplerUniform("useDiffuse", 1);
        shader->SetSamplerUniform("gDiffuseTex", 0);
        diffuseTex->BindTexture();
        return;
    }
    // なければマテリアルの色を使う
    shader->SetSamplerUniform("useDiffuse", 0);
    shader->SetVectorUniform("matDiffuseColor", m_OwnerPolicy->GetMaterial()->DiffuseColor);
}
