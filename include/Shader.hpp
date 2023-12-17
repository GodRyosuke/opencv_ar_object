#pragma once

#include <string>
#include <vector>

#include "glad/glad.h"
#include "glm/glm.hpp"


class Shader {
public:
	struct ShaderDesc {
		enum ShaderType {
			VERTEX_FRAGMENT,
			VERTEX_GEOMETRY_FRAGMENT,
			NUM_SHADER_TYPE
		};
		
		const std::string m_Name;
		const std::vector<std::string> m_FilePaths;
		const ShaderType m_ShaderType;
	};

	Shader();
	~Shader();
	bool CreateShaderProgram(std::string vertFilePath, std::string fragFilePath);
	void UseProgram();
	void SetMatrixUniform(std::string uniformName, glm::mat4 mat);
	void SetVectorUniform(std::string uniformName, glm::vec3 vec);
	void SetFloatUniform(std::string uniformName, float data);
	void SetSamplerUniform(std::string uniformName, GLuint tex);
    static Shader* CompileShaderFromDesc(ShaderDesc desc);

private:
	GLuint mShaderProgram;
};

