#pragma once

#include "shaderProgram.hpp"

#include <memory>

namespace ShaderPrograms
{
	void init();

	extern std::unique_ptr<const ShaderProgram> frame;
	extern std::unique_ptr<const ShaderProgram> plane;
	extern std::unique_ptr<const ShaderProgram> quad;
}
