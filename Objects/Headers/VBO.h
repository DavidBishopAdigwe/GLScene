#pragma once
#include <glad/glad.h>


class VBO
{

public:
	GLuint ID;

	~VBO();
	 VBO(GLsizei size, const void* data, GLenum usage = GL_STATIC_DRAW);
	void operator==(const VBO&) = delete;
	VBO(const VBO&) = delete;

	void bind();
	void unbind();
};
