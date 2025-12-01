#include "guis/leftPanel.hpp"

#include "interpolationType.hpp"

#include <imgui/imgui.h>

#include <algorithm>

LeftPanel::LeftPanel(Scene& scene, const glm::ivec2& viewportSize) :
	m_scene{scene},
	m_viewportSize{viewportSize}
{ }

void LeftPanel::update()
{
	ImGui::SetNextWindowPos({0, 0}, ImGuiCond_Always);
	ImGui::SetNextWindowSize({width, static_cast<float>(m_viewportSize.y)}, ImGuiCond_Always);
	ImGui::Begin("leftPanel", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar);
	ImGui::PushItemWidth(100);

	ImGui::SeparatorText("Camera");
	updateCamera();

	ImGui::SeparatorText("Interpolation type");
	ImGui::Spacing();
	ImGui::Text("Left pane");
	updateInterpolationType(
		[this] () { return m_scene.getInterpolationTypeLeft(); },
		[this] (InterpolationType type) { m_scene.setInterpolationTypeLeft(type); },
		"##interpolationTypeLeft");
	ImGui::Spacing();
	ImGui::Text("Right pane");
	updateInterpolationType(
		[this] () { return m_scene.getInterpolationTypeRight(); },
		[this] (InterpolationType type) { m_scene.setInterpolationTypeRight(type); },
		"##interpolationTypeRight");

	ImGui::SeparatorText("Start");
	updatePosAndOrientation(
		[this] () { return m_scene.getStartPos(); },
		[this] (const glm::vec3& pos) { m_scene.setStartPos(pos); },
		[this] () { return m_scene.getStartEulerAngles(); },
		[this] (const glm::vec3& eulerAngles) { m_scene.setStartEulerAngles(eulerAngles); },
		[this] () { return m_scene.getStartQuat(); },
		[this] (const glm::vec4& quat) { m_scene.setStartQuat(quat); },
		[this] () { m_scene.normalizeStartQuat(); },
		"##start");

	ImGui::SeparatorText("End");
	updatePosAndOrientation(
		[this] () { return m_scene.getEndPos(); },
		[this] (const glm::vec3& pos) { m_scene.setEndPos(pos); },
		[this] () { return m_scene.getEndEulerAngles(); },
		[this] (const glm::vec3& eulerAngles) { m_scene.setEndEulerAngles(eulerAngles); },
		[this] () { return m_scene.getEndQuat(); },
		[this] (const glm::vec4& quat) { m_scene.setEndQuat(quat); },
		[this] () { m_scene.normalizeEndQuat(); },
		"##end");

	ImGui::SeparatorText("Animation");
	updateAnimationTime();
	ImGui::Spacing();
	updateIntermediateFrames();
	ImGui::Spacing();
	updateButtons();
	ImGui::Spacing();
	updateTime();

	ImGui::PopItemWidth();
	ImGui::End();
}

void LeftPanel::updateCamera()
{
	m_scene.updateCameraGUI();
}

void LeftPanel::updateInterpolationType(const std::function<InterpolationType(void)>& getter,
	const std::function<void(InterpolationType)>& setter, const std::string& suffix)
{
	ImGui::PushItemWidth(170);

	InterpolationType interpolationType = getter();
	if (ImGui::BeginCombo(suffix.c_str(),
		interpolationTypeLabels[static_cast<int>(interpolationType)].c_str()))
	{
		for (int i = 0; i < 3; ++i)
		{
			bool isSelected = i == static_cast<int>(interpolationType);
			if (ImGui::Selectable(interpolationTypeLabels[i].c_str(), isSelected))
			{
				setter(static_cast<InterpolationType>(i));
			}
		}
		ImGui::EndCombo();
	}

	ImGui::PopItemWidth();
}

