#pragma once

#include "Component/MeshComponent.hpp"
#include <vector>
#include "glm/glm.hpp"

class SkinMeshComponent : public MeshComponent
{
public:
    SkinMeshComponent(class Actor* owner, std::string mesh_path, class Shader* shader, bool isSkeletal = false);
    //void Draw(class Shader* shader) override;

    void Update() override;


    // Play an animation. Returns the length of the animation
    float PlayAnimation(const class Animation* anim, int animIdx, float playRate = 1.0f);
private:
    void SetBoneMatrices(class Shader* shader) override;
    void ComputeMatrixPalette();

    std::vector<glm::mat4> mMatrixPallete;
    //const class Skeleton* mSkeleton;
    const class Animation* mAnimation;
    float mAnimPlayRate;
    float mAnimTime;
    float mAnimTicks;
    float mDuration;
    int mAnimIdx;
};
