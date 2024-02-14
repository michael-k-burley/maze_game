
#include "../headers/TileMap.hpp"

//width & height of tilemap or screen? //Could get numRow & numCols from map.size()???
TileMap::TileMap(int cellSize, glm::mat4& projection, const std::vector<std::vector<int>>& map) 
{
    int numRows = map.size() -1;
    int numCols = map[0].size() -1;
    //GENERATE VERTICES
    for(int i{}; i <= numRows; i++)
    {
        for(int j{}; j <= numCols; j++)
        {
            //Set vertex position
            vec.push_back(j * cellSize);
            vec.push_back(i * cellSize);
        
            //Set vertex texture coordinates  (Not completely correct as)
            vec.push_back( (j % 2)? 1.0f : 0.0f );     // alterating rows get textures flipped vertically
            vec.push_back( (i % 2)? 1.0f : 0.0f );     // odd position textures on same row flipped horizontally
        }
    }
    
    //GENERATE INDICES
    //(numCols+1) * (numRows+1) - (numCols + 2);
int bound = (numCols+1) * numRows - 1; //why -1???

    for(int i{}; i < bound; i++) //Magik Const alert
    {
        if((i+1) % (numCols+1) == 0)  continue; //Skip right end
        
        indices.push_back(i);
        indices.push_back(i + 1);
        indices.push_back(i + (numCols + 1));
    
        indices.push_back((i+1));
        indices.push_back((i+1) + (numCols + 1));
        indices.push_back((i+1) + (numCols));
    }

    //OPEN GL
    tilemapShader = new Shader("resources/shaders/tile.vs", "resources/shaders/tile.fs");

    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO); 
    // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
    
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vec.size(), vec.data(), GL_STATIC_DRAW);
    
    //Position
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    //Texture coord
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2*sizeof(float)));
    glEnableVertexAttribArray(1);

    //Texture tile "type" (ie. walkway or wall ) in seperate buffer ... so can easily change map ie. overwrite buffer
    glGenBuffers(1, &VBO_bitmap);
    glBindBuffer(GL_ARRAY_BUFFER, VBO_bitmap); //Set current VBO to active, within context of current VAO

    std::vector<float> bitmap; //Must be float ya goofy ball

    //Set vertex texture tile identifier from bitmap
    for(int i{}; i <= numRows; i++)
        for(int j{}; j <= numCols; j++)
            bitmap.push_back( map[i][j] );

    //Copy bitmap data into bitmap buffer
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * bitmap.size(), bitmap.data(), GL_DYNAMIC_DRAW); 

    //layout location, # values, type of values, size of each vertex in bytes, pointer to offset
    glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, 1 * sizeof(float), (void*)0); 
    glEnableVertexAttribArray(2);

    //Index buffer object
    glGenBuffers(1, &IBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW); 

    //TEXTURE
    walkwayText = new Texture("resources/textures/floor/metal-grate.jpeg");
    wallText = new Texture("resources/textures/floor/red-brick.jpeg");

    tilemapShader->use(); // don't forget to activate the shader before setting uniforms!  

    //Set uniform sampler2D in tilemap fragment shader
    glUniform1i(glGetUniformLocation(tilemapShader->getID(), "texture0"), 0);   // set it manually
    tilemapShader->setInt("texture1", 1);                                       // or with shader class

    //Set tilemap shaders projection matrix
    tilemapShader->setMat4("projection", projection); 
}

void TileMap::draw()
{
    tilemapShader->use();
  
    //Set texture units to active for tilemap shader
    glActiveTexture(GL_TEXTURE0); 
    walkwayText->bind(0);
   
    glActiveTexture(GL_TEXTURE1); 
    wallText->bind(1); 

    //Bind tile map VAO and draw
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, nullptr);
   
}

void TileMap::updateBitmap(const std::vector<std::vector<int>>& map) {  
            
    std::vector<float> bitmap; //Must be float ya goofy ball

    //Set vertex texture tile identifier from bitmap
    for(int i{}; i < static_cast<int>(map.size()); i++)
        for(int j{}; j < static_cast<int>(map[0].size()); j++)
            bitmap.push_back( map[i][j] );

    //Bind relevant objects
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO_bitmap); 

    //Copy updated bitmap into buffer
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(float) * bitmap.size(), bitmap.data());
};
