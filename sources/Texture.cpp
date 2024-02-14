
#include "../headers/Texture.hpp"
 
#include "stb_image.h" //MUST BE HERE AND NOT HEADER

Texture::Texture(const std::string& path) 
        : m_RendererID(0), m_FilePath{path}, m_LocalBuffer{nullptr}, m_Width(0), m_Height(0), m_BPP(0)
{
    glGenTextures(1, &m_RendererID);
    glBindTexture(GL_TEXTURE_2D, m_RendererID);

    //Without this line textures may load upside down
    //stbi_set_flip_vertically_on_load(true);

    // //Get width, height, Bits Per Pixel and number of desired colour channels from texture image
    m_LocalBuffer = stbi_load(path.c_str(), &m_Width, &m_Height, &m_BPP, 4); 

    if(m_LocalBuffer){ //NOTE: png - RGBA, jpeg - RGB

        //Attach texture image (ie. localbuffer) to currently bound texture object (on GPU)
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_Width, m_Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_LocalBuffer);
//How openGL stores data - Internal format ^^ was GL_RGB8            // ^^ Format - of the data we are giving to openGL

        // set the texture wrapping/filtering options (on the currently bound texture object)
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        //Generate mipmaps for currently bound texture
        glGenerateMipmap(GL_TEXTURE_2D);

        //Unbind currently bound texture (Not strictly necessary but good practise)
        glBindTexture(GL_TEXTURE_2D, 0);

        //Deletes texture data off of CPU ie. texture data is only on GPU now
        stbi_image_free(m_LocalBuffer);

    }else{
        std::cout << "Failed to load texture" << std::endl;
    }
}

Texture::~Texture()
{
    glDeleteTextures(1, &m_RendererID);
}

void Texture::bind(unsigned int slot /* =0 */) const{

    glActiveTexture(GL_TEXTURE0 + slot);
    glBindTexture(GL_TEXTURE_2D, m_RendererID);
}

void Texture::unbind() const{

    glBindTexture(GL_TEXTURE_2D, 0);
}
