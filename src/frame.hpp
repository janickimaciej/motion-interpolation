#pragma once

#include "cameras/camera.hpp"
#include "frameMesh.hpp"
#include "shaderProgram.hpp"
#include "shaderPrograms.hpp"

#include <glm/glm.hpp>

class Frame
{
public:
	Frame();

	void render() const;

	glm::vec3 getPos() const;
	void setPos(const glm::vec3& pos);

private:
	const ShaderProgram& m_shaderProgram = *ShaderPrograms::frame;
	glm::vec3 m_pos{0, 0, 0};
	FrameMesh m_mesh{};

	void updateShaders() const;
};
