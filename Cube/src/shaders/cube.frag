



#version 420 core
out vec4 color;

in VS_OUT
{
    vec2 TexCoord;
    vec3 NormalCoord;
    vec3 FragPos;
} fs_in;

layout(std140) uniform SunLight
{
    vec3 direction;
  
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
} sunLight;  

struct Material
{
    sampler2D texture_diffuse1;
    sampler2D texture_specular1;
    float shininess;
};

struct PointLight
{
    vec3 position;
    
    float constant;
    float linear;
    float quadratic;
	
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

uniform vec3 view_pos;
uniform Material material;
uniform PointLight pointLight;

vec3 CalcSunLight(vec3 normal, vec3 viewDir);
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);

void main()
{
    vec3 norm = normalize(fs_in.NormalCoord);
    vec3 viewDir = normalize(view_pos - fs_in.FragPos);

    vec3 result = CalcSunLight(norm, viewDir);
    result += CalcPointLight(pointLight, norm, fs_in.FragPos, viewDir);    
    
    color = vec4(result, 1.0);
}

vec3 CalcSunLight(vec3 normal, vec3 viewDir)
{
    vec3 lightDir = normalize(-sunLight.direction);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    // combine results
    vec3 ambient = sunLight.ambient * vec3(texture(material.texture_diffuse1, fs_in.TexCoord));
    vec3 diffuse = sunLight.diffuse * diff * vec3(texture(material.texture_diffuse1, fs_in.TexCoord));
    vec3 specular = sunLight.specular * spec * vec3(texture(material.texture_specular1, fs_in.TexCoord));
    return (ambient + diffuse + specular);
}  

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - fragPos);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    // attenuation
    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance +  light.quadratic * (distance * distance));    
    // combine results
    vec3 ambient = light.ambient * vec3(texture(material.texture_diffuse1, fs_in.TexCoord));
    vec3 diffuse = light.diffuse * diff * vec3(texture(material.texture_diffuse1, fs_in.TexCoord));
    vec3 specular = light.specular * spec * vec3(texture(material.texture_specular1, fs_in.TexCoord));
    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;
    return (ambient + diffuse + specular);
} 