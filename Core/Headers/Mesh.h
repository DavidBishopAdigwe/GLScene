#pragma once
#include <vector>

#include "Objects/Headers/EBO.h"
#include "Objects/Headers/VAO.h"
#include "Objects/Headers/VBO.h"

class Mesh
{
public:
	Mesh();
	Mesh(std::vector<GLfloat> vertices, std::vector<GLuint> indices);
	Mesh(const GLfloat* vertices, const GLuint* indices, int indexCount, GLsizei sizeOfVertices, GLsizei sizeOfIndices);

	VAO* vao{};
	VBO* vbo{};
	EBO* ebo{};

	size_t m_indexCount;

	void draw() const;

	~Mesh();

};
