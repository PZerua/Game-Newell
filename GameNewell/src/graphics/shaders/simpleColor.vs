#version 330
layout (location = 0) in vec3 aVertex;
layout (location = 3) in mat4 aModel;

uniform mat4 uProjection;

void main(void)
{
	gl_Position = uProjection * aModel * vec4( aVertex, 1.0 );
}