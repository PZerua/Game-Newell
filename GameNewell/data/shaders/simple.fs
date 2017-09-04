#version 330
in vec2 Uv;
uniform sampler2D color_texture;

void main (void)
{
    vec4 color = texture2D( color_texture, Uv );
    gl_FragColor = color;
}