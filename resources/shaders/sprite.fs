#version 330 core

in vec2 TexCoords;

out vec4 FragColour;

uniform sampler2D spriteSheet;
uniform vec2 spriteIndex;  // row, col of sprite on sprite sheet

const float SPRITE_COLS = 1 / 4.0; // Inverse of number cols
const float SPRITE_ROWS = 1 / 4.0; // Inverse of number rows

void main() {
    FragColour = texture(spriteSheet, vec2((TexCoords.x + spriteIndex.x) * SPRITE_COLS, (TexCoords.y + spriteIndex.y) * SPRITE_ROWS));
   //  FragColour = texture(spriteSheet, vec2( (TexCoords.x / SPRITE_COLS) + pos.x * (1.0 / SPRITE_COLS),
   //                                          (TexCoords.y / SPRITE_ROWS) + pos.y * (1.0 / SPRITE_ROWS)));
}