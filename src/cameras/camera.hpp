#pragma once

#include <glm/glm.hpp>

class Camera
{
public:
	Camera(float nearPlane, float farPlane);
	virtual ~Camera() = default;
	void use() const;
	virtual void updateGUI() = 0;
	glm::mat4 getMatrix() const;
	void setViewportSize(const glm::ivec2& viewportSize);
	glm::vec3 getPos() const;

	void addPitch(float pitchRad);
	void addYaw(float yawRad);
	void setTargetPos(const glm::vec3& pos);
	void moveX(float x);
	void moveY(float y);
	virtual void zoom(float zoom) = 0;

protected:
	glm::ivec2 m_viewportSize{};
	float m_nearPlane{};
	float m_farPlane{};
	float m_radius = 5;

	glm::mat4 m_viewMatrixInverse{1};
	glm::mat4 m_projectionMatrix{1};

	void updateViewMatrix();
	virtual void updateProjectionMatrix() = 0;

private:
	glm::vec3 m_targetPos{0, 0, 0};
	float m_pitchRad = 0;
	float m_yawRad = 0;

	void updateShaders() const;
};
