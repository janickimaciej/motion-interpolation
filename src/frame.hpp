#pragma once

#include "cameras/camera.hpp"
#include "frameMesh.hpp"
#include "shaderProgram.hpp"
#include "shaderPrograms.hpp"

#include <glm/glm.hpp>

#include <memory>

class Frame
{
public:
	Frame(bool intermediate = true);

	void render() const;

	glm::vec3 getPos() const;
	void setPos(const glm::vec3& pos);

private:
	static std::unique_ptr<FrameMesh> m_mainFrameMesh;
	static std::unique_ptr<FrameMesh> m_intermediateFrameMesh;
	const ShaderProgram& m_shaderProgram = *ShaderPrograms::frame;
	glm::vec3 m_pos{0, 0, 0};
	bool m_intermediate{};

	void updateShaders() const;
};
