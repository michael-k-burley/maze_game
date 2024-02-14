
#ifndef BUTTON_HPP
#define BUTTON_HPP

#include <iostream>
#include <vector>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Shader.hpp"
#include "Texture.hpp"

class Button {

    private:

        unsigned int VBO, VAO, IBO;
        Shader* shader;
        Texture* text;

        float rotate = 0.0;     //degrees ccw
        glm::vec2 position;     //Position in Screen Space  (Top Left Corner)
        glm::vec2 size;         //Size in screen space

    public:
        Button(glm::vec2 topLeftCorner, glm::vec2 sz, const char* filePath, glm::mat4& projection);
        
        void draw();
        bool isInside(const double x, const double y) { return (x > position.x && x < position.x + size.x) 
                                                            && (y > position.y && y < position.y + size.y); };
};

#endif
