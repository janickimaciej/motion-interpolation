#include "frame.hpp"

Frame::Frame(bool intermediate) :
	m_intermediate{intermediate}
{
	if (m_mainFrameMesh == nullptr)
	{
		m_mainFrameMesh = std::make_unique<FrameMesh>(false);
		m_intermediateFrameMesh = std::make_unique<FrameMesh>(true);
	}
}

void Frame::render() const
{
	updateShaders();
	if (m_intermediate)
	{
		m_intermediateFrameMesh->render();
	}
	else
	{
		m_mainFrameMesh->render();
	}
}

glm::vec3 Frame::getPos() const
{
	return m_pos;
}

void Frame::setPos(const glm::vec3& pos)
{
	m_pos = pos;
}

std::unique_ptr<FrameMesh> Frame::m_mainFrameMesh = nullptr;
std::unique_ptr<FrameMesh> Frame::m_intermediateFrameMesh = nullptr;

void Frame::updateShaders() const
{
	m_shaderProgram.use();
	m_shaderProgram.setUniform("posWorld", m_pos);
}
