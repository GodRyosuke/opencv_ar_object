#version 330 core

// 位置だけ受け取って描画

layout (location = 0) in vec3 Position;                                              
layout (location = 1) in vec3 Normal; 
layout (location = 2) in vec2 TexCoord; 
layout (location = 3) in vec3 Tangent; 


uniform mat4 ModelTransform;
uniform mat4 CameraView;
uniform mat4 CameraProj;
// SpotLight用
uniform mat4 LightView;
uniform mat4 OrthoLightProj;
// uniform mat4 LightProj;
const int NUM_CASCADES = 3;
uniform mat4 CropMats[NUM_CASCADES];
uniform mat4 OrthoLightProjs[NUM_CASCADES];
// out float ClipSpacePosZ;

// out vec4 LightSpacePos[NUM_CASCADES];
// out vec4 LightSpacePos0;         
// out vec4 OrthoLightSpacePos;                                                    
out vec2 TexCoord0;                                                                 
out vec3 Normal0;                                                                   
out vec3 Tangent0;                                                                   
out vec3 WorldPos0; 
// out vec4 vPos;

void main()
{
	gl_Position = CameraProj * CameraView * ModelTransform * vec4(Position, 1.0);
	// vPos = CameraView * ModelTransform * vec4(Position, 1.0);
    WorldPos0 = (ModelTransform * vec4(Position, 1.0)).xyz;                          
	Normal0 = (ModelTransform * vec4(Normal, 0.0)).xyz;                            
	TexCoord0 = TexCoord;
	Tangent0 = (ModelTransform * vec4(Tangent, 0.0)).xyz;

	// Directional Light
	// LightSpacePos0 = OrthoLightProj * LightView * ModelTransform * vec4(Position, 1.0);
	// for (int i = 0 ; i < NUM_CASCADES ; i++) {
    //     LightSpacePos[i] = CropMats[i] * OrthoLightProjs[i] * LightView * ModelTransform * vec4(Position, 1.0);
    // }
	// ClipSpacePosZ = gl_Position.z;
	// Spot Light
	// PersLightSpacePos = CameraProj * LightView * ModelTransform * vec4(Position, 1.0);
}
