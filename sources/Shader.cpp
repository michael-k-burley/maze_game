
#include "../headers/Shader.hpp"

// constructor generates the shader on the fly
Shader::Shader(const char* vertexPath, const char* fragmentPath)
        : m_RendererID{0}, vertexFilePath{vertexPath}, fragmentFilePath{fragmentPath}
{
    //Read file to string
    std::string vertexCode = readFileToString(vertexPath);
    std::string fragmentCode = readFileToString(fragmentPath);

    // Create shader program
    m_RendererID = createProgram(vertexCode, fragmentCode);
}

std::string Shader::readFileToString(const char* filePath){

    std::string fileStr;
    std::ifstream shaderFile;

    // ensure ifstream objects can throw exceptions:
    shaderFile.exceptions (std::ifstream::failbit | std::ifstream::badbit);

    try {   
        // open file
        shaderFile.open(filePath);

        std::stringstream shaderStream;
        // read file's buffer contents into streams
        shaderStream << shaderFile.rdbuf();

        // close file handler
        shaderFile.close();

        // convert stream into string
        fileStr = shaderStream.str();

    }catch (std::ifstream::failure& e){
        std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ: " << e.what() << std::endl;
    }

    return fileStr;
}

unsigned int Shader::compileShader(unsigned int shaderType, std::string shaderCode){

    unsigned int compiledShader = glCreateShader(shaderType);
    const char* constShaderCode = shaderCode.c_str();

    glShaderSource(compiledShader, 1, &constShaderCode, NULL);
    glCompileShader(compiledShader);

    if(shaderType == GL_VERTEX_SHADER){//IS this right??? //Does this matter???
        checkCompileErrors(compiledShader, "VERTEX"); 

    }else if(shaderType == GL_FRAGMENT_SHADER){
        checkCompileErrors(compiledShader, "FRAGMENT");
    }

    return compiledShader;
}

unsigned int Shader::createProgram(std::string vertexCode, std::string fragmentCode){

    unsigned int vertexShader = compileShader(GL_VERTEX_SHADER, vertexCode);
    unsigned int fragmentShader = compileShader(GL_FRAGMENT_SHADER, fragmentCode);
    
    unsigned int programID = glCreateProgram();

    glAttachShader(programID, vertexShader);
    glAttachShader(programID, fragmentShader);
    glLinkProgram(programID);

    // delete the shaders as they're linked into our program now and no longer necessary
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    checkCompileErrors(programID, "PROGRAM");

    // int success;    //Is zero if unsuccessful
    // glGetShaderiv(programID, GL_LINK_STATUS, &success);

    return programID;
}

Shader::~Shader() { glDeleteProgram(m_RendererID); } 

// activate the shader
void Shader::use() const { glUseProgram(m_RendererID); }

// deactivate the shader
void Shader::unuse() const { glUseProgram(0); }

// ------------------------------------------------------------------------
// Set Uniform funcs
// ------------------------------------------------------------------------
void Shader::setBool(const std::string &name, bool value) const
{         
    glUniform1i(glGetUniformLocation(m_RendererID, name.c_str()), (int)value); 
}

void Shader::setInt(const std::string &name, int value) const
{ 
    glUniform1i(glGetUniformLocation(m_RendererID, name.c_str()), value); 
}

void Shader::setFloat(const std::string &name, float value) const
{ 
    glUniform1f(glGetUniformLocation(m_RendererID, name.c_str()), value); 
}

void Shader::setVec2(const std::string &name, glm::vec2 vec) const
{
    glUniform2fv(glGetUniformLocation(m_RendererID, name.c_str()), 1, glm::value_ptr(vec));
}

void Shader::setVec3(const std::string &name, glm::vec3 vec) const
{
    glUniform3fv(glGetUniformLocation(m_RendererID, name.c_str()), 1, glm::value_ptr(vec));
}

void Shader::setMat4(const std::string &name, glm::mat4 matrix) const
{
    glUniformMatrix4fv(glGetUniformLocation(m_RendererID, name.c_str()), 1, GL_FALSE, glm::value_ptr(matrix));
}

// utility function for checking shader compilation/linking errors.
// ------------------------------------------------------------------------
void Shader::checkCompileErrors(unsigned int shader, std::string type)
{
    int success;
    char infoLog[1024];
    if (type != "PROGRAM")
    {
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(shader, 1024, NULL, infoLog);
            std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
        }
    }
    else
    {
        glGetProgramiv(shader, GL_LINK_STATUS, &success);
        if (!success)
        {
            glGetProgramInfoLog(shader, 1024, NULL, infoLog);
            std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
        }
    }
}




