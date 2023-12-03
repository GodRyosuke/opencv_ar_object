#version 450

uniform sampler2D gDiffuseTex;                                                                 
in vec2 TexCoord0;
out vec4 FragColor;


void main()
{
	// Sample color from texture
    FragColor = texture(gDiffuseTex, TexCoord0);
    // FragColor = vec4(colorVec.r, colorVec.r, colorVec.r, 1.0);
    // FragColor = texture(gDiffuseTex, TexCoord0);
}