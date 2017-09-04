#version 330
layout (location = 0) in vec3 aVertex;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec2 aUv;

uniform mat4 u_mvp;
out vec2 Uv;

void main(void)
{
	Uv = aUv;
	gl_Position = u_mvp * vec4( aVertex, 1.0 );
}