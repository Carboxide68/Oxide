#pragma once

#include "Oxide/Core/Base.h"
#include "Oxide/Renderer/TextureSettings.h"

#include <vector>
#include <unordered_map>

namespace Oxide {

    class Texture2D {
    public:

        virtual ~Texture2D() = default;

        virtual void Bind(uint32_t slot) = 0;
        virtual void Load(const std::string& imagePath) = 0;

        virtual const std::string& GetName() const = 0;
        virtual void SetName(const std::string& name) = 0;

        virtual const std::string& GetShaderName() const = 0;
        virtual void SetShaderName(const std::string& name) = 0;

        virtual void SetSettings(const TextureSettings& settings) = 0;
        virtual TextureSettings GetSettings() const = 0;

        virtual uint32_t GetHeight() const = 0;
        virtual uint32_t GetWidth() const = 0;
        virtual uint32_t GetRendererID() const = 0;

        virtual void SetData(void *data, uint32_t size) = 0;

        static Ref<Texture2D> Create(const std::string& imagePath);
        static Ref<Texture2D> Create(const int& x, const int& y);

    private:

        std::string m_Name;
        std::string m_ShaderName;

        uint32_t m_Height;
        uint32_t m_Width;

    };

    class TextureCubeMap {
    public:

        virtual ~TextureCubeMap() = default;

        virtual void Load(const std::vector<std::string>& imagePaths) = 0;
        virtual void Bind(uint32_t slot) = 0;

        static Ref<TextureCubeMap> Create();

    private:

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