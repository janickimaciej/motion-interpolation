#include "guis/leftPanel.hpp"

#include "interpolationType.hpp"

#include <imgui/imgui.h>

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
		"##start");

	ImGui::SeparatorText("End");
	updatePosAndOrientation(
		[this] () { return m_scene.getEndPos(); },
		[this] (const glm::vec3& pos) { m_scene.setEndPos(pos); },
		"##end");

	ImGui::SeparatorText("Animation");
	updateButtons();

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
	const std::function<void(const glm::vec3&)>& posSetter, const std::string& suffix)
{
	ImGui::PushItemWidth(68);

	glm::vec3 pos = posGetter();
	glm::vec3 prevPos = pos;

	constexpr float speed = 0.01f;
	ImGui::Text("Position");
	ImGui::DragFloat(("x" + suffix).c_str(), &pos.x, speed);
	ImGui::SameLine();
	ImGui::DragFloat(("y" + suffix).c_str(), &pos.y, speed);
	ImGui::SameLine();
	ImGui::DragFloat(("z" + suffix).c_str(), &pos.z, speed);

	if (pos != prevPos)
	{
		posSetter(pos);
	}

	ImGui::PopItemWidth();
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
