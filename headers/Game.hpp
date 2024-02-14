
#ifndef GAME_HPP
#define GAME_HPP

#include "Shader.hpp"
#include "Sprite.hpp"
#include "TileMap.hpp"
#include "MazeMaker.hpp"
#include "Button.hpp" 

#include <iostream>
#include <vector>  

#include <glad/glad.h>
#include <GLFW/glfw3.h>

class Game{

    private:
        GLFWwindow* window; //Ptr to GLFW window instance

        class Window
        {
            private:

            public:
                glm::vec4 clearColour {1.0f, 1.0f, 1.0f, 1.0f}; //{0.5f, 0.6f, 0.8f, 1.0f};
                glm::vec2 windowDim, gameWindowDim, menuBarDim;
        };

        Window wWindow; //Wrapper Window Instance 

        enum class GameState { MAINMENU, ACTIVE, FINISHED, EXIT };
        GameState state;

        TileMap* tilemap;
        Sprite* player;
        glm::vec2 startSquare;
        
        float cellsize; //This is the size of each square cell on the screen
        std::vector<std::vector<int>> bitmap; //Bitmap of maze // Zero for walkway, One for wall
        std::vector<std::vector<int>> generateMaze();

        bool isValidMove(const Sprite* sp, const Sprite::Direction& dir);

        Button *menuBarBanner, *startButton, *newMapButton, *quitButton;
        Button *youWinScreen; // Technically is just a none clickable button
        double lastAnswered;            //This keeps track of the time between calls to update mouse clicks location
        const double waitTime = 0.5f;   //Time in seconds between subsequent calls to update a players postion

    public:
        // Game();
        // ~Game();

        bool init(const char* Title, unsigned int width, unsigned int height, bool fullScreen);
        void handleEvents();
        void update();
        void render();
        void clean();

        bool isRunning() { return (state != GameState::EXIT); }
        void handleMouseClicks();
}; 


#endif
