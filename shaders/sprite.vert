#version 450 core

// 位置だけ受け取って描画

layout (location = 0) in vec3 Position;                                              
layout (location = 1) in vec2 TexCoord; 


uniform mat4 ModelTransform;
uniform mat4 SpriteViewProj;
// uniform mat4 CameraView;
// uniform mat4 CameraProj;

out vec2 TexCoord0;

void main()
{
	gl_Position = SpriteViewProj * ModelTransform * vec4(Position, 1.0);
    // WorldPos0 = (ModelTransform * vec4(Position, 1.0)).xyz;                          
	// Normal0 = (ModelTransform * vec4(Normal, 0.0)).xyz;                            
	TexCoord0 = TexCoord;
	// Tangent0 = (ModelTransform * vec4(Tangent, 0.0)).xyz;

	// Directional Light
	// LightSpacePos0 = OrthoLightProj * LightView * ModelTransform * vec4(Position, 1.0);
	// for (int i = 0 ; i < NUM_CASCADES ; i++) {
    //     LightSpacePos[i] = OrthoLightProjs[i] * LightView * ModelTransform * vec4(Position, 1.0);
    // }
	// ClipSpacePosZ = gl_Position.z;
	// Spot Light
	// PersLightSpacePos = CameraProj * LightView * ModelTransform * vec4(Position, 1.0);
}