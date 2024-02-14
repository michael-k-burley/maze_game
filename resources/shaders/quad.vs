#version 330 core

layout (location = 0) in vec2 aPos;

out vec2 TexCoords;

uniform mat4 model;
uniform mat4 projection;

void main() //The output of the vertex shader requires the coordinates to be in clip-space
{
   TexCoords = aPos; // Texture coordinates are same as position coordinates
   gl_Position = projection * model * vec4(aPos, 0.0f, 1.0f); //proj * view * model
};