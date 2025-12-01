#pragma once

#include <array>
#include <string>

enum class InterpolationType
{
	euler,
	quaternionLinear,
	quaternionSlerp
};

inline const std::array<std::string, 3> interpolationTypeLabels
{
	"Euler",
	"Quaternion linear",
	"Quaternion slerp"
};
