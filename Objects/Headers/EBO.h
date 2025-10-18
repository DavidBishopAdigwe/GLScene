#pragma once
#include <glad/glad.h>

class EBO
{
public:
	GLuint ID;
	EBO(GLsizei size, const void* data, GLenum usage = GL_STATIC_DRAW);

	// both not needed
	void operator==(const EBO&) = delete;
	EBO(const EBO&) = delete;

	void bind();
	void unbind();
	~EBO();

};
