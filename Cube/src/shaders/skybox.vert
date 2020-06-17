



#version 420 core
out VS_OUT
{
    vec3 TexCoords;
} vs_out;

layout(std140) uniform VPMatrices
{
    mat4 view_matrix;
    mat4 projection_matrix;
};

void main()
{
    vec3[4] vertices = vec3[4](vec3(-1.0, -1.0, 1.0),
                                vec3(1.0, -1.0, 1.0),
                                vec3(-1.0, 1.0, 1.0),
                                vec3(1.0, 1.0, 1.0));

    mat4 inverseProjection = inverse(projection_matrix);
    mat3 inverseModelview = transpose(mat3(view_matrix));
    vec3 unprojected = (inverseProjection * vec4(vertices[gl_VertexID], 1.0)).xyz;
    vs_out.TexCoords = inverseModelview * unprojected;

    gl_Position = vec4(vertices[gl_VertexID], 1.0);
}  