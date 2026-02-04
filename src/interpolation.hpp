#pragma once

#include "frame.hpp"

#include <chrono>
#include <vector>

class Interpolation
{
	using TimePoint = std::chrono::time_point<std::chrono::high_resolution_clock,
		std::chrono::duration<float, std::nano>>;

public:
	Interpolation(Frame& eulerFrame, std::vector<Frame>& eulerFrames, Frame& quatLinearFrame,
		std::vector<Frame>& quatLinearFrames, Frame& quatSlerpFrame,
		std::vector<Frame>& quatSlerpFrames);
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
	glm::vec3 getStartEulerAngles() const;
	void setStartEulerAngles(const glm::vec3& eulerAngles);
	glm::vec4 getStartQuat() const;
	void setStartQuat(const glm::vec4& quat);
	void normalizeStartQuat();

	glm::vec3 getEndPos() const;
	void setEndPos(const glm::vec3& pos);
	glm::vec3 getEndEulerAngles() const;
	void setEndEulerAngles(const glm::vec3& eulerAngles);
	glm::vec4 getEndQuat() const;
	void setEndQuat(const glm::vec4& quat);
	void normalizeEndQuat();

private:
	Frame& m_eulerFrame;
	std::vector<Frame>& m_eulerFrames;
	Frame& m_quatLinearFrame;
	std::vector<Frame>& m_quatLinearFrames;
	Frame& m_quatSlerpFrame;
	std::vector<Frame>& m_quatSlerpFrames;

	TimePoint m_startTime{};
	float m_currTime = 0;
	float m_endTime = 5;
	bool m_running = false;

	glm::vec3 m_startPos{-1, 0, 0};
	glm::vec3 m_startEulerAngles{0, 0, 0};
	glm::vec4 m_startQuat{0, 0, 0, 1};

	glm::vec3 m_endPos{1, 0, 0};
	glm::vec3 m_endEulerAngles{0, 0, 0};
	glm::vec4 m_endQuat{0, 0, 0, 1};

	glm::vec3 interpolatePos(float time) const;
	glm::vec3 interpolateEulerAngles(float time) const;
	glm::vec4 interpolateQuatLinear(float time) const;
	glm::vec4 interpolateQuatSlerp(float time) const;
	static glm::vec4 eulerAnglesToQuat(const glm::vec3& eulerAngles);
	static glm::vec3 quatToEulerAngles(const glm::vec4& quat);
	static glm::vec4 quatProduct(const glm::vec4& q1, const glm::vec4& q2);
	static TimePoint now();
};
