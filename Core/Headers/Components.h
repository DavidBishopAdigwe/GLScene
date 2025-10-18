#pragma once
#include <memory>

#include "Material.h"
#include "Math/Vec3.h"


class Camera;
class Mesh;
class Material;


class Component
{

public:
   virtual ~Component() = default;
   bool active{};
   static size_t ID;

protected:

};


class Transform: public Component
{
public:
   Transform() = default;
   Transform(Vec3f position, float rotationAngle = 0.0f,
             Vec3f rotationAxis =
             Vec3f(0.0f, 0.0f, 1.0f),
             Vec3f scale = Vec3f(1.0f)): m_position(position),
                                               m_scale(scale),
                                               m_currentAngle(rotationAngle),
                                               m_currentRotationAxis(rotationAxis){}
private:
   Vec3f m_position{};
   Vec3f m_scale{1.0f};
   float m_currentAngle{};
   Vec3f m_currentRotationAxis{0.0f, 0.0f, 1.0f};

public:
   void setPosition(Vec3f position);
   Vec3f getPosition() const;

   void setScale(Vec3f scale);
   void setScale(float scalar);
   Vec3f getScale() const;
   Vec3f getRotationAxis() const;
   float getRotationAngle() const;

   void rotate(float angle, Vec3f rotationAxis);

};

class MeshRenderer: public Component
{
public:
   std::shared_ptr<Material> m_material{};
   std::shared_ptr<Mesh> m_mesh{};

public:
   MeshRenderer() = default;
   MeshRenderer(const std::shared_ptr<Material>& material, const std::shared_ptr<Mesh>& mesh): m_material(material), m_mesh(mesh){}

   std::shared_ptr<Material>& getMaterial()
   {
      return m_material;
   }

   void useShader() const
   {
      m_material->useShader();
   }
};

class Light: public Component
{
public:
   Light() = default;
   Light(int id, Vec3f color, float intensity): m_lightID(id), m_intensity(intensity),
                                        m_color(color){}
protected:

   // TODO: give ownership to scene class
   int m_lightID{};
   int m_sceneAmount{1};
   float m_intensity{1.0f};
   Vec3f m_color{1.0f};
   virtual void use(MeshRenderer& renderer, Transform& transform){renderer.useShader();}
};

class DirectionalLight: public Light
{
public:
   DirectionalLight() = default;
   DirectionalLight(Vec3f direction, int id, Vec3f color = Vec3f(1.0f),
                                     float intensity = 1.0f):
   Light(id, color, intensity),
   m_direction(direction)
   {

   }
   Vec3f m_direction{};
   void setDirection(Vec3f d);
   [[nodiscard]] Vec3f getDirection() const;

    void use(MeshRenderer& renderer, Transform& transform) override
    {
       Light::use(renderer, transform);
       const std::string uniformStr{"directionalLight[" + std::to_string(m_lightID) + "]."};

       renderer.getMaterial()->sendShaderInput((uniformStr + "direction"), m_direction);
       renderer.getMaterial()->sendShaderInput((uniformStr + "color"), m_color);
    }
};

class PointLight: public Light
{
public:
   PointLight() = default;
   PointLight(float constant, float linear, float quadratic,
               int id = 0, Vec3f color = Vec3f(1.0f), float intensity = 1.0f):
   Light(id, color, intensity),
              m_constant(constant), m_linear(linear), m_quadratic(quadratic)
   {
   }

   float m_constant{1.0f};
   float m_linear{1.0f};
   // Ogre3D quadratic = 1.0 / (0.01 * distance * distance)
   float m_quadratic{2.0f};


   void use(MeshRenderer& renderer, Transform& transform) override
   {
      Light::use(renderer, transform);
      const std::string uniformStr{"pointLight[" + std::to_string(m_lightID) + "]."};

      if (m_sceneAmount < 1) return;
      renderer.getMaterial()->sendShaderInput((uniformStr + "constant"), m_constant);
      renderer.getMaterial()->sendShaderInput((uniformStr + "linear"), m_linear);
      renderer.getMaterial()->sendShaderInput((uniformStr + "quadratic"), m_quadratic);

      renderer.getMaterial()->sendShaderInput((uniformStr + "color"), m_color);
      renderer.getMaterial()->sendShaderInput((uniformStr + "position"), transform.getPosition());
      renderer.getMaterial()->sendShaderInput((uniformStr + "intensity"), m_intensity);
   }
};

class SpotLight: public Light
{
public:
   SpotLight() = default;
   SpotLight(Vec3f direction, float innerAngle, float outerAngle,
             int id = 0, Vec3f color = Vec3f(1.0f), float intensity = 1.0f):
   Light(id, color, intensity),
   m_direction(direction),
   // Inner and Outer cutoff values, cosine value of their respective angles
   m_innerCutoff(glm::cos(glm::radians(innerAngle))),
   m_outerCutoff(glm::cos(glm::radians(outerAngle))){}


   Vec3f m_direction{};

   float m_innerCutoff{};
   float m_outerCutoff{};

   void use(MeshRenderer& renderer, Transform& transform) override
   {
      Light::use(renderer, transform);

      const std::string uniformStr = "spotLight[" + std::to_string(m_lightID) + "].";

      auto& mat = renderer.getMaterial();

      mat->sendShaderInput(uniformStr + "innerCutoff", m_innerCutoff);
      mat->sendShaderInput(uniformStr + "outerCutoff", m_outerCutoff);
      mat->sendShaderInput(uniformStr + "color", m_color);
      mat->sendShaderInput(uniformStr + "direction", m_direction);
      mat->sendShaderInput(uniformStr + "position", transform.getPosition());
      mat->sendShaderInput(uniformStr + "intensity", m_intensity);


   }
};


