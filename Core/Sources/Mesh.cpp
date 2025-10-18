#include "Core/Headers/Mesh.h"


Mesh::Mesh()
{
}

Mesh::Mesh(std::vector<GLfloat> vertices, std::vector<GLuint> indices): m_indexCount(indices.size())
{
	vao = new VAO();
	vbo = new VBO(static_cast<GLsizei>(vertices.size()), std::data(vertices));
	ebo = new EBO(static_cast<GLsizei>(indices.size()),  std::data(indices));

	vao->bind();
	vbo->bind();
	ebo->bind();

	vao->linkAttrib(*vbo, 0, 3, 5, 0);
	vao->linkAttrib(*vbo, 1, 2, 5, 3);

	vao->unbind();
	vbo->unbind();
	ebo->unbind();
}

Mesh::Mesh(const GLfloat* vertices, const GLuint* indices, int indexCount, GLsizei sizeOfVertices, GLsizei sizeOfIndices) : m_indexCount(indexCount)
{
	vao = new VAO();
	vbo = new VBO(sizeOfVertices, vertices);
	ebo = new EBO(sizeOfIndices,  indices);

	vao->bind();
	vbo->bind();
	ebo->bind();

	vao->linkAttrib(*vbo, 0, 3, 8, 0);
	vao->linkAttrib(*vbo, 1, 3, 8, 3);
	vao->linkAttrib(*vbo, 2, 2, 8, 6);


	vao->unbind();
	vbo->unbind();
	ebo->unbind();
}

void Mesh::draw() const
{
	vao->bind();

	glDrawElements(GL_TRIANGLES, m_indexCount, GL_UNSIGNED_INT, 0);
}

Mesh::~Mesh()
{
	delete vao;
	delete vbo;
	delete ebo;
}