void LeftPanel::updatePosAndOrientation(const std::function<glm::vec3(void)>& posGetter,
	const std::function<void(const glm::vec3&)>& posSetter,
	const std::function<glm::vec3(void)>& eulerAnglesGetter,
	const std::function<void(const glm::vec3&)>& eulerAnglesSetter,
	const std::function<glm::vec4(void)>& quatGetter,
	const std::function<void(const glm::vec4&)>& quatSetter,
	const std::function<void(void)>& normalizeQuat, const std::string& suffix)
{
	ImGui::PushItemWidth(69);

	glm::vec3 pos = posGetter();
	glm::vec3 prevPos = pos;

	constexpr float speedPos = 0.01f;
	ImGui::Text("Position");
	ImGui::DragFloat(("x" + suffix + "Pos").c_str(), &pos.x, speedPos);
	ImGui::SameLine();
	ImGui::DragFloat(("y" + suffix + "Pos").c_str(), &pos.y, speedPos);
	ImGui::SameLine();
	ImGui::DragFloat(("z" + suffix + "Pos").c_str(), &pos.z, speedPos);

	if (pos != prevPos)
	{
		posSetter(pos);
	}

	glm::vec3 eulerAngles = glm::degrees(eulerAnglesGetter());
	glm::vec3 prevEulerAngles = eulerAngles;

	constexpr float speedEulerAngles = 0.2f;
	ImGui::Text("Euler angles");
	ImGui::DragFloat(("x" + suffix + "EulerAngles").c_str(), &eulerAngles.x, speedEulerAngles,
		-180.0f, 180.f, "%.2f", ImGuiSliderFlags_AlwaysClamp);
	ImGui::SameLine();
	ImGui::DragFloat(("y" + suffix + "EulerAngles").c_str(), &eulerAngles.y, speedEulerAngles,
		-90.0f, 90.f, "%.2f", ImGuiSliderFlags_AlwaysClamp);
	ImGui::SameLine();
	ImGui::DragFloat(("z" + suffix + "EulerAngles").c_str(), &eulerAngles.z, speedEulerAngles,
		-180.0f, 180.f, "%.2f", ImGuiSliderFlags_AlwaysClamp);

	if (eulerAngles != prevEulerAngles)
	{
		eulerAnglesSetter(glm::radians(eulerAngles));
	}

	ImGui::PopItemWidth();
	ImGui::PushItemWidth(47);

	glm::vec4 quat = quatGetter();
	glm::vec4 prevQuat = quat;

	constexpr float speedQuat = 0.01f;
	ImGui::Text("Quaternion");
	ImGui::DragFloat(("x" + suffix + "Quaternion").c_str(), &quat.x, speedQuat);
	ImGui::SameLine();
	ImGui::DragFloat(("y" + suffix + "Quaternion").c_str(), &quat.y, speedQuat);
	ImGui::SameLine();
	ImGui::DragFloat(("z" + suffix + "Quaternion").c_str(), &quat.z, speedQuat);
	ImGui::SameLine();
	ImGui::DragFloat(("w" + suffix + "Quaternion").c_str(), &quat.w, speedPos);

	if (quat != prevQuat)
	{
		quatSetter(quat);
	}

	ImGui::PopItemWidth();
	ImGui::Spacing();

	if (ImGui::Button(("Normalize" + suffix + "Normalize").c_str()))
	{
		normalizeQuat();
	}
}

void LeftPanel::updateAnimationTime()
{
	float animationTime = m_scene.getAnimationTime();
	float prevAnimationTime = animationTime;

	constexpr float speed = 0.1f;
	ImGui::Text("Time");
	ImGui::DragFloat("##animationTime", &animationTime, speed, 0.01f, 3600.0f, "%.2f",
		ImGuiSliderFlags_AlwaysClamp);

	if (animationTime != prevAnimationTime)
	{
		m_scene.setAnimationTime(animationTime);
	}
}

void LeftPanel::updateIntermediateFrames()
{
	int intermediateFrames = m_scene.getIntermediateFrameCount();
	int prevIntermediateFrames = intermediateFrames;

	constexpr float speed = 0.1f;
	ImGui::Text("Intermediate frames");
	ImGui::DragInt("##intermediateFrames", &intermediateFrames, speed, 2, 100, "%d",
		ImGuiSliderFlags_AlwaysClamp);

	if (intermediateFrames != prevIntermediateFrames)
	{
		m_scene.setIntermediateFrameCount(intermediateFrames);
	}

	ImGui::SameLine();

	bool render = m_scene.getRenderIntermediateFrames();
	bool prevRender = render;

	ImGui::Checkbox("render", &render);

	if (render != prevRender)
	{
		m_scene.setRenderIntermediateFrames(render);
	}
}

void LeftPanel::updateButtons()
{
	if (ImGui::Button("Start"))
	{
		m_scene.startInterpolation();
	}

	ImGui::SameLine();

	if (ImGui::Button("Stop"))
	{
		m_scene.stopInterpolation();
	}

	ImGui::SameLine();

	if (ImGui::Button("Reset"))
	{
		m_scene.resetInterpolation();
	}
}

void LeftPanel::updateTime()
{
	ImGui::Text("t = %.2f s", m_scene.getTime());
}
