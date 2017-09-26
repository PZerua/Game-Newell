#version 450

uniform sampler2DArray uTextureArray;

in vec2 oUvs;
flat in uint oTextureIndex;
out vec4 FragColor;

void main (void)
{
	if (oTextureIndex != -1)
		FragColor = texture(uTextureArray, vec3(oUvs, oTextureIndex));
	else 
		FragColor = vec4(1.0, 0.0, 1.0, 1.0);
}