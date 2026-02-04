#include "interpolation.hpp"

#include <glm/gtc/constants.hpp>

#include <algorithm>
#include <cmath>
#include <cstddef>

Interpolation::Interpolation(Frame& eulerFrame, std::vector<Frame>& eulerFrames,
	Frame& quatLinearFrame, std::vector<Frame>& quatLinearFrames,
	Frame& quatSlerpFrame, std::vector<Frame>& quatSlerpFrames) :
	m_eulerFrame{eulerFrame},
	m_eulerFrames{eulerFrames},
	m_quatLinearFrame{quatLinearFrame},
	m_quatLinearFrames{quatLinearFrames},
	m_quatSlerpFrame{quatSlerpFrame},
	m_quatSlerpFrames{quatSlerpFrames}
{ }

void Interpolation::start()
{
	if (m_running)
	{
		return;
	}

	m_startTime = now() - std::chrono::duration<float>(m_currTime);
	m_running = true;
}

void Interpolation::stop()
{
	m_running = false;
}

void Interpolation::reset()
{
	stop();
	m_currTime = 0;
	updateFrames();
}

void Interpolation::update()
{
	if (!m_running)
	{
		return;
	}

	m_currTime =
		std::chrono::duration_cast<std::chrono::duration<float>>(now() - m_startTime).count();
	if (m_currTime >= m_endTime)
	{
		m_currTime = m_endTime;
		m_running = false;
	}
	updateFrames();
}

void Interpolation::updateFrames()
{
	m_eulerFrame.setPos(interpolatePos(m_currTime));
	m_eulerFrame.setEulerAngles(interpolateEulerAngles(m_currTime));

	m_quatLinearFrame.setPos(interpolatePos(m_currTime));
	m_quatLinearFrame.setQuat(interpolateQuatLinear(m_currTime));

	m_quatSlerpFrame.setPos(interpolatePos(m_currTime));
	m_quatSlerpFrame.setQuat(interpolateQuatSlerp(m_currTime));

	std::size_t intermediateFrameCount = m_eulerFrames.size();
	float dTime = m_endTime / (intermediateFrameCount - 1);
	for (int i = 0; i < intermediateFrameCount; ++i)
	{
		float time = i * dTime;

		m_eulerFrames[i].setPos(interpolatePos(time));
		m_eulerFrames[i].setEulerAngles(interpolateEulerAngles(time));

		m_quatLinearFrames[i].setPos(interpolatePos(time));
		m_quatLinearFrames[i].setQuat(interpolateQuatLinear(time));

		m_quatSlerpFrames[i].setPos(interpolatePos(time));
		m_quatSlerpFrames[i].setQuat(interpolateQuatSlerp(time));
	}
}

float Interpolation::getTime() const
{
	return m_currTime;
}

float Interpolation::getEndTime() const
{
	return m_endTime;
}

void Interpolation::setEndTime(float time)
{
	m_endTime = time;
	m_currTime = std::min(m_currTime, time);
	updateFrames();
}

glm::vec3 Interpolation::getStartPos() const
{
	return m_startPos;
}

void Interpolation::setStartPos(const glm::vec3& pos)
{
	m_startPos = pos;
	updateFrames();
}

glm::vec3 Interpolation::getStartEulerAngles() const
{
	return m_startEulerAngles;
}

void Interpolation::setStartEulerAngles(const glm::vec3& eulerAngles)
{
	m_startEulerAngles = eulerAngles;
	m_startQuat = eulerAnglesToQuat(eulerAngles);
	updateFrames();
}

glm::vec4 Interpolation::getStartQuat() const
{
	return m_startQuat;
}

void Interpolation::setStartQuat(const glm::vec4& quat)
{
	m_startQuat = quat;
	m_startEulerAngles = quatToEulerAngles(glm::normalize(quat));
	updateFrames();
}

void Interpolation::normalizeStartQuat()
{
	m_startQuat = glm::normalize(m_startQuat);
}

glm::vec3 Interpolation::getEndPos() const
{
	return m_endPos;
}

void Interpolation::setEndPos(const glm::vec3& pos)
{
	m_endPos = pos;
	updateFrames();
}

glm::vec3 Interpolation::getEndEulerAngles() const
{
	return m_endEulerAngles;
}

