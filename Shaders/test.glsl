#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 col;

out vec3 aCol;

void main() {  
aCol = col;
    gl_Position = vec4(aPos, 1.0);
}