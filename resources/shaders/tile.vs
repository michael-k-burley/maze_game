#version 330 core

layout (location = 0) in vec2 aPos;
layout (location = 1) in vec2 aTextCoor;
layout (location = 2) in float aType;

out vec2 textCoor;
flat out float type;

uniform mat4 projection;

void main() //The output of the vertex shader requires the coordinates to be in clip-space
{
   textCoor = aTextCoor;
   type = aType;
   gl_Position = projection * vec4(aPos, 0.0f, 1.0f); //proj * view * model //not sure models necessary
};