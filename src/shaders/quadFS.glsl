#version 420 core

in vec2 texturePos;

uniform sampler2D textureSampler;

out vec4 outColor;

void main()
{
	outColor = vec4(texture(textureSampler, texturePos).rgb, 1);
}
