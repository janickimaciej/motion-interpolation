#pragma once

#include "quad.hpp"

class Plane
{
public:
	Plane(float scale);
	void render() const;

private:
	float m_scale{};
	Quad m_quad{};

	void updateShaders() const;
};
