#pragma once

#include "Component.hpp"
// #include "btBulletDynamicsCommon.h"

class MeshComponent : public Component {
public:
    MeshComponent(class Actor* owner, std::string mesh_path, class Shader* shader, bool isSkeletal = false);
    virtual void Draw();
    // void DrawShadow();

    // void SetMesh(const class wMesh* mesh) { mwMesh = mesh; }
    // void SetMesh(const class Mesh* mesh) { mMesh = mesh; }
    // void SetUseNormalMap(bool frag) { mUseNormalMap = frag; }
    // void SetShader(class Shader* shader) { mShader = shader; }
    // void SetShadowShader(class Shader* shader) { mShadowShader = shader;}
    // bool GetIsSkeletal() const { return mIsSkeletal; }
    // bool GetUseNormalMap() const { return mUseNormalMap; }
    // bool IsCastShadow() { return mIsCastShadow; }
    const bool m_IsSkeletal;
    // btConvexHullShape* GetConvexShape();
    
protected:
    virtual void SetBoneMatrices(class Shader* shader) {}

    // const class wMesh* mwMesh;
    // std::vector<BindTexturePolicy*> mBindTexturePolicies;
    const class Mesh* m_Mesh;
    // bool mIsSkeletal;
    // bool mUseNormalMap;
    // bool mIsCastShadow; 
    class Shader* m_Shader;
    // class Shader* m_ShadowShader;
};
