#include "Oxide/Renderer/RendererAPI.h"
#include "Platform/OpenGL/OpenGLRendererAPI.h"

namespace Oxide {

    RendererAPI::API RendererAPI::s_API = RendererAPI::API::OpenGL;

    Scope<RendererAPI> RendererAPI::Create() {

        switch (s_API) {
            case RendererAPI::API::OpenGL: return CreateScope<OpenGLRendererAPI>();
            case RendererAPI::API::None: CO_CORE_ASSERT(false, "RendererAPI::None is currently not supported"); return nullptr;
        }

        CO_CORE_ASSERT(false, "This RendererAPI does not exist!");
        return nullptr;

    }
}