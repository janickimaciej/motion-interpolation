#pragma once

#include <glm/glm.hpp>

class Camera
{
public:
	Camera(const glm::ivec2& viewportSize, float nearPlane, float farPlane);
	virtual ~Camera() = default;

	void use() const;
	virtual void updateGUI() = 0;
	void setViewportSize(const glm::ivec2& viewportSize);

	void moveX(float x);
	void moveY(float y);
	void addPitch(float pitchRad);
	void addYaw(float yawRad);
	virtual void zoom(float zoom) = 0;

protected:
	float m_nearPlane{};
	float m_farPlane{};
	float m_radius = 5;

	glm::mat4 m_viewMatrixInverse{1};
	glm::mat4 m_projectionMatrix{1};

	void updateViewMatrix();
	virtual void updateProjectionMatrix() = 0;

	float getAspectRatio() const;

private:
	glm::ivec2 m_viewportSize{};
	glm::vec3 m_targetPos{0, 0, 0};
	float m_pitchRad = 0;
	float m_yawRad = 0;

	glm::vec3 getPos() const;
	void updateShaders() const;
};
