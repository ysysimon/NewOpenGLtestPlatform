#shader vertex
#version 330 core
layout (location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTexCoords;
layout(location = 3) in vec3 aTtangent;

out VS_OUT {
    vec3 FragPos;
    vec3 Normal;
    vec2 TexCoords;
    vec4 FragPosLightSpace;
    mat3 TBN;
} vs_out;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat4 lightSpaceMatrix;

void main()
{
    gl_Position = projection * view * model * vec4(aPos, 1.0);
    vs_out.FragPos = vec3(model * vec4(aPos, 1.0));
    vs_out.Normal = transpose(inverse(mat3(model))) * aNormal;
    vs_out.TexCoords = aTexCoords;
    vs_out.FragPosLightSpace =  lightSpaceMatrix * vec4(vs_out.FragPos, 1.0);
    vec3 T = normalize(vec3(model * vec4(aTtangent, 0.0)));
    //vec3 B = normalize(vec3(model * vec4(bitangent, 0.0)));
    vec3 N = normalize(vec3(model * vec4(aNormal, 0.0)));
    // re-orthogonalize T with respect to N
    T = normalize(T - dot(T, N) * N);
    // then retrieve perpendicular vector B with the cross product of T and N

    vec3 B = cross(N, T);
    vs_out.TBN = mat3(T, B, N);
}


#shader fragment
#version 330 core

layout(location = 0) out vec4 FragColor;
layout(location = 1) out vec4 BrightColor;

struct Material {
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
    float intensity;
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

in VS_OUT {
    vec3 FragPos;
    vec3 Normal;
    vec2 TexCoords;
    vec4 FragPosLightSpace;
    mat3 TBN;
} fs_in;

uniform vec3 viewPos;
uniform bool EnablePointLight;
uniform bool EnableDistantLight;
uniform bool EnableSpotLight;
uniform bool transformColorSpace;

uniform sampler2D Diffuse1;
uniform sampler2D Diffuse2;
uniform sampler2D Diffuse3;
uniform sampler2D Specular1;
uniform sampler2D Specular2;

uniform sampler2D shadowMap;
uniform sampler2D Normal;

vec3 CalculateDistantLight(DistantLight distantLight,vec3 normal,vec3 viewDir, vec3 diffuseColor,vec3 specularTexture);
vec3 CalculatePointLight(PointLight pointLight, vec3 normal, vec3 viewDir, vec3 diffuseColor, vec3 specularTexture);
vec3 CalculateSpotLight(SpotLight spotLight, vec3 normal, vec3 viewDir, vec3 diffuseColor, vec3 specularTexture);
float ShadowCalculation(vec4 fragPosLightSpace, vec3 normal, vec3 lightDir);

// sRGB => XYZ => D65_2_D60 => AP1 => RRT_SAT
const mat3 ACESInputMat = mat3
(
    vec3(0.59719, 0.07600, 0.02840),
    vec3(0.35458, 0.90834, 0.13383),
    vec3(0.04823, 0.01566, 0.83777)

    //vec3( 0.59719, 0.35458, 0.04823 ),
    //vec3( 0.07600, 0.90834, 0.01566 ),
    //vec3( 0.02840, 0.13383, 0.83777 )
);

// ODT_SAT => XYZ => D60_2_D65 => sRGB
const mat3 ACESOutputMat = mat3
(
    vec3(1.60475, -0.10208, -0.00327),
    vec3(-0.53108, 1.10813, -0.07276),
    vec3(-0.07367, -0.00605, 1.07602)

    //vec3(  1.60475, -0.53108, -0.07367),
    //vec3( -0.10208,  1.10813, -0.00605),
    //vec3( -0.00327, -0.07276,  1.07602)
);

vec3 RRTAndODTFit(vec3 v)
{
    vec3 a = v * (v + 0.0245786f) - 0.000090537f;
    vec3 b = v * (0.983729f * v + 0.4329510f) + 0.238081f;
    return a / b;
}


void main()
{
    vec3 result = vec3(0);
    vec3 diffuseColor = texture(Diffuse1, fs_in.TexCoords).rgb;
    if (transformColorSpace)
    {
        //diffuseColor = ACESInputMat * diffuseColor;
        diffuseColor = ACESInputMat * diffuseColor;
    }
    vec3 specularTexture = texture(Specular1, fs_in.TexCoords).rgb;
    //vec3 normal = normalize(fs_in.Normal);
    vec3 normal = texture(Normal, fs_in.TexCoords).rgb;
    normal = normalize(normal * 2.0 - 1.0);
    normal = normalize(fs_in.TBN * normal);
    vec3 viewDir = normalize(viewPos - fs_in.FragPos);

    if (EnableDistantLight)
    {
        result += CalculateDistantLight(distantLight, normal, viewDir, diffuseColor, specularTexture);
    }

    if (EnablePointLight)
    {
        result += CalculatePointLight(pointLight, normal, viewDir, diffuseColor, specularTexture);
    }

    if (EnableSpotLight)
    {
        result += CalculateSpotLight(spotLight, normal, viewDir, diffuseColor, specularTexture);
    }
    
    FragColor = vec4(result, 1.0);

    float brightness = dot(FragColor.rgb, vec3(0.2126, 0.7152, 0.0722));
    if (brightness > 1.0)
    {
        BrightColor = vec4(FragColor.rgb, 1.0);
    }
    else
    {
        BrightColor = vec4(0.0, 0.0, 0.0, 1.0);
    }
};

vec3 CalculateDistantLight(DistantLight distantLight, vec3 normal, vec3 viewDir, vec3 diffuseColor, vec3 specularTexture)
{
    // ambient
    vec3 ambient = distantLight.ambientCoefficient * distantLight.color * diffuseColor;

    // diffuse 
    //norm = normalize(Normal);
    vec3 lightDir = normalize(-distantLight.direction);
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = distantLight.diffuseCoefficient * distantLight.color * diff * diffuseColor;

    //specular
    //viewDir = normalize(viewPos - FragPos);
    vec3 halfwayDir = normalize(lightDir + viewDir);
    //vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(normal, halfwayDir), 0.0), material.shininess);
    vec3 specular = distantLight.specularCoefficient * distantLight.color * (spec * specularTexture);
    //shadow
    float shadow = ShadowCalculation(fs_in.FragPosLightSpace, normal, lightDir);

    return ambient + (1.0 - shadow) * (diffuse + specular);
}

vec3 CalculatePointLight(PointLight pointLight, vec3 normal, vec3 viewDir, vec3 diffuseColor, vec3 specularTexture)
{
    float distance = length(pointLight.position - fs_in.FragPos);
    //float attenuation = 1.0 / (pointLight.K + pointLight.linear * distance +
    //    pointLight.quadratic * (distance * distance));
    //Physical attenuation
    float attenuation = 1.0 / (distance * distance);

    // ambient
    vec3 ambient = pointLight.ambientCoefficient * pointLight.color * diffuseColor;

    // diffuse 
    //norm = normalize(Normal);
    vec3 lightDir = normalize(pointLight.position - fs_in.FragPos);
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = pointLight.diffuseCoefficient * pointLight.color * diff * diffuseColor;

    //specular
    //viewDir = normalize(viewPos - FragPos);
    //vec3 reflectDir = reflect(-lightDir, normal);
    vec3 halfwayDir = normalize(lightDir + viewDir);
    float spec = pow(max(dot(normal, halfwayDir), 0.0), material.shininess);
    vec3  specular = pointLight.specularCoefficient * pointLight.color * (spec * specularTexture);

    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;

    return pointLight.intensity*(ambient + diffuse + specular);
}

vec3 CalculateSpotLight(SpotLight spotLight, vec3 normal, vec3 viewDir, vec3 diffuseColor, vec3 specularTexture)
{
    // spotlight (soft edges)
    vec3 lightDir = normalize(spotLight.position - fs_in.FragPos);
    float theta = dot(lightDir, normalize(-spotLight.spotDirection));
    float epsilon = (spotLight.cutoffAngleCos - spotLight.outerCutOffAngleCos);
    float intensity = clamp((theta - spotLight.outerCutOffAngleCos) / epsilon, 0.0, 1.0);


    // ambient
    vec3 ambient = spotLight.ambientCoefficient * spotLight.color * diffuseColor;

    // diffuse 
    //norm = normalize(Normal);
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = spotLight.diffuseCoefficient * spotLight.color * diff * diffuseColor;

    //specular
    //viewDir = normalize(viewPos - FragPos);
    //vec3 reflectDir = reflect(-lightDir, normal);
    vec3 halfwayDir = normalize(lightDir + viewDir);
    float spec = pow(max(dot(normal, halfwayDir), 0.0), material.shininess);
    vec3 specular = spotLight.specularCoefficient * spotLight.color * (spec * specularTexture);

    ambient *= intensity;
    diffuse *= intensity;
    specular *= intensity;

    return ambient + diffuse + specular;
}
    
float ShadowCalculation(vec4 fragPosLightSpace, vec3 normal, vec3 lightDir)
{
    // 执行透视除法
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    // 变换到[0,1]的范围
    projCoords = projCoords * 0.5 + 0.5;
    // 取得最近点的深度(使用[0,1]范围下的fragPosLight当坐标)
    float closestDepth = texture(shadowMap, projCoords.xy).r; 
    // 取得当前片段在光源视角下的深度
    float currentDepth = projCoords.z;
    // 计算bias
    float bias = max(0.05 * (1.0 - dot(normal, lightDir)), 0.005);
    // 检查当前片段是否在阴影中
    /*float shadow = currentDepth - bias > closestDepth ? 1.0 : 0.0;*/
    //PCF
    float shadow = 0.0;
    vec2 texelSize = 1.0 / textureSize(shadowMap, 0);
    for (int x = -1; x <= 1; ++x)
    {
        for (int y = -1; y <= 1; ++y)
        {
            float pcfDepth = texture(shadowMap, projCoords.xy + vec2(x, y) * texelSize).r;
            shadow += currentDepth - bias > pcfDepth ? 1.0 : 0.0;
        }
    }
    shadow /= 9.0;


    // Keep the shadow at 0.0 when outside the far_plane region of the light's frustum.
    if (projCoords.z > 1.0)
        shadow = 0.0;

    return shadow;
}
    

