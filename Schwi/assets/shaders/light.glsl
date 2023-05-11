#type vertex
#version 330 core
layout(location = 0) in vec3 a_Pos;
layout(location = 1) in vec2 a_TexCoord;
layout(location = 2) in vec3 a_Normal;

out vec3 FragPos;
out vec3 Normal;
out vec2 TexCoords;

uniform mat4 u_ViewProjection;
uniform mat4 u_Transform;

void main()
{
    FragPos = vec3(u_Transform * vec4(a_Pos, 1.0));
    Normal = mat3(transpose(inverse(u_Transform))) * a_Normal;
    TexCoords = a_TexCoord;

    gl_Position = u_ViewProjection * vec4(FragPos, 1.0);
}

#type fragment
#version 330 core
out vec4 FragColor;

struct Material {
    vec3 color;
    sampler2D diffuse;
    sampler2D specular;
    float shininess;
};

struct DirLight {
    vec3 direction;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct PointLight {
    vec3 color;
    vec3 position;

    float constant;
    float linear;
    float quadratic;

    float intensity;
};

struct SpotLight {
    vec3 position;
    vec3 direction;
    float cutOff;
    float outerCutOff;

    float constant;
    float linear;
    float quadratic;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

#define MAX_LIGHTS 4

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;

uniform Material u_Material;
uniform float shininess;
uniform vec3 u_ViewPos;
uniform DirLight u_DirLight[MAX_LIGHTS];
uniform PointLight u_PointLight[MAX_LIGHTS];
uniform SpotLight u_SpotLight[MAX_LIGHTS];
uniform int DirLightNum;
uniform int PointLightNum;
uniform int SpotLightNum;

float Ambient = 0.1;
// function prototypes
vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir);
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);
vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir);

void main()
{
    // properties
    vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(u_ViewPos - FragPos);
    vec3 result=vec3(0.0);

    // == =====================================================
    // Our lighting is set up in 3 phases: directional, point lights and an optional flashlight
    // For each phase, a calculate function is defined that calculates the corresponding color
    // per lamp. In the main() function we take all the calculated colors and sum them up for
    // this fragment's final color.
    // == =====================================================
    // phase 1: directional lighting
    for (int i = 0; i < DirLightNum; i++)
        result += CalcDirLight(u_DirLight[i], norm, viewDir);
    // phase 2: point lights
    for (int i = 0; i < PointLightNum; i++)
        result += CalcPointLight(u_PointLight[i], norm, FragPos, viewDir);
    // phase 3: spot light
    for (int i = 0; i < SpotLightNum; i++)
        result += CalcSpotLight(u_SpotLight[i], norm, FragPos, viewDir);

    FragColor = vec4(result, 1.0);
}

// calculates the color when using a directional light.
vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir)
{
    vec3 lightDir = normalize(-light.direction);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), u_Material.shininess);
    // combine results
    vec3 ambient = light.ambient * vec3(texture(u_Material.diffuse, TexCoords));
    vec3 diffuse = light.diffuse * diff * vec3(texture(u_Material.diffuse, TexCoords));
    vec3 specular = light.specular * spec * vec3(texture(u_Material.specular, TexCoords));
    return (ambient + diffuse + specular);
}

// calculates the color when using a point light.
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - fragPos);
	//  environment light
    vec3 ambient = Ambient * light.color * vec3(texture(u_Material.diffuse, TexCoords));
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = vec3(texture(u_Material.diffuse, TexCoords)) * light.color * diff;
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), u_Material.shininess);
    vec3 specular = vec3(texture(u_Material.specular, TexCoords)) * spec * light.color;
    // attenuation
    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));
    // combine results
    return (ambient + diffuse + specular) * attenuation * light.intensity;
}

// calculates the color when using a spot light.
vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - fragPos);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), u_Material.shininess);
    // attenuation
    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));
    // spotlight intensity
    float theta = dot(lightDir, normalize(-light.direction));
    float epsilon = light.cutOff - light.outerCutOff;
    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);
    // combine results
    vec3 ambient = light.ambient * vec3(texture(u_Material.diffuse, TexCoords));
    vec3 diffuse = light.diffuse * diff * vec3(texture(u_Material.diffuse, TexCoords));
    vec3 specular = light.specular * spec * vec3(texture(u_Material.specular, TexCoords));
    ambient *= attenuation * intensity;
    diffuse *= attenuation * intensity;
    specular *= attenuation * intensity;
    return (ambient + diffuse + specular);
}