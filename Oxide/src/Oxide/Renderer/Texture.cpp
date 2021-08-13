
#include "Oxide/Renderer/Texture.h"
#include "Oxide/Core/Log.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image/stb_image.h>
#include <GL/glew.h>

namespace Oxide {

    Ref<Texture2D> Texture2D::Create(const std::string& imagePath) {
        return Ref<Texture2D>(new Texture2D(imagePath));
    }

    Ref<Texture2D> Texture2D::Create(const uint32_t& width, const uint32_t& height) {
        return Ref<Texture2D>(new Texture2D(width, height));
    }

    Ref<Texture2D> Texture2D::Create() {
        return Ref<Texture2D>(new Texture2D());
    }

    Texture2D::Texture2D(const std::string& imagePath) {
        m_Settings = TextureSettings::STANDARD_SETTING;
        glGenTextures(1, &m_RendererID);
        Load(imagePath);
        m_Name = imagePath;
    }

    Texture2D::Texture2D(const uint32_t& width, const uint32_t& height) : m_Width(width), m_Height(height) {
        m_Settings = TextureSettings::STANDARD_SETTING;
        glGenTextures(1, &m_RendererID);
        glBindTexture(GL_TEXTURE_2D, m_RendererID);

        m_InternalFormat = GL_RGBA8;
        m_DataFormat = GL_RGBA;

        glTexImage2D(GL_TEXTURE_2D, 0, m_InternalFormat, m_Width, m_Height, 0, m_DataFormat, GL_UNSIGNED_BYTE, nullptr);
        UpdateSettings();

    }

    Texture2D::Texture2D() {
        m_Settings = TextureSettings::STANDARD_SETTING;
        glGenTextures(1, &m_RendererID);

    }

    Texture2D::~Texture2D() {
        glDeleteTextures(1, &m_RendererID);
    }

    void Texture2D::UpdateSettings() {

        ZoneScoped
        
        TracyGpuZone("UpdateTexSettings")
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

    void Texture2D::SetSettings(const TextureSettings& settings) {
        m_Settings = settings;
    }

    TextureSettings Texture2D::GetSettings() const {
        return m_Settings;
    }

    void Texture2D::SetName(const std::string& name) {
        m_Name = name;
    }

    const std::string& Texture2D::GetName() const {
        return m_Name;
    }

    uint32_t Texture2D::GetHeight() const {
        return m_Height;
    }

    uint32_t Texture2D::GetWidth() const {
        return m_Width;
    }

    uint32_t Texture2D::GetHandle() const {
        return m_RendererID;
    }

    void Texture2D::SetData(void* data, uint32_t size) {
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

    void Texture2D::Bind(uint32_t slot) {
        glActiveTexture(GL_TEXTURE0 + slot);
        glBindTexture(GL_TEXTURE_2D, m_RendererID);
    }

    void Texture2D::Load(const std::string& imagePath) {
        ZoneScopedN("LoadTexture")

        int x, y, channels;
        unsigned char *data;
        {
        ZoneScopedN("stbi_load")
        data = stbi_load(imagePath.c_str(), &x, &y, &channels, 0);
        }
        m_Width = (uint32_t)x;
        m_Height = (uint32_t)y;

        if (!data) {
            CO_CORE_WARN("Failed to load image %s!", imagePath.c_str());
            return;
        }

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
        TracyGpuZone("TexImage")
        glBindTexture(GL_TEXTURE_2D, m_RendererID);
        glTexImage2D(GL_TEXTURE_2D, 0, m_InternalFormat, (GLsizei)m_Width, (GLsizei)m_Height, 0, m_DataFormat, GL_UNSIGNED_BYTE, data);
        UpdateSettings();

        stbi_image_free(data);
    }

    void TextureLibrary::Add(const Ref<Texture2D>& texture) {
        m_Texture2Ds[texture->GetName()] = texture;
    }

    bool TextureLibrary::Exists(const std::string& imagePath) const {
        return (m_Texture2Ds.find(imagePath) != m_Texture2Ds.end());
    }

    Ref<Texture2D> TextureLibrary::Get(const std::string& name) {
        return Exists(name) ? m_Texture2Ds[name] : nullptr;
    }
}