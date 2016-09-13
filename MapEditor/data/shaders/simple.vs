attribute vec3 a_vertex;
attribute vec2 a_uv;

uniform mat4 u_mvp;

//this will store the color for the pixel shader
varying vec2 v_coord;

void main(void)
{
    v_coord = a_uv;
	gl_Position = u_mvp * vec4( a_vertex, 1.0 );
}