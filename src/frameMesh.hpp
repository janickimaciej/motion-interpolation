#pragma once

class FrameMesh
{
public:
	FrameMesh(bool intermediate);
	~FrameMesh();
	void render() const;

private:
	unsigned int m_VAO{};
	bool m_intermediate{};
};
