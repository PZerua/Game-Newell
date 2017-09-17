#version 330
layout (location = 0) in vec2 aVertex;
layout (location = 1) in vec3 aColor;

uniform mat4 u_mvp;
out vec3 color;

void main(void)
{
	color = aColor;
	gl_Position = u_mvp * vec4( aVertex, 0.0, 1.0 );
}