#version 330 core

in vec2 TexCoords;

out vec4 FragColour;

uniform sampler2D img;

void main() {
    FragColour = texture(img, TexCoords);
}
