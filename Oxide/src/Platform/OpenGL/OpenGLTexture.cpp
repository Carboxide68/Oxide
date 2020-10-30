
#include "Platform/OpenGL/OpenGLTexture.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image/stb_image.h>
#include <GL/glew.h>

namespace Oxide {

    OpenGLTexture2D::OpenGLTexture2D(const std::string& imagePath) {
        m_Settings = TextureSettings::STANDARD_SETTING;
        int x, y, channels;
        unsigned char *data = stbi_load(imagePath.c_str(), &x, &y, &channels, 0);

        m_Width = (uint32_t)x;
        m_Height = (uint32_t)y;

        CO_CORE_ASSERT(data, "Failed to load image %s!", imagePath);

        if (channels == 3) {
            m_InternalFormat = GL_RGB8;
            m_DataFormat = GL_RGB;
        } else if (channels == 4) {
            m_InternalFormat = GL_RGBA8;
            m_DataFormat = GL_RGBA;
        } else if (channels == 1) {
            m_InternalFormat = GL_R8;
            m_DataFormat = GL_RED;
        }

        glGenTextures(1, &m_RendererID);
        glBindTexture(GL_TEXTURE_2D, m_RendererID);
        glTexImage2D(GL_TEXTURE_2D, 0, m_InternalFormat, (GLsizei)m_Width, (GLsizei)m_Height, 0, m_DataFormat, GL_UNSIGNED_BYTE, data);
        UpdateSettings();

        stbi_image_free(data);

    }

    OpenGLTexture2D::OpenGLTexture2D(const uint32_t& width, const uint32_t& height) : m_Width(width), m_Height(height) {
        m_Settings = TextureSettings::STANDARD_SETTING;
        glGenTextures(1, &m_RendererID);
        glBindTexture(GL_TEXTURE_2D, m_RendererID);

        m_InternalFormat = GL_RGBA8;
        m_DataFormat = GL_RGBA;

        glTexImage2D(GL_TEXTURE_2D, 0, m_InternalFormat, m_Width, m_Height, 0, m_DataFormat, GL_UNSIGNED_BYTE, nullptr);
        UpdateSettings();

    }

    OpenGLTexture2D::OpenGLTexture2D() {
        m_Settings = TextureSettings::STANDARD_SETTING;
        glGenTextures(1, &m_RendererID);

    }

    OpenGLTexture2D::~OpenGLTexture2D() {
        glDeleteTextures(1, &m_RendererID);
    }

    void OpenGLTexture2D::UpdateSettings() {

        glBindTexture(GL_TEXTURE_2D, m_RendererID);

        switch (m_Settings.WrapType) {
            case TextureSettings::Wrap::NONE:
            case TextureSettings::Wrap::REPEAT:
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_REPEAT);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
                break;
            case TextureSettings::Wrap::CLAMP:
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
                break;
        }

        switch(m_Settings.MagFilter) {
            case TextureSettings::Mag::NONE:
            case TextureSettings::Mag::LINEAR: glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); break;
            case TextureSettings::Mag::NEAREST: glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST); break;
        }

        if (m_Settings.Mipmap == 0) {
            switch(m_Settings.MinFilter) {
                case TextureSettings::Min::NONE:
                case TextureSettings::Min::LINEAR: glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); break;
                case TextureSettings::Min::NEAREST: glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST); break;
            }
        } else {
            glGenerateMipmap(GL_TEXTURE_2D);
            switch(m_Settings.MinFilter) {
                case TextureSettings::Min::NONE:
                case TextureSettings::Min::LINEAR: glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR); break;
                case TextureSettings::Min::NEAREST: glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST); break;
            }
        }
    }

    void OpenGLTexture2D::SetSettings(const TextureSettings& settings) {
        m_Settings = settings;
    }

    TextureSettings OpenGLTexture2D::GetSettings() const {
        return m_Settings;
    }

    void OpenGLTexture2D::SetName(const std::string& name) {
        m_Name = name;
    }

    const std::string& OpenGLTexture2D::GetName() const {
        return m_Name;
    }

    void OpenGLTexture2D::SetShaderName(const std::string& name) {
        m_ShaderName = name;
    }

    const std::string& OpenGLTexture2D::GetShaderName() const {
        return m_ShaderName;
    }

    uint32_t OpenGLTexture2D::GetHeight() const {
        return m_Height;
    }

    uint32_t OpenGLTexture2D::GetWidth() const {
        return m_Width;
    }

    uint32_t OpenGLTexture2D::GetRendererID() const {
        return m_RendererID;
    }

    void OpenGLTexture2D::SetData(void* data, uint32_t size) {
        uint32_t bpp;
        switch (m_DataFormat) {
            case GL_RGBA:   bpp = 4; break;
            case GL_RGB:    bpp = 3; break;
            case GL_RED:    bpp = 1; break;
            default:        bpp = 0; break;
        }
        CO_CORE_ASSERT(size == (m_Width * m_Height * bpp), "Data must fill entire buffer!");
        glBindTexture(GL_TEXTURE_2D, m_RendererID);
        glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, m_Width, m_Height, m_DataFormat, GL_UNSIGNED_BYTE, data);
    }

    void OpenGLTexture2D::Bind(uint32_t slot) {
        glActiveTexture(GL_TEXTURE0 + slot);
        glBindTexture(GL_TEXTURE_2D, m_RendererID);
    }

    void OpenGLTexture2D::Load(const std::string& imagePath) {
        
    }

}