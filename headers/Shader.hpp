
#ifndef SHADER_HPP
#define SHADER_HPP

#include <glad/glad.h>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#include <unistd.h>

#include <glm/glm.hpp> 
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Shader
{
public:
    Shader(const char* vertexPath, const char* fragmentPath);
    ~Shader();

    void use() const;
    void unuse() const;

    std::string readFileToString(const char* filePath);
    unsigned int createProgram(std::string vertexShader, std::string fragmentShader);
    unsigned int compileShader(unsigned int shaderType, std::string shaderCode);

    void setBool(const std::string &name, bool value) const;
    void setInt(const std::string &name, int value) const;
    void setFloat(const std::string &name, float value) const; 

    void setVec2(const std::string &name, glm::vec2 vec) const;
    void setVec3(const std::string &name, glm::vec3 vec) const;
    void setMat4(const std::string &name, glm::mat4 matrix) const; 

    void deleteProgram();

    inline unsigned int getID() { return m_RendererID; }

private:

    unsigned int m_RendererID;
    std::string vertexFilePath;
    std::string fragmentFilePath;
    
    // utility function for checking shader compilation/linking errors.
    void checkCompileErrors(unsigned int shader, std::string type);
};

#endif
