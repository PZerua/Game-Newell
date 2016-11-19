varying vec2 v_coord;
uniform sampler2D color_texture;

void main (void)
{
    vec4 color = texture2D( color_texture, v_coord );
    gl_FragColor = color;
}