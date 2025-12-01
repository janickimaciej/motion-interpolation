#include "plane/plane.hpp"

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
	m_shaderProgram.use();
	m_shaderProgram.setUniform("scale", m_scale);
}
