#include "Texture.hpp"

#include <iostream>

#include "stb_image.h"
#include <opencv2/opencv.hpp>

#include "Util.hpp"


Texture::Texture(std::string filePath, GLenum texUnit)
	:m_TexUnit(texUnit)
{
	if (!Load(filePath, texUnit)) {
		Util::Print("failed to load texture\n");
	}
}


Texture::Texture(GLuint textureData,  GLenum texUnit, int width, int height)
	:texture_data(textureData)
	,width(width)
	,height(height)
	,m_TexUnit(texUnit)
{
}

bool Texture::Load(std::string filePath, GLenum textureUnit)
{
	// cv::Mat src_img;
	m_Img = cv::imread(filePath, cv::IMREAD_COLOR);
	cv::flip(m_Img, m_Img, 0);
	cv::cvtColor(m_Img, m_Img, cv::COLOR_BGR2RGB);
	// int numColCh;
	// stbi_set_flip_vertically_on_load(true);
	// unsigned char* pictureData = stbi_load(filePath.c_str(), &width, &height, &numColCh, 0);

	// if (!pictureData) {
	// 	Util::Print("error: failed to load texture: ", filePath, "\n");
	// 	return false;
	// }
	
	glGenTextures(1, &texture_data);
	glActiveTexture(textureUnit);
	glBindTexture(GL_TEXTURE_2D, texture_data);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	// auto colorCh = GL_RGBA;
	// if (colorch > 0) {
	// 	colorCh = colorch;
	// } else {
	// 	if (numColCh == 3) {
	// 		colorCh = GL_RGB;
	// 	} else if (numColCh == 1) {
	// 		colorCh = GL_RED;
	// 	}
	// }
	//auto colorCh = GL_BGRA;
	//if (numColCh == 3) {
	//	colorCh = GL_BGR;
	//}
	width = m_Img.cols;
	height = m_Img.rows;
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_Img.cols, m_Img.rows, 0, GL_RGB, GL_UNSIGNED_BYTE, m_Img.data);
	// Generates MipMaps
	glGenerateMipmap(GL_TEXTURE_2D);
	glBindTexture(texture_data, 0);		// unbind
	// stbi_image_free(pictureData);

	return true;
}

bool Texture::stbiLoad(std::string filePath, GLenum textureUnit, int colorch)
{
	// Load from file
	int numColCh;
	stbi_set_flip_vertically_on_load(true);
	unsigned char* pictureData = stbi_load(filePath.c_str(), &width, &height, &numColCh, 0);

	if (!pictureData) {
		Util::Print("error: failed to load texture: ", filePath, "\n");
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

// Texture::Texture(std::vector<std::string> filePaths)
// {
// 	glGenTextures(1, &texture_data);
// 	glBindTexture(GL_TEXTURE_CUBE_MAP, texture_data);
// 	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
// 	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
// 	// These are very important to prevent seams
// 	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
// 	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
// 	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
// 	// This might help with seams on some systems
// 	//glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);

// 	// Cycles through all the textures and attaches them to the cubemap object
// 	for (unsigned int i = 0; i < 6; i++)
// 	{
// 		int width, height, nrChannels;
// 		unsigned char* data = stbi_load(filePaths[i].c_str(), &width, &height, &nrChannels, 0);
// 		if (data)
// 		{
// 			stbi_set_flip_vertically_on_load(false);
// 			glTexImage2D
// 			(
// 				GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
// 				0,
// 				GL_RGB,
// 				width,
// 				height,
// 				0,
// 				GL_RGB,
// 				GL_UNSIGNED_BYTE,
// 				data
// 			);
// 			stbi_image_free(data);
// 		}
// 		else
// 		{
// 			std::cout << "Failed to load texture: " << filePaths[i] << std::endl;
// 			stbi_image_free(data);
// 		}
// 	}

// 	glBindTexture(texture_data, 0);
// }

void Texture::BindTexture() const
{
	glActiveTexture(m_TexUnit);
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


