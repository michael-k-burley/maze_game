
#include "../headers/Game.hpp" 

//Further Ideas:: Exit screen, display time, allow for retrys, etc.

bool Game::init(const char* Title, unsigned int width, unsigned int height, bool fullScreen){

    //*** Set game screen attributes **//  
    wWindow.windowDim.x = width; 
    wWindow.windowDim.y = height;

    wWindow.menuBarDim.x = width;
    wWindow.menuBarDim.y = 100;     //Maybe don't hard code this

    //Make fixed size or relative ?
    wWindow.gameWindowDim.x = wWindow.windowDim.x; 
    wWindow.gameWindowDim.y = wWindow.windowDim.y - wWindow.menuBarDim.y; 


    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    #ifdef __APPLE__
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    #endif

    // glfw window creation
    // --------------------
    window = glfwCreateWindow(wWindow.windowDim.x, wWindow.windowDim.y, "Maze Game", NULL, NULL); //Added height of menu bar
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return false;
    }

    //Generate the current context for GLFW window
    glfwMakeContextCurrent(window);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return false;
    }

    //Necessary in order for background of RGBA encoded .png image to be transparent (instead of black)
    // ie. needed to render semi-transparent images
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    //Initalize game's member variables
    state = GameState::MAINMENU;
    cellsize = 40.0f; //This is the size of each cell on screen Ex. 800 x 600 screen would have 480,000 cells = 800 x 600      

    //Generate maze (0 for walkway, 1 for wall)
    bitmap = MazeMaker::makeMaze(wWindow.gameWindowDim.y /cellsize, wWindow.gameWindowDim.x /cellsize);

    //Create projection matrix from world space to clip space // (left, right, bottom, top)
    glm::mat4 gameProjection = glm::ortho(0.0f, static_cast<float>(wWindow.gameWindowDim.x), static_cast<float>(wWindow.gameWindowDim.y), 0.0f, -1.0f, 1.0f); 

    //Create new player sprite
    startSquare = glm::vec2(0, floor(wWindow.gameWindowDim.y/(cellsize*2))); //<<< Floor if even & odd?
    player = new Sprite("resources/textures/sprites/retro-sprite-guy.png", cellsize, gameProjection, startSquare, bitmap);
    //Aside png have transparent backgrounds (Use website remove.bg to convert jpeg to png)

    //Initialize tile map (NOTE: cellsize must evenly divide both screen width & height for tilemap to render properly)
    tilemap = new TileMap(cellsize, gameProjection, bitmap);

    //Menu bar Init                         // left, right, bottom, top
    glm::mat4 menuProjection = glm::ortho(0.0f, static_cast<float>(wWindow.windowDim.x), static_cast<float>(wWindow.menuBarDim.y), 0.0f, -1.0f, 1.0f); 

    glm::vec2 buttonSize = {100.0f, 50.0f};  //Hard coded <<<
    glm::vec2 TLC = {(wWindow.menuBarDim.x - buttonSize.x)/2, (wWindow.menuBarDim.y - buttonSize.y)/2}; //Top Left Corner of Dead Center
    float offset = 400.0f; //Hard coded madgik constant alertS
    
    //Initalize quads / buttons for header menu bar
    menuBarBanner = new Button({0,0}, wWindow.menuBarDim, "resources/textures/gameBanner.png", menuProjection);
    startButton  = new Button({TLC.x - offset, TLC.y}, buttonSize, "resources/textures/buttons/startGame.png", menuProjection);
    newMapButton = new Button(TLC, buttonSize, "resources/textures/buttons/nextMap.png", menuProjection);
    quitButton   = new Button({TLC.x + offset, TLC.y}, buttonSize, "resources/textures/buttons/exitGame.png", menuProjection);

    glm::vec2 winScreenSize = {500.0f, 350.0f};  //Hard coded <<<
    glm::vec2 gameScreenCenter = {(wWindow.gameWindowDim.x - winScreenSize.x)/2, (wWindow.gameWindowDim.y - winScreenSize.y)/2};
    youWinScreen   = new Button(gameScreenCenter, winScreenSize, "resources/textures/youWin.png", gameProjection);

    return true; 
}

