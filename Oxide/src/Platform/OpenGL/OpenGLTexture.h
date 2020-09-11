#pragma once

#include "Oxide/Core/Base.h"
#include "Oxide/Renderer/Texture.h"

typedef uint32_t GLenum;

namespace Oxide {

    class OpenGLTexture2D : public Texture2D {

    public:

        OpenGLTexture2D(const std::string& imagePath);
        OpenGLTexture2D(const uint32_t& x, const uint32_t& y);
        OpenGLTexture2D();

        ~OpenGLTexture2D();

        virtual void Bind(uint32_t slot) override;

        virtual void Load(const std::string& imagePath) override;

        virtual const std::string& GetName() const override;
        virtual void SetName(const std::string& name) override;

        virtual void SetSettings(const TextureSettings& settings) override;
        virtual TextureSettings GetSettings() const override;

        virtual const std::string& GetShaderName() const override;
        virtual void SetShaderName(const std::string& name) override;

        virtual uint32_t GetHeight() const override;
        virtual uint32_t GetWidth() const override;
        virtual uint32_t GetRendererID() const override;

        virtual void SetData(void *data, uint32_t size) override;

        void UpdateSettings();

    private:

        std::string m_Name;
        std::string m_ShaderName;
        TextureSettings m_Settings;

        uint32_t m_Height;
        uint32_t m_Width;

        uint32_t m_RendererID;
        GLenum m_InternalFormat, m_DataFormat;

    };

}