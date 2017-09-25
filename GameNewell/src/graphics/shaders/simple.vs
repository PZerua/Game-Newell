#version 450
layout (location = 0) in vec2 aVertex;
layout (location = 2) in vec2 aUvs;
layout (location = 3) in mat4 aModel;

uniform mat4 uProjection;

out vec2 vUvs;

void main(void)
{
	vUvs = aUvs;
	gl_Position = uProjection * aModel * vec4( aVertex, 0.0, 1.0 );
}