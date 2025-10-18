#pragma once
#include <GL/GL.h>


namespace Floor
{
	inline constexpr GLfloat vertices[32]
	{
		-0.5f, 0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
		 0.5f, 0.5f, 0.0f,	0.0f, 0.0f, 1.0f,1.0f, 1.0f,
		-0.5f,-0.5f, 0.0f,	0.0f, 0.0f, 1.0f,0.0f, 0.0f,
		 0.5f,-0.5f, 0.0f,	0.0f, 0.0f, 1.0f,1.0f, 0.0f
	};

	inline constexpr GLuint indices[6]
	{
		0, 1, 3,
		0, 2, 3
	};
}
