#pragma once


#include <string>
#include <vector>

#include <opencv2/opencv.hpp>
#include "glad/glad.h"

#define STB_IMAGE_IMPLEMENTATION

class Texture {
public:
	Texture(std::string filePath, GLenum texUnit = GL_TEXTURE0);

	Texture(GLuint textureData,  GLenum texUnit, int width, int height);
	// Simple Load Texture
	// For Cube Maps
	// Texture(std::vector<std::string> filePaths);

	static bool LoadTextureFromFile(std::string fileName, GLuint& textureObj);
	virtual void BindTexture() const;
	void BindCubeMapTexture();
	void UnBindTexture() const;

	int GetWidth() const { return width; }
	int GetHeight() const { return height; }

	const GLenum m_TexUnit;
	

protected:
	int width;
	int height;
	GLuint texture_data;


private:
	bool Load(std::string filePath, GLenum textureUnit = GL_TEXTURE0);
	bool stbiLoad(std::string filePath, GLenum textureUnit = GL_TEXTURE0, int colorCh = -1);
	cv::Mat m_Img;
};