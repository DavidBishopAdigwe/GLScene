#include "Core/Headers/Material.h"


void Material::use(const glm::mat4 &projection, const glm::mat4 &view, const glm::mat4 &model,
	const glm::vec3 viewPosition)
{
	m_shader->use();
	if (textureExists)
	{
		m_texture->bind();
		m_shader->setUniformi("diffuseMap", 0);
		m_shader->setUniformi("specularMap", 1);
		m_shader->setUniformi("emissiveMap", 2);
	}

	m_shader->setUniformf("ambientStrength", m_ambientStrength);
	m_shader->setUniformf("diffuseStrength", m_diffuseStrength);
	m_shader->setUniformf("specularStrength", m_specularStrength);
	m_shader->setUniformf("shininess", m_shininess);

	m_shader->setUniformMat4("projection", projection);
	m_shader->setUniformMat4("view", view);
	m_shader->setUniformMat4("model", model);
	m_shader->setUniformVec3("color", m_baseColor);
	m_shader->setUniformVec3("viewPos", viewPosition);
}

void Material::setTexture(const std::shared_ptr<Texture>& texture)
{
	textureExists = true;
	m_texture = texture;
}

void Material::sendShaderInput(const std::string& name, float value)
{
	m_shader->setUniformf(name.c_str(), value);
}

void Material::sendShaderInput(const std::string& name, int value)
{
	m_shader->setUniformi(name.c_str(), value);
}

void Material::sendShaderInput(const std::string& name, Vec3f value)
{
	m_shader->setUniformVec3(name.c_str(), glm::vec3(value.x, value.y, value.z));
}

void Material::useShader()
{
	m_shader->use();
}

void Material::setColor(Vec3f color)
{
	m_baseColor = color;
}

void Material::setColor(float r, float g, float b)
{
	m_baseColor = Vec3f(r,g,b);
}

Vec3f Material::getColor() const
{
	return m_baseColor;
}


void Material::setAmbience(float ambientStr)
{
	m_ambientStrength = ambientStr;
}

void Material::setDiffuse(float diffuseStr)
{
	m_diffuseStrength = diffuseStr;
}

void Material::setSpecular(float specularStr)
{
	m_specularStrength = specularStr;
}

void Material::setShininess(float shininess)
{
	m_shininess = shininess;
}

float Material::getDiffuse()
{
	return m_diffuseStrength;
}

float Material::getShininess() const
{
	return m_shininess;
}

float Material::getSpecular() const
{
	return m_specularStrength;
}

float Material::getAmbience() const
{
	return m_ambientStrength;
}

