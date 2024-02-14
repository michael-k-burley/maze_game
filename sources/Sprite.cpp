
#include "../headers/Sprite.hpp"

#include "stb_image.h"  //MUST BE HERE AND NOT HEADER

Sprite::Sprite(const char* filePath, float& cellSize, glm::mat4& projection, glm::vec2& startSquare, std::vector<std::vector<int>>& bitmap) 
: text(new Texture(filePath)), position(startSquare), cellsize(cellSize), map(bitmap)
{
    lastAnswered = glfwGetTime(); //Initialize last answered

    //Create shader for rendering sprite
    spriteShader = new Shader("resources/shaders/sprite.vs", "resources/shaders/sprite.fs"); //Should user pass in?

    //Set shader to currently active for openGL context
    spriteShader->use();

    //Set projection matrix for sprite shader
    spriteShader->setMat4("projection", projection);

    //Set default sprite on sprite sheet
    spriteShader->setVec2("spriteIndex", spriteIndex); 
    
    /* QUAD */          //Need to scale by aspect ratio (ie. width/height)???
    float vertices[] = {
         0.0f, 0.0f,      // top left  
         1.0f, 0.0f,      // top right 
         0.0f, 1.0f,      // bottom left  
         1.0f, 1.0f       // bottom right
    };   //position 

    //Read column wise only the position vertices 
    // int row = 4; int col = 4; 
    // for(int i{4}; i < 8; i++)  vertices[((i%row)*col) + (i/row)] *= ar; //could multiply y by ar || divide x by ar

    /* Indices for quad EBO/IBO */                                          
    unsigned int indices[] = {
       0, 1, 2,
       2, 3, 1
    }; 

    //Create openGL VAO & VBOs
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    //Position
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    //Index buffer object
    glGenBuffers(1, &IBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(unsigned int), indices, GL_STATIC_DRAW);

    /* TEXTURE */
    //text = new Texture(filePath); 
}

void Sprite::draw(){

    spriteShader->use();

    // It is advised to first scale, then rotate, and finally translate the object. Because multiplying matrices 
    // occurs from right to left, we transform the matrix in reverse order: translate, rotate, and then scale. 
    glm::mat4 model = glm::mat4(1.0f); 

    model = glm::translate(model, glm::vec3(this->position * cellsize, 0.0f));  
    model = glm::scale(model, glm::vec3(1.0f, 1.0f, 0.0f) * cellsize); 
    // Mulitplying by cellsize allows sprite to deal in bitmap space

    //Set shaders model matrix
    spriteShader->setMat4("model", model);

    //Set sprite on sprite sheet
    spriteShader->setVec2("spriteIndex", spriteIndex); //col, row
  
    //Set sprite texture active for sprite shader
    glActiveTexture(GL_TEXTURE0); 
    text->bind(0);

    //Set sprite VAO active and draw sprite
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
}

void Sprite::move(Sprite::Direction dir)
{
    //Early exit if not enough time has elapsed between calls (Prevents player moving too fast)
    if(glfwGetTime() - lastAnswered < waitTime)  return; //NOTE: getTime() returns time in seconds since glfw started

    if(dir == Sprite::Direction::UP)         { this->position.y -= 1; spriteIndex = glm::vec2(0.0f, 1.0f); } //UP
    else if(dir == Sprite::Direction::DOWN)  { this->position.y += 1; spriteIndex = glm::vec2(0.0f, 0.0f); } //DOWN
    else if(dir == Sprite::Direction::LEFT)  { this->position.x -= 1; spriteIndex = glm::vec2(0.0f, 2.0f); } //LEFT
    else if(dir == Sprite::Direction::RIGHT) { this->position.x += 1; spriteIndex = glm::vec2(1.0f, 3.0f); } //RIGHT 

    //Update last answered to last time player's position was updated
    lastAnswered = glfwGetTime(); // lastCalled;

}
