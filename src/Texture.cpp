#include "Texture.hpp"
#include <iostream>
#include "stb_image.h"

Texture::Texture()
{

}

Texture::Texture(GLuint textureData, int width, int height)
	:texture_data(textureData)
	,width(width)
	,height(height)
{
	
}

bool Texture::Load(std::string filePath, GLenum textureUnit, int colorch)
{
	// Load from file
	int numColCh;
	stbi_set_flip_vertically_on_load(true);
	unsigned char* pictureData = stbi_load(filePath.c_str(), &width, &height, &numColCh, 0);

	if (!pictureData) {
		printf("error: failed to load texture: %s\n", filePath.c_str());
		return false;
	}
	
	glGenTextures(1, &texture_data);
	glActiveTexture(textureUnit);
	glBindTexture(GL_TEXTURE_2D, texture_data);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	auto colorCh = GL_RGBA;
	if (colorch > 0) {
		colorCh = colorch;
	} else {
		if (numColCh == 3) {
			colorCh = GL_RGB;
		} else if (numColCh == 1) {
			colorCh = GL_RED;
		}
	}
	//auto colorCh = GL_BGRA;
	//if (numColCh == 3) {
	//	colorCh = GL_BGR;
	//}
	glTexImage2D(GL_TEXTURE_2D, 0, colorCh, width, height, 0, colorCh, GL_UNSIGNED_BYTE, pictureData);
	// Generates MipMaps
	glGenerateMipmap(GL_TEXTURE_2D);
	glBindTexture(texture_data, 0);		// unbind
	stbi_image_free(pictureData);
	return true;
}

Texture::Texture(std::vector<std::string> filePaths)
{
	glGenTextures(1, &texture_data);
	glBindTexture(GL_TEXTURE_CUBE_MAP, texture_data);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	// These are very important to prevent seams
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	// This might help with seams on some systems
	//glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);

	// Cycles through all the textures and attaches them to the cubemap object
	for (unsigned int i = 0; i < 6; i++)
	{
		int width, height, nrChannels;
		unsigned char* data = stbi_load(filePaths[i].c_str(), &width, &height, &nrChannels, 0);
		if (data)
		{
			stbi_set_flip_vertically_on_load(false);
			glTexImage2D
			(
				GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
				0,
				GL_RGB,
				width,
				height,
				0,
				GL_RGB,
				GL_UNSIGNED_BYTE,
				data
			);
			stbi_image_free(data);
		}
		else
		{
			std::cout << "Failed to load texture: " << filePaths[i] << std::endl;
			stbi_image_free(data);
		}
	}

	glBindTexture(texture_data, 0);
}

void Texture::BindTexture(GLenum TextureUnit) const
{
	glActiveTexture(TextureUnit);
	glBindTexture(GL_TEXTURE_2D, texture_data);
}

void Texture::BindCubeMapTexture()
{
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, texture_data);
}

void Texture::UnBindTexture() const
{
	glBindTexture(texture_data, 0);
}


