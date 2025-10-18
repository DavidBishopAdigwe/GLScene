#version 330 core

out vec4 FragColor;

in vec3 Normal;
in vec3 FragPos;

uniform float ambientStrength;
uniform float diffuseStrength;
uniform float specularStrength;
uniform float shininess;

uniform vec3 viewPos;
uniform vec3 lightPos;

uniform vec3 color;
uniform vec3 lightColor;





void main() 
{ 
	float distanceToLight = length(vec3(lightPos) - FragPos);
	float baseDistance = 15.0;
	float maxDistance = 50.0;
	float intensity = 1.0; 
	if (distanceToLight > baseDistance) {
	intensity = baseDistance/distanceToLight;
	}
	if (distanceToLight > maxDistance) {intensity = 0;}
	// swap to a uniform intensity later
  	vec3 norm = normalize(Normal);
	vec3 lightDir = normalize(vec3(lightPos) - FragPos);

	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = diffuseStrength * diff * lightColor;

	vec3 ambient = ambientStrength * lightColor;


	vec3 viewDir = normalize(viewPos- FragPos);
	vec3 reflectedDir = reflect(-lightDir, norm);
	float spec = pow(max(dot(viewDir, reflectedDir), 0.0), shininess);
	vec3 specular = specularStrength * spec * lightColor;

	vec3 result = (ambient + diffuse + specular) * color * intensity;
	FragColor = vec4(result, 1.0f);
}