#pragma once

#include <array>
#include <string>

enum class InterpolationType
{
	euler,
	quatLinear,
	quatSlerp
};

inline const std::array<std::string, 3> interpolationTypeLabels
{
	"Euler",
	"Quaternion linear",
	"Quaternion slerp"
};
