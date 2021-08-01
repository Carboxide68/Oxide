#include "Oxide/Core/Window.h"
#include "Oxide/Renderer/Renderer.h"
#include "Oxide/Renderer/Texture.h"

namespace Oxide {

    std::unordered_map<uint64_t, Window*> Window::m_WindowInstances;
    
    Scope<Renderer> Game = Renderer::Create();

    TextureLibrary TextureLib = TextureLibrary();

}