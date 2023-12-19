#pragma once

#include <string>
#include <vector>
#include <unordered_map>

#include "glm/glm.hpp"
#include <assimp/Importer.hpp>      // C++ importer interface
#include <assimp/scene.h>       // Output data structure
#include <assimp/postprocess.h> // Post processing flags

#define MAX_NUM_BONES_PER_VERTEX 4
#define ASSIMP_LOAD_FLAGS (aiProcess_Triangulate | aiProcess_GenSmoothNormals |  aiProcess_JoinIdenticalVertices )
#define INVALID_MATERIAL 0xFFFFFFFF

class Mesh {
public:
    struct Material {
        glm::vec3 AmbientColor;
        glm::vec3 DiffuseColor;
        glm::vec3 SpecColor;
        float SpecPower;
        float Alpha;
        glm::vec3 Reflection;
        glm::vec3 Emissive;
        glm::vec3 Bump;
        glm::vec3 NormalMap;
        class Texture* DiffuseTexture;
        class Texture* NormalTexture;
        class Texture* MetallicTexture;
        class Texture* RoughnessTexture;
        class Texture* AOTexture;
    };

    Mesh();
    // Mesh(class Renderer* renderer);
    ~Mesh();
    bool Load(std::string filePath, bool isSkeletal = false);
    void Draw(class Shader* shader, float timeInSeconds) const;

    // void SetMeshPos(glm::vec3 pos) { mMeshPos = pos; }
    // void SetMeshRotate(glm::mat4 rot) { mMeshRotate = rot; }
    // void SetMeshScale(float scale) { mMeshScale = scale; }
    // glm::mat4 GetWorldMat();
    class VertexArray* GetVertexArray() const { return m_VAO; }
    const int GetSubMeshNum() const { return m_SubMeshes.size(); }
    // void GetMeshEntry(const int subMeshIdx, unsigned int& numIndices,
    //     unsigned int& baseVertex, unsigned int& baseIndex, Material& mat) const;
    const class BindTexturePolicy* GetMeshEntry(const int subMeshIdx, unsigned int& numIndices,
        unsigned int& baseVertex, unsigned int& baseIndex) const;
    // const class BindTexturePolicy* GetMeshEntry(const int subMeshIdx, unsigned int& numIndices,
    //     unsigned int& baseVertex, unsigned int& baseIndex) const;
    const class Skeleton* GetSkeleton () const{ return m_Skeleton; }
    const aiScene* GetaiScene() const { return m_pScene; }
    const std::vector<glm::vec3> GetPositions() const { return m_Positions; }
    
protected:
    struct BasicMeshEntry {
        // BasicMeshEntry()
        // {
        //     NumIndices = 0;
        //     BaseVertex = 0;
        //     BaseIndex = 0;
        //     MaterialIndex = INVALID_MATERIAL;
        // }

        const unsigned int NumIndices;
        const unsigned int BaseVertex;
        const unsigned int BaseIndex;
        const unsigned int MaterialIndex;
    };

    // virtual void PopulateBuffers();
    // virtual void ReserveVertexSpace();
    virtual void LoadMesh(const aiMesh* pMesh, unsigned int meshIdx);
    // virtual void GetGlobalInvTrans() {}
    // virtual void UpdateTransform(class Shader* shader, float timeInSeconds);

    const aiScene* m_pScene;
    Assimp::Importer m_Importer;    // Importer保持せんかったら、Sceneも保持できない!!

    unsigned int m_NumVertices;
    unsigned int m_NumIndices;
    std::vector<const BasicMeshEntry*> m_SubMeshes;


private:
    // const class Texture* LoadTextureFromPath(std::string path);

    unsigned int mVertexArray;


    // std::vector<Material> m_Materials;
    std::vector<glm::vec3> m_Positions;
    std::vector<glm::vec3> m_Normals;
    std::vector<glm::vec3> m_Tangents;
    std::vector<glm::vec2> m_TexCoords;
    std::vector<unsigned int> m_Indices;

    class VertexArray* m_VAO;
    class Skeleton* m_Skeleton;

    class Renderer* mRenderer;

    glm::vec3 mMeshPos;
    glm::mat4 mMeshRotate;
    float mMeshScale;

    std::string m_MeshName;
    // std::string mObjFileRoot;
    // std::string ObjFileName;
    std::vector<const class BindTexturePolicy*> m_Policies;
    std::unordered_map<std::string, class Texture*> m_Textures;
};


class BindTexturePolicy {
public:
    BindTexturePolicy(Mesh::Material* material);
    ~BindTexturePolicy();
    void BindAllTexture(class Shader* shader) const;
    void SetBindRule(class BindTextureRule* rule) { mRules.push_back(rule); }
    const Mesh::Material* GetMaterial() const { return m_Material; }

private:
    std::vector<class BindTextureRule*> mRules;
    Mesh::Material* m_Material;
};

class BindTextureRule
{
public:
    BindTextureRule(const BindTexturePolicy* policy);
    // BindTextureRule(const class Texture* tex);
    virtual void Bind(class Shader* shader) const = 0;
    
protected:
    const BindTexturePolicy* m_OwnerPolicy;
//     void SetTexture(const class Texture* tex) { mTexture = tex; }
//     const class Texture* mTexture;
};

class BindDiffuseTextureRule : public BindTextureRule {
public:
    BindDiffuseTextureRule(const BindTexturePolicy* policy);

    // BindDiffuseTextureRule(const class Texture* tex);
    // BindDiffuseTextureRule(std::string texturePath, Renderer* ren);
    void Bind(class Shader* shader) const override;
    // void SetMaterialDiffuseColor(glm::vec3 color) { mMaterialColor = color; }

// private:
//     glm::vec3 mMaterialColor;
};

// class BindNormalTextureRule : public BindTextureRule {
// public:
//     BindNormalTextureRule(const class Texture* tex);
//     BindNormalTextureRule(std::string texturePath, Renderer* ren);
//     void Bind(class Shader* shader) override;
// };

// class BindMetallicTextureRule : public BindTextureRule {
// public:
//     BindMetallicTextureRule(const class Texture* tex);
//     BindMetallicTextureRule(std::string texturePath, Renderer* ren);
    
//     void Bind(class Shader* shader) override;
// };

// class BindRoughnessTextureRule : public BindTextureRule {
// public:
//     BindRoughnessTextureRule(const class Texture* tex);
//     BindRoughnessTextureRule(std::string texturePath, Renderer* ren);
//     void Bind(class Shader* shader) override;
// };

// class BindAOTextureRule : public BindTextureRule {
// public:
//     BindAOTextureRule(const class Texture* tex);
//     BindAOTextureRule(std::string texturePath, Renderer* ren);
//     void Bind(class Shader* shader) override;
// };

