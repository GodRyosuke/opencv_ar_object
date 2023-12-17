#version 330 core                                                                      
                                                                                    
// const int MAX_POINT_LIGHTS = 2;                                                     
// const int MAX_SPOT_LIGHTS = 2;                                                      
                                                                                    
// in vec4 LightSpacePos;
uniform sampler2D gDiffuseTex;                                                                 

in vec2 TexCoord0;                                                                  
in vec3 Normal0;                                                                    
in vec3 Tangent0;                                                                    
in vec3 WorldPos0; 

out vec4 FragColor;

void main()
{
    // vec4 SampledColor = texture2D(gDiffuseTex, TexCoord0.xy); 
	// vec4 aoColor = texture2D(gAOMap, TexCoord0.xy); 
    vec3 line_color = vec3(1.0f, 0.0f, 1.0f);
    FragColor = vec4(line_color, 1.0f);
    // FragColor = SampledColor;
	// FragColor = SampledColor * vec4(Phong, 1.0f);

}