



#version 450 core

layout (location = 0) in vec4 offset;

const vec4 vertices[] = vec4[]( vec4(0.25, -0.25, 0.5, 1.0),
								vec4(-0.25, -0.25, 0.5, 1.0),
								vec4(0.25, 0.25, 0.5, 1.0));

void main(void)
{
	gl_Position = vertices[gl_VertexID] + offset;
}
