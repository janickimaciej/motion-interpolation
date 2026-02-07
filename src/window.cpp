#include "window.hpp"

#include "shaderPrograms.hpp"

#include <cmath>
#include <string>

Window::Window()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_SAMPLES, 4);
	static const std::string windowTitle = "motion-iterpolation";
	m_windowPtr = glfwCreateWindow(m_initialSize.x, m_initialSize.y, windowTitle.c_str(), nullptr,
		nullptr);
	glfwSetWindowUserPointer(m_windowPtr, this);
	glfwMakeContextCurrent(m_windowPtr);
	glfwSwapInterval(1);

	glfwSetFramebufferSizeCallback(m_windowPtr, callbackWrapper<&Window::resizeCallback>);
	glfwSetCursorPosCallback(m_windowPtr, callbackWrapper<&Window::cursorMovementCallback>);
	glfwSetScrollCallback(m_windowPtr, callbackWrapper<&Window::scrollCallback>);

	gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress));

	updateViewport();
	ShaderPrograms::init();
}

Window::~Window()
{
	glfwTerminate();
}

void Window::init(Scene& scene)
{
	m_scene = &scene;
}

bool Window::shouldClose() const
{
	return glfwWindowShouldClose(m_windowPtr);
}

void Window::swapBuffers() const
{
	glfwSwapBuffers(m_windowPtr);
}

void Window::pollEvents() const
{
	glfwPollEvents();
}

const glm::ivec2& Window::viewportSize() const
{
	return m_viewportSize;
}

GLFWwindow* Window::getPtr()
{
	return m_windowPtr;
}

void Window::resizeCallback(int width, int height)
{
	if (width == 0 || height == 0)
	{
		return;
	}

	m_viewportSize = {width - LeftPanel::width, height};
	m_scene->updateViewportSize();
	updateViewport();
}

void Window::cursorMovementCallback(double x, double y)
{
	glm::vec2 currPos{static_cast<float>(x), static_cast<float>(y)};
	glm::vec2 offset = currPos - m_lastCursorPos;
	m_lastCursorPos = currPos;

	if ((!isKeyPressed(GLFW_KEY_LEFT_SHIFT) &&
		isButtonPressed(GLFW_MOUSE_BUTTON_MIDDLE))
		||
		(isKeyPressed(GLFW_KEY_LEFT_ALT) &&
		isButtonPressed(GLFW_MOUSE_BUTTON_LEFT)))
	{
		static constexpr float sensitivity = 0.002f;
		m_scene->addPitchCamera(-sensitivity * offset.y);
		m_scene->addYawCamera(sensitivity * offset.x);
	}

	if ((isKeyPressed(GLFW_KEY_LEFT_SHIFT) &&
		isButtonPressed(GLFW_MOUSE_BUTTON_MIDDLE))
		||
		(isKeyPressed(GLFW_KEY_RIGHT_SHIFT) &&
		isButtonPressed(GLFW_MOUSE_BUTTON_LEFT)))
	{
		static constexpr float sensitivity = 0.001f;
		m_scene->moveXCamera(-sensitivity * offset.x);
		m_scene->moveYCamera(sensitivity * offset.y);
	}

	if (isKeyPressed(GLFW_KEY_RIGHT_ALT) &&
		isButtonPressed(GLFW_MOUSE_BUTTON_LEFT))
	{
		static constexpr float sensitivity = 1.005f;
		m_scene->zoomCamera(std::pow(sensitivity, -offset.y));
	}
}

void Window::scrollCallback(double, double yOffset)
{
	if (isCursorInGUI())
	{
		return;
	}

	static constexpr float sensitivity = 1.1f;
	m_scene->zoomCamera(std::pow(sensitivity, static_cast<float>(yOffset)));
}

void Window::updateViewport() const
{
	glViewport(LeftPanel::width, 0, m_viewportSize.x, m_viewportSize.y);
}

glm::vec2 Window::getCursorPos() const
{
	double x{};
	double y{};
	glfwGetCursorPos(m_windowPtr, &x, &y);
	return {static_cast<float>(x), static_cast<float>(y)};
}

bool Window::isButtonPressed(int button)
{
	return glfwGetMouseButton(m_windowPtr, button) == GLFW_PRESS;
}

bool Window::isKeyPressed(int key)
{
	return glfwGetKey(m_windowPtr, key) == GLFW_PRESS;
}

bool Window::isCursorInGUI()
{
	glm::vec2 cursorPos = getCursorPos();
	return cursorPos.x <= LeftPanel::width;
}
