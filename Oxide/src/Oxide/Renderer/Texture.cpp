
#include "Oxide/Renderer/CRenderer.h"

#include "Oxide/Renderer/Texture.h"
#include "Platform/OpenGL/OpenGLTexture.h"

namespace Oxide {

    Ref<Texture2D> Texture2D::Create(const std::string& imagePath) {
        switch(CRenderer::GetAPI()) {
            case CRenderer::API::None: CO_CORE_ASSERT(false, "Renderer::API::None is currently not supported!"); return nullptr;
            case CRenderer::API::OpenGL: return CreateRef<OpenGLTexture2D>(imagePath);
        };

        CO_CORE_ASSERT(false, "This Renderer API does not exist!");
        return nullptr;
    }

    Ref<Texture2D> Texture2D::Create(const int& x, const int& y) {

        switch(CRenderer::GetAPI()) {
            case CRenderer::API::None: CO_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
            case CRenderer::API::OpenGL: return CreateRef<OpenGLTexture2D>(x, y);
        };

        CO_CORE_ASSERT(false, "This Renderer API does not exist!");
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