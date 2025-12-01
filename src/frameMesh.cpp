#include "frameMesh.hpp"

#include <glad/glad.h>

FrameMesh::FrameMesh(bool intermediate) :
	m_intermediate{intermediate}
{
	glGenVertexArrays(1, &m_VAO);
}

FrameMesh::~FrameMesh()
{
	glDeleteVertexArrays(1, &m_VAO);
}

void FrameMesh::render() const
{
	glBindVertexArray(m_VAO);
	glLineWidth(m_intermediate ? 1 : 5);
	glDrawArrays(GL_POINTS, 0, 1);
	glBindVertexArray(0);
}
