#include "shaderPrograms.hpp"

#include <string>

namespace ShaderPrograms
{
	std::string path(const std::string& shaderName);

	std::unique_ptr<const ShaderProgram> frame{};
	std::unique_ptr<const ShaderProgram> plane{};
	std::unique_ptr<const ShaderProgram> quad{};

	void init()
	{
		frame = std::make_unique<const ShaderProgram>(path("frameVS"), path("frameGS"),
			path("frameFS"));
		plane = std::make_unique<const ShaderProgram>(path("planeVS"), path("planeFS"));
		quad = std::make_unique<const ShaderProgram>(path("quadVS"), path("quadFS"));
	}

	std::string path(const std::string& shaderName)
	{
		return "src/shaders/" + shaderName + ".glsl";
	}
}
