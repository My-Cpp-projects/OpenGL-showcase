



#version 420 core
out vec4 color;

in VS_OUT
{
    vec2 TexCoord;
    vec3 NormalCoord;
    vec3 FragPos;
} fs_in;

struct Material
{
    vec3 specular;
    float shininess;
};

struct Light 
{
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

uniform sampler2D texture1;
uniform vec3 view_pos;
uniform vec3 light_pos;
uniform Material material;
uniform Light light;

void main(void)
{
    // calculate ambient
    vec3 ambient = texture(texture1, fs_in.TexCoord).rgb * light.ambient;

    // calculate diffuse
    vec3 norm = normalize(fs_in.NormalCoord);
    vec3 lightDir = normalize(light_pos - fs_in.FragPos);  
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse =  texture(texture1, fs_in.TexCoord).rgb * diff * light.diffuse;

    // calculate specular
    float specularStrength = 0.5;
    vec3 viewDir = normalize(view_pos - fs_in.FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = material.specular * spec * light.specular;

    vec3 result = ambient + diffuse + specular;
    color = vec4(result, 1.0);
}