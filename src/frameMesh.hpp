#pragma once

class FrameMesh
{
public:
	FrameMesh();
	~FrameMesh();
	void render() const;

private:
	unsigned int m_VAO{};
};
