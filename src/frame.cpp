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
	updateModelMatrix();
}

void Frame::setEulerAngles(const glm::vec3& angles)
{
	glm::mat4 rotationXMatrix
	{
		1, 0, 0, 0,
		0, std::cos(angles.x), std::sin(angles.x), 0,
		0, -std::sin(angles.x), std::cos(angles.x), 0,
		0, 0, 0, 1
	};

	glm::mat4 rotationYMatrix
	{
		std::cos(angles.y), 0, -std::sin(angles.y), 0,
		0, 1, 0, 0,
		std::sin(angles.y), 0, std::cos(angles.y), 0,
		0, 0, 0, 1
	};

	glm::mat4 rotationZMatrix
	{
		std::cos(angles.z), std::sin(angles.z), 0, 0,
		-std::sin(angles.z), std::cos(angles.z), 0, 0,
		0, 0, 1, 0,
		0, 0, 0, 1
	};

	m_rotationMatrix = rotationZMatrix * rotationYMatrix * rotationXMatrix;

	updateModelMatrix();
}

void Frame::setQuat(const glm::vec4& quat)
{
	float xx = quat.x * quat.x;
	float yy = quat.y * quat.y;
	float zz = quat.z * quat.z;
	float ww = quat.w * quat.w;
	float xy = quat.x * quat.y;
	float xz = quat.x * quat.z;
	float xw = quat.x * quat.w;
	float yz = quat.y * quat.z;
	float yw = quat.y * quat.w;
	float zw = quat.z * quat.w;

	m_rotationMatrix =
		{
			ww + xx - yy - zz, 2 * (xy + zw), 2 * (xz - yw), 0,
			2 * (xy - zw), ww - xx + yy - zz, 2 * (yz + xw), 0,
			2 * (xz + yw), 2 * (yz - xw), ww - xx - yy + zz, 0,
			0, 0, 0, 1
		};

	updateModelMatrix();
}

std::unique_ptr<FrameMesh> Frame::m_mainFrameMesh = nullptr;
std::unique_ptr<FrameMesh> Frame::m_intermediateFrameMesh = nullptr;

void Frame::updateShaders() const
{
	m_shaderProgram.use();
	m_shaderProgram.setUniform("modelMatrix", m_modelMatrix);
}

void Frame::updateModelMatrix()
{
	glm::mat4 posMatrix
	{
		1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		m_pos.x, m_pos.y, m_pos.z, 1
	};

	m_modelMatrix = posMatrix * m_rotationMatrix;
}
