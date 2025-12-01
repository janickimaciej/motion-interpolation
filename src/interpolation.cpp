#include "interpolation.hpp"

#include <algorithm>
#include <cstddef>

Interpolation::Interpolation(Frame& eulerFrame, std::vector<Frame>& eulerFrames,
	Frame& quaternionLinearFrame, std::vector<Frame>& quaternionLinearFrames,
	Frame& quaternionSlerpFrame, std::vector<Frame>& quaternionSlerpFrames) :
	m_eulerFrame{eulerFrame},
	m_eulerFrames{eulerFrames},
	m_quaternionLinearFrame{quaternionLinearFrame},
	m_quaternionLinearFrames{quaternionLinearFrames},
	m_quaternionSlerpFrame{quaternionSlerpFrame},
	m_quaternionSlerpFrames{quaternionSlerpFrames}
{ }

void Interpolation::start()
{
	if (m_running)
	{
		return;
	}

	m_startTime = now() - std::chrono::duration<float>(m_currentTime);
	m_running = true;
}

void Interpolation::stop()
{
	m_running = false;
}

void Interpolation::reset()
{
	stop();
	m_currentTime = 0;
	updateFrames();
}

void Interpolation::update()
{
	if (!m_running)
	{
		return;
	}

	m_currentTime =
		std::chrono::duration_cast<std::chrono::duration<float>>(now() - m_startTime).count();
	if (m_currentTime >= m_endTime)
	{
		m_currentTime = m_endTime;
		m_running = false;
	}
	updateFrames();
}

void Interpolation::updateFrames()
{
	glm::vec3 currentPos = interpolatePos(m_currentTime);
	m_eulerFrame.setPos(currentPos);
	m_quaternionLinearFrame.setPos(currentPos);
	m_quaternionSlerpFrame.setPos(currentPos);

	std::size_t intermediateFrameCount = m_eulerFrames.size();
	float dTime = m_endTime / (intermediateFrameCount - 1);
	for (int i = 0; i < intermediateFrameCount; ++i)
	{
		float time = i * dTime;
		glm::vec3 pos = interpolatePos(time);
		m_eulerFrames[i].setPos(pos);
		m_quaternionLinearFrames[i].setPos(pos);
		m_quaternionSlerpFrames[i].setPos(pos);
	}
}

float Interpolation::getTime() const
{
	return m_currentTime;
}

float Interpolation::getEndTime() const
{
	return m_endTime;
}

void Interpolation::setEndTime(float time)
{
	m_endTime = time;
	m_currentTime = std::min(m_currentTime, time);
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

glm::vec3 Interpolation::getEndPos() const
{
	return m_endPos;
}

void Interpolation::setEndPos(const glm::vec3& pos)
{
	m_endPos = pos;
	updateFrames();
}

glm::vec3 Interpolation::interpolatePos(float time) const
{
	return m_startPos + (m_endPos - m_startPos) * time / m_endTime;
}

Interpolation::TimePoint Interpolation::now()
{
	return std::chrono::high_resolution_clock::now();
}
