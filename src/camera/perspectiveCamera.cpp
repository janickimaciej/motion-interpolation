#include "camera/perspectiveCamera.hpp"

#include <cmath>

PerspectiveCamera::PerspectiveCamera(float fovYDeg, float nearPlane,
	float farPlane) :
	Camera{nearPlane, farPlane},
	m_gui{*this},
	m_fovYDeg{fovYDeg}
{
	updateProjectionMatrix();
}

void PerspectiveCamera::updateGUI()
{
	m_gui.update();
}

float PerspectiveCamera::getFOVYDeg() const
{
	return m_fovYDeg;
}

void PerspectiveCamera::setFOVYDeg(float fovYDeg)
{
	m_fovYDeg = fovYDeg;
	updateProjectionMatrix();
}

void PerspectiveCamera::zoom(float zoom)
{
	m_radius /= zoom;
	updateViewMatrix();
}

void PerspectiveCamera::updateProjectionMatrix()
{
	float aspectRatio = static_cast<float>(m_viewportSize.x) / m_viewportSize.y;
	float fovYRad = glm::radians(m_fovYDeg);
	float cot = std::cos(fovYRad / 2) / std::sin(fovYRad / 2);

	m_projectionMatrix =
	{
		cot / aspectRatio, 0, 0, 0,
		0, cot, 0, 0,
		0, 0, -(m_farPlane + m_nearPlane) / (m_farPlane - m_nearPlane), -1,
		0, 0, -2 * m_farPlane * m_nearPlane / (m_farPlane - m_nearPlane), 0
	};
}
