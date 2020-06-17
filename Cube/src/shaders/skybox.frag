



#version 420 core
out vec4 FragColor;

in VS_OUT
{
    vec3 TexCoords;
} vs_in;

uniform samplerCube skybox;

void main()
{    
    FragColor = texture(skybox, vs_in.TexCoords);
}