#pragma once

#include "frame.hpp"

#include <chrono>
#include <vector>

class Interpolation
{
	using TimePoint = std::chrono::time_point<std::chrono::high_resolution_clock,
		std::chrono::duration<float, std::nano>>;

public:
	Interpolation(Frame& eulerFrame, std::vector<Frame>& eulerFrames, Frame& quaternionLinearFrame,
		std::vector<Frame>& quaternionLinearFrames, Frame& quaternionSlerpFrame,
		std::vector<Frame>& quaternionSlerpFrames);
	void start();
	void stop();
	void reset();
	void update();
	void updateFrames();
	float getTime() const;

	float getEndTime() const;
	void setEndTime(float time);
	glm::vec3 getStartPos() const;
	void setStartPos(const glm::vec3& pos);
	glm::vec3 getEndPos() const;
	void setEndPos(const glm::vec3& pos);

private:
	Frame& m_eulerFrame;
	std::vector<Frame>& m_eulerFrames;
	Frame& m_quaternionLinearFrame;
	std::vector<Frame>& m_quaternionLinearFrames;
	Frame& m_quaternionSlerpFrame;
	std::vector<Frame>& m_quaternionSlerpFrames;

	TimePoint m_startTime{};
	float m_currentTime = 0;
	float m_endTime = 5;
	bool m_running = false;

	glm::vec3 m_startPos{0, 0, 0};
	glm::vec3 m_endPos{1, 1, 1};

	glm::vec3 interpolatePos(float time) const;
	static TimePoint now();
};
