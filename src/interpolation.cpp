#include "interpolation.hpp"

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

float Interpolation::getTime() const
{
	return m_currentTime;
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

void Interpolation::updateFrames()
{
	glm::vec3 pos = m_startPos + (m_endPos - m_startPos) * m_currentTime / m_endTime;
	m_eulerFrame.setPos(pos);
	m_quaternionLinearFrame.setPos(pos);
	m_quaternionSlerpFrame.setPos(pos);
}

Interpolation::TimePoint Interpolation::now()
{
	return std::chrono::high_resolution_clock::now();
}
