
#ifndef SPRITE_HPP
#define SPRITE_HPP

#include <iostream>
#include <vector>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Shader.hpp"
#include "Texture.hpp"

//#include "Game.hpp" //Circular Dependency

class Sprite {

    private: 
        unsigned int VBO, VAO, IBO;
        Shader* spriteShader;
        Texture* text;
        glm::vec2 spriteIndex {1.0f, 3.0f}; // col, row of sprite sheet (maybe enum class)

        float rotate = 0.0;     //degrees ccw

        glm::vec2 position;     //Position in Game Space (ie. relative to bitmap)
        float &cellsize;        // Reference to definition in Game.cpp init()
        std::vector<std::vector<int>>& map;

        double lastAnswered;            //This keeps track of the time between calls to update player's position
        const double waitTime = 0.2f;   //Time in seconds between subsequent calls to update a players postion

    public:
        Sprite(const char* filePath, float& cellSize, 
                glm::mat4& projection, glm::vec2& startSquare, std::vector<std::vector<int>>& bitmap);
        ~Sprite();

        void draw();
        
        int xpos() const { return static_cast<int>(this->position.x); };
        int ypos() const { return static_cast<int>(this->position.y); };

        enum class Direction{ UP, DOWN, LEFT, RIGHT };
        void move(Sprite::Direction dir);

        void setPosition(const glm::vec2 pos) { position = pos; }
        void setSpriteLookRight() { spriteIndex = {1.0f, 3.0f}; }
};

#endif
