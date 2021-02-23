#shader vertex
#version 330 core
layout (location = 0) in vec3 position;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    gl_Position = projection * view * model * vec4(position, 1.0);
    
};


#shader fragment
#version 330 core

layout(location = 0) out vec4 FragColor;


uniform vec3 lightColor;

void main()
{
    FragColor = vec4(lightColor, 1.0);
};

