

#version 420 core

out vec4 color;

in VS_OUT
{
    vec2 TexCoord;
} fs_in;

uniform sampler2D texture1;

void main(void)
{
    color = texture(texture1, fs_in.TexCoord);
}