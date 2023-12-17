#include "Component/SkinMeshComponent.hpp"

#include "Actor/Actor.hpp"
#include "Shader.hpp"
#include "Skeleton.hpp"
#include "Animation.hpp"
#include "Mesh.hpp"
#include "Manager.hpp"

SkinMeshComponent::SkinMeshComponent(Actor* owner, std::string mesh_path, Shader* shader, bool isSkeletal)
    :MeshComponent(owner, mesh_path, shader, true)
    ,mAnimation(nullptr)
    ,mAnimTime(0.f)
{
}

void SkinMeshComponent::SetBoneMatrices(class Shader* shader)
{
    for (int i = 0; i < mMatrixPallete.size(); i++) {
        std::string uniformName = "uMatrixPalette[" + std::to_string(i) + ']';
        shader->SetMatrixUniform(uniformName, mMatrixPallete[i]);
    }
}

void SkinMeshComponent::Update()
{
    if (mAnimation && m_Mesh->GetSkeleton())
    {
        mAnimTime += m_Owner->GetManager()->GetDeltaTime() * mAnimPlayRate;
        // Wrap around anim time if past duration
        // float duration = mAnimation->GetDuration(mAnimIdx);
        float animTicks = mAnimation->GetAnimTicks(mAnimTime, mAnimIdx);
        mAnimTicks = fmod(animTicks, mDuration - 1);
        mAnimTicks++;

        //while (animTicks > duration)
        //{
        //    animTicks -= duration;
        //}

        // Recompute matrix palette
        ComputeMatrixPalette();
    }
}

float SkinMeshComponent::PlayAnimation(const Animation* anim, int animIdx, float playRate)
{
    mAnimation = anim;
    mAnimTime = 0.0f;
    mAnimPlayRate = playRate;
    mAnimIdx = animIdx;
    if (!mAnimation) { return 0.0f; }

    mMatrixPallete.resize(m_Mesh->GetSkeleton()->GetNumBones());
    for (int i = 0; i < mMatrixPallete.size(); i++) {
        mMatrixPallete[i] = glm::mat4(1.f);
    }
    ComputeMatrixPalette();
    mDuration = mAnimation->GetDuration(animIdx);
    return mDuration;
}

void SkinMeshComponent::ComputeMatrixPalette()
{
    //const std::vector<Matrix4>& globalInvBindPoses = mSkeleton->GetGlobalInvBindPoses();
    //std::vector<Matrix4> currentPoses;
    if ((mAnimTicks < 1.f) || (mDuration < mAnimTicks)) {
        mAnimTicks = 1.f;
    }
    mAnimation->GetGlobalPoseAtTime(mMatrixPallete, m_Mesh->GetSkeleton(), mAnimTicks, mAnimIdx);
    //for (int i = 0; i < mMatrixPallete.size(); i++) {
    //    mMatrixPallete[i] = glm::mat4(1.f);
    //}
    // Setup the palette for each bone
    //for (size_t i = 0; i < mSkeleton->GetNumBones(); i++)
    //{
    //    // Global inverse bind pose matrix times current pose matrix
    //    mPalette.mEntry[i] = globalInvBindPoses[i] * currentPoses[i];
    //}
}


//void SkinMeshComponent::Draw(Shader* shader)
//{
//    if (mMesh)
//    {
//        // Set the world transform
//        shader->SetMatrixUniform("ModelTransform",
//            mOwner->GetWorldTransform());
//        // Set the matrix palette
//        //shader->SetMatrixUniforms("uMatrixPalette", &mPalette.mEntry[0],
//        //    MAX_NUM_BONES_PER_VERTEX);
//        // Set specular power
//        shader->SetFloatUniform("uSpecPower", mMesh->GetSpecPower());
//        // Set the active texture
//        Texture* t = mMesh->GetTexture(mTextureIndex);
//        if (t)
//        {
//            t->SetActive();
//        }
//        // Set the mesh's vertex array as active
//        VertexArray* va = mMesh->GetVertexArray();
//        va->SetActive();
//        // Draw
//        glDrawElements(GL_TRIANGLES, va->GetNumIndices(), GL_UNSIGNED_INT, nullptr);
//    }
//}

