
#ifndef TEXTURE_HPP
#define TEXTURE_HPP

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <string>

class Texture{

    public:
        Texture(const std::string& path);
        ~Texture();

        void bind(unsigned int slot /* =0 */) const;
        void unbind() const;

        int getWidth() const { return m_Width; }
        int getHeight() const { return m_Height; }

    private:
        unsigned int m_RendererID;
        std::string m_FilePath;
        unsigned char* m_LocalBuffer;
        int m_Width, m_Height, m_BPP; //Bits Per Pixel

};

#endif
