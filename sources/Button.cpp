
#include "Button.hpp"

#include "stb_image.h"  //MUST BE HERE AND NOT HEADER

Button::Button(glm::vec2 topLeftCorner, glm::vec2 sz, const char* filePath, glm::mat4& projection)
: position(topLeftCorner), size(sz)
{
    //Create shader for rendering sprite
    shader = new Shader("resources/shaders/quad.vs", "resources/shaders/quad.fs"); //Should user pass in?

    //Set shader to currently active for openGL context
    shader->use();

    //Set projection matrix for sprite shader
    shader->setMat4("projection", projection);
    
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
    text = new Texture(filePath);
}


void Button::draw(){

    shader->use();

    // It is advised to first scale, then rotate, and finally translate the object. Because multiplying matrices 
    // occurs from right to left, we transform the matrix in reverse order: translate, rotate, and then scale. 
    glm::mat4 model = glm::mat4(1.0f); 
    model = glm::translate(model, glm::vec3(this->position, 0.0f)); 

    /*
    Because we specified the quad's vertices with (0,0) as the top-left coordinate, all rotations will rotate around 
    this point of (0,0). The origin of rotation is at the top-left of the quad, which produces undesirable results. 
    What we want to do is move the origin of rotation to the center of the quad so the quad neatly rotates around this 
    origin, instead of rotating around the top-left of the quad. We solve this by translating the quad by half its size 
    first, so its center is at coordinate (0,0) before rotating. 
    */
   
    // model = glm::translate(model, glm::vec3(0.5f * size.x, 0.5f * size.y, 0.0f)); 
    // model = glm::rotate(model, glm::radians(this->rotate), glm::vec3(0.0f, 0.0f, 1.0f)); 
    // model = glm::translate(model, glm::vec3(-0.5f * size.x, -0.5f * size.y, 0.0f));
  
    model = glm::scale(model, glm::vec3(this->size, 0.0f)); //MAgik COnst alert

    //Set shaders model matrix
    shader->setMat4("model", model);

    //Set sprite texture active for sprite shader
    glActiveTexture(GL_TEXTURE0); 
    text->bind(0);

    //Set sprite VAO active and draw sprite
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
}


