#include "Main/Engine.h"
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "Core/FileManager.h"

float vertices[]
{
	-0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f,
	 0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f,
	0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f
};
unsigned int vao, vbo, ebo;
int main()
{

	Engine engine{};
	engine.run();
	return 0;
}
