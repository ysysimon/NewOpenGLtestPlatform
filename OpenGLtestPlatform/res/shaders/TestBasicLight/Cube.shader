#shader vertex
#version 330 core
layout (location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 Normal;
out vec3 FragPos;

void main()
{
    gl_Position = projection * view * model * vec4(aPos, 1.0);
    FragPos = vec3(model * vec4(aPos, 1.0));
    Normal = aNormal;
};


#shader fragment
#version 330 core

struct Material {
    vec3 ambientColor;
    vec3 diffuseColor;
    vec3 specularColor;
    float shininess;
};
uniform Material material;

struct Light {
    vec3 position;
    vec3 color;

    float ambientCoefficient;
    float diffuseCoefficient;
    float specularCoefficient;
};
uniform Light light;

layout(location = 0) out vec4 FragColor;

in vec3 Normal;
in vec3 FragPos;


//uniform vec3 lightPos;
//uniform vec3 objectColor;
//uniform vec3 lightColor;
uniform vec3 viewPos;

void main()
{
    // ambient
    //float ambientStrength = 0.1;
    //vec3 ambient = ambientStrength * lightColor;
    vec3 ambient = light.ambientCoefficient * light.color * material.ambientColor;

    // diffuse 
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(light.position - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    //vec3 diffuse = diff * lightColor;
    vec3 diffuse = light.diffuseCoefficient * light.color * (diff * material.diffuseColor);

    //specular
    //float specularStrength = 0.5;
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    //float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    //vec3 specular = specularStrength * spec * lightColor;
    vec3 specular = light.specularCoefficient * light.color * (spec * material.specularColor);

    //vec3 result = (ambient + diffuse + specular) * objectColor;
    vec3 result = ambient + diffuse + specular;
    FragColor = vec4(result, 1.0);
};

