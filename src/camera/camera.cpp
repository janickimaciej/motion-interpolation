#include "camera/camera.hpp"

#include "shaderPrograms.hpp"

#include <glm/gtc/constants.hpp>

#include <cmath>

Camera::Camera(const glm::ivec2& viewportSize, float nearPlane, float farPlane) :
	m_viewportSize{viewportSize},
	m_nearPlane{nearPlane},
	m_farPlane{farPlane}
{
	updateViewMatrix();
}

void Camera::use() const
{
	updateShaders();
}

void Camera::setViewportSize(const glm::ivec2& viewportSize)
{
	m_viewportSize = viewportSize;
	updateProjectionMatrix();
}

void Camera::moveX(float x)
{
	m_targetPos += m_radius * glm::mat3{m_viewMatrixInverse} * glm::vec3{x, 0, 0};

	updateViewMatrix();
}

void Camera::moveY(float y)
{
	m_targetPos += m_radius * glm::mat3{m_viewMatrixInverse} * glm::vec3{0, y, 0};

	updateViewMatrix();
}

void Camera::addPitch(float pitchRad)
{
	m_pitchRad += pitchRad;

	static constexpr float bound = glm::radians(89.0f);
	if (m_pitchRad < -bound)
	{
		m_pitchRad = -bound;
	}
	if (m_pitchRad > bound)
	{
		m_pitchRad = bound;
	}

	updateViewMatrix();
}

void Camera::addYaw(float yawRad)
{
	m_yawRad += yawRad;

	static constexpr float pi = glm::pi<float>();
	while (m_yawRad < -pi)
	{
		m_yawRad += 2 * pi;
	}
	while (m_yawRad >= pi)
	{
		m_yawRad -= 2 * pi;
	}

	updateViewMatrix();
}

void Camera::updateViewMatrix()
{
	glm::vec3 pos = getPos();

	glm::vec3 direction = glm::normalize(pos - m_targetPos);
	glm::vec3 right = glm::normalize(glm::cross(glm::vec3{0, 1, 0}, direction));
	glm::vec3 up = glm::cross(direction, right);

	m_viewMatrixInverse =
	{
		right.x, right.y, right.z, 0,
		up.x, up.y, up.z, 0,
		direction.x, direction.y, direction.z, 0,
		pos.x, pos.y, pos.z, 1
	};
}

float Camera::getAspectRatio() const
{
	return static_cast<float>(m_viewportSize.x) / m_viewportSize.y;
}

glm::vec3 Camera::getPos() const
{
	return m_targetPos + m_radius *
		glm::vec3
		{
			-std::cos(m_pitchRad) * std::sin(m_yawRad),
			-std::sin(m_pitchRad),
			std::cos(m_pitchRad) * std::cos(m_yawRad)
		};
}

void Camera::updateShaders() const
{
	glm::mat4 projectionViewMatrix = m_projectionMatrix * glm::inverse(m_viewMatrixInverse);
	glm::mat4 projectionViewMatrixInverse = glm::inverse(projectionViewMatrix);

	ShaderPrograms::frame->use();
	ShaderPrograms::frame->setUniform("projectionViewMatrix", projectionViewMatrix);

	ShaderPrograms::plane->use();
	ShaderPrograms::plane->setUniform("projectionViewMatrix", projectionViewMatrix);
	ShaderPrograms::plane->setUniform("projectionViewMatrixInverse", projectionViewMatrixInverse);
}
