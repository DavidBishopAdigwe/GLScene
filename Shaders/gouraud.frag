#version 330 core

in vec3 vertColor;
out vec4 FragC;

void main() {
FragC = vec4(vertColor, 1.0);
}