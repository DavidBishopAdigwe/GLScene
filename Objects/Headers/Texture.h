#pragma once
#include <glad/glad.h>
#include <stb/stb_image.h>


class Texture
{
public:
	Texture(const char* fileName, GLenum wrapType, GLenum imageFormat, bool flipImageOnLoad = true, GLuint desiredFormat = GL_RGBA);
	Texture(const char* diffuseMapFilename, const char* specularMapFilename, GLenum wrapType, GLenum diffuseImageFormat, GLenum specularImageFormat, bool flipImgOnLoad = true);
	void setSpecularMap(const char* fileName)
	{
		glGenTextures(1, &specularID);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, specularID);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);



	}
	GLuint diffuseID{};
	GLuint specularID{};
	GLuint emissiveID{};
	GLenum glIndexUnit;
	bool hasSpecularMap{};
	bool hasEmissiveMap{};
	void setSpecularMap(const char* fileName, GLuint imageFormat, GLuint wrapType = GL_REPEAT, bool flipImgOnLoad = true);
	void setEmissiveMap(const char* fileName, GLuint imageFormat, GLuint wrapType = GL_REPEAT, bool flipImgOnLoad = true);
	void setWrap(GLenum wrapAxis, GLenum wrapType);
	void setImageData(const char* fileName, GLint mipmapLevel, GLint desiredFormat, GLenum imageFormat, bool flipImageOnLoad);
	void bind();
	void unbind();

	~Texture();

};
