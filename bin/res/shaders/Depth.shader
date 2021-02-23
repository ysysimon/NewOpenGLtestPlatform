#shader vertex
#version 330 core

layout(location = 0) in vec3 position;

uniform mat4 SpaceMatrix;
uniform mat4 model;

void main()
{
    gl_Position = SpaceMatrix * model * vec4(position, 1.0f);
}

#shader fragment
#version 330 core

void main()
{
    // gl_FragDepth = gl_FragCoord.z;
}