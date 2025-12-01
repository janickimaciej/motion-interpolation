#include "frame.hpp"

Frame::Frame()
{ }

void Frame::render() const
{
	updateShaders();
	m_mesh.render();
}

glm::vec3 Frame::getPos() const
{
	return m_pos;
}

void Frame::setPos(const glm::vec3& pos)
{
	m_pos = pos;
}

void Frame::updateShaders() const
{
	m_shaderProgram.use();
	m_shaderProgram.setUniform("posWorld", m_pos);
}
