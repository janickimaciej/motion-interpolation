#pragma once

#include "camera/camera.hpp"
#include "frameMesh.hpp"

#include <glm/glm.hpp>

#include <memory>

class Frame
{
public:
	Frame(bool intermediate = true);

	void render() const;

	glm::vec3 getPos() const;
	void setPos(const glm::vec3& pos);
	void setEulerAngles(const glm::vec3& angles);
	void setQuat(const glm::vec4& quat);

private:
	static std::unique_ptr<FrameMesh> m_mainFrameMesh;
	static std::unique_ptr<FrameMesh> m_intermediateFrameMesh;
	glm::vec3 m_pos{0, 0, 0};
	glm::mat4 m_rotationMatrix{1};
	glm::mat4 m_modelMatrix{1};
	bool m_intermediate{};

	void updateShaders() const;
	void updateModelMatrix();
};
