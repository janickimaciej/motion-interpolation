#include "plane/plane.hpp"

#include "shaderPrograms.hpp"

Plane::Plane(float scale) :
	m_scale{scale}
{ }

void Plane::render() const
{
	updateShaders();
	m_quad.render();
}

void Plane::updateShaders() const
{
	ShaderPrograms::plane->use();
	ShaderPrograms::plane->setUniform("scale", m_scale);
}
