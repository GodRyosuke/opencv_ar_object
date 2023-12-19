#version 330 core

// 位置だけ受け取って描画

layout (location = 0) in vec3 Position;                                              
layout (location = 1) in vec3 Normal; 
layout (location = 2) in vec2 TexCoord; 
layout (location = 3) in vec3 Tangent; 
layout (location = 4) in ivec4 BoneIDs;
layout (location = 5) in vec4 Weights;

uniform mat4 ModelTransform;
uniform mat4 CameraView;
uniform mat4 CameraProj;
// SpotLight用
uniform mat4 LightView;
// uniform mat4 OrthoLightViewProj;
uniform mat4 OrthoLightProj;
// uniform mat4 LightProj;
uniform mat4 uMatrixPalette[150];
const int NUM_CASCADES = 3;
uniform mat4 CropMats[NUM_CASCADES];
uniform mat4 OrthoLightProjs[NUM_CASCADES];
// out float ClipSpacePosZ;
// out vec4 LightSpacePos[NUM_CASCADES];
// out vec4 PersLightSpacePos;         
// out vec4 OrthoLightSpacePos;
// out vec4 LightSpacePos0;
out vec2 TexCoord0;                                                                 
out vec3 Normal0;                                                                   
out vec3 Tangent0;                                                                   
out vec3 WorldPos0; 
// out vec4 vPos;

void main()
{
	mat4 BoneTransform = uMatrixPalette[BoneIDs[0]] * Weights[0];
		BoneTransform += uMatrixPalette[BoneIDs[1]] * Weights[1];
		BoneTransform += uMatrixPalette[BoneIDs[2]] * Weights[2];
		BoneTransform += uMatrixPalette[BoneIDs[3]] * Weights[3];

	vec4 PosL = BoneTransform * vec4(Position, 1.0);
	vec4 NormalL = BoneTransform * vec4(Normal, 0.0);
	vec4 TangentL = BoneTransform * vec4(Tangent, 0.0);

	gl_Position = CameraProj * CameraView * ModelTransform * PosL;
	// vPos = CameraView * ModelTransform * vec4(PosL, 1.0);

	// gl_Position = vec4(Position, 1.0);
	// LightSpacePos = CameraProj * LightView * ModelTransform * vec4(Position, 1.0);                                 
    // WorldPos0 = (ModelTransform * vec4(Position, 1.0)).xyz;                          
    WorldPos0 = (ModelTransform * PosL).xyz;           
	Normal0 = (ModelTransform * NormalL).xyz;               
	// Normal0 = (ModelTransform * vec4(Normal, 0.0)).xyz;                            
	TexCoord0 = TexCoord;
	Tangent0 = (ModelTransform * TangentL).xyz;

	// Directional Light
	// OrthoLightSpacePos = OrthoLightViewProj * ModelTransform * vec4(Position, 1.0);
	// Spot Light
	// PersLightSpacePos = CameraProj * LightView * ModelTransform * vec4(Position, 1.0);
	// LightSpacePos0 = OrthoLightProj * LightView * ModelTransform * vec4(Position, 1.0);
	// for (int i = 0 ; i < NUM_CASCADES ; i++) {
    //     LightSpacePos[i] = CropMats[i] * OrthoLightProjs[i] * LightView * ModelTransform * vec4(Position, 1.0);
    // }
	// ClipSpacePosZ = gl_Position.z;

}
