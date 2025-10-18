#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 2) in vec3 aNormal;


uniform float ambientStrength;
uniform float specularStrength;
uniform float shininess;

uniform vec3 color;
uniform vec3 lightColor;
uniform vec3 lightPos;

uniform vec3 viewPos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 vertColor;





void main() {
	
	float distanceToLight = length(lightPos - aPos);
	float baseDistance = 15.0;
	float maxDistance = 50.0;
	float intensity = 1.0; 
	if (distanceToLight > baseDistance) {
	intensity = baseDistance/distanceToLight;
	}
	if (distanceToLight > maxDistance) {intensity = 0;}
	vec3 norm = normalize(aNormal);
	vec3 ambient = ambientStrength * lightColor;

	vec3 lightDir = normalize(lightPos - aPos);
	float diff = max(dot(lightDir, norm), 0.0);
	vec3 diffuse = diff * lightColor;

	vec3 viewDir = normalize(viewPos - aPos);
	vec3 reflectDir = reflect(-lightDir, norm);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
	vec3 specular = specularStrength * spec * lightColor;

	
	vertColor = (ambient + diffuse + specular) * color * intensity; 


	gl_Position = projection * view * model * vec4(aPos, 1.0);
}