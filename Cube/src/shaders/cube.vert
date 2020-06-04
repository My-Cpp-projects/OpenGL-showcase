



#version 420 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal_coords;
layout (location = 2) in vec2 texture_coord;

out VS_OUT
{
    vec2 TexCoord;
    vec3 NormalCoord;
    vec3 FragPos;
} vs_out;

layout(std140) uniform VPMatrices
{
    mat4 view_matrix;
    mat4 projection_matrix;
};

uniform mat4 model_matrix;

void main(void)
{
    gl_Position = projection_matrix * view_matrix * model_matrix * vec4(position, 1.0);
    vs_out.TexCoord = texture_coord;
    vs_out.NormalCoord = normal_coords;
    vs_out.FragPos = vec3(model_matrix * vec4(position, 1.0));
}