void Game::handleEvents(){

    glfwPollEvents();

    if(glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS && glfwGetTime() - lastAnswered >= waitTime)  handleMouseClicks();

    //If user presses escape key then close the window (Currently being over written in update by Finished)
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS || glfwWindowShouldClose(window))  state = GameState::EXIT; 

    // Don't respond to input unless game is started
    if(state != GameState::ACTIVE)  return;

    //Process user input to update player position
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS    && isValidMove(player, Sprite::Direction::UP))     player->move(Sprite::Direction::UP);
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS  && isValidMove(player, Sprite::Direction::DOWN))   player->move(Sprite::Direction::DOWN);
    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS  && isValidMove(player, Sprite::Direction::LEFT))   player->move(Sprite::Direction::LEFT);
    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS && isValidMove(player, Sprite::Direction::RIGHT))  player->move(Sprite::Direction::RIGHT);
    
}

void Game::handleMouseClicks()
{
    //Get current cursor position
    double xpos, ypos;
    glfwGetCursorPos(window, &xpos, &ypos); 

    //Check if cursor position is inside of any buttons
    if(startButton->isInside(xpos, ypos)){
        state = GameState::ACTIVE;

    }else if(newMapButton->isInside(xpos, ypos)){
        
        //Create new bitmap and update tilemap
        bitmap = MazeMaker::makeMaze(wWindow.gameWindowDim.y /cellsize, wWindow.gameWindowDim.x /cellsize);
        tilemap->updateBitmap(bitmap);
        
        //Reset player to correct square and 
        player->setPosition(startSquare);
        player->setSpriteLookRight(); //Hacky but resets sprite to look right

        //Reset state so user must press start button
        state = GameState::MAINMENU;

    }else if(quitButton->isInside(xpos, ypos)){
        state = GameState::EXIT;
    }

    //Update last answered to last time player's position was updated
    lastAnswered = glfwGetTime(); // lastCalled;
}

bool Game::isValidMove(const Sprite* sp, const Sprite::Direction& dir){

    bool isValid = false;

    //1+ since need to offset for top row of bitmap being all zeros
    if(dir == Sprite::Direction::UP)           
        isValid = (bitmap[1+ sp->ypos() - 1][sp->xpos()]) != 1; //1 is wall 
    if(dir == Sprite::Direction::DOWN)  
        isValid = (bitmap[1+ sp->ypos() + 1][sp->xpos()]) != 1; 
    if(dir == Sprite::Direction::LEFT)          
        isValid = (bitmap[1+ sp->ypos()][sp->xpos() - 1]) != 1; 
    if(dir == Sprite::Direction::RIGHT) 
        isValid = (bitmap[1+ sp->ypos()][sp->xpos() + 1]) != 1;  

    return isValid;
}

void Game::update(){  //Idea: Correct possbly invalid game states???

    if(state != GameState::ACTIVE)  return;

    //Player went left on start // Reset player position to starting position
    if(player->xpos() < 0) player->move(Sprite::Direction::RIGHT); 

    //Player has reached final square //-2 since map has walls on either side
    if(player->xpos() == static_cast<int>(bitmap[0].size()) -2) state = GameState::FINISHED;

}

void Game::render(){ //glScissor() <<< can split screen

    //Anything you draw into a sub-window would be relative to that window's origin and not the entire window. 
    //So for each sub-window, you have your own coordinate system. 
    glClearColor(wWindow.clearColour.r, wWindow.clearColour.g, wWindow.clearColour.b, wWindow.clearColour.a);    
    glClear(GL_COLOR_BUFFER_BIT);

    //Split top of screen for menu bar (Maybe put in own function)
    glViewport(0, wWindow.gameWindowDim.y, wWindow.windowDim.x, wWindow.menuBarDim.y); // lower left corner (x,y), width, height
    menuBarBanner->draw();
    startButton->draw();
    newMapButton->draw();
    quitButton->draw();

    //Split bottom of screen for maze
    glViewport(0, 0, wWindow.windowDim.x, wWindow.gameWindowDim.y);  
    tilemap->draw(); 
    player->draw();

    if(state == GameState::FINISHED)
    {
        //glViewport(0, 0, wWindow.windowDim.x, wWindow.gameWindowDim.y);  
        youWinScreen->draw();
    }

    //Swap frame buffers
    glfwSwapBuffers(window);
}

void Game::clean(){

    //Delete all heap allocated entities (necessary?)
    //tileMap->~TileMap();
    //sp->~Sprite();

    glfwTerminate(); // glfw: terminate, clearing all previously allocated GLFW resources.
}
