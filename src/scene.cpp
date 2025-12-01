#include "scene.hpp"

#include "shaderPrograms.hpp"

static constexpr float viewHeight = 10.0f;
static constexpr float fovYDeg = 60.0f;
static constexpr float nearPlane = 0.1f;
static constexpr float farPlane = 1000.0f;

Scene::Scene(const glm::ivec2& viewportSize) :
	m_viewportSize{viewportSize},
	m_camera{fovYDeg, nearPlane, farPlane},
	m_eulerFrames(m_intermediateFrameCount),
	m_quaternionLinearFrames(m_intermediateFrameCount),
	m_quaternionSlerpFrames(m_intermediateFrameCount),
	m_interpolation{m_eulerFrame, m_eulerFrames, m_quaternionLinearFrame, m_quaternionLinearFrames,
		m_quaternionSlerpFrame, m_quaternionSlerpFrames}
{
	updateViewportSize();

	addPitchCamera(glm::radians(-30.0f));
	addYawCamera(glm::radians(15.0f));

	m_leftFramebuffer->bind();
	setUpFramebuffer();
	m_leftFramebuffer->unbind();

	m_rightFramebuffer->bind();
	setUpFramebuffer();
	m_rightFramebuffer->unbind();

	setUpFramebuffer();

	m_interpolation.updateFrames();
}

void Scene::update()
{
	m_interpolation.update();
}

void Scene::render()
{
	m_leftFramebuffer->bind();
	clearFramebuffer();
	m_camera.use();
	renderFrames(m_interpolationTypeLeft);
	renderGrid();
	m_leftFramebuffer->unbind();

	m_rightFramebuffer->bind();
	clearFramebuffer();
	m_camera.use();
	renderFrames(m_interpolationTypeRight);
	renderGrid();
	m_rightFramebuffer->unbind();

	clearFramebuffer();

	m_leftFramebuffer->bindTexture();
	ShaderPrograms::quad->use();
	ShaderPrograms::quad->setUniform("right", false);
	m_quad.render();

	m_rightFramebuffer->bindTexture();
	ShaderPrograms::quad->use();
	ShaderPrograms::quad->setUniform("right", true);
	m_quad.render();
}

void Scene::updateViewportSize()
{
	glm::ivec2 halfViewportSize = {m_viewportSize.x / 2, m_viewportSize.y};
	m_camera.setViewportSize(halfViewportSize);
	m_leftFramebuffer = std::make_unique<Framebuffer>(halfViewportSize);
	m_rightFramebuffer = std::make_unique<Framebuffer>(halfViewportSize);
}

void Scene::addPitchCamera(float pitchRad)
{
	m_camera.addPitch(pitchRad);
}

void Scene::addYawCamera(float yawRad)
{
	m_camera.addYaw(yawRad);
}

void Scene::moveXCamera(float x)
{
	m_camera.moveX(x);
}

void Scene::moveYCamera(float y)
{
	m_camera.moveY(y);
}

void Scene::zoomCamera(float zoom)
{
	m_camera.zoom(zoom);
}

void Scene::updateCameraGUI()
{
	m_camera.updateGUI();
}

void Scene::startInterpolation()
{
	m_interpolation.start();
}

void Scene::stopInterpolation()
{
	m_interpolation.stop();
}

void Scene::resetInterpolation()
{
	m_interpolation.reset();
}

InterpolationType Scene::getInterpolationTypeLeft() const
{
	return m_interpolationTypeLeft;
}

void Scene::setInterpolationTypeLeft(InterpolationType type)
{
	m_interpolationTypeLeft = type;
}

InterpolationType Scene::getInterpolationTypeRight() const
{
	return m_interpolationTypeRight;
}

void Scene::setInterpolationTypeRight(InterpolationType type)
{
	m_interpolationTypeRight = type;
}

glm::vec3 Scene::getStartPos() const
{
	return m_interpolation.getStartPos();
}

void Scene::setStartPos(const glm::vec3& pos)
{
	m_interpolation.setStartPos(pos);
}

glm::vec3 Scene::getEndPos() const
{
	return m_interpolation.getEndPos();
}

void Scene::setEndPos(const glm::vec3& pos)
{
	m_interpolation.setEndPos(pos);
}

float Scene::getAnimationTime() const
{
	return m_interpolation.getEndTime();
}

void Scene::setAnimationTime(float time)
{
	m_interpolation.setEndTime(time);
}

int Scene::getIntermediateFrameCount() const
{
	return m_intermediateFrameCount;
}

void Scene::setIntermediateFrameCount(int count)
{
	m_intermediateFrameCount = count;
	m_eulerFrames.resize(count);
	m_quaternionLinearFrames.resize(count);
	m_quaternionSlerpFrames.resize(count);
	m_interpolation.updateFrames();
}

bool Scene::getRenderIntermediateFrames() const
{
	return m_renderIntermediateFrames;
}

void Scene::setRenderIntermediateFrames(bool render)
{
	m_renderIntermediateFrames = render;
}

float Scene::getTime() const
{
	return m_interpolation.getTime();
}

void Scene::setUpFramebuffer() const
{
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_MULTISAMPLE);
}

void Scene::clearFramebuffer() const
{
	static constexpr glm::vec3 backgroundColor{0.1f, 0.1f, 0.1f};
	glClearColor(backgroundColor.r, backgroundColor.g, backgroundColor.b, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Scene::renderGrid() const
{
	m_plane.render();
}

void Scene::renderFrames(InterpolationType type)
{
	switch (type)
	{
		case InterpolationType::euler:
			renderFrames(m_eulerFrame, m_eulerFrames);
			break;

		case InterpolationType::quaternionLinear:
			renderFrames(m_quaternionLinearFrame, m_quaternionLinearFrames);
			break;

		case InterpolationType::quaternionSlerp:
			renderFrames(m_quaternionSlerpFrame, m_quaternionSlerpFrames);
			break;
	}
}

void Scene::renderFrames(const Frame& mainFrame, const std::vector<Frame>& intermediateFrames) const
{
	mainFrame.render();
	if (m_renderIntermediateFrames)
	{
		for (const Frame& frame : intermediateFrames)
		{
			frame.render();
		}
	}
}
