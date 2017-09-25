#version 450

uniform sampler2DArray uTextureArray;

in vec2 oUvs;
flat in uint oTextureIndex;
out vec4 FragColor;

void main (void)
{
    FragColor = texture(uTextureArray, vec3(oUvs, oTextureIndex));
}