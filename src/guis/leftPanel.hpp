#pragma once

#include "scene.hpp"

#include <glm/glm.hpp>

#include <functional>
#include <string>

class LeftPanel
{
public:
	static constexpr int width = 272;

	LeftPanel(Scene& scene, const glm::ivec2& viewportSize);
	void update();

private:
	Scene& m_scene;
	const glm::ivec2& m_viewportSize;

	void updateCamera();
	void updateInterpolationType(const std::function<InterpolationType(void)>& getter,
		const std::function<void(InterpolationType)>& setter, const std::string& suffix);
	void updatePosAndOrientation(const std::function<glm::vec3(void)>& posGetter,
		const std::function<void(const glm::vec3&)>& posSetter,
		const std::function<glm::vec3(void)>& eulerAnglesGetter,
		const std::function<void(const glm::vec3&)>& eulerAnglesSetter,
		const std::function<glm::vec4(void)>& quatGetter,
		const std::function<void(const glm::vec4&)>& quatSetter,
		const std::function<void(void)>& normalizeQuat, const std::string& suffix);
	void updateAnimationTime();
	void updateIntermediateFrames();
	void updateButtons();
	void updateTime();
};
