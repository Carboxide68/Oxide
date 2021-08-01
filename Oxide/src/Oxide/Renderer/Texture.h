#pragma once

#include "Oxide/Core/Base.h"
#include "Oxide/Renderer/TextureSettings.h"
#include "Oxide/Renderer/OpenGLCommon.h"

#include <vector>
#include <unordered_map>

typedef uint32_t GLenum;

namespace Oxide {

    class Texture2D {

    public:

        ~Texture2D();

        void Bind(uint32_t slot);

        void Load(const std::string& imagePath);

        const std::string& GetName() const;
        void SetName(const std::string& name);

        void SetSettings(const TextureSettings& settings);
        TextureSettings GetSettings() const;

        uint32_t GetHeight() const;
        uint32_t GetWidth() const;
        uint32_t GetHandle() const;

        void SetData(void *data, uint32_t size);

        void UpdateSettings();

        static Ref<Texture2D> Create(const std::string& imagePath);
        static Ref<Texture2D> Create(const uint32_t& x, const uint32_t& y);
        static Ref<Texture2D> Create();

    private:

        Texture2D(const std::string& imagePath);
        Texture2D(const uint32_t& x, const uint32_t& y);
        Texture2D();

        std::string m_Name;
        std::string m_ShaderName;
        TextureSettings m_Settings;

        uint32_t m_Width;
        uint32_t m_Height;

        uint32_t m_RendererID;
        GLenum m_InternalFormat, m_DataFormat;

    };


    class TextureLibrary {

    public:

        void Add(const Ref<Texture2D>& texture);

        bool Exists(const std::string& imagePath) const;

        Ref<Texture2D> Get(const std::string& imagePath);

    private:

        std::unordered_map<std::string, Ref<Texture2D>> m_Texture2Ds;

    };

    extern TextureLibrary TextureLib;

}