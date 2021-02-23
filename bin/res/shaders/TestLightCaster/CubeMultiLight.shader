#shader vertex
#version 330 core
layout (location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTexCoords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 Normal;
out vec3 FragPos;
out vec2 TexCoords;

void main()
{
    gl_Position = projection * view * model * vec4(aPos, 1.0);
    FragPos = vec3(model * vec4(aPos, 1.0));
    Normal = aNormal;

    TexCoords = aTexCoords;
}


#shader fragment
#version 330 core

struct Material {
    sampler2D diffuseColorMap;
    sampler2D specularColorMap;
    vec3 specularColor;
    float shininess;
};
uniform Material material;

struct PointLight {
    vec3 color;

    float ambientCoefficient;
    float diffuseCoefficient;
    float specularCoefficient;

    vec3 position;
    float K;
    float linear;
    float quadratic;
};
uniform PointLight pointLight;

struct DistantLight {
    vec3 color;

    float ambientCoefficient;
    float diffuseCoefficient;
    float specularCoefficient;

    vec3 direction;
};
uniform DistantLight distantLight;

struct SpotLight {
    vec3 color;

    float ambientCoefficient;
    float diffuseCoefficient;
    float specularCoefficient;

    vec3 position;
    float cutoffAngleCos;
    float outerCutOffAngleCos;
    vec3 spotDirection;
};
uniform SpotLight spotLight;

layout(location = 0) out vec4 FragColor;

in vec3 Normal;
in vec3 FragPos;
in vec2 TexCoords;

uniform vec3 viewPos;
uniform bool EnablePointLight;
uniform bool EnableDistantLight;
uniform bool EnableSpotLight;

vec3 CalculateDistantLight(DistantLight distantLight,vec3 normal,vec3 viewDir);
vec3 CalculatePointLight(PointLight pointLight, vec3 normal, vec3 viewDir);
vec3 CalculateSpotLight(SpotLight spotLight, vec3 normal, vec3 viewDir);

void main()
{
    vec3 result = vec3(0);
    vec3 normal = normalize(Normal);
    vec3 viewDir = normalize(viewPos - FragPos);

    if (EnableDistantLight)
    {
        result += CalculateDistantLight(distantLight, normal, viewDir);
    }

    if (EnablePointLight)
    {
        result += CalculatePointLight(pointLight, normal, viewDir);
    }

    if (EnableSpotLight)
    {
        result += CalculateSpotLight(spotLight, normal, viewDir);
    }

    FragColor = vec4(result, 1.0);
};

vec3 CalculateDistantLight(DistantLight distantLight, vec3 normal, vec3 viewDir)
{
    // ambient
    vec3 ambient = distantLight.ambientCoefficient * distantLight.color * texture(material.diffuseColorMap, TexCoords).rgb;

    // diffuse 
    //norm = normalize(Normal);
    vec3 lightDir = normalize(-distantLight.direction);
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = distantLight.diffuseCoefficient * distantLight.color * diff * texture(material.diffuseColorMap, TexCoords).rgb;

    //specular
    //viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = distantLight.specularCoefficient * distantLight.color * (spec * texture(material.specularColorMap, TexCoords).rgb);

    return ambient + diffuse + specular;
}

vec3 CalculatePointLight(PointLight pointLight, vec3 normal, vec3 viewDir)
{
    float distance = length(pointLight.position - FragPos);
    float attenuation = 1.0 / (pointLight.K + pointLight.linear * distance +
        pointLight.quadratic * (distance * distance));
    // ambient
    vec3 ambient = pointLight.ambientCoefficient * pointLight.color * texture(material.diffuseColorMap, TexCoords).rgb;

    // diffuse 
    //norm = normalize(Normal);
    vec3 lightDir = normalize(pointLight.position - FragPos);
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = pointLight.diffuseCoefficient * pointLight.color * diff * texture(material.diffuseColorMap, TexCoords).rgb;

    //specular
    //viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3  specular = pointLight.specularCoefficient * pointLight.color * (spec * texture(material.specularColorMap, TexCoords).rgb);

    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;

    return ambient + diffuse + specular;
}

vec3 CalculateSpotLight(SpotLight spotLight, vec3 normal, vec3 viewDir)
{
    // spotlight (soft edges)
    vec3 lightDir = normalize(spotLight.position - FragPos);
    float theta = dot(lightDir, normalize(-spotLight.spotDirection));
    float epsilon = (spotLight.cutoffAngleCos - spotLight.outerCutOffAngleCos);
    float intensity = clamp((theta - spotLight.outerCutOffAngleCos) / epsilon, 0.0, 1.0);


    // ambient
    vec3 ambient = spotLight.ambientCoefficient * spotLight.color * texture(material.diffuseColorMap, TexCoords).rgb;

    // diffuse 
    //norm = normalize(Normal);
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = spotLight.diffuseCoefficient * spotLight.color * diff * texture(material.diffuseColorMap, TexCoords).rgb;

    //specular
    //viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = spotLight.specularCoefficient * spotLight.color * (spec * texture(material.specularColorMap, TexCoords).rgb);

    ambient *= intensity;
    diffuse *= intensity;
    specular *= intensity;

    return ambient + diffuse + specular;
}
    
    
    

