#shader vertex
#version 330 core
layout(location = 0) in vec3 position;
layout(location = 2) in vec2 texCoords;

out vec2 TexCoords;

void main()
{
    gl_Position = vec4(position, 1.0f);
    TexCoords = texCoords;
}


#shader fragment
#version 330 core
out vec4 FragColor;
in vec2 TexCoords;

uniform sampler2D hdrBuffer;
uniform sampler2D bloomBlur;
uniform float exposure;
uniform bool enableBloom;
uniform bool IDT;
uniform bool RRT;
uniform bool ODT;
uniform bool EOTF;

vec3 ACESToneMapping(vec3 color, float adapted_lum);
vec3 RRTAndODTFit(vec3 v);

// sRGB => XYZ => D65_2_D60 => AP1 => RRT_SAT
const mat3 ACESInputMat = mat3
(
    vec3( 0.59719, 0.07600, 0.02840 ),
    vec3( 0.35458, 0.90834, 0.13383 ),
    vec3( 0.04823, 0.01566, 0.83777 )

    //vec3( 0.59719, 0.35458, 0.04823 ),
    //vec3( 0.07600, 0.90834, 0.01566 ),
    //vec3( 0.02840, 0.13383, 0.83777 )
);

vec3 RRTAndODTFit(vec3 v)
{
    vec3 a = v * (v + 0.0245786f) - 0.000090537f;
    vec3 b = v * (0.983729f * v + 0.4329510f) + 0.238081f;
    return a / b;
}


// ODT_SAT => XYZ => D60_2_D65 => sRGB
const mat3 ACESOutputMat = mat3
(
    vec3(  1.60475, -0.10208, -0.00327),
    vec3( -0.53108,  1.10813, -0.07276),
    vec3( -0.07367, -0.00605,  1.07602)

    //vec3(  1.60475, -0.53108, -0.07367),
    //vec3( -0.10208,  1.10813, -0.00605),
    //vec3( -0.00327, -0.07276,  1.07602)
);

void main()
{
    const float gamma = 2.2;
    vec3 hdrColor = texture(hdrBuffer, TexCoords).rgb;
    if (enableBloom)
    {
        vec3 bloomColor = texture(bloomBlur, TexCoords).rgb;
        hdrColor += bloomColor; // additive blending
    }

    // ACES 
    vec3 result = hdrColor;

    if (IDT)
    {
        result = ACESInputMat * result;
    }

    if (RRT)
    {
        //result = ACESToneMapping(hdrColor, exposure);
        result = RRTAndODTFit(result);
    }

    if (ODT)
    {
        result = ACESOutputMat * result;
    }
        
    // exposure
    //vec3 result = vec3(1.0) - exp(-hdrColor * exposure);

    if (EOTF)
    {
        // gamma correct
        result = pow(result, vec3(1.0 / gamma));
    }

    FragColor = vec4(result, 1.0f);
}


vec3 ACESToneMapping(vec3 color, float adapted_lum)
{
    const float A = 2.51f;
    const float B = 0.03f;
    const float C = 2.43f;
    const float D = 0.59f;
    const float E = 0.14f;

    color *= adapted_lum;
    return (color * (A * color + B)) / (color * (C * color + D) + E);
}
