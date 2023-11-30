#pragma once


#include <string>
#include "glad/glad.h"
#include <vector>
#define STB_IMAGE_IMPLEMENTATION

class Texture {
public:
	Texture();
	Texture(GLuint textureData, int width, int height);
	// Simple Load Texture
	bool Load(std::string filePath, GLenum textureUnit = GL_TEXTURE0, int colorCh = -1);
	// For Cube Maps
	Texture(std::vector<std::string> filePaths);

	static bool LoadTextureFromFile(std::string fileName, GLuint& textureObj);
	virtual void BindTexture(GLenum TextureUnit = GL_TEXTURE0) const;
	void BindCubeMapTexture();
	void UnBindTexture() const;

	int getWidth() { return width; }
	int getHeight() { return height; }

protected:

	int width;
	int height;
	GLuint texture_data;


private:

};