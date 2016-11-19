attribute vec3 a_vertex;

uniform mat4 u_mvp;
uniform vec3 color;

void main(void)
{
	gl_Position = u_mvp * vec4( a_vertex, 1.0 );
}