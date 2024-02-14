
#include "../headers/Game.hpp"

//NOTE: int index = ((i%row)*col) + (i/row); //how to iterate over columns with single for loop
     
#define STB_IMAGE_IMPLEMENTATION //MUST BE HERE then include
#include "stb_image.h"

#include <iostream>

// Defaults             
unsigned int SCR_WIDTH = 1000;  //want odd number of points so that even number of tiles
unsigned int SCR_HEIGHT = 680 +100;  //width must have even jumps, height doesn't matter?
bool FullScreen = false;

#include <glm/glm.hpp>
#include <glm/gtx/string_cast.hpp> //for glm::to_string(mat) to print matrix

#include <vector>

int main()
{ 
    Game* game = new Game();
    bool success = game->init("Game Title", SCR_WIDTH, SCR_HEIGHT, FullScreen);

    if(!success){
        std:: cout << "ERROR: with init" << std::endl;
        return -1;
    }

    const int FPS = 60;                  //Frames Per Second
    const float frameDelay = 1000 / FPS; //Milliseconds per frame (?)
    double frameStart, frameEnd;         //Delta time

    // render loop
    while(game->isRunning())
    {
        frameStart = glfwGetTime();

        game->handleEvents();
        game->update();
        game->render();

        frameEnd = glfwGetTime() - frameStart; 

        if(frameEnd < frameDelay)  sleep((frameDelay - frameEnd) / 1000); //Sleep for millisecond difference
       
    }
    
    game->clean();
    delete game;

    return 0;
}

