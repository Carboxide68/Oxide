#include "Oxide/Renderer/Renderer.h"
#include "Platform/OpenGL/OpenGLRenderer.h"

namespace Oxide {

    Renderer::API Renderer::s_API = Renderer::API::OpenGL;

    Scope<Renderer> Renderer::Create() {

        switch (s_API) {
            case Renderer::API::OpenGL: return CreateScope<OpenGLRenderer>();
            case Renderer::API::None: CO_CORE_ASSERT(false, "RendererAPI::None is currently not supported"); return nullptr;
        }

        CO_CORE_ASSERT(false, "This RendererAPI does not exist!");
        return nullptr;

    }
}