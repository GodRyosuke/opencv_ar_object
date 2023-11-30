#pragma once

#include <string>

#include "glad/glad.h"
#include "glm/glm.hpp"

class Shader {
public:
	Shader();
	~Shader();
	bool CreateShaderProgram(std::string vertFilePath, std::string fragFilePath);
	void UseProgram();
	void SetMatrixUniform(std::string uniformName, glm::mat4 mat);
	void SetVectorUniform(std::string uniformName, glm::vec3 vec);
	void SetFloatUniform(std::string uniformName, float data);
	void SetSamplerUniform(std::string uniformName, GLuint tex);

private:
	GLuint mShaderProgram;
};