#version 420 core

layout (location = 0) in vec3 inPosQuad;

uniform bool right;

out vec2 texturePos;

void main()
{
	texturePos = (inPosQuad.xy + 1) / 2;
	float x = inPosQuad.x / 2.0 + (right ? 0.5 : -0.5);
	gl_Position = vec4(x, inPosQuad.yz, 1);
}
