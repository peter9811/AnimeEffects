#version 140

uniform sampler2D uDestTexture;

in vec2 vDestCoord;

out vec4 oFragColor;

void main(void)
{
    oFragColor = texelFetch(uDestTexture, ivec2(vDestCoord), 0);
}
