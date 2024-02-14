
#ifndef TILEMAP_HPP
#define TILEMAP_HPP

#include "Shader.hpp"
#include "Texture.hpp"

#include <vector>
#include <iostream> //TESTING

class TileMap {

    public:
        TileMap(int cellSize, glm::mat4& projection, const std::vector<std::vector<int>>& map); //Should take in bitmap maybe?
        ~TileMap();

        void draw();
        void updateBitmap(const std::vector<std::vector<int>>& map);

    private:
        unsigned int VAO, VBO, IBO, VBO_bitmap;
        Shader* tilemapShader;
        Texture *walkwayText, *wallText; //texture: 0,1

        std::vector<float> vec{};
        std::vector<unsigned int> indices{}; 
};

#endif
