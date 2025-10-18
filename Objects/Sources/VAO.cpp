#include "Objects/Headers/VAO.h"



VAO::VAO()
{
	glGenVertexArrays(1, &ID);
}

void VAO::linkAttrib(VBO& vbo, GLuint location, GLsizei size, GLint stride, GLint offset)
{
	vbo.bind();
	glVertexAttribPointer(location, size, GL_FLOAT, GL_FALSE, stride * sizeof(GLfloat),(void*)(offset * sizeof(GLfloat)));
	glEnableVertexAttribArray(location);
}

void VAO::bind()
{
	glBindVertexArray(ID);
}

void VAO::unbind()
{
	glBindVertexArray(0);
}

VAO::~VAO()
{
	glDeleteVertexArrays(1, &ID);
}