void Interpolation::setEndEulerAngles(const glm::vec3& eulerAngles)
{
	m_endEulerAngles = eulerAngles;
	m_endQuat = eulerAnglesToQuat(eulerAngles);
	updateFrames();
}

glm::vec4 Interpolation::getEndQuat() const
{
	return m_endQuat;
}

void Interpolation::setEndQuat(const glm::vec4& quat)
{
	m_endQuat = quat;
	m_endEulerAngles = quatToEulerAngles(glm::normalize(quat));
	updateFrames();
}

void Interpolation::normalizeEndQuat()
{
	m_endQuat = glm::normalize(m_endQuat);
}

glm::vec3 Interpolation::interpolatePos(float time) const
{
	return m_startPos + (m_endPos - m_startPos) * time / m_endTime;
}

glm::vec3 Interpolation::interpolateEulerAngles(float time) const
{
	glm::vec3 start = m_startEulerAngles;
	glm::vec3 end = m_endEulerAngles;

	if (end.x - start.x > glm::pi<float>()) end.x -= 2 * glm::pi<float>();
	if (start.x - end.x > glm::pi<float>()) start.x -= 2 * glm::pi<float>();
	if (end.z - start.z > glm::pi<float>()) end.z -= 2 * glm::pi<float>();
	if (start.z - end.z > glm::pi<float>()) start.z -= 2 * glm::pi<float>();

	return start + (end - start) * time / m_endTime;
}

glm::vec4 Interpolation::interpolateQuatLinear(float time) const
{
	glm::vec4 start = glm::normalize(m_startQuat);
	glm::vec4 end = glm::normalize(m_endQuat);

	return glm::normalize(start + (end - start) * time / m_endTime);
}

glm::vec4 Interpolation::interpolateQuatSlerp(float time) const
{
	glm::vec4 start = glm::normalize(m_startQuat);
	glm::vec4 end = glm::normalize(m_endQuat);

	glm::vec4 startInv{-glm::vec3{start}, start.w};

	glm::vec4 product = quatProduct(startInv, end);
	glm::vec3 productV = product;
	float angle = 2 * std::atan2(glm::length(productV), product.w) * time / m_endTime;
	glm::vec3 axis = productV == glm::vec3{0, 0, 0} ? glm::vec3{0, 0, 0} : glm::normalize(productV);
	return quatProduct(start, glm::vec4{std::sin(angle / 2.0f) * axis, std::cos(angle / 2.0f)});
}

glm::vec4 Interpolation::eulerAnglesToQuat(const glm::vec3& eulerAngles)
{
	glm::vec4 quat{};

	float cx = std::cos(eulerAngles.x * 0.5f);
	float sx = std::sin(eulerAngles.x * 0.5f);
	float cy = std::cos(eulerAngles.y * 0.5f);
	float sy = std::sin(eulerAngles.y * 0.5f);
	float cz = std::cos(eulerAngles.z * 0.5f);
	float sz = std::sin(eulerAngles.z * 0.5f);

	quat.x = sx * cy * cz - cx * sy * sz;
	quat.y = cx * sy * cz + sx * cy * sz;
	quat.z = cx * cy * sz - sx * sy * cz;
	quat.w = cx * cy * cz + sx * sy * sz;

	return quat;
}

glm::vec3 Interpolation::quatToEulerAngles(const glm::vec4& quat)
{
	glm::vec3 eulerAngles{};

	eulerAngles.x = std::atan2(2 * (quat.w * quat.x + quat.y * quat.z),
		1 - 2 * (quat.x * quat.x + quat.y * quat.y));
	eulerAngles.y = std::asin(2 * (quat.w * quat.y - quat.x * quat.z));
	eulerAngles.z = std::atan2(2 * (quat.w * quat.z + quat.x * quat.y),
		1 - 2 * (quat.y * quat.y + quat.z * quat.z));

	return eulerAngles;
}

glm::vec4 Interpolation::quatProduct(const glm::vec4& q1, const glm::vec4& q2)
{
	glm::vec3 q1V = q1;
	glm::vec3 q2V = q2;
	glm::vec3 crossProduct = glm::cross(q1V, q2V);
	float dotProduct = glm::dot(q1V, q2V);
	glm::vec3 qV = crossProduct + q1.w * q2V + q2.w * q1V;
	return glm::vec4{qV, q1.w * q2.w - dotProduct};
}

Interpolation::TimePoint Interpolation::now()
{
	return std::chrono::high_resolution_clock::now();
}
