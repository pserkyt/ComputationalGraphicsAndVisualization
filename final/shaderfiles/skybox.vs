#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec2 texCoord;

out vec2 UV;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    vec4 pos =  projection * view * model * vec4(position, 1.0);
    // Output position of the vertex, in clip space : MVP * position
    gl_Position =  pos.xyww;
    
    // We swap the y-axis by substracing our coordinates from 1. This is done because most images have the top y-axis inversed with OpenGL's top y-axis.
    UV = vec2(texCoord.x, 1.0 - texCoord.y);
    //UV = texCoord;
    //texCoord0 = position;
}

