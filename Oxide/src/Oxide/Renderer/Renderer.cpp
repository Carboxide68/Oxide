#include "Oxide/Renderer/Renderer.h"

namespace Oxide {

    Renderer::Renderer() {

        m_RendererAPI = RendererAPI::Create();
        m_RendererAPI->Init();

    }

    void Renderer::BeginFrame() {

        m_RendererAPI->ClearBuffers();

    }

    void Renderer::EndFrame() {

    }
 
}