#version 330 core

in vec2 texCoords;
out vec4 FragColor;

uniform sampler2D diffuseMap;
void main() {
	FragColor = vec4(texture(diffuseMap, texCoords));
}