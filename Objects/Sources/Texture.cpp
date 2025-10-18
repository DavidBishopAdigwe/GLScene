#include "Objects/Headers/Texture.h"

Texture::Texture(const char* fileName, GLenum wrapType, GLenum imageFormat, bool flipImageOnLoad, GLuint desiredFormat)
{
	glGenTextures(1, &diffuseID);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, diffuseID);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	setWrap(GL_TEXTURE_WRAP_S, wrapType);
	setWrap(GL_TEXTURE_WRAP_T, wrapType);
	setWrap(GL_TEXTURE_WRAP_R, wrapType);

	setImageData(fileName, 0, desiredFormat, imageFormat, flipImageOnLoad);

	glBindTexture(GL_TEXTURE_2D, 0);


}

Texture::Texture(const char *diffuseMapFilename, const char *specularMapFilename, GLenum wrapType,
	GLenum diffuseImageFormat, GLenum specularImageFormat, bool flipImgOnLoad)
{
	hasSpecularMap = true;
	glGenTextures(1, &diffuseID);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, diffuseID);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	setWrap(GL_TEXTURE_WRAP_S, wrapType);
	setWrap(GL_TEXTURE_WRAP_T, wrapType);
	setWrap(GL_TEXTURE_WRAP_R, wrapType);

	setImageData(diffuseMapFilename, 0, GL_RGBA, diffuseImageFormat, flipImgOnLoad);

	glBindTexture(GL_TEXTURE_2D, 0);

	glGenTextures(1, &specularID);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, specularID);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	setWrap(GL_TEXTURE_WRAP_S, wrapType);
	setWrap(GL_TEXTURE_WRAP_T, wrapType);
	setWrap(GL_TEXTURE_WRAP_R, wrapType);

	setImageData(specularMapFilename, 0, GL_RGBA, specularImageFormat, flipImgOnLoad);

	glBindTexture(GL_TEXTURE_2D, 0);


}

void Texture::setSpecularMap(const char* fileName, GLuint imageFormat, GLuint wrapType, bool flipImgOnLoad )
{
	hasSpecularMap = true;

	glGenTextures(1, &specularID);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, specularID);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	setWrap(GL_TEXTURE_WRAP_S, wrapType);
	setWrap(GL_TEXTURE_WRAP_T, wrapType);
	setWrap(GL_TEXTURE_WRAP_R, wrapType);

	setImageData(fileName, 0, GL_RGBA, imageFormat, flipImgOnLoad);

	glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::setEmissiveMap(const char *fileName, GLuint imageFormat, GLuint wrapType, bool flipImgOnLoad)
{
	hasEmissiveMap = true;
	glGenTextures(1, &emissiveID);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, emissiveID);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	setWrap(GL_TEXTURE_WRAP_S, wrapType);
	setWrap(GL_TEXTURE_WRAP_T, wrapType);
	setWrap(GL_TEXTURE_WRAP_R, wrapType);

	setImageData(fileName, 0, GL_RGBA, imageFormat, flipImgOnLoad);

	glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::setWrap(GLenum wrapAxis, GLenum wrapType)
{
	glTexParameteri(GL_TEXTURE_2D, wrapAxis, wrapType);
}

void Texture::setImageData(const char* fileName, GLint mipmapLevel, GLint desiredFormat, GLenum imageFormat,
	bool flipImageOnLoad)
{
	stbi_set_flip_vertically_on_load(flipImageOnLoad);
	int width, height, nrColCh;
	unsigned char* imageData = stbi_load(fileName, &width, &height, &nrColCh, 0);

	stbi_set_flip_vertically_on_load(false);

	glTexImage2D(GL_TEXTURE_2D, mipmapLevel, desiredFormat, width, height, 0, imageFormat, GL_UNSIGNED_BYTE, imageData);

	glGenerateMipmap(GL_TEXTURE_2D);
}

void Texture::bind()
{
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, diffuseID);

	if (hasSpecularMap)
	{
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, specularID);
	}

	if (hasEmissiveMap)
	{
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, emissiveID);
	}


}



void Texture::unbind()
{
	glBindTexture(GL_TEXTURE_2D, 0);
}

Texture::~Texture()
{
	glDeleteTextures(1, &diffuseID);
}




