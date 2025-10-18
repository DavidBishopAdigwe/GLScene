#version 330 core

out vec4 FragColor;

uniform vec3 color;
uniform vec3 lightColor;
uniform float ambientStrength;

void main() {
	vec3 ambientVector = ambientStrength * lightColor;
	FragColor = vec4((ambientVector * lightColor), 1.0f);
}