#version 330
layout (location = 0) in vec3 aVertex;

uniform mat4 u_mvp;

void main(void)
{
	gl_Position = u_mvp * vec4( aVertex, 1.0 );
}