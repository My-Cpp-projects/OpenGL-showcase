



#version 420 core
out vec4 color;

uniform vec3 light_color;

void main(void)
{
    color = vec4(light_color, 1.0);
}