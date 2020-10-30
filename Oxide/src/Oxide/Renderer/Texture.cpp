
#include "Oxide/Renderer/RendererAPI.h"

#include "Oxide/Renderer/Texture.h"
#include "Platform/OpenGL/OpenGLTexture.h"

namespace Oxide {

    Ref<Texture2D> Texture2D::Create(const std::string& imagePath) {
        switch(RendererAPI::GetAPI()) {
            case RendererAPI::API::None: CO_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
            case RendererAPI::API::OpenGL: return CreateRef<OpenGLTexture2D>(imagePath);
        };

        CO_CORE_ASSERT(false, "This RendererAPI does not exist!");
        return nullptr;
    }

    Ref<Texture2D> Texture2D::Create(const int& x, const int& y) {

        switch(RendererAPI::GetAPI()) {
            case RendererAPI::API::None: CO_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
            case RendererAPI::API::OpenGL: return CreateRef<OpenGLTexture2D>(x, y);
        };

        CO_CORE_ASSERT(false, "This RendererAPI does not exist!");
        return nullptr;
    }

    void TextureLibrary::Add(const Ref<Texture2D>& texture) {
        CO_CORE_ASSERT(!Exists(texture->GetName()), "Texture already exists in library!");
        m_Texture2Ds[texture->GetName()] = texture;
    }

    bool TextureLibrary::Exists(const std::string& imagePath) const {
        return (m_Texture2Ds.find(imagePath) != m_Texture2Ds.end());
    }

    Ref<Texture2D> TextureLibrary::Get(const std::string& name) {
        return Exists(name) ? m_Texture2Ds[name] : nullptr;
    }
}