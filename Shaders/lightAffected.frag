#version 330 core



#define DIR 1
#define POINT 1
#define SPOT 1


struct DirectionalLight{
	float intensity;
	vec3 direction;
	vec3 color;
};

struct PointLight{
	float intensity;
	vec3 position;
	vec3 color;

	float constant;
	float linear;
	float quadratic;
};

struct SpotLight {
	float intensity;
	vec3 position;
	vec3 direction;
	vec3 color;

	float innerCutoff;
	float outerCutoff;
};

in vec3 FragPos;
in vec3 Normal;

uniform vec3 viewPos;

uniform float ambientStrength;
uniform float diffuseStrength;
uniform float specularStrength;
uniform float shininess;


uniform DirectionalLight directionalLight[DIR];
uniform PointLight pointLight[POINT];


vec3 baseColor = vec3(0.5, 0.5, 0.5);

out vec4 FragColor;

vec3 calcDirLights(DirectionalLight light, vec3 normal, vec3 viewDir) 	
{
	vec3 ambient = ambientStrength * light.color;

	float diff = diffuseStrength * max(dot(normal, -light.direction), 0.0);
	
	vec3 diffuse = diff * diffuseStrength * light.color;

	vec3 halfway = (viewDir + light.direction) / length (viewDir + light.direction);
	float spec = pow(max(dot(normal, halfway), 0.0), shininess);
	
	vec3 specular = spec * specularStrength * light.color;

	return (ambient + diffuse + specular);
}

vec3 calcPointLights(PointLight light, vec3 normal, vec3 viewDir) {

	vec3 lightDir = (light.position - FragPos);
	float dist = length(lightDir);
	 lightDir = normalize(lightDir);
	float intensity = ( 1 / (light.constant + (light.linear * dist) + (light.quadratic * (dist * dist))));
	float ambient = ambientStrength;

	float diff = max(dot(lightDir, normal), 0.0);

	float diffuse = diffuseStrength * diff;

	vec3 reflectDir = reflect(-lightDir, normal);
	float specular = specularStrength * pow(max(dot(viewDir, reflectDir), 0.0), shininess);


	return (ambient + diffuse + specular) * intensity * light.color;
}

void main()	 {

	vec3 norm = normalize(Normal);
	vec3 viewDir = normalize(viewPos - FragPos);
	vec3 result = vec3(0.0);

	result = calcDirLights(directionalLight[0], norm, viewDir);
	for (int i = 0; i < POINT; ++i)  {
	if (pointLight[i] == PointLight(0, vec3(0), vec3(0),0,0,0)) continue;

		result += calcPointLights(pointLight[i], norm, viewDir); 
	}

	FragColor = vec4(result * baseColor, 1.0);

}	