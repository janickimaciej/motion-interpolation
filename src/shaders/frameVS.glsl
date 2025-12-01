#version 420 core

uniform mat4 modelMatrix;
uniform mat4 projectionViewMatrix;

void main()
{
	vec4 pos = modelMatrix * vec4(0, 0, 0, 1);
	gl_Position = projectionViewMatrix * pos;
}
