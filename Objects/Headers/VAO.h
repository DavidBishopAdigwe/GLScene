#pragma once

#include <glad/glad.h>
#include "Objects/Headers/VBO.h"


class VAO
{
public:
	GLuint ID{};

	 VAO();
	~VAO();
	void operator==(const VAO&) = delete;
	VAO(const VAO&) = delete;

	void linkAttrib(VBO& vbo,GLuint location, GLsizei size, GLint stride, GLint offset);
	void bind();
	void unbind();

};