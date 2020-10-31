#include "Oxide/Renderer/Renderer.h"

namespace Oxide {

    Renderer::Renderer() {

        m_RendererAPI = RendererAPI::Create();

    }

    void Renderer::Init() {
        m_RendererAPI->Init();
    }

    void Renderer::BeginFrame() {

        m_RendererAPI->ClearBuffers();

    }

    void Renderer::EndFrame() {

    }

    void Renderer::SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) {

        m_Viewport = {x, y, width, height};
        m_RendererAPI->SetViewport(x, y, width, height);

    }

    void Renderer::SetViewport(uint32_t width, uint32_t height) {

        m_Viewport.width = width;
        m_Viewport.height = height;
        m_RendererAPI->SetViewport(0, 0, width, height);

    }

    void Renderer::SetClearColor(const float& r, const float& g, const float& b, const float& a) {
        m_RendererAPI->SetClearColor({r, g, b, a});

    }

    void Renderer::SetClearColor(const glm::vec4& color) {
        m_RendererAPI->SetClearColor(color);
        
    }
 
}