#include "Oxide/Renderer/CRenderer.h"
#include "Platform/OpenGL/OpenGLRenderer.h"

namespace Oxide {

    CRenderer::API CRenderer::s_API = CRenderer::API::OpenGL;

    Scope<CRenderer> CRenderer::Create() {

        switch (s_API) {
            case CRenderer::API::OpenGL: return CreateScope<OpenGLRenderer>();
            case CRenderer::API::None: CO_CORE_ASSERT(false, "RendererAPI::None is currently not supported"); return nullptr;
        }

        CO_CORE_ASSERT(false, "This RendererAPI does not exist!");
        return nullptr;

    }

    void CRenderer::SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) {
        SetViewport({x, y, width, height});
    }

    void CRenderer::SetClearColor(const glm::vec4& color) {
        SetClearColor(color.x, color.y, color.z, color.w);
    }
}