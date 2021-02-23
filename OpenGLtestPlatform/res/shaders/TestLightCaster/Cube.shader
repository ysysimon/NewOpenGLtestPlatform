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
};


#shader fragment
#version 330 core

struct Material {
    sampler2D diffuseColorMap;
    sampler2D specularColorMap;
    vec3 specularColor;
    float shininess;
};
uniform Material material;

struct DistantLight {
    vec3 direction;
    vec3 color;

    float ambientCoefficient;
    float diffuseCoefficient;
    float specularCoefficient;
};
uniform DistantLight distantLight;

layout(location = 0) out vec4 FragColor;

in vec3 Normal;
in vec3 FragPos;
in vec2 TexCoords;

uniform vec3 viewPos;

void main()
{
    // ambient
    vec3 ambient = distantLight.ambientCoefficient * distantLight.color * texture(material.diffuseColorMap, TexCoords).rgb;

    // diffuse 
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(-distantLight.direction);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = distantLight.diffuseCoefficient * distantLight.color * diff * texture(material.diffuseColorMap, TexCoords).rgb;

    //specular
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = distantLight.specularCoefficient * distantLight.color * (spec * texture(material.specularColorMap, TexCoords).rgb);


    vec3 result = ambient + diffuse + specular;
    FragColor = vec4(result, 1.0);
};

