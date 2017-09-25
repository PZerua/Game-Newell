#version 450

uniform sampler2DArray uTextureArray;

in vec2 vUvs;
out vec4 FragColor;

void main (void)
{
    FragColor = texture(uTextureArray, vec3(vUvs, 0));
}