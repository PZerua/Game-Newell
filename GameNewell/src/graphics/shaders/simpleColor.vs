#version 330
layout (location = 0) in vec3 aVertex;
layout (location = 1) in vec4 aColor;

uniform mat4 u_mvp;
out vec4 color;

void main(void)
{
	color = aColor;
	gl_Position = u_mvp * vec4( aVertex, 1.0 );
}