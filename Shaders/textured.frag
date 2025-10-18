#version 330 core


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
in vec2 texCoords;
in vec3 Normal;
in vec3 FragPos;
out vec4 FragColor;

uniform vec3 color;
uniform vec3 viewPos;

uniform float ambientStrength;
uniform float diffuseStrength;
uniform float specularStrength;
uniform float shininess;

uniform sampler2D diffuseMap;
uniform sampler2D specularMap;
uniform sampler2D emissiveMap;

#define POINT 4
#define DIR 1
#define SPOT 1


uniform DirectionalLight directionalLight;
uniform PointLight pointLight[4];
uniform SpotLight spotLight[SPOT];


vec3 calcDirLight(DirectionalLight light, vec3 normal, vec3 viewDir);
vec3 calcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);

void main() {
	vec3 norm = normalize(Normal);
	vec3 viewDir = normalize(viewPos - FragPos);

	vec3 result = calcDirLight(directionalLight, norm, viewDir);
	for (int i = 0; i < POINT; i++) {
		result += calcPointLight(pointLight[i], norm, FragPos, viewDir);
	}

	FragColor = vec4(result * color, 1.0f) ;
}

vec3 calcDirLight(DirectionalLight light, vec3 normal, vec3 viewDir) {
	vec3 lightDir = normalize(-FragPos);

	float diff = max(dot(lightDir, normal), 0.0) * diffuseStrength;
	vec3 diffuseTex = vec3(texture(diffuseMap, texCoords));
	vec3 diffuse = diffuseTex * diff;
	diffuse *= light.color;

	vec3 ambient = ambientStrength * light.color * diffuseTex;

	vec3 reflectedDir = normalize(reflect(-light.direction, normal));
	float spec = pow(max(dot(viewDir, reflectedDir), 0.0), shininess);
	vec3 specular = vec3(texture(specularMap, texCoords)) * spec * specularStrength * light.color;

	return (ambient + diffuse + specular ) * light.color;
}

vec3 calcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir) {

	vec3 lightDir = normalize(light.position - fragPos);
	// diffuse shading
	float diff = max(dot(normal, lightDir), 0.0);
	// specular shading
	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
	// attenuation
	float distance    = length(light.position - fragPos);
	float attenuation = 1.0 / (light.constant + light.linear * distance +
	light.quadratic * (distance * distance));
	// combine results
	vec3 ambient  = ambientStrength  * vec3(texture(diffuseMap, texCoords)) ;
	vec3 diffuse  = diffuseStrength  * diff * vec3(texture(diffuseMap, texCoords)) ;
	vec3 specular = specularStrength * spec * vec3(texture(specularMap, texCoords));
	ambient  *= attenuation;
	diffuse  *= attenuation;
	specular *= attenuation;
	return (ambient + diffuse + specular ) *light.color;

}

vec3 calcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir) {
	vec3 ambient = vec3(texture(diffuseMap, texCoords)) * ambientStrength;

	vec3 lightDir = normalize(light.position - fragPos);
	float diff = max(dot(lightDir, normal), 0.0);
	vec3 diffuse = vec3(texture(diffuseMap, texCoords)) * diffuseStrength * diff;

	vec3 reflectedDir = reflect(-lightDir, normal);
	float spec = pow(max(dot(reflectedDir, viewDir), 0.0), shininess);
	vec3 specular = spec * specularStrength * vec3(texture(specularMap, texCoords));

	float theta = max(dot(normalize(light.direction), -lightDir), 0.0);
	float epsilon = light.innerCutoff - light.outerCutoff;
	float intensity = clamp(((theta - light.outerCutoff)/epsilon), 0.0, 1.0);
	return  intensity * (ambient + diffuse + specular ) * light.color;
}